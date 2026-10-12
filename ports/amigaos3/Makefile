# PF-SP-001 — isolated host tests and optional AmigaOS/m68k MiniGL build.
HOST_CC ?= cc
HOST_CFLAGS ?= -std=c99 -O2 -Wall -Wextra -Werror -pedantic
CC68K ?= m68k-amigaos-gcc
AMIGA_CFLAGS ?= -std=gnu99 -O2 -Wall -Wextra -Werror=implicit-function-declaration -m68020-60 -m68881
MINIGL_INCLUDE ?=
MINIGL_IMPORT_LIB ?=
BUILD := build
HOST_TEST := $(BUILD)/pc_tile_convert_test
CORE_TEST := $(BUILD)/pc_core_test
AMIGA_SMOKE := $(BUILD)/pc_minigl_smoke

.PHONY: all host-test host-core-test host-core-sanitize amiga-preflight amiga-smoke clean
all: host-test

$(BUILD):
	mkdir -p $(BUILD)

$(HOST_TEST): tests/pc_tile_convert_test.c src/pc_tile_convert.c include/pc_tile_convert.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/pc_tile_convert_test.c src/pc_tile_convert.c

$(CORE_TEST): tests/pc_core_test.c src/pc_core.c include/pc_core.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ src/pc_core.c tests/pc_core_test.c

host-core-test: $(CORE_TEST)
	./$(CORE_TEST)

host-test: $(HOST_TEST) $(CORE_TEST)
	./$(HOST_TEST)
	./$(CORE_TEST)

host-core-sanitize: | $(BUILD)
	$(HOST_CC) -std=c99 -g -O1 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude -o $(BUILD)/pc_core_test_san src/pc_core.c tests/pc_core_test.c
	./$(BUILD)/pc_core_test_san

amiga-preflight:
	@command -v "$(CC68K)" >/dev/null 2>&1 || { echo "BLOCKED: missing $(CC68K)"; exit 2; }
	@test -n "$(MINIGL_INCLUDE)" && test -f "$(MINIGL_INCLUDE)/proto/minigl.h" || { echo "BLOCKED: set MINIGL_INCLUDE to MiniGL SDK include root"; exit 2; }
	@test -n "$(MINIGL_IMPORT_LIB)" && test -f "$(MINIGL_IMPORT_LIB)" || { echo "BLOCKED: set MINIGL_IMPORT_LIB to matching libminigl.a"; exit 2; }
	@echo "SDK preflight passed: $(CC68K)"

amiga-smoke: amiga-preflight | $(BUILD)
	$(CC68K) $(AMIGA_CFLAGS) -Iinclude -I"$(MINIGL_INCLUDE)" -o $(AMIGA_SMOKE) src/pc_minigl_smoke.c src/pc_tile_convert.c "$(MINIGL_IMPORT_LIB)"
	@echo "Built $(AMIGA_SMOKE); execute manually in WinUAE for runtime evidence"

clean:
	rm -rf $(BUILD)


# PF-SP-003: GCC13 opt-in, independent of MiniGL SDK and host baseline.
# Mirrors vxplatform ADR-0014: explicit GCC_BEBBO_ROOT, fail-closed version
# and target guards, separate O0/O2 artifact directories and C runtime.
GCC13_CPU ?= -m68020
GCC13_FPU ?= -msoft-float
GCC13_CFLAGS ?= -std=c99 -Wall -Wextra -Werror -fno-omit-frame-pointer
GCC13_LDFLAGS ?= -noixemul
GCC13_BUILD := $(BUILD)/m68k-gcc13
GCC13_CC = $(GCC_BEBBO_ROOT)/bin/m68k-amigaos-gcc
VAMOS ?= vamos
RUST_ORACLE ?=

.PHONY: gcc13-preflight gcc13-build-tests gcc13-check-hunks gcc13-vamos-tests
gcc13-preflight:
	@GCC_BEBBO_ROOT="$(GCC_BEBBO_ROOT)" sh scripts/gcc13-preflight.sh

