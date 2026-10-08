# Isolated bootstrap: does not add C to PhotoCraft's Cargo workspace.
# GNU make + host C99 compiler can run host-test on Linux/WSL2.
HOST_CC ?= cc
HOST_CFLAGS ?= -std=c99 -O2 -Wall -Wextra -Werror -pedantic
CC68K ?= m68k-amigaos-gcc
AMIGA_CFLAGS ?= -std=gnu99 -O2 -Wall -Wextra -m68020-60 -m68881
MINIGL_INCLUDE ?=
MINIGL_IMPORT_LIB ?=

BUILD := build
HOST_TEST := $(BUILD)/pc_tile_convert_test
AMIGA_SMOKE := $(BUILD)/pc_minigl_smoke

.PHONY: all host-test amiga-smoke clean
all: host-test

$(BUILD):
	mkdir -p $(BUILD)

$(HOST_TEST): tests/pc_tile_convert_test.c src/pc_tile_convert.c include/pc_tile_convert.h | $(BUILD)
	$(HOST_CC) $(HOST_CFLAGS) -Iinclude -o $@ tests/pc_tile_convert_test.c src/pc_tile_convert.c

host-test: $(HOST_TEST)
	./$(HOST_TEST)

amiga-smoke: | $(BUILD)
	@test -n "$(MINIGL_INCLUDE)" -a -f "$(MINIGL_INCLUDE)/proto/minigl.h" || { echo "Set MINIGL_INCLUDE to the directory containing proto/minigl.h"; exit 2; }
	@test -n "$(MINIGL_IMPORT_LIB)" -a -f "$(MINIGL_IMPORT_LIB)" || { echo "Set MINIGL_IMPORT_LIB to the official libminigl.a"; exit 2; }
	$(CC68K) $(AMIGA_CFLAGS) -Iinclude -I$(MINIGL_INCLUDE) -o $(AMIGA_SMOKE) src/pc_minigl_smoke.c src/pc_tile_convert.c $(MINIGL_IMPORT_LIB)

clean:
	rm -rf $(BUILD)
