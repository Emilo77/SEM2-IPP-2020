#ifndef _CORNER_VARIOUS_TESTS_H
#define _CORNER_VARIOUS_TESTS_H

#include <assert.h>
#include <stdbool.h>

#include "../poly.h"
#include "helper_functions_from_example.h"

// pozostałe testy, które nie pasują do innych kategorii
void corner_various_tests() {
  // tworzenie wielomianów w dziwny sposób
  assert(TestEq(P(P(P(P(P(C(0), 0), 0), 0), 0), 0), C(0), true));
  assert(TestEq(P(P(P(P(P(C(1), 0), 0), 0), 0), 0), C(1), true));
  assert(TestEq(P(P(P(P(P(C(1), 1), 0), 0), 0), 0), P(C(1), 1), false));
}

#endif
