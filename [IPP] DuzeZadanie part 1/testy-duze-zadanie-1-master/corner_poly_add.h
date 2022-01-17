#ifndef _CORNER_POLY_ADD_H
#define _CORNER_POLY_ADD_H

#include <assert.h>
#include <stdio.h>

#include "helper_functions_from_example.h"

void corner_poly_add() {
  // p = (((1 + x_3) + x_2) + x_1)
  // q = (((-x_3) - x_2) - x_1)
  // p + q = 1
  assert(TestAdd(P(P(P(P(C(1), 0, C(1), 1), 0, C(1), 1), 0, C(1), 1), 0),
                 P(P(P(P(C(-1), 1), 0, C(-1), 1), 0, C(-1), 1), 0), C(1)));

  // p = 1
  // q = (( ( (x_4) + x_3) + x_2) + x_1)
  // p + q = (( ( (1 + x_4) + x_3 ) + x_2) + x_1)
  assert(TestAdd(
      C(1), P(P(P(P(P(C(1), 1), 0, C(1), 1), 0, C(1), 1), 0, C(1), 1), 0),
      P(P(P(P(P(C(1), 0, C(1), 1), 0, C(1), 1), 0, C(1), 1), 0, C(1), 1), 0)));

  // p = (1 + 2y + (1 + z)y^2 ) + xy^2
  // q = -p
  // p + q = 0
  assert(TestAdd(
      P(P(C(1), 0, C(2), 1, P(C(1), 0, C(1), 1), 2), 0, P(C(1), 2), 1),
      P(P(C(-1), 0, C(-2), 1, P(C(-1), 0, C(-1), 1), 2), 0, P(C(-1), 2), 1),
      C(0)));

  assert(TestAdd(C(0), C(0), C(0)));

  // p = ((3 + z)) + (((-1)z)y)x
  // q = (5 + y) + (((1)z)y)x
  // p + q = ((8 + z) + y)
  assert(TestAdd(P(P(P(C(3), 0, C(1), 1), 0), 0, P(P(C(-1), 1), 1), 1),
                 P(P(C(5), 0, C(1), 1), 0, P(P(C(1), 1), 1), 1),
                 P(P(P(C(8), 0, C(1), 1), 0, C(1), 1), 0)));
}

#endif