# Explicit full builds: -O0 is debugging baseline, -O2 checks codegen regressions.
# No .a archive may leak from vxplatform GCC16 or legacy GCC6 into these tests.
gcc13-build-tests: gcc13-preflight
	@set -eu; for opt in O0 O2; do \
	  mkdir -p "$(GCC13_BUILD)/$$opt"; \
	  flags="$(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -$$opt"; \
	  "$(GCC13_CC)" $$flags -Iinclude $(GCC13_LDFLAGS) \
	    src/pc_core.c tests/pc_core_test.c -o "$(GCC13_BUILD)/$$opt/pc_core_test"; \
	  "$(GCC13_CC)" $$flags -Iinclude $(GCC13_LDFLAGS) \
	    src/pc_tile_convert.c tests/pc_tile_convert_test.c -o "$(GCC13_BUILD)/$$opt/pc_tile_convert_test"; \
	  "$(GCC13_CC)" $$flags -Iinclude $(GCC13_LDFLAGS) \
	    src/pc_core.c tests/core_oracle.c -o "$(GCC13_BUILD)/$$opt/core_oracle"; \
	  echo "Built GCC13 $$opt Amiga executables"; \
	done

gcc13-check-hunks:
	@python3 scripts/check-hunk.py $(GCC13_BUILD)/O0/pc_core_test \
	  $(GCC13_BUILD)/O0/pc_tile_convert_test $(GCC13_BUILD)/O0/core_oracle \
	  $(GCC13_BUILD)/O2/pc_core_test $(GCC13_BUILD)/O2/pc_tile_convert_test \
	  $(GCC13_BUILD)/O2/core_oracle

# Must run on host with vamos installed, not inside the compiler-only image.
gcc13-vamos-tests:
	@VAMOS="$(VAMOS)" M68K_BUILD_DIR="$(GCC13_BUILD)" \
	  RUST_ORACLE="$(RUST_ORACLE)" sh scripts/vamos-core-tests.sh

# PF-SP-002 experimental generic encoded raster and flat layer prototypes.
RASTER_TEST := $(BUILD)/pc_raster_test
DOCUMENT_TEST := $(BUILD)/pc_document_test
DOCUMENT_ORACLE := $(BUILD)/document_oracle
GROUP_ORACLE := $(BUILD)/group_oracle
MASK_ORACLE := $(BUILD)/mask_oracle
TYPED_ORACLE := $(BUILD)/typed_oracle

$(RASTER_TEST): tests/pc_raster_test.c src/pc_raster.c src/pc_core.c include/pc_raster.h include/pc_core.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/pc_raster_test.c src/pc_raster.c src/pc_core.c

$(DOCUMENT_TEST): tests/pc_document_test.c src/pc_document.c src/pc_raster.c src/pc_core.c include/pc_document.h src/pc_raster_owner_internal.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/pc_document_test.c src/pc_document.c src/pc_raster.c src/pc_core.c

$(MASK_ORACLE): tests/mask_oracle.c src/pc_document.c src/pc_raster.c src/pc_core.c include/pc_document.h src/pc_raster_owner_internal.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/mask_oracle.c src/pc_document.c src/pc_raster.c src/pc_core.c

$(GROUP_ORACLE): tests/group_oracle.c src/pc_document.c src/pc_raster.c src/pc_core.c include/pc_document.h src/pc_raster_owner_internal.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/group_oracle.c src/pc_document.c src/pc_raster.c src/pc_core.c

$(DOCUMENT_ORACLE): tests/document_oracle.c src/pc_document.c src/pc_raster.c src/pc_core.c include/pc_document.h src/pc_raster_owner_internal.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/document_oracle.c src/pc_document.c src/pc_raster.c src/pc_core.c

$(TYPED_ORACLE): tests/typed_oracle.c src/pc_raster.c src/pc_core.c include/pc_raster.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/typed_oracle.c src/pc_raster.c src/pc_core.c

