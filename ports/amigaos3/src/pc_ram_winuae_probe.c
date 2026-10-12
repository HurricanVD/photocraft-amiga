/* PF-TD-001 RAM-06: native AmigaOS 3.2 Shell-only Fast-RAM observer.
 * Not a GUI, not the full PhotoCraft process, and not a product RAM minimum.
 * The original deterministic C99 regression suite runs unchanged.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <stdio.h>
#include "pc_raster.h"

/* Compile the exact same tests as the host/m68k baseline under another
 * entry point so the observer can cover every tracked allocation. */
#define main pc_raster_atomic_suite_main
#include "../tests/pc_raster_atomic_test.c"
#undef main

static ULONG start_fast, minimum_fast, start_largest, minimum_largest;
static unsigned long samples;

static void observe_raster_allocation(void *context)
{
    ULONG free_fast=AvailMem(MEMF_FAST);
    ULONG largest=AvailMem(MEMF_FAST|MEMF_LARGEST);
    (void)context;
    if(free_fast<minimum_fast)minimum_fast=free_fast;
    if(largest<minimum_largest)minimum_largest=largest;
    ++samples;
}

int main(void)
{
    int rc;
    PcMemoryStats stats;
    ULONG end_fast,end_largest;
    start_fast=AvailMem(MEMF_FAST);
    start_largest=AvailMem(MEMF_FAST|MEMF_LARGEST);
    minimum_fast=start_fast;
    minimum_largest=start_largest;
    samples=0;
    printf("PF-TD-001 RAM-06 target Fast start=%lu largest_start=%lu\n",
           (unsigned long)start_fast,(unsigned long)start_largest);
    pc_raster_test_set_observer(observe_raster_allocation,0);
    rc=pc_raster_atomic_suite_main();
    pc_raster_test_set_observer(0,0);
    end_fast=AvailMem(MEMF_FAST);
    end_largest=AvailMem(MEMF_FAST|MEMF_LARGEST);
    pc_raster_test_stats(&stats);
    printf("PF-TD-001 RAM-06 target Fast min=%lu end=%lu largest_min=%lu largest_end=%lu observations=%lu\n",
           (unsigned long)minimum_fast,(unsigned long)end_fast,
           (unsigned long)minimum_largest,(unsigned long)end_largest,samples);
    printf("PF-TD-001 RAM-06 raster_heap_peak=%lu raster_heap_end=%lu return_code=%d\n",
           (unsigned long)stats.peak_bytes,
           (unsigned long)stats.current_bytes,rc);
    /* Strict bounded-suite check; absolute target RAM release requires the
     * manual WinUAE configuration and report. */
    if(rc || !samples || stats.current_bytes) return 20;
    return 0;
}
