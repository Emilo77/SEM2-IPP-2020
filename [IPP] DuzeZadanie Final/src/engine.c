/** @file
  Implementacje klasy obsługiwania instrukcji kalkulatora do wielomianów
  rzadkich
  @author KamilBugala <kb417522@mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 22.05.2021
*/

#define _GNU_SOURCE
#include "poly.h"
#include "calc.h"
#include "engine.h"

/**
 * Funkcja funkcja porównująca długość ciągu znaków
 * @param[in] s : stos @f$arg1@f$
 * @param[in] line_length : długość ciągu znaków @f$arg2@f$
 * @return : true jeżeli długości są równe, false wpp.
 */
static bool StrLengthCompare(char* s, int line_length) {
    int ind = 0;
    int size = 0;
    while(s[ind] != '\n') {
        size++;
        ind++;
    }
    return line_length == size+1;
}
/**
 * Funkcja sprawdzająca, czy instrukcja jest prawidłowa
 * @param[in] char_line : tablica znaków
 * @param[in] line_length : tablica znaków
 * @return dobra instrukcja: INSTR_GOOD, błąd przy instukcji at: WRONG_AT,
 * błąd przy instrukcji deg_by: WRONG_DEG_BY, zła instrukcja: WRONG_COMMAND
 */
static int InstrCheck(char *char_line, int line_length) {

    if(strcmp(char_line, "ZERO\n") == 0 && StrLengthCompare("ZERO\n",
                                                            line_length))
        return ZERO;
    if(strcmp(char_line, "IS_COEFF\n") == 0 && StrLengthCompare("IS_COEFF\n",
                                                                line_length))
        return IS_COEFF;
    if(strcmp(char_line, "IS_ZERO\n") == 0 && StrLengthCompare("IS_ZERO\n",
                                                               line_length))
        return IS_ZERO;
    if(strcmp(char_line, "CLONE\n") == 0 && StrLengthCompare("CLONE\n",
                                                             line_length))
        return CLONE;
    if(strcmp(char_line, "ADD\n") == 0 && StrLengthCompare("ADD\n",
                                                           line_length))
        return ADD;
    if(strcmp(char_line, "MUL\n") == 0 && StrLengthCompare("MUL\n",
                                                           line_length))
        return MUL;
    if(strcmp(char_line, "NEG\n") == 0 && StrLengthCompare("NEG\n",
                                                           line_length))
        return NEG;
    if(strcmp(char_line, "SUB\n") == 0 && StrLengthCompare("SUB\n",
                                                           line_length))
        return SUB;
    if(strcmp(char_line, "IS_EQ\n") == 0 && StrLengthCompare("IS_EQ\n",
                                                             line_length))
        return IS_EQ;
    if(strcmp(char_line, "DEG\n") == 0 && StrLengthCompare("DEG\n",
                                                           line_length))
        return DEG;
    if(strcmp(char_line, "PRINT\n") == 0 && StrLengthCompare("PRINT\n",
                                                             line_length))
        return PRINT;
    if(strcmp(char_line, "POP\n") == 0 && StrLengthCompare("POP\n",
                                                           line_length))
        return POP;

    if(strncmp(char_line, "AT", 2) == 0) {
        if(char_line[2] == '\0')
            return WRONG_COMMAND;
        if(char_line[2] != ' ')
            return WRONG_AT;
        if(char_line[3] == '-' && (char_line[4] == '\n' || char_line[4] == EOF))
            return WRONG_AT;
        if(!isdigit(char_line[3]) && char_line[3] != '-')
            return WRONG_AT;
        int ind = 4;
        while(!(char_line[ind] == '\n' || char_line[ind] == EOF)) {
            if(!isdigit(char_line[ind]))
                return WRONG_AT;
            ind++;
        }
        return AT;

    }
    if(strncmp(char_line, "DEG_BY", 6) == 0) {
        if(char_line[6] == '\0')
            return WRONG_COMMAND;
        if(char_line[6] != ' ')
            return WRONG_DEG_BY;
        if(char_line[7] == '\n' || char_line[7] == EOF)
            return WRONG_DEG_BY;
         int ind = 7;
        while(!(char_line[ind] == '\n' || char_line[ind] == EOF)) {
            if(!isdigit(char_line[ind]))
                return WRONG_DEG_BY;
            ind++;
        }
        return DEG_BY;
    }
    if(strncmp(char_line,"COMPOSE", 7) == 0) {
        if(char_line[7] == '\n' || char_line[7] == EOF)
            return WRONG_COMPOSE;
        if(char_line[7] != ' ')
            return WRONG_COMMAND;
        if(char_line[8] == '\n' || char_line[8] == EOF)
            return WRONG_COMPOSE;
        int ind = 8;
        while(!(char_line[ind] == '\n' || char_line[ind] == EOF)) {
            if(!isdigit(char_line[ind]))
                return WRONG_COMPOSE;
            ind++;
        }
        return COMPOSE;
    }
    return WRONG_COMMAND;
}

/**
 * Funkcja pomocnicza do funkcji PolyCheck, sprawdza wielomiany będące
 * współczynnikami.
 * @param[in] c : tablica znaków
 * @return nie da się stworzyć wielomianu: WRONG_POLY, wielomian prawidłowy:
 * POLY_GOOD
 */
static int PolyCheckCoeff(char *c) {
    if(c[0] == '-' && (c[1] == '\n' || c[1] == EOF))
        return WRONG_POLY;
    if(c[0] != '-' && !isdigit(c[0]))
        return WRONG_POLY;
    int ind = 1;
    while(c[ind] != '\n') {
        if(!isdigit(c[ind]))
            return WRONG_POLY;
        ind++;
    }
    return POLY_GOOD;
}
/**
 * Funkcja pomocnicza do funkcji PolyCheck, sprawdza wielomiany nie będące
 * współczynnikami.
 * @param[in] c : tablica znaków
 * @return nie da się stworzyć wielomianu: WRONG_POLY, wielomian prawidłowy:
 * POLY_GOOD
 */
static int PolyCheckNotCoeff(char *c) {
    size_t ind = 1;
    int more_left_brackets = 1;
    size_t right_brackets = 0;
    size_t comma = 0;
    while(c[ind] != '\n' && c[ind] != EOF) {
        if(c[ind-1] != '(' && c[ind-1] != '+' && c[ind] =='(')
            return WRONG_POLY;
        if(c[ind-1] != ')' && !isdigit(c[ind-1]) && c[ind] == ',')
            return WRONG_POLY;
        if(!isdigit(c[ind-1]) && c[ind] == ')')
            return WRONG_POLY;
        if(c[ind-1] != '(' && c[ind] == '-')
            return WRONG_POLY;
        if(c[ind-1] != ')' && c[ind] == '+')
            return WRONG_POLY;
        if(c[ind-1] == '+' && c[ind] != '(')
            return WRONG_POLY;
        if(!isdigit(c[ind-1]) && c[ind-1] != '(' && c[ind-1] != ',' &&
        c[ind-1] != '-' && isdigit(c[ind]))
            return WRONG_POLY;
        if(c[ind-1] == ')' && c[ind] == '(')
            return WRONG_POLY;
        if(c[ind] == '(')
            more_left_brackets++;
        if(c[ind] == ')') {
            right_brackets++;
            more_left_brackets--;
        }
        if(more_left_brackets < 0)
            return WRONG_POLY;
        ind++;
        if(c[ind] == ',')
            comma ++;
    }
    if(comma != right_brackets)
        return WRONG_POLY;
    if(more_left_brackets > 0)
        return WRONG_POLY;
    if(c[ind-1] != ')' && (c[ind] == '\n' || c[ind] == EOF))
        return WRONG_POLY;

    return POLY_GOOD;
}
/**
 * Funkcja sprawdzająca, czy tablica znaków może reprezentować wielomian
 * @param[in] c : tablica znaków
 * @return nie da się stworzyć wielomianu: WRONG_POLY, wielomian prawidłowy:
 * POLY_GOOD
 */
