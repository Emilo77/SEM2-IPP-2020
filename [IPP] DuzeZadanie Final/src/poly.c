/** @file
  Implementacje klasy wielomianów rzadkich wielu zmiennych
  Założenia do implementacji:
  Elementy wielomianu (jednomiany) są posortowane rosnąco według wykładnika
  Zakładamy, że mogą być zarówno wielomiany będące współczynnikami, jak i
  mogą powstać wielomiany zawierający jednomiany z wykładnikami 0.
  Za wielomian tożsamościowo równy zero uznajemy wielomian
  zawierający jeden jednomian, którego współczynnik jest równy 0.
  @author KamilBugala <kb417522@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 02.05.2021
*/

#include "poly.h"
#include "calc.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/**
 * Funkcja porównująca dwa jednomiany na podstawie ich wykładników.
 * Używana do sortowania.
 * @param[in] arg1 : jednomian @f$arg1@f$
 * @param[in] arg2 : jednomian @f$arg2@f$
 * @return -1, 0 lub 1
 */
static int ComparingFunction(const void *arg1, const void *arg2) {
    Mono a = *(Mono *) arg1;
    Mono b = *(Mono *) arg2;
    if (a.exp < b.exp)
        return -1;
    if (a.exp > b.exp)
        return 1;
    return 0;
}

/**
 * Funkcja Sortująca tablicę jednomianów
 * @param[in] arr : tablica jednomianów
 * @param[in] arr_size : rozmiar tablicy arr
 */
static void PolySortArray(Mono *arr, size_t arr_size) {
    if (arr == NULL)
        return;

    qsort(arr, arr_size, sizeof(Mono), ComparingFunction);
}

/**
 * Funkcja usuwająca jednomiany tożsamościowe z PolyZero().
 * @param[in] p : wielomian do modyfikacji
 */
static void PolyDeleteZeros(Poly *p) {
    for(size_t k = 0; k < 2; k++) {
        if(!PolyIsCoeff(p)) {
            for (size_t i = 0; i < p->size; i++) {
                PolyDeleteZeros(&p->arr[i].p);
                if (PolyIsZero(&p->arr[i].p)) {
                    PolyDestroy(&p->arr[i].p);
                    (p->size)--;
                    for (size_t j = i; j < p->size; j++) {
                        p->arr[j] = p->arr[j + 1];
                    }
                }
            }
            p->arr = realloc(p->arr, p->size * sizeof(Mono));
        }
    }
}

/**
 * PolyMultiplyVoid modyfikuje otrzymany wielomian, zwraca
 * wielomian przemnożony przez stałą.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] multiply : współczynnik
 */
static void PolyMultiplyVoid(Poly *p, poly_coeff_t multiply) {
    if(multiply == 1) {
        return;
    }
    if (PolyIsCoeff(p)) {
        p->coeff *= multiply;
    } else {
        for (size_t i = 0; i < p->size; i++) {
            PolyMultiplyVoid(&p->arr[i].p, multiply);
        }
    }
    if(multiply != -1) {
        PolyDeleteZeros(p);
    }
}

void PolyDestroy(Poly *p) {
    if (PolyIsCoeff(p))
        return;

    for (size_t i = 0; i < p->size; i++) {
        PolyDestroy(&p->arr[i].p);
    }
    free(p->arr);
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) {
        return PolyFromCoeff(p->coeff);
    }
    Poly copy;
    copy.size = p->size;
    copy.arr = malloc(sizeof(Mono) * copy.size);
    CHECK_PTR(&copy.arr);
    for (size_t i = 0; i < copy.size; i++) {
        copy.arr[i] = MonoClone(&p->arr[i]);
    }
    return copy;
}

/**
 * Funkcja wspomagająa do PolyToCoeff
 * Sprawdza, czy wielomian da się uprościć.
 * Pobiera informację o współczynniku wielomianu.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] result : jeśli 0, wielomian nie może być współczynnikiem, 1 wpp.
 * @param[in] coeff : wpsółczynnik uproszczonego wielomianu p
 */
