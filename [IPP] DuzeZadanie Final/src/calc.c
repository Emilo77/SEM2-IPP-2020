/** @file
  Implementacje klasy kalkulatora do drugiej części dużego zadania
  Założenia do implementacji:
  @author KamilBugala <kb417522@mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 22.05.2021
*/

#define _GNU_SOURCE
#include "poly.h"
#include "calc.h"
#include "engine.h"


void Zero(Stack *s){
    Poly p = PolyZero();
    StackPush(s, &p);
}

void IsCoeff(Stack *s, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p;
        Poly q;
        StackPop(s, &p);
        q = PolyToCoeff(&p);
        if(PolyIsCoeff(&q))
            printf("1\n");
        else printf("0\n");
        StackPush(s, &p);
    }
}

void IsZero(Stack *s, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        if(PolyIsZero(StackTop(s)))
            printf("1\n");
        else printf("0\n");
    }
}

void Clone(Stack *s, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p = PolyClone(StackTop(s));
        StackPush(s, &p);
    }
}

void Add(Stack *s, size_t line_number) {
    if(IsStackBarelyEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p1;
        Poly p2;
        StackPop(s, &p1);
        StackPop(s, &p2);
        Poly sum = PolyAdd(&p1, &p2);
        PolyDestroy(&p1);
        PolyDestroy(&p2);
        StackPush(s, &sum);
    }
}

void Mul(Stack *s, size_t line_number) {
    if(IsStackBarelyEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p1;
        Poly p2;
        StackPop(s, &p1);
        StackPop(s, &p2);
        Poly mul = PolyMul(&p1, &p2);
        PolyDestroy(&p1);
        PolyDestroy(&p2);
        StackPush(s, &mul);
    }
}

void Neg(Stack *s, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p;
        StackPop(s, &p);
        Poly r = PolyNeg(&p);
        StackPush(s, &r);
        PolyDestroy(&p);
    }
}

void Sub(Stack *s, size_t line_number) {
    if(IsStackBarelyEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p1;
        Poly p2;
        StackPop(s, &p1);
        StackPop(s, &p2);
        Poly sub = PolySub(&p1, &p2);
        PolyDestroy(&p1);
        PolyDestroy(&p2);
        StackPush(s, &sub);
    }
}

void IsEq(Stack *s, size_t line_number) {
    if(IsStackBarelyEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly *p1;
        Poly *p2;
        Poly temp;
        p1 = StackTop(s);
        StackPop(s,&temp);
        p2 = StackTop(s);
        StackPush(s,&temp);
        if(PolyIsEq(p1, p2))
            printf("1\n");
        else printf("0\n");
    }
}

void Deg(Stack *s, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly *p;
        p = StackTop(s);
        printf("%d\n",PolyDeg(p));
    }
}

void DegBy(Stack *s, poly_exp_t idx, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly *p;
        p = StackTop(s);
        printf("%d\n",PolyDegBy(p, idx));
    }
}

void At(Stack *s, poly_coeff_t x, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p;
        StackPop(s, &p);
        Poly p_at = PolyAt(&p, x);
        StackPush(s, &p_at);
        PolyDestroy(&p);
    }
}

void Print(Stack *s, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly *p;
        p = StackTop(s);
        PolyPrint(p);
        printf("\n");
    }
}

void Pop(Stack *s, size_t line_number) {
    if(IsStackEmpty(s)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p;
        StackPop(s, &p);
        PolyDestroy(&p);
    }
}

void Compose(Stack *s, unsigned int value, size_t line_number) {
    if(value == UINT_MAX || TooFewStackElements(s, value + 1)) {
        fprintf(stderr, UNDERFLOW,line_number);
    }
    else {
        Poly p;
        StackPop(s, &p);
        Poly *p_arr = malloc(value * sizeof(Poly));
        CHECK_PTR(p_arr);
        for(size_t i = 0; i < value; i++) {
            Poly r;
            StackPop(s, &r);
            p_arr[value - 1 - i] = r;
        }
        Poly result;
        result = PolyCompose(&p, value, p_arr);
        StackPush(s, &result);
        for(size_t i = 0; i < value; i++) {
            PolyDestroy(&p_arr[i]);
        }
        free(p_arr);
        PolyDestroy(&p);
    }
}

void MakeCalculations(Stack *s) {
    size_t line_number = 1;
    char *buffer = NULL;
    size_t buffer_size;
    int line_length = 0;
    errno = 0;
    while((line_length = getline(&buffer, &buffer_size, stdin)) != -1) {
        if(errno == ENOMEM) {
            exit(1);
        }
        if(buffer[line_length - 1] != '\n') {
            buffer = realloc(buffer, (line_length + 2) * sizeof(char));
            CHECK_PTR(buffer);
            buffer[line_length + 1] = '\0';
            buffer[line_length] = '\n';
            line_length += 1;
        }
        Engine(buffer, s, line_number, line_length);
        line_number++;
    }
    free(buffer);
}
/**
 * Funkcja main.
 * @return wynik programu
 */
int main() {
    Stack s;
    StackInit(&s);
    MakeCalculations(&s);
    StackDestroy(&s);
}
