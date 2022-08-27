#include "s21_smart_calc.h"

struct stack_node* init(char* info) {
    struct stack_node* sample = (struct stack_node*)malloc(sizeof(struct stack_node));
    if (info != NULL) {
        char* tmp = malloc(strlen(info) * sizeof(char));
        snprintf(tmp, strlen(info) + 1, "%s", info);
        sample->info = tmp;
    } else {
        sample->info = NULL;
    }
    sample->prev = NULL;
    return sample;
}

struct stack_node* append(struct stack_node* head, char* info) {
    struct stack_node* sample;

    if (head == NULL) {
        sample = init(info);
    } else if (head->info == NULL) {
        sample = head;
        char* tmp = malloc(strlen(info) * sizeof(char));
        snprintf(tmp, strlen(info) + 1, "%s", info);
        sample->info = tmp;
    } else {
        sample = (struct stack_node*)malloc(sizeof(struct stack_node));
        char* tmp = malloc(strlen(info) * sizeof(char));
        snprintf(tmp, strlen(info) + 1, "%s", info);
        sample->info = tmp;
        sample->prev = head;
    }
    return sample;
}

struct stack_node* pop(struct stack_node* head) {
    struct stack_node* sample = head->prev;
    free(head->info);
    free(head);
    return sample;
}
