#ifndef _ADD_MONOS_TESTS
#define _ADD_MONOS_TESTS

#include <assert.h>
#include <stdbool.h>

#include "../poly.h"
#include "helper_functions_from_example.h"

void corner_add_monos() {
  {
    // odwrotna kolejność monos w tablicy
    Mono m[] = {M(C(1), 4), M(C(2), 3), M(C(3), 2), M(C(4), 1), M(C(5), 0)};
    assert(TestAddMonos(5, m, P(C(5), 0, C(4), 1, C(3), 2, C(2), 3, C(1), 4)));
  }
  {
    // dla każdego wykładnika po 3 jednomiany, wszystkie się zerują (pomieszana
    // kolejność na wejściu)
    Mono m[] = {M(C(-1), 1), M(C(-1), 0), M(C(2), 0), M(C(1), 3), M(C(-1), 0),
                M(C(-2), 3), M(C(-1), 1), M(C(2), 1), M(C(1), 3)};
    assert(TestAddMonos(9, m, C(0)));
  }
  {
    // Czy głęboki jednomian się wyzeruje
    // (1 - 2y + y^2) + x^3(y ^5 + y^7(1 + z^2 ))
    Poly p = P(P(C(1), 0, C(-2), 1, C(1), 2), 0,
               P(C(1), 5, P(C(1), 0, C(1), 2), 6), 3);
    Poly neg_p = PolyNeg(&p);
    Poly p_clone = PolyClone(&p), neg_p_clone = PolyClone(&neg_p);

    Mono m[] = {M(p, 0), M(neg_p, 0), M(neg_p_clone, 0), M(p_clone, 0)};
    assert(TestAddMonos(4, m, C(0)));
  }

  {
    // pusta tablica monos
    Mono m[] = {};
    assert(TestAddMonos(0, m, C(0)));
  }
}

#endif
