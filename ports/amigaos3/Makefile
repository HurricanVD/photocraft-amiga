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