static void PolyToCoeffHelper(Poly *p, int *result, poly_coeff_t *coeff) {
    if (PolyIsCoeff(p)) {
        *coeff = p->coeff;
        return;
    }
    if (p->size == 1 && p->arr[0].exp == 0) {
        PolyToCoeffHelper(&p->arr[0].p, result, coeff);
        return;
    }
    *result = 0;
}

Poly PolyToCoeff(Poly *p) {
    int conv_possible = 1;
    poly_coeff_t coeff = 1;
    PolyToCoeffHelper(p, &conv_possible, &coeff);
    if (conv_possible) {
        return PolyFromCoeff(coeff);
    }
    return *p;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsZero(p) && PolyIsZero(q))
        return PolyZero();
    if (PolyIsZero(p)) {
        return PolyClone(q);
    }
    if (PolyIsZero(q)) {
        return PolyClone(p);
    }
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff + q->coeff);
    }
    if (PolyIsCoeff(p)) {
        Mono mono = MonoFromPoly(p, 0);
        Poly poly = {.size = 1, .arr = &mono};
        return PolyAdd(&poly, q);
    }
    if (PolyIsCoeff(q)) {
        Mono mono = MonoFromPoly(q, 0);
        Poly poly = {.size = 1, .arr = &mono};
        return PolyAdd(&poly, p);
    }
    Mono *copy = malloc((p->size + q->size) * sizeof(Mono));
    CHECK_PTR(copy);

    size_t p_index = 0;
    size_t q_index = 0;
    size_t index = 0;
    size_t copy_size = p->size + q->size;
    while (p_index < p->size && q_index < q->size) {
        Mono MonoP = p->arr[p_index];
        Mono MonoQ = q->arr[q_index];

        if (MonoP.exp == MonoQ.exp) {
            copy[index] = (Mono) {.exp = MonoP.exp, .p = PolyAdd(&MonoP.p,
                                                                 &MonoQ.p)};
            p_index++;
            q_index++;
            copy_size--;
        } else if (MonoP.exp < MonoQ.exp) {
            copy[index].exp = MonoP.exp;
            copy[index].p = PolyClone(&MonoP.p);
            p_index++;
        } else {
            copy[index].exp = MonoQ.exp;
            copy[index].p = PolyClone(&MonoQ.p);
            q_index++;
        }
        index++;
    }
    if (p_index != p->size) {
        for (size_t i = p_index; i < p->size; i++) {
            copy[index].exp = p->arr[i].exp;
            copy[index].p = PolyClone(&p->arr[i].p);
            index++;
        }
    }
    if (q_index != q->size) {
        for (size_t i = q_index; i < q->size; i++) {
            copy[index].exp = q->arr[i].exp;
            copy[index].p = PolyClone(&q->arr[i].p);
            index++;
        }
    }
    Poly result = PolyAddMonos(copy_size, copy);
    free(copy);
    return result;
}


Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (count == 0 || monos == NULL) {
        return PolyZero();
    }
    Poly copy;
    copy.size = count;
    copy.arr = malloc(count * sizeof(Mono));
    CHECK_PTR(&copy.arr);
    for (size_t i = 0; i < count; i++) {
        copy.arr[i] = monos[i];
    }
    PolySortArray(copy.arr, copy.size);
    size_t index = 0;
    size_t arr_index = 0;
    Mono *arr_result = malloc(count * sizeof(Mono));
    CHECK_PTR(arr_result);

    while (index < copy.size - 1) {
        if (MonoGetExp(&copy.arr[index]) == MonoGetExp(&copy.arr[index + 1])) {
            Poly trap = PolyAdd(&copy.arr[index].p, &copy.arr[index + 1].p);
            PolyDestroy(&copy.arr[index].p);
            PolyDestroy(&copy.arr[index + 1].p);
            copy.arr[index + 1].p = trap;
        } else {
            if (PolyIsZero(&copy.arr[index].p)) {
                PolyDestroy(&copy.arr[index].p);
            } else {
                arr_result[arr_index] = copy.arr[index];
                arr_index++;
            }
        }
        index++;
    }

    if (!PolyIsZero(&copy.arr[index].p)) {
        arr_result[arr_index] = copy.arr[index];
        arr_index++;
    } else {
        PolyDestroy(&copy.arr[index].p);
    }
    free(copy.arr);
    if (arr_index == 0) {
        free(arr_result);
        return PolyZero();
    } else {
        Poly result = (Poly) {.size = arr_index, .arr = arr_result};
        return result;
    }
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsZero(p) || PolyIsZero(q)) {
        return PolyZero();
    }
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff * q->coeff);
    }
    if (PolyIsCoeff(p)) {
        Poly r = PolyClone(q);
        PolyMultiplyVoid(&r, p->coeff);
        if (PolyIsZero(&r)) {
            PolyDestroy(&r);
            return PolyZero();
        }
        PolyDeleteZeros(&r);
        return r;
    }
    if (PolyIsCoeff(q)) {
        Poly r = PolyClone(p);
        PolyMultiplyVoid(&r, q->coeff);
        if (PolyIsZero(&r)) {
            PolyDestroy(&r);
            return PolyZero();
        }
        PolyDeleteZeros(&r);
        return r;
    }
    Mono *arr = malloc(p->size * q->size * sizeof(Mono));
    size_t index = 0;
    for (size_t i = 0; i < p->size; i++) {
        for (size_t j = 0; j < q->size; j++) {
            arr[index].exp = MonoGetExp(&p->arr[i]) + MonoGetExp(&q->arr[j]);
            arr[index].p = PolyMul(&p->arr[i].p, &q->arr[j].p);
            index++;
        }
    }
    Poly s = PolyAddMonos(p->size * q->size, arr);
    free(arr);
    PolyDeleteZeros(&s);
    return s;
}