.PHONY: host-typed-test host-typed-sanitize gcc13-build-typed gcc13-check-typed-hunks gcc13-vamos-typed-tests
host-typed-test: $(RASTER_TEST) $(DOCUMENT_TEST) $(TYPED_ORACLE) $(DOCUMENT_ORACLE) $(GROUP_ORACLE) $(MASK_ORACLE)
	./$(RASTER_TEST)
	./$(DOCUMENT_TEST)

host-test: host-typed-test

host-typed-sanitize: | $(BUILD)
	$(HOST_CC) -std=c99 -O1 -g -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude -o $(BUILD)/pc_raster_test_san tests/pc_raster_test.c src/pc_raster.c src/pc_core.c
	./$(BUILD)/pc_raster_test_san
	$(HOST_CC) -std=c99 -O1 -g -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude -o $(BUILD)/pc_document_test_san tests/pc_document_test.c src/pc_document.c src/pc_raster.c src/pc_core.c
	./$(BUILD)/pc_document_test_san

host-core-sanitize: host-typed-sanitize

gcc13-build-typed: gcc13-preflight
	@set -eu; for opt in O0 O2; do \
	  mkdir -p "$(GCC13_BUILD)/$$opt"; \
	  flags="$(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -$$opt"; \
	  "$(GCC13_CC)" $$flags -Iinclude $(GCC13_LDFLAGS) \
	    src/pc_raster.c src/pc_core.c tests/pc_raster_test.c \
	    -o "$(GCC13_BUILD)/$$opt/pc_raster_test"; \
	  "$(GCC13_CC)" $$flags -Iinclude $(GCC13_LDFLAGS) \
	    src/pc_document.c src/pc_raster.c src/pc_core.c tests/pc_document_test.c \
	    -o "$(GCC13_BUILD)/$$opt/pc_document_test"; \
	  "$(GCC13_CC)" $$flags -Iinclude $(GCC13_LDFLAGS) \
	    src/pc_raster.c src/pc_core.c tests/typed_oracle.c \
	    -o "$(GCC13_BUILD)/$$opt/typed_oracle"; \
	  echo "Built GCC13 $$opt typed raster / flat document / oracle HUNK"; \
	done

gcc13-check-typed-hunks:
	@python3 scripts/check-hunk.py $(GCC13_BUILD)/O0/pc_raster_test $(GCC13_BUILD)/O0/pc_document_test $(GCC13_BUILD)/O0/typed_oracle \
	  $(GCC13_BUILD)/O2/pc_raster_test $(GCC13_BUILD)/O2/pc_document_test $(GCC13_BUILD)/O2/typed_oracle

gcc13-check-hunks: gcc13-check-typed-hunks

gcc13-vamos-typed-tests:
	@VAMOS="$(VAMOS)" M68K_BUILD_DIR="$(GCC13_BUILD)" RUST_ORACLE_TYPED="$(RUST_ORACLE_TYPED)" sh scripts/vamos-typed-tests.sh

# PF-SP-010: bounded, host-testable colour/alpha staging for RTG M2.
PRESENT_CONVERT_TEST := $(BUILD)/pc_present_convert_test
$(PRESENT_CONVERT_TEST): tests/pc_present_convert_test.c src/pc_present_convert.c src/pc_tile_convert.c include/pc_present_convert.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/pc_present_convert_test.c src/pc_present_convert.c src/pc_tile_convert.c

.PHONY: host-present-convert-test
host-present-convert-test: $(PRESENT_CONVERT_TEST)
	./$(PRESENT_CONVERT_TEST)

host-test: host-present-convert-test
.PHONY: host-present-convert-sanitize
host-present-convert-sanitize: | $(BUILD)
	$(HOST_CC) -std=c99 -g -O1 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude -o $(BUILD)/pc_present_convert_san tests/pc_present_convert_test.c src/pc_present_convert.c src/pc_tile_convert.c
	./$(BUILD)/pc_present_convert_san

host-core-sanitize: host-present-convert-sanitize

