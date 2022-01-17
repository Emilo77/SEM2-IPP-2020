#ifdef NDEBUG
#undef NDEBUG
#endif

#include "stress.h"
#include "stresslib.h"
#include "helper_functions_from_example.h"

#include <stdio.h>

#define CORR_DEP 10
#define CORR_TC 30

#define PERF_DEP 12
#define PERF_TC 150

Poly ans[CORR_DEP][CORR_TC];

void GenOutput() {
	printf("void SetOutput() {\n");
	for(size_t dep = 0; dep < CORR_DEP; dep++) {
		for(size_t i = 0; i < CORR_TC; i++) {
			Poly p = GenRandomPoly(dep);
			printf("    ans[%lu][%lu] = ", dep, i);
			PolyPrint(&p);
			printf(";\n");
			PolyDestroy(&p);
		}	
	}
	printf("}\n");
}

#include "stress_output.h"

void StressCorrect() {
	ResetRng();

	/*
	GenOutput();
	return;
	*/

	SetOutput();

	for(size_t dep = 0; dep < CORR_DEP; dep++) {
		for(size_t i = 0; i < CORR_TC; i++) {
			Poly p = GenRandomPoly(dep);
			assert(PolyIsEq(&p, &ans[dep][i]));
			PolyDestroy(&p);
			PolyDestroy(&ans[dep][i]);
		}
	}
}

void StressPerformance() {
	ResetRng();

	for(size_t i = 0; i < PERF_TC; i++) {
		Poly p = GenRandomPoly(PERF_DEP);
		PolyDestroy(&p);
	}
}
