/** @file
  Interfejs klasy obsługiwania instrukcji kalkulatora do wielomianów rzadkich
  wielu zmiennych
  @author Kamil Bugala <kb417522@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_ENGINE_H
#define POLYNOMIALS_ENGINE_H

/**
 * Funkcja obsługująca instrukcje kalkulatora do wielomianów rzadkich wielu
  zmiennych
 * @param[in] c : ciąg charów @f$arg1@f$
 * @param[in] s : stos @f$arg2@f$
 * @param[in] line_number : nr linii @f$arg3@f$
 * @param[in] line_length : długość linii @f$arg4@f$
 */
void Engine(char *c, Stack *s, size_t line_number, int line_length);

#endif //POLYNOMIALS_ENGINE_H