Poly PolyNeg(const Poly *p) {
    Poly copy = PolyClone(p);
    PolyMultiplyVoid(&copy, -1);
    return copy;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly r = PolyNeg(q);
    Poly res = PolyAdd(p, &r);
    PolyDestroy(&r);
    return res;
}

/**
 * Funkcja wspomagająca funkcję PolyDegBy.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] var_idx : szukany indeks przy x wielomianu
 * @param[in] ind : aktualny indeks przy x wielomianu,
 * oznacza również stopień zagłębienia rekurencji
 * @param[in] deg_max : najwyższy stopień jednomianu przy indeksie ind
 */
static void PolyDegByHelper(const Poly *p, size_t var_idx, size_t *ind,
                            poly_exp_t *deg_max) {
    if (PolyIsCoeff(p) || *ind > var_idx) {
        (*ind)--;
        return;
    }
    for (size_t i = 0; i < p->size; i++) {
        if (*ind == var_idx && p->arr[i].exp > *deg_max) {
            *deg_max = p->arr[i].exp;
        }
        (*ind)++;
        PolyDegByHelper(&p->arr[i].p, var_idx, ind, deg_max);
    }
    (*ind)--;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsZero(p)) {
        return -1;
    }
    if (PolyIsCoeff(p)) {
        return 0;
    }
    poly_exp_t deg_max = 0;
    size_t ind = 0;
    PolyDegByHelper(p, var_idx, &ind, &deg_max);
    return deg_max;
}

/**
 * Funkcja wspomagająca funkcję PolyDeg.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] deg_max : najwyższy stopień wielomianu przy indeksie ind
 * @param[in] deg : aktualny stopień wielomianu
 */
static void PolyDegHelper(const Poly *p, poly_exp_t *deg_max, poly_exp_t *deg) {
    if (PolyIsCoeff(p)) {
        return;
    }
    for (size_t i = 0; i < p->size; i++) {
        *deg += p->arr[i].exp;
        if (*deg > *deg_max)
            *deg_max = *deg;

        PolyDegHelper(&p->arr[i].p, deg_max, deg);
        *deg -= p->arr[i].exp;
    }
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p)) {
        return -1;
    }
    poly_exp_t deg_max = 0;
    poly_exp_t deg = 0;
    PolyDegHelper(p, &deg_max, &deg);
    return deg_max;
}

/**
 * Funkcja zwraca sprawdzająca równość jednomianów.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return 1 jeżeli jednomiany są różne, 0 wpp.
 */