# PF-SP-010: compile native M2 with the same pinned GCC13/NDK lane.
# This is compile-only evidence, NOT a WinUAE graphics runtime PASS.
.PHONY: gcc13-m2-compile
gcc13-m2-compile: gcc13-preflight
	@mkdir -p $(GCC13_BUILD)/m2
	"$(GCC13_CC)" $(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -O2 -Iinclude -c src/pc_present_convert.c -o $(GCC13_BUILD)/m2/pc_present_convert.o
	"$(GCC13_CC)" $(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -O2 -Iinclude -c src/pc_present_m2.c -o $(GCC13_BUILD)/m2/pc_present_m2.o
	@echo "COMPILE_ONLY: RTG M2 target objects; no WinUAE result"

# PF-SP-011: optional real AmigaOS HUNK shell, no MiniGL import library.
# Link check only; manual WinUAE ReAction/RTG acceptance remains mandatory.
.PHONY: gcc13-native-shell
gcc13-native-shell: gcc13-preflight | $(BUILD)
	@mkdir -p $(GCC13_BUILD)/shell
	"$(GCC13_CC)" $(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -O2 -Iinclude $(GCC13_LDFLAGS) src/pc_amiga_shell.c src/pc_canvas_doc.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c src/pc_present_m2.c src/pc_present_convert.c -o $(GCC13_BUILD)/shell/photocraft-amiga-document-preview
	@python3 scripts/check-hunk.py $(GCC13_BUILD)/shell/photocraft-amiga-document-preview
	@echo "BUILD_ONLY: PhotoCraft ReAction RTG shell HUNK; manual WinUAE pending"

# PF-SP-007: bounded normal CPU compositor, flat RGBA8 subset.
COMPOSE_TEST := $(BUILD)/pc_compose_test
$(COMPOSE_TEST): tests/pc_compose_test.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c include/pc_compose.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/pc_compose_test.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c

.PHONY: host-compose-test gcc13-build-compose gcc13-vamos-compose
host-compose-test: $(COMPOSE_TEST)
	./$(COMPOSE_TEST)
host-test: host-compose-test

.PHONY: host-compose-sanitize
host-compose-sanitize: | $(BUILD)
	$(HOST_CC) -std=c99 -g -O1 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude -o $(BUILD)/pc_compose_test_san tests/pc_compose_test.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c
	./$(BUILD)/pc_compose_test_san
host-core-sanitize: host-compose-sanitize

gcc13-build-compose: gcc13-preflight
	@set -eu; for opt in O0 O2; do \
	  mkdir -p "$(GCC13_BUILD)/$$opt"; \
	  "$(GCC13_CC)" $(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -$$opt -Iinclude $(GCC13_LDFLAGS) \
	  tests/pc_compose_test.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c -o "$(GCC13_BUILD)/$$opt/pc_compose_test"; \
	done

.PHONY: gcc13-check-compose-hunks
gcc13-check-compose-hunks:
	@python3 scripts/check-hunk.py $(GCC13_BUILD)/O0/pc_compose_test $(GCC13_BUILD)/O2/pc_compose_test

gcc13-check-hunks: gcc13-check-compose-hunks

gcc13-vamos-compose:
	@set -eu; for opt in O0 O2; do \
	  "$(VAMOS)" -S -C 20 -m 8192 -s 128 "$(GCC13_BUILD)/$$opt/pc_compose_test" > "$(GCC13_BUILD)/$$opt/compose.log" 2>&1; \
	  grep -q '^PASS: PhotoCraft scoped flat RGBA8 normal compositor/opacity/mask$$' "$(GCC13_BUILD)/$$opt/compose.log"; \
	  cat "$(GCC13_BUILD)/$$opt/compose.log"; \
	done

# Differential against original Rust normal-blend arithmetic (tolerance 1 U8).
COMPOSE_ORACLE := $(BUILD)/compose_oracle
$(COMPOSE_ORACLE): tests/compose_oracle.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/compose_oracle.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c
host-compose-test: $(COMPOSE_ORACLE)
	./$(COMPOSE_ORACLE) > $(BUILD)/oracle-c-compose.txt

gcc13-build-compose-oracle: gcc13-preflight
	@set -eu; for opt in O0 O2; do \
	  mkdir -p "$(GCC13_BUILD)/$$opt"; \
	  "$(GCC13_CC)" $(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -$$opt -Iinclude $(GCC13_LDFLAGS) \
	    tests/compose_oracle.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c -o "$(GCC13_BUILD)/$$opt/compose_oracle"; \
	done
gcc13-check-compose-oracle-hunks:
	@python3 scripts/check-hunk.py $(GCC13_BUILD)/O0/compose_oracle $(GCC13_BUILD)/O2/compose_oracle
gcc13-vamos-compose-oracle:
	@set -eu; for opt in O0 O2; do \
	  "$(VAMOS)" -S -C 20 -m 8192 -s 128 "$(GCC13_BUILD)/$$opt/compose_oracle" > "$(GCC13_BUILD)/$$opt/oracle-c-compose.txt" 2>&1; \
	  python3 scripts/compare-compose.py "$(RUST_COMPOSE_ORACLE)" "$(GCC13_BUILD)/$$opt/oracle-c-compose.txt"; \
	done

# Additive prerequisites: preserve the previous compositor tests, and
# add the independent original-Rust oracle, without overriding recipes.
gcc13-build-compose: gcc13-build-compose-oracle
gcc13-check-compose-hunks: gcc13-check-compose-oracle-hunks
gcc13-vamos-compose: gcc13-vamos-compose-oracle

# Native PcDocument -> CPU compositor -> bounded RGBA8 bands acceptance.
CANVAS_DOCUMENT_TEST := $(BUILD)/pc_canvas_doc_test
$(CANVAS_DOCUMENT_TEST): tests/pc_canvas_doc_test.c src/pc_canvas_doc.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c include/pc_canvas_doc.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/pc_canvas_doc_test.c src/pc_canvas_doc.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c

.PHONY: host-canvas-document-test host-canvas-document-sanitize
host-canvas-document-test: $(CANVAS_DOCUMENT_TEST)
	./$(CANVAS_DOCUMENT_TEST)
host-test: host-canvas-document-test

host-canvas-document-sanitize: | $(BUILD)
	$(HOST_CC) -std=c99 -g -O1 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude -o $(BUILD)/pc_canvas_doc_test_san tests/pc_canvas_doc_test.c src/pc_canvas_doc.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c
	./$(BUILD)/pc_canvas_doc_test_san
host-core-sanitize: host-canvas-document-sanitize

.PHONY: gcc13-build-canvas-document gcc13-check-canvas-document-hunks gcc13-vamos-canvas-document
gcc13-build-canvas-document: gcc13-preflight
	@set -eu; for opt in O0 O2; do \
	  mkdir -p "$(GCC13_BUILD)/$$opt"; \
	  "$(GCC13_CC)" $(GCC13_CFLAGS) $(GCC13_CPU) $(GCC13_FPU) -$$opt -Iinclude $(GCC13_LDFLAGS) \
	    tests/pc_canvas_doc_test.c src/pc_canvas_doc.c src/pc_compose.c src/pc_document.c src/pc_raster.c src/pc_core.c -o "$(GCC13_BUILD)/$$opt/pc_canvas_doc_test"; \
	done
gcc13-check-canvas-document-hunks:
	@python3 scripts/check-hunk.py $(GCC13_BUILD)/O0/pc_canvas_doc_test $(GCC13_BUILD)/O2/pc_canvas_doc_test
gcc13-check-hunks: gcc13-check-canvas-document-hunks
gcc13-vamos-canvas-document:
	@set -eu; for opt in O0 O2; do \
	  "$(VAMOS)" -S -C 20 -m 8192 -s 128 "$(GCC13_BUILD)/$$opt/pc_canvas_doc_test" > "$(GCC13_BUILD)/$$opt/canvas-document.log" 2>&1; \
	  grep -q '^PASS: PhotoCraft PcDocument native canvas bands, mask, visibility and bounds$$' "$(GCC13_BUILD)/$$opt/canvas-document.log"; \
	  cat "$(GCC13_BUILD)/$$opt/canvas-document.log"; \
	done
