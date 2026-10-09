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
