#include <check.h>
#include "s21_smart_calc.h"

START_TEST(test_s21_parse_to_tokens) {
    int tok_len;
    char **tokens = s21_parse_to_tokens("cos( 45 * 3 + 4) -7", &tok_len);
    ck_assert_int_eq(tok_len, 10);
    if (tokens != NULL) {
        for (int i = 0; i < tok_len; i++) {
            free(*(tokens + i));
        }
        free(tokens);
    }

    tokens = s21_parse_to_tokens("cos) 45 * 3 + 4) -7", &tok_len);
    ck_assert_int_eq(tok_len, 0);
    ck_assert_ptr_null(tokens);

    tokens = s21_parse_to_tokens("cos(( 45 * 3 + 4) -7", &tok_len);
    ck_assert_int_eq(tok_len, 0);
    ck_assert_ptr_null(tokens);

    tokens = s21_parse_to_tokens("cos(( 45 * 3 + 4) -7)", &tok_len);
    ck_assert_int_eq(tok_len, 12);
    if (tokens != NULL) {
        for (int i = 0; i < tok_len; i++) {
            free(*(tokens + i));
        }
        free(tokens);
    }

    tokens = s21_parse_to_tokens("(( 45 * 3 + 4) -7)", &tok_len);
    ck_assert_int_eq(tok_len, 11);
    if (tokens != NULL) {
        for (int i = 0; i < tok_len; i++) {
            free(*(tokens + i));
        }
        free(tokens);
    }

    tokens = s21_parse_to_tokens(" 45 * 3 + 4 -7)", &tok_len);
    ck_assert_int_eq(tok_len, 0);

    tokens = s21_parse_to_tokens("( 45 * 3 + 4 -7)", &tok_len);
    ck_assert_int_eq(tok_len, 9);
    if (tokens != NULL) {
        for (int i = 0; i < tok_len; i++) {
            free(*(tokens + i));
        }
        free(tokens);
    }

    tokens = s21_parse_to_tokens("( 45 * 3 + cos(1) + 4 -7)", &tok_len);
    ck_assert_int_eq(tok_len, 14);
    if (tokens != NULL) {
        for (int i = 0; i < tok_len; i++) {
            free(*(tokens + i));
        }
        free(tokens);
    }
}
END_TEST

START_TEST(test_s21_calc) {
    double ans;
    s21_calc("1 + 2", &ans);
    ck_assert_double_eq_tol(3, ans, pow(10, -7));

    s21_calc("ln(3)*2", &ans);

    s21_calc("1 + 2 * ln(1 *7 -4)", &ans);
    ck_assert_double_eq_tol(3.1972245, ans, pow(10, -7));

    s21_calc("-10", &ans);
    ck_assert_double_eq_tol(-10, ans, pow(10, -7));

    s21_calc("(-10)*(-10)", &ans);
    ck_assert_double_eq_tol(100, ans, pow(10, -7));

    s21_calc("(-100)*(-100)*(-100)", &ans);
    ck_assert_double_eq_tol(-1000000, ans, pow(10, -7));
}
END_TEST

Suite *s21_decimal_test_suite() {
    Suite *s = suite_create("s21_calc_test");

    TCase *tc_s21_parse_to_tokens = tcase_create("test_s21_parse_to_tokens");
    suite_add_tcase(s, tc_s21_parse_to_tokens);
    tcase_add_test(tc_s21_parse_to_tokens, test_s21_parse_to_tokens);

    TCase *tc_calc = tcase_create("test_s21_calc");
    suite_add_tcase(s, tc_calc);
    tcase_add_test(tc_calc, test_s21_calc);

    return s;
}


int main() {
    int no_failed = 0;

    Suite *s = s21_decimal_test_suite();

    SRunner *runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
