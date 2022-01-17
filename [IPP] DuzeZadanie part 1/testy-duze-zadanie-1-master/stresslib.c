#ifdef NDEBUG
#undef NDEBUG
#endif

#include "stresslib.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// nalezy wywolac te funkcje w kazdym tescie przed uzyciem pierwszego Random 
void ResetRng() {
	srand(0);
}

uint32_t RandomUint() {
	const uint32_t bits = 15;
	const uint32_t mod = (1 << bits);
	assert(RAND_MAX >= mod - 1);
	return (rand() % mod) | ((rand() % mod) << bits) | ((rand() % (32 - 2 * bits)) << (2 * bits));
}

uint64_t RandomUlong() {
	return RandomUint() | (((uint64_t) RandomUint()) << 32);
}

int RandomInt(int a, int b) {
	assert(a <= b);
	return a + (RandomUint() % (b - a + 1));
}

int64_t RandomLong(int64_t a, int64_t b) {
	assert(a <= b);
	return a + (RandomUlong() % (b - a + 1));
}

uint32_t RandomIndex(uint32_t n) {
	return RandomInt(0, n - 1);
}

#include "helper_functions_from_example.h"

// p(x_1, x_2, ...) -> p(x_0, x_1, ...)
static Poly PolyPush(const Poly *p) {
	Poly pc = PolyClone(p);
	Mono m[] = {MonoFromPoly(&pc, 0)};
	return PolyAddMonos(1, m); // sometimes my genius is... it's almost frightening
}

enum Operations {
	ADD,
	SUB,
	MUL,
	NEG,
	AT,
	PUSH,
	OP_CNT
};

static Poly GenRandomPolyBinary(size_t depth, Poly (*op)(const Poly *, const Poly *)) {
	Poly p = GenRandomPoly(depth - 1);
	Poly q = GenRandomPoly(depth - 1);
	Poly res = op(&p, &q);
	PolyDestroy(&p);
	PolyDestroy(&q);
	return res;
}

const poly_coeff_t min_coeff = -10;
const poly_coeff_t max_coeff = 10;
const poly_exp_t max_exp = 10;

Poly GenRandomPoly(size_t depth) {
	if(depth == 0) {
		poly_coeff_t c = RandomLong(min_coeff, max_coeff);
		poly_exp_t n = RandomInt(0, max_exp);
		Poly pc = PolyFromCoeff(c);
		if(c == 0) n = 0; // żeby assert działał
		Mono m[] = {MonoFromPoly(&pc, n)};
		return PolyAddMonos(1, m);
	}

	enum Operations operation = RandomIndex(OP_CNT);
	switch(operation) {
		Poly p, res;
	case ADD:
		return GenRandomPolyBinary(depth, PolyAdd);
	case SUB:
		return GenRandomPolyBinary(depth, PolySub);
	case MUL:
		return GenRandomPolyBinary(depth, PolyMul);
	case NEG:
		p = GenRandomPoly(depth - 1);
		res = PolyNeg(&p);
		PolyDestroy(&p);
		return res;
	case AT:
		p = GenRandomPoly(depth - 1);
		poly_coeff_t x = RandomLong(min_coeff, max_coeff);
		res = PolyAt(&p, x);
		PolyDestroy(&p);
		return res;
	case PUSH:
		p = GenRandomPoly(depth - 1);
		res = PolyPush(&p);
		PolyDestroy(&p);
		return res;
	case OP_CNT: // żeby nie mieć warninga
		break;
	}

	assert(false);
}

void PolyPrint(const Poly *p) {
	if(PolyIsCoeff(p)) {
		if(p->coeff == LONG_MIN)
			printf("C(LONG_MIN)");
		else
			printf("C(%ld)", p->coeff);
	}
	else {
		printf("P(");		
		for(size_t i = 0; i < p->size; i++) {
			if(i) printf(", ");
			PolyPrint(&p->arr[i].p);
			printf(", %d", MonoGetExp(p->arr + i));
		}
		printf(")");
	}
}
