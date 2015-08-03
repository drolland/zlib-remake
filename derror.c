#include "derror.h"
#include <stdlib.h>

enum {
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_ERROR,
    ERROR_LEVEL_CRITICAL
};

void check_error(DError* err) {
    if (err != NULL) {
        if (err->level >= ERROR_LEVEL_CRITICAL ) {
            printf("A critical error code %d as occured, terminating...", err->code);
            exit(1);
        } else {
            printf("An error code %d as occured,err->code");
        }
    }
}