static int PolyCheck(char *c) {

    int ind = 0;
    while(c[ind] != '\n') {
        if(strchr(LEGAL_CHAR, (int) c[ind]) == NULL)
            return WRONG_POLY;
        ind++;
    }
    if(isdigit((int) c[0]) || c[0] == '-') {
        return PolyCheckCoeff(c);
    }
    else return PolyCheckNotCoeff(c);
}

/**
 * Funkcja sprawdzająca, jakim typem jest tablica znaków, może być ona
 * instrukcją lub wielomianem, sprawdza również poprawność.
 * @param[in] c : tablica znaków
 * @param[in] line_length : długość linii
 * @return konkretny rodzaj instrukcji lub wielomianu
 */
static int LineTypeCheck(char *c, int line_length) {

    if(isalpha(c[0])) {
        return InstrCheck(c,line_length);
    }
    else if( c[0] == '\n' || c[0] == EOF || c[0] == '#') {
        return IGNORE;
    }
    else {
        return PolyCheck(c);
    }
}


/**
 * Funkcja wykonująca instrukcje.
 * @param[in] c : tablica znaków
 * @param[in] s : stos
 * @param[in] line : długość wiersza
 * @param[in] line_type : nr wiersza
 * @return konkretny rodzaj instrukcji lub wielomianu
 */
static void DoInstruction(char *c, Stack *s, size_t line, int line_type) {
    switch(line_type) {
        case ZERO: Zero(s);
            break;
        case IS_COEFF: IsCoeff(s, line);
            break;
        case IS_ZERO: IsZero(s, line);
            break;
        case CLONE: Clone(s, line);
            break;
        case ADD: Add(s, line);
            break;
        case MUL: Mul(s, line);
            break;
        case NEG: Neg(s, line);
            break;
        case SUB: Sub(s, line);
            break;
        case IS_EQ: IsEq(s, line);
            break;
        case DEG: Deg(s, line);
            break;
        case PRINT: Print(s, line);
            break;
        case POP: Pop(s, line);
            break;
        case AT: {
            char *pointer = c + 3;
            char *endptr = NULL;
            poly_coeff_t value = strtol(pointer, &endptr, 10);
            if (errno != 0 || (*endptr != '\n' && *endptr != EOF)) {
                fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", line);
                errno = 0;
                return;
            }
            At(s, value, line);
            break;
        }
        case DEG_BY: {
            char *pointer = c + 7;
            char *endptr = NULL;
            unsigned long int power = strtoul(pointer, &endptr, 10);
            if (errno != 0 || (*endptr != '\n' && *endptr != EOF)) {
                fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", line);
                errno = 0;
                return;
            }
            DegBy(s, power, line);
            break;
        }
        default: {
            char *pointer = c + 8;
            char *endptr = NULL;
            unsigned long int value = strtoul(pointer, &endptr, 10);
            if (errno != 0 || (*endptr != '\n' && *endptr != EOF)) {
                fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", line);
                errno = 0;
                return;
            }
            Compose(s, value, line);
        }
    }
}

void Engine(char *c, Stack *s, size_t line_number, int line_length) {
    int line_type;
    line_type = LineTypeCheck(c, line_length);
    switch(line_type) {
    case ZERO: case IS_COEFF: case IS_ZERO: case CLONE: case ADD: case MUL:
    case NEG: case SUB: case IS_EQ: case DEG: case PRINT: case POP: case AT:
    case DEG_BY: case COMPOSE: {
        DoInstruction(c, s, line_number, line_type);
            break;
        }
        case WRONG_COMMAND: {
            fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line_number);
            break;
        }
        case WRONG_DEG_BY: {
            fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n",
                    line_number);
            break;
        }
        case WRONG_AT: {
            fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", line_number);
            break;
        }
        case WRONG_COMPOSE: {
            fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", line_number);
            break;
        }
        case POLY_GOOD: {

                int status = POLY_GOOD;
                int move = 0;
                Poly p = PolyFromString(c, &move, &status);
                if(status == WRONG_POLY) {
                    fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
                    PolyDestroy(&p);
                }
                else {

                    StackPush(s, &p);
                }
            break;
        }
        case WRONG_POLY: {
            fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
            break;
        }
        case IGNORE: {}
    }
    line_number ++;
}



