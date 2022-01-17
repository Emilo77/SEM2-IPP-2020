#ifndef _CORNER_CASES_H
#define _CORNER_CASES_H

#include "corner_add_monos.h"
#include "corner_poly_add.h"
#include "corner_various_tests.h"

void CornerCases() {
  corner_poly_add();
  corner_add_monos();
  corner_various_tests();
}

#endif
