#include "poly.h"
#include "stack.h"


void StackInit(Stack* s) {
    s->arr = malloc(64 * sizeof(Poly));
    CHECK_PTR(s->arr);
    s->size = 64;
    s->top = 0;
}

void StackDestroy(Stack* s) {
    for(size_t i = 0; i < s->top; i ++) {
        PolyDestroy(&s->arr[i]);
    }
    free(s->arr);
}

void CheckStackSize(Stack* s) {
    if(s->top == s->size && 2*s->size > s->size) {
        s->arr = realloc(s->arr,2 * s->size * sizeof(Poly));
        CHECK_PTR(s->arr);
        s->size *=2;
    }
}

bool IsStackEmpty(Stack* s) {
    return s->top == 0;
}

bool IsStackBarelyEmpty(Stack* s) {
    return s->top == 0 || s->top == 1;
}

bool TooFewStackElements(Stack* s, size_t k) {
    return s->top < k;
}

void StackPush(Stack *s, Poly* p) {
    CheckStackSize(s);
    s->top++;
    s->arr[s->top-1] = *p;
}

void StackPop(Stack *s, Poly* p) {
    *p = s->arr[s->top-1];
    s->top--;
}

Poly* StackTop(Stack *s) {
    Poly* p = &s->arr[s->top-1];
    return p;
}