static bool PolyIsEqHelper(const Mono *m, const Mono *n) {
    if (m->exp != n->exp)
        return false;
    return PolyIsEq(&m->p, &n->p);
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    Poly copy_p = PolyToCoeff((Poly *) p);
    Poly copy_q = PolyToCoeff((Poly *) q);

    if (PolyIsCoeff(&copy_p) && PolyIsCoeff(&copy_q)) {
        if (copy_p.coeff != copy_q.coeff) {
            return false;
        }
        return true;
    }
    if (PolyIsCoeff(&copy_p)) {
        return false;
    }
    if (PolyIsCoeff(&copy_q)) {
        return false;
    }
    if (copy_p.size != copy_q.size) {
        return false;
    }
    for (size_t i = 0; i < p->size; i++) {
        if (!PolyIsEqHelper(&copy_p.arr[i], &copy_q.arr[i]))
            return false;
    }
    return true;
}

/**
 * Funkcja zwraca potęgę liczby.
 * @param[in] base : podstawa potęgi
 * @param[in] power : wykładnik potęgi
 * @return base do potęgi power
 */
static poly_coeff_t Power(poly_coeff_t base, poly_coeff_t power) {
    if (power == 0)
        return 1;
    else if (power % 2 == 1) {
        poly_coeff_t value = Power(base, power - 1);
        return base * value;
    } else {
        poly_coeff_t value = Power(base, power / 2);
        return value * value;
    }
}

Poly PolyAt(const Poly *p, poly_coeff_t value) {
    if (PolyIsCoeff(p)) {
        return PolyFromCoeff(p->coeff);
    } else {
        Poly CopyP = PolyClone(p);
        for (size_t i = 0; i < p->size; i++) {
            poly_coeff_t power = CopyP.arr[i].exp;
            PolyMultiplyVoid(&CopyP.arr[i].p, Power(value, power));
        }
        Poly r = PolyZero();
        for (size_t i = 0; i < p->size; i++) {
            Poly OldPoly = r;
            r = PolyAdd(&r, &CopyP.arr[i].p);
            PolyDestroy(&OldPoly);
        }
        PolyDestroy(&CopyP);
        return r;
    }
}

void MonoPrint(Mono *q) {
    printf("(");
    PolyPrint(&q->p);
    printf(",%d", q->exp);
    printf(")");
}

void PolyPrint(Poly *p) {
    Poly copy = PolyToCoeff(p);
    if (PolyIsZero(&copy)) {
        printf("0");
    } else if (PolyIsCoeff(&copy)) {
        printf("%ld", copy.coeff);
    } else {
        for (size_t i = 0; i < copy.size; i++) {
            if (i != 0) {
                printf("+");
            }
            MonoPrint(&copy.arr[i]);
        }
    }
}

/**
 * Funkcja modyfikująca tablicę znaków.
 * Zmienia każdy napotkany znak nowej linii na znak '\0'.
 * @param[in] c : tablica znaków
 */
static void ChangeStringNullBytes(char *c) {
    int left_brackets = 0;
    int right_brackets = 0;
    size_t ind = 0;
    while (c[ind] != '\n' && c[ind] != EOF && c[ind] != '\0') {
        if (c[ind] == '(')
            left_brackets++;
        if (c[ind] == ')')
            right_brackets++;
        if (c[ind] == '+' && left_brackets == right_brackets) {
            c[ind] = '\0';
        }
        ind++;
    }
    c[ind] = '\0';
}

/**
 * Funkcja pomocnicza, zliczająca ilość jednomianów potrzebnych
 * do utworzenia wielomianu.
 * Rozpoznaje nowy jednomian w tablicy poprzez rozpopznawanie znaków '\0'.
 * @param[in] c : tablica znaków
 * @return: ilość jednomianów
 */
static size_t CountMonos(char *c) {

    size_t ind = 1;
    size_t count = 0;
    while (c[ind] != '\0' || c[ind + 1] != '\0') {
        if (c[ind - 1] == ')' && c[ind] == '\0' && c[ind + 1] == '(') {
            count++;
        }
        ind++;
    }
    return count + 1;
}

