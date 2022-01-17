#include "helper_functions_from_example.h"

void CannotChangeArguments() {
	Poly a = P(C(1), 0, C(-1), 1,           C(2), 3);
	Poly b = P(C(1), 0, C(1),  1, C(-1), 2, C(4), 3);
	Poly copy_a = PolyClone(&a);
	Poly copy_b = PolyClone(&b);

	(void) PolyIsZero(&a);
	assert(PolyIsEq(&a, &copy_a));

	(void) PolyIsCoeff(&a);
	assert(PolyIsEq(&a, &copy_a));

	Poly c = PolyAdd(&a, &b);
	PolyDestroy(&c);
	assert(PolyIsEq(&a, &copy_a) && PolyIsEq(&b, &copy_b));

	c = PolyMul(&a, &b);
	PolyDestroy(&c);
	assert(PolyIsEq(&a, &copy_a) && PolyIsEq(&b, &copy_b));

	c = PolyNeg(&a);
	PolyDestroy(&c);
	assert(PolyIsEq(&a, &copy_a));

	c = PolySub(&a, &b);
	PolyDestroy(&c);
	assert(PolyIsEq(&a, &copy_a) && PolyIsEq(&b, &copy_b));

	(void) PolyDegBy(&a, 1);
	assert(PolyIsEq(&a, &copy_a));

	(void) PolyDeg(&a);
	assert(PolyIsEq(&a, &copy_a));

	c = PolyAt(&a, 1);
	PolyDestroy(&c);
	assert(PolyIsEq(&a, &copy_a));

	PolyDestroy(&a);
	PolyDestroy(&b);
	PolyDestroy(&copy_a);
	PolyDestroy(&copy_b);
}

