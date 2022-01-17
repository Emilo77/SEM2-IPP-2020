#include "test.h"
#include "example.h"
#include "cannot_change_arguments.h"
#include "stress.h"
#include "corner_cases.h"
#include "poly_at_complexity.h"

void Testuj() {
	RUN_TEST(Example);
	RUN_TEST(CannotChangeArguments);
	RUN_TEST(CornerCases);
	RUN_TEST(PolyAtComplexity);
	// nie dawać testów pod, bo stress się trudno debuguje
	RUN_TEST(StressCorrect);
	RUN_TEST(StressPerformance);
}
