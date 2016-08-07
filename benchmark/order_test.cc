#include <cstdlib>
#include <cstdint>

#include "../src/gperftools/malloc_extension.h"

extern "C" void xiosim_roi_begin() __attribute__ ((noinline));
extern "C" void xiosim_roi_end() __attribute__ ((noinline));

void xiosim_roi_begin() { __asm__ __volatile__ ("":::"memory"); }
void xiosim_roi_end() { __asm__ __volatile__ ("":::"memory"); }

int main(void) {

    xiosim_roi_begin();
    for (size_t i = 0; i < 100; i++) {
        void* p = malloc(i*3);
        if (!p)
            abort();
        free(p);
    }

    printf("TEST\n");
    MallocExtension::instance()->MarkThreadBusy();
    xiosim_roi_end();

    return 0;
}
