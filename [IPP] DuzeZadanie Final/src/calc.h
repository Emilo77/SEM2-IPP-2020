/** @file
  Interfejs klasy kalkulatora do wielomianów rzadkich wielu zmiennych
  @author Kamil Bugala <kb417522@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_CALC_H
#define POLYNOMIALS_CALC_H

#include <stdlib.h>
#include "errno.h"
#include "string.h"
#include "ctype.h"
#include "stack.h"
#include "limits.h"

///Makro do 0 w kodzie ASCII
#define ASCII_0 48
///Makro do 9 w kodzie ASCII
#define ASCII_9 58
///Makro do dozolonych znaków w wielomianie
#define LEGAL_CHAR "()-+,0123456789"
///Definicja do obsługi instrukcji ZERO
#define ZERO 1
///Definicja do obsługi instrukcji IS_COEFF
#define IS_COEFF 2
///Definicja do obsługi instrukcji IS_ZER0
#define IS_ZERO 3
///Definicja do obsługi instrukcji CLONE
#define CLONE 4
///Definicja do obsługi instrukcji ADD
#define ADD 5
///Definicja do obsługi instrukcji MUL
#define MUL 6
///Definicja do obsługi instrukcji NEG
#define NEG 7
///Definicja do obsługi instrukcji SUB
#define SUB 8
///Definicja do obsługi instrukcji IS_EQ
#define IS_EQ 9
///Definicja do obsługi instrukcji DEG
#define DEG 10
///Definicja do obsługi instrukcji PRINT
#define PRINT 11
///Definicja do obsługi instrukcji POP
#define POP 12
///Definicja do obsługi instrukcji AT
#define AT 13
///Definicja do obsługi instrukcji DEG_BY
#define DEG_BY 14
///Definicja do obsługi instrukcji COMPOSE
#define COMPOSE 15
///Definicja do obsługi błędu WRONG_COMMAND
#define WRONG_COMMAND 16
///Definicja do obsługi błędu WRONG_DEG_BY
#define WRONG_DEG_BY 17
///Definicja do obsługi błędu WRONG_AT
#define WRONG_AT 18
///Definicja do obsługi instrukcji POLY_GOOD
#define POLY_GOOD 19
///Definicja do obsługi instrukcji WRONG_POLY
#define WRONG_POLY 20
///Definicja do obsługi błędu WRONG_COMPOSE
#define WRONG_COMPOSE 21
///Definicja do obsługi instrukcji IGNORE
#define IGNORE 22
///Definicja do informacji o błędzie STACK UNDERFLOW
#define UNDERFLOW "ERROR %ld STACK UNDERFLOW\n"


/**
 * Funkcja wstawiająca na wierzchołek stosu wielomian tożsamościowo równy zeru.
 * @param[in] s : stos @f$arg1@f$
 */
void Zero(Stack *s);

/**
 * Funkcja sprawdzająca, czy wielomian na wierzchu stosu jest współczynnikiem.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 * @return Czy wielomian na wierzchu stosu jest współczynnikiem?
 */
void IsCoeff(Stack *s, size_t line_number);

/**
 * Funkcja sprawdzająca, czy wielomian na wierzchu stosu jest tożsamościowo
 * równy zeru.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 * @return Czy wielomian na wierzchu stosu jest równy zero?
 */
void IsZero(Stack *s, size_t line_number);

/**
 * Funkcja wstawiająca na stos kopię wielomianu z wierzchołka.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 */
void Clone(Stack *s, size_t line_number);

/**
 * Funkcja dodaje dwa wielomiany z wierzchołka stosu, usuwa je i wstawia na
 * wierzch stosu ich sumę.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 */
void Add(Stack *s, size_t line_number);

/**
 * Funkcja mnoży dwa wielomiany z wierzchołka stosu, usuwa je i wstawia na
 * wierzch stosu ich iloczyn.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 */
void Mul(Stack *s, size_t line_number);

/**
 * Funkcja negująca wielomian z wierzchu stosu.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 */
void Neg(Stack *s, size_t line_number);

/**
 * Funkcja odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem,
 * usuwa je i wstawia na wierzch stosu ich różnicę.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 */
void Sub(Stack *s, size_t line_number);

/**
 * Funkcja sprawdzająca czy dwa wielomiany z wierzchołku stosu są równe.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 * @return wypisuje 1 jeśli równe, 0 jeśli różnią się
 */
void IsEq(Stack *s, size_t line_number);

/**
 * Funkcja wypisująca stopień wielomianu (-1 dla wielomianu tożsamościowo
 * równego zeru).
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 * @return wypisuje stopień wielomianu
 */
void Deg(Stack *s, size_t line_number);

/**
 * Funkcja wypisująca stopień wielomianu ze względu na zmienną o numerze idx
 * (-1 dla wielomianów tożsamościowo równego zeru)
 * @param[in] s : stos @f$arg1@f$
 * @param[in] idx : numer idx @f$arg2@f$
 * @param[in] line_number : nr wiersza @f$arg3@f$
 * @return wypisuje stopień wielomianu ze względu na zmienną o numerze idx
 */
void DegBy(Stack *s, poly_exp_t idx, size_t line_number);

/**
 * Funkcja wylicza wartość wielomianu w punkcie x, usuwa wielomian z
 * wierzchołka i ustawia na stos wynik operacji.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] x : argument x @f$arg2@f$
 * @param[in] line_number : nr wiersza @f$arg3@f$
 */
void At(Stack *s, poly_coeff_t x, size_t line_number);

/**
 * Funkcja wwypisująca na standardowe wyjście wielomian z wierzchołka stosu
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 * @return wypisuje na standardowe wyjście wielomian
 */
void Print(Stack *s, size_t line_number);

/**
 * Funkcja usuwa wielomian z wierzchołka stosu
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_number : nr wiersza @f$arg2@f$
 */
void Pop(Stack *s, size_t line_number);
/**
 * Funkcja wykonuje złożenie wielomianu z innymi wielomianami ze stosu.
 * @param[in] s : stos @f$arg1@f$
 * @param[in] value : ilość wielomianów @f$arg2@f$
 *  * @param[in] line_number : numer wiersza @f$arg3@f$
 * @return wypisuje stopień wielomianu
 */
void Compose(Stack *s, unsigned int value, size_t line_number);

/**
 * Główna funkcja obsługująca działanie kalkulatora.
 * @param[in] s : stos @f$arg1@f$
 */
void MakeCalculations(Stack *s);

#endif //POLYNOMIALS_CALC_H
