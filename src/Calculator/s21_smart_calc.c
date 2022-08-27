#include "s21_smart_calc.h"

int s21_calc(char *str_expression, double *ans) {
    int ret_val = 1;
    int token_len;
    char **tokens = s21_parse_to_tokens(str_expression, &token_len);
    if (tokens != NULL) {
        struct stack_node *rpn = s21_get_rpn_from_tokens(tokens, token_len);
        s21_calculate_rpn(rpn, ans);
        while (rpn != NULL) {
            rpn = pop(rpn);
        }

        for (int i = 0; i < token_len; i++) {
            free(*(tokens + i));
        }
        free(tokens);
    } else {
        ret_val = 0;
    }

    return ret_val;
}

int s21_calculate_rpn(struct stack_node *rpn, double *ans) {
    int ret_val = 0;
    struct stack_node* nums_stack = NULL;
    while (rpn != NULL) {
        if ((rpn->info[0] >= 48 && rpn->info[0] <= 57) || rpn->info[0] == '.' || (rpn->info[0] == '-' && rpn->info[1] != '\0')) {
            nums_stack = append(nums_stack, rpn->info);
        } else if (strchr("+-*/^", rpn->info[0])) {
            double a, b;
            a = strtod(nums_stack->info, NULL);
            nums_stack = pop(nums_stack);
            b = strtod(nums_stack->info, NULL);
            nums_stack = pop(nums_stack);

            double ans;

            switch (rpn->info[0]) {
                case '+':
                    ans = add(a, b);
                    break;
                case '-':
                    ans = sub(a, b);
                    break;
                case '*':
                    ans = mul(a, b);
                    break;
                case '/':
                    ans = division(a, b);
                    break;
                case '^':
                    ans = pow(b, a);
                    break;
                case '%':
                    ans = fmod(a, b);
                    break;
            }
            char str_ans[20];
            snprintf(str_ans, sizeof(str_ans), "%.9lf", ans);
            nums_stack = append(nums_stack, str_ans);
        } else {
            double a;
            a = strtod(nums_stack->info, NULL);
            nums_stack = pop(nums_stack);

            double ans = 0;

            if (!strcmp("sin", rpn->info)) {
                ans = sin(a);
            } else if (!strcmp("cos", rpn->info)) {
                ans = cos(a);
            } else if (!strcmp("tan", rpn->info)) {
                ans = tan(a);
            } else if (!strcmp("sqrt", rpn->info)) {
                ans = sqrt(a);
            } else if (!strcmp("ln", rpn->info)) {
                ans = log(a);
            } else if (!strcmp("log", rpn->info)) {
                ans = log10(a);
            } else if (!strcmp("asin", rpn->info)) {
                ans = asin(a);
            } else if (!strcmp("acos", rpn->info)) {
                ans = acos(a);
            } else if (!strcmp("atan", rpn->info)) {
                ans = atan(a);
            }

            char str_ans[20];
            snprintf(str_ans, sizeof(str_ans), "%.9lf", ans);
            nums_stack = append(nums_stack, str_ans);
        }
        rpn = rpn->prev;
    }
    *ans = strtod(nums_stack->info, NULL);
    nums_stack = pop(nums_stack);
    while (nums_stack) {
        nums_stack = pop(nums_stack);
    }

    return ret_val;
}

double add(double a, double b) {
    return a + b;
}

double sub(double a, double b) {
    return b - a;
}

double mul(double a, double b) {
    return a * b;
}

double division(double a, double b) {
    return (double)(b) / a;
}

