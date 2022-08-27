#ifndef S21_SMART_CALC_H
#define S21_SMART_CALC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct stack_node {
    char *info;
    struct stack_node* prev;
};


struct stack_node* init(char* info);
struct stack_node* append(struct stack_node* head, char* info);
struct stack_node* pop(struct stack_node* head);

// START->CALCULATOR

typedef struct Expectations {
    int expect_num;
    int expect_func;
    int expect_oper;
    int expect_open_bracket;
    int expect_close_bracket;
    int bracket_opened;
} expectations_t;

#define NUM_EXPRESSION 0  // ""
#define SIN_EXPRESSION 1  // "^sin"
#define COS_EXPRESSION 2  // "^cos"
#define TAN_EXPRESSION 3  // "^tan"
#define ASIN_EXPRESSION 4  // "^asin"
#define ACOS_EXPRESSION 5  // "^acos"
#define ATAN_EXPRESSION 6  // "^atan"
#define SQRT_EXPRESSION 7  // "^sqrt"
#define LN_EXPRESSION 8  // "^ln"
#define LOG_EXPRESSION 9  // "^log"

#define COUNT_EXPRESSIONS 10

static const char expressions[COUNT_EXPRESSIONS][50] = {
    {"^[-]?([0-9]+([.][0-9]*)?|[.][0-9]+)"},
    {"^sin"},
    {"^cos"},
    {"^tan"},
    {"^asin"},
    {"^acos"},
    {"^atan"},
    {"^sqrt"},
    {"^ln"},
    {"^log"}
};

int s21_calc(char *str_expression, double *ans);
char **s21_parse_to_tokens(char *str_expression, int *n);
regex_t *s21_get_reg_tokens();
struct stack_node *s21_get_rpn_from_tokens(char **tokens, int token_len);
int s21_get_prec(char c);
int s21_calculate_rpn(struct stack_node *rpn, double *ans);
double division(double a, double b);
double mul(double a, double b);
double sub(double a, double b);
double add(double a, double b);

#ifdef __cplusplus
}
#endif

// END->CALCULATOR


#endif  // S21_SMART_CALC_H
