#pragma once

#include <stdint.h>
#include <limits.h>

#include "../poly.h"

// nalezy wywolac te funkcje w kazdym tescie przed uzyciem pierwszego Random 
void ResetRng();

uint32_t RandomUint();

uint64_t RandomUlong();

int RandomInt(int a, int b);

int64_t RandomLong(int64_t a, int64_t b);

uint32_t RandomIndex(uint32_t n);

Poly GenRandomPoly(size_t depth);

void PolyPrint(const Poly *p);