struct stack_node *s21_get_rpn_from_tokens(char **tokens, int token_len) {
    struct stack_node *out = NULL;
    struct stack_node *opers = NULL;
    for (int i = 0; i < token_len; i++) {
        if ((*(tokens + i)[0] >= '0' && *(tokens + i)[0] <= '9') || *(tokens + i)[0] == '.') {
            out = append(out, *(tokens + i));
        } else if ((*(tokens + i)[0] >= 'a' && *(tokens + i)[0] <= 'z')) {
            opers = append(opers, *(tokens + i));
        } else if (strchr("+-*/^", **(tokens + i))) {
            int prec = s21_get_prec(**(tokens + i));
            while (opers != NULL && opers->info[0] != '(' && s21_get_prec(opers->info[0]) >= prec) {
                out = append(out, opers->info);
                opers = pop(opers);
            }
            opers = append(opers, *(tokens + i));
        } else if (**(tokens + i) == '(') {
            opers = append(opers, *(tokens + i));
        } else if (**(tokens + i) == ')') {
            while (opers->info[0] != '(') {
                out = append(out, opers->info);
                opers = pop(opers);
            }
            opers = pop(opers);
            if (opers != NULL) {
                if (opers->info[0] >= 'a' && opers->info[0] <= 'z') {
                    out = append(out, opers->info);
                    opers = pop(opers);
                }
            }
        }
    }
    while (opers != NULL) {
        out = append(out, opers->info);
        opers = pop(opers);
    }
    struct stack_node* reversed_out = NULL;
    while (out != NULL) {
        reversed_out = append(reversed_out, out->info);
        out = pop(out);
    }

    return reversed_out;
}

