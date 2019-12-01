#include "cxw_str.h"

char *mergeStr(int n, ...) {
    int i = 0;
    va_list argptr;
    size_t size = 0;
    va_start(argptr, n);
    for (i = 0; i < n; ++i) {
        size += strlen(va_arg(argptr, char *));
    }
    va_end(argptr);
    char *pResulut = (char *) malloc(size);
    va_start(argptr, n);
    strcpy(pResulut, va_arg(argptr, char *));
    for (i = 1; i < n; ++i) {
        strcat(pResulut, va_arg(argptr, char *));
    }
    va_end(argptr);
    return pResulut;
}