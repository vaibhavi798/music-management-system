#include <string.h>
#include "util.h"

void safeCopy(char *dest, const char *src, size_t destSize) {
    if (destSize == 0) return;
    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
}
