#pragma once

#include <stdlib.h>

void* checked_malloc(size_t size);

void* mpool_alloc(size_t size);

void mpool_release();

void mpool_free();