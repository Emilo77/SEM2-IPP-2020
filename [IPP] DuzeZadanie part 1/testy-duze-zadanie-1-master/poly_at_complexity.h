#include "../poly.h"
#include "helper_functions_from_example.h"

void PolyAtComplexity() {
  Poly p = P(C(1), (int)2e9);
  for (int i = 0; i < 10000; i++) {
    Poly q = PolyAt(&p, i + 1);
    PolyDestroy(&q);
  }
  PolyDestroy(&p);
}
