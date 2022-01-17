/** @file
  Interfejs klasy stosu do obsługi kalkulatora
  @author Kamil Bugala <kb417522@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H
#include <stdlib.h>

/**
 * To jest struktura stosu.
 */
typedef struct Stack {
    Poly* arr; ///< Dynamicznie alokowana tablica przechowująca elementy stosu.
    size_t size; ///< Rozmiar tablicy arr.
    size_t top; ///< Ilość zajętego miejsca w tablicy arr.
} Stack;

/**
 * Funkcja inicjująca stos
 * @param[in] *s : wskaźnik na stos
 */
void StackInit(Stack* s);

/**
 * Funkcja usuwająca stos
 * @param[in] *s : wskaźnik na stos
 */
void StackDestroy(Stack* s);
/**
 * Funkcja sprawdzająca rozmiar stosu
 * @param[in] *s : wskaźnik na stos
 */
void CheckStackSize(Stack* s);

/**
 * Funkcja sprawdzająca, czy stos jest pusty
 * @param[in] *s : wskaźnik na stos
 */
bool IsStackEmpty(Stack* s);

/**
 * Funkcja sprawdzająca, czy stos jest prawie pusty (czy ma co najwyżej 1
 * element)
 * @param[in] s : wskaźnik na stos
 */
bool IsStackBarelyEmpty(Stack* s);

/**
 * Funkcja sprawdzająca, czy stos zawiera co najmniej k elementów.
 * @param[in] s : wskaźnik na stos
 * @param[in] k : minimalna ilość elementów
 * @return czy stos zawiera co najmniej k elementów
 */
bool TooFewStackElements(Stack* s, size_t k);

/**
 * Funkcja dodająca wielomian na wierzch stosu
 * @param[in] *s : wskaźnik na stos
 * @param[in] *p : wskaźnik na wielomian
 */
void StackPush(Stack *s, Poly* p);

/**
 * Funkcja ustawia wskaźnik p na wielomian z wierzchołka stosu. Usuwa
 * wielomian z wierzchu stosu.
 * @param[in] *s : wskaźnik na stos
 * @param[in] *p : wskaźnik na wielomian
 */
void StackPop(Stack *s, Poly* p);

/**
 * Funkcja zwracająca wskaźnik na wielomian z wierzchołka stosu.
 * @param[in] *s : wskaźnik na stos
 * @return wskaźnik na wielomian z wierzchołka stosu.
 */
Poly* StackTop(Stack *s);

#endif //POLYNOMIALS_STACK_H
