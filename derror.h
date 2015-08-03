#pragma once
#include <stdio.h>

typedef struct derror {
    int code;
    char level;
} DError;

void check_error(DError* err);