Mono MonoFromString(char *c, int *move, int *status) {
    Mono m;
    int ind = 1;
    int left_brackets = 0;
    int right_brackets = 0;
    char *buffer;
    while (c[ind] != ')' || c[ind + 1] != '\0') {
        if (c[ind] == '(')
            left_brackets++;
        if (c[ind] == ')')
            right_brackets++;

        if (c[ind] == ',' && left_brackets == right_brackets) {
            c[ind] = '\0';
            buffer = c + ind + 1;
            long exp = strtol(buffer, &buffer, 10);

            c[ind + 1] = '\0';
            if (exp > INT_MAX) {
                *status = WRONG_POLY;
            }
            m.exp = (poly_exp_t) exp;
        }
        ind++;
    }
    m.p = PolyFromString(c + 1, move, status);
    *move = ind + 1;
    return m;
}

Poly PolyFromString(char *c, int *move, int *status) {

    if ((c[0] >= ASCII_0 && c[0] <= ASCII_9) || c[0] == '-') {
        char *pointer = c;
        poly_coeff_t coeff;
        errno = 0;
        coeff = strtol(pointer, &pointer, 10);
        if (errno == ERANGE) {
            *status = WRONG_POLY;
        }
        return PolyFromCoeff(coeff);
    } else {
        ChangeStringNullBytes(c);
        size_t arr_size = CountMonos(c);
        Mono *mono_arr = malloc(arr_size * sizeof(Mono));
        CHECK_PTR(mono_arr);
        Mono m = MonoFromString(c, move, status);
        mono_arr[0] = m;
        size_t arr_ind = 1;
        size_t ind = *move;
        while (c[ind] != '\0' || c[ind + 1] != '\0') {
            if (c[ind] == '\0') {
                mono_arr[arr_ind] = MonoFromString(c + ind + 1, move, status);
                ind += *move;
                arr_ind++;
            } else ind++;
        }
        Poly p = PolyAddMonos(arr_size, mono_arr);
        free(mono_arr);
        return p;
    }
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    Poly p = PolyAddMonos(count, monos);
    free(monos);
    return p;
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    if (count == 0 || monos == NULL) {
        return PolyZero();
    }
    Mono *copy = malloc(count * sizeof(Mono));
    CHECK_PTR(&copy);
    for (size_t i = 0; i < count; i++) {
        copy[i] = MonoClone(&monos[i]);
    }
    return PolyOwnMonos(count, copy);
}

/**
 * Tworzy wielomian będący wynikiem potęgowania wielomianu.
 * @param[in] p : wielomian
 * @param[in] x : wykładnik potęgi
 * @return stworzony wielomian
 */
static Poly PolyExp(const Poly *p, size_t x) {
    if (x == 0)
        return PolyFromCoeff(1);
    if (x % 2 == 1) {
        Poly q = PolyExp(p, x - 1);
        Poly result = PolyMul(p, &q);
        PolyDestroy(&q);
        return result;
    }
    Poly q = PolyExp(p, x / 2);
    Poly result = PolyMul(&q, &q);
    PolyDestroy(&q);
    return result;
}
/**
 * Funkcja pomocnicza do funkcji składania wielomianów.
 * @param[in] p : wielomian
 * @param[in] k : rozmiar tablicy wielomianów
 * @param[in] q : tablica wielomianów
 * @param[in] level : indeks zmiennej, stopień zagłębienia rekurencyjnie
 * @return : utworzony wielomian
 */
static Poly PolyComposeHelper(const Poly *p, size_t k, const Poly q[], int
level) {
    if (PolyIsCoeff(p)) { return PolyClone(p); }

    Poly result = PolyZero();
    Poly exp_result;
    Poly add_result;
    Poly mul_result;

    for (size_t i = 0; i < p->size; i++) {

        Poly copy = PolyComposeHelper(&p->arr[i].p, k, q, level + 1);
        if (level >= (int) k) {
            if (p->arr[i].exp == 0) {
                exp_result = PolyFromCoeff(1);
            } else exp_result = PolyZero();

        } else {
            exp_result = PolyExp(&q[level], p->arr[i].exp);
        }
        mul_result = PolyMul(&copy, &exp_result);

        PolyDestroy(&copy);
        PolyDestroy(&exp_result);

        add_result = PolyAdd(&result, &mul_result);

        PolyDestroy(&result);
        PolyDestroy(&mul_result);

        result = add_result;
    }
    return result;
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    return PolyComposeHelper(p, k, q, 0);

}