char **s21_parse_to_tokens(char *str_expression, int *n) {
    static char my_expression_str[255];
    char **tokens = NULL;
    int count_tokens = 0;
    int ret_val = 1;
    int count = 0;

    for (int i = 0; i < (int)strlen(str_expression); i++) {
        if (str_expression[i] != ' ') {
            my_expression_str[count++] = str_expression[i];
        }
    }
    my_expression_str[count] = '\0';

    regex_t *reg_exps = s21_get_reg_tokens();
    if (reg_exps == NULL)
        ret_val = -1;

    if (ret_val == 1) {
        size_t nmatch = 2;
        regmatch_t pmatch[2];

        expectations_t expectations = {
            .expect_num = 1,
            .expect_func = 1,
            .expect_oper = 0,
            .expect_open_bracket = 1,
            .expect_close_bracket = 0,
            .bracket_opened = 0
        };

        char *p = my_expression_str;
        char tmp[50];
        while (*p != '\0') {
            if (expectations.expect_num && !regexec(&reg_exps[NUM_EXPRESSION], p, nmatch, pmatch, 0)) {
                strncpy(tmp, p, pmatch[0].rm_eo - pmatch[0].rm_so);
                tmp[pmatch[0].rm_eo - pmatch[0].rm_so] = '\0';
                if (!count_tokens) {
                    count_tokens++;
                    tokens = malloc(sizeof(char *));
                    if (tokens != NULL) {
                        *(tokens) = malloc(50 * sizeof(char));
                    } else {
                        ret_val = -2;
                        break;
                    }
                } else {
                    count_tokens++;
                    tokens = realloc(tokens, count_tokens * sizeof(char *));
                    *(tokens + count_tokens - 1) = malloc(50 * sizeof(char));
                }
                snprintf(*(tokens + count_tokens - 1), strlen(tmp) + 1, "%s", tmp);
                p += (strlen(tmp));
                expectations.expect_func = 0;
                expectations.expect_num = 0;
                expectations.expect_oper = 1;
                expectations.expect_close_bracket = expectations.bracket_opened;
                continue;
            }
            if (expectations.expect_func) {
                int checked = 0;
                for (int i = 1; i < COUNT_EXPRESSIONS; i++) {
                    if (!regexec(&reg_exps[i], p, nmatch, pmatch, 0)) {
                        strncpy(tmp, p, pmatch[0].rm_eo - pmatch[0].rm_so);
                        tmp[pmatch[0].rm_eo - pmatch[0].rm_so] = '\0';
                        if (!count_tokens) {
                            count_tokens++;
                            tokens = malloc(sizeof(char *));
                            if (tokens != NULL) {
                                *(tokens) = malloc(5 * sizeof(char));
                            } else {
                                ret_val = -2;
                                break;
                            }
                        } else {
                            count_tokens++;
                            tokens = realloc(tokens, count_tokens * sizeof(char *));
                            *(tokens + count_tokens - 1) = malloc(5 * sizeof(char));
                        }
                        snprintf(*(tokens + count_tokens - 1), strlen(tmp) + 1, "%s", tmp);
                        p += (strlen(tmp));
                        expectations.expect_num = 0;
                        expectations.expect_func = 0;
                        expectations.expect_oper = 0;
                        expectations.expect_open_bracket = 1;
                        expectations.expect_close_bracket = 0;
                        checked = 1;
                        break;
                    }
                }
                if (checked)
                    continue;
            }
            if (expectations.expect_oper) {
                if (strchr("+-*/^", *p)) {
                    tmp[0] = *p;
                    tmp[1] = '\0';
                    count_tokens++;
                    tokens = realloc(tokens, count_tokens * sizeof(char *));
                    *(tokens + count_tokens - 1) = malloc(5 * sizeof(char));
                    snprintf(*(tokens + count_tokens - 1), strlen(tmp) + 1, "%s", tmp);
                    p += (strlen(tmp));
                    expectations.expect_oper = 0;
                    expectations.expect_num = 1;
                    expectations.expect_func = 1;
                    expectations.expect_close_bracket = 0;
                    expectations.expect_open_bracket = 1;
                    continue;
                }
            }
            if (expectations.expect_open_bracket) {
                if (*p == '(') {
                    tmp[0] = *p;
                    tmp[1] = '\0';
                    if (!count_tokens) {
                        count_tokens++;
                        tokens = malloc(sizeof(char *));
                        if (tokens != NULL) {
                            *(tokens) = malloc(5 * sizeof(char));
                        } else {
                            ret_val = -2;
                            break;
                        }
                    } else {
                        count_tokens++;
                        tokens = realloc(tokens, count_tokens * sizeof(char *));
                        *(tokens + count_tokens - 1) = malloc(5 * sizeof(char));
                    }
                    snprintf(*(tokens + count_tokens - 1), strlen(tmp) + 1, "%s", tmp);
                    p += (strlen(tmp));
                    expectations.expect_oper = 0;
                    expectations.expect_num = 1;
                    expectations.expect_func = 1;
                    expectations.expect_close_bracket = 1;
                    expectations.expect_open_bracket = 1;
                    expectations.bracket_opened++;
                    continue;
                }
            }
            if (expectations.expect_close_bracket) {
                if (*p == ')') {
                    tmp[0] = *p;
                    tmp[1] = '\0';
                    count_tokens++;
                    tokens = realloc(tokens, count_tokens * sizeof(char *));
                    *(tokens + count_tokens - 1) = malloc(5 * sizeof(char));
                    snprintf(*(tokens + count_tokens - 1), strlen(tmp) + 1, "%s", tmp);
                    p += (strlen(tmp));
                    expectations.expect_oper = 1;
                    expectations.expect_num = 0;
                    expectations.expect_func = 1;
                    expectations.expect_close_bracket = 1;
                    expectations.expect_open_bracket = 1;
                    expectations.bracket_opened--;
                    if (expectations.bracket_opened < 0) {
                        break;
                    }
                    continue;
                }
            }
            for (int i = 0; i < count_tokens; i++) {
                free(*(tokens + i));
            }
            free(tokens);
            tokens = NULL;
            count_tokens = 0;
            break;
        }
        if (expectations.bracket_opened) {
            for (int i = 0; i < count_tokens; i++) {
                free(*(tokens + i));
            }
            free(tokens);
            tokens = NULL;
            count_tokens = 0;
        }
    }

    for (int k = 0; k < COUNT_EXPRESSIONS; k++) {
        regfree(reg_exps + k);
    }
    free(reg_exps);

    reg_exps = NULL;
    *n = count_tokens;

    return tokens;
}

regex_t *s21_get_reg_tokens() {
    regex_t *reg_exps = malloc(COUNT_EXPRESSIONS * sizeof(regex_t));

    for (int i = 0; i < COUNT_EXPRESSIONS; i++) {
        if (regcomp(reg_exps + i, expressions[i], REG_EXTENDED) != 0) {
            for (int k = 0; k < i; k++) {
                regfree(reg_exps + k);
            }
            free(reg_exps);
            reg_exps = NULL;
            break;
        }
    }

    return reg_exps;
}

int s21_get_prec(char c) {
    int ret;
    switch (c) {
        case '-':
        case '+':
            ret = 1;
            break;
        case '*':
        case '/':
        case '%':
            ret = 2;
            break;
        case '^':
            ret = 3;
            break;
    }
    return ret;
}
