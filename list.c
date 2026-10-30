#include "list.h"
#include <stdlib.h>

/**
 * Внутренняя структура узла списка.
 * Скрыта в файле реализации (list.c), чтобы вызывающий код не зависел
 * от деталей внутреннего представления и не мог манипулировать указателями узлов напрямую.
 */
struct Node {
    int value;           /* Значение элемента */
    struct Node *next;   /* Указатель на следующий узел */
};

ListErrorCode list_init(List *list) {
    if (list == NULL) {
        return LIST_ERR_INVALID_ARGUMENT;
    }

    list->head = NULL;
    list->size = 0;
    return LIST_SUCCESS;
}

void list_destroy(List *list) {
    if (list == NULL) {
        return;
    }

    /* Итеративное освобождение цепочки узлов одним проходом (O(N)).
     * Важно: указатель на следующий узел (next) сохраняется ДО вызова free(curr),
     * чтобы избежать чтения уже освобожденной памяти (use-after-free).
     * Рекурсия намеренно не используется, чтобы исключить переполнение стека вызовов.
     */
    Node *curr = list->head;
    while (curr != NULL) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }

    /* Зануление полей для безопасности при повторном вызове list_destroy */
    list->head = NULL;
    list->size = 0;
}

ListErrorCode list_push_front(List *list, int value) {
    if (list == NULL) {
        return LIST_ERR_INVALID_ARGUMENT;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return LIST_ERR_NO_MEMORY;
    }

    new_node->value = value;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;

    return LIST_SUCCESS;
}

ListErrorCode list_push_back(List *list, int value) {
    if (list == NULL) {
        return LIST_ERR_INVALID_ARGUMENT;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return LIST_ERR_NO_MEMORY;
    }

    new_node->value = value;
    new_node->next = NULL;

    /* По ТЗ в списке хранится только указатель на первый узел,
     * поэтому для вставки в конец требуется полный проход до последнего узла.
     * Используем указатель на указатель (curr), что позволяет единообразно обработать
     * как пустой список (curr указывает на &list->head), так и непустой (curr указывает на &last->next).
     */
    Node **curr = &list->head;
    while (*curr != NULL) {
        curr = &(*curr)->next;
    }
    *curr = new_node;
    list->size++;

    return LIST_SUCCESS;
}

ListErrorCode list_remove_first(List *list, int value) {
    if (list == NULL) {
        return LIST_ERR_INVALID_ARGUMENT;
    }

    /* Удаление с использованием указателя на указатель (pointer-to-pointer).
     *
     * Разбор трех случаев из ТЗ:
     * 1. Удаляемый узел — первый (голова):
     *    *curr является list->head. Присваивание *curr = to_delete->next
     *    напрямую перенаправляет указатель головы на следующий (второй) узел списка.
     * 2. Удаляемый узел — в середине:
     *    *curr является полем next предыдущего узла (prev->next).
     *    Присваивание *curr = to_delete->next связывает предыдущий узел со следующим,
     *    бесшовно исключая удаляемый узел из цепочки.
     * 3. Удаляемый узел — последний (хвост):
     *    Аналогично случаю 2, но to_delete->next равен NULL, поэтому
     *    поле next предыдущего узла становится NULL, формируя новый конец списка.
     *
     * Подход с Node** устраняет необходимость в дублирующих ветках if/else.
     */
    Node **curr = &list->head;
    while (*curr != NULL) {
        if ((*curr)->value == value) {
            Node *to_delete = *curr;
            *curr = to_delete->next;
            free(to_delete);
            list->size--;
            return LIST_SUCCESS;
        }
        curr = &(*curr)->next;
    }

    return LIST_ERR_NOT_FOUND;
}

ListErrorCode list_find(const List *list, int value, size_t *out_position) {
    if (list == NULL || out_position == NULL) {
        return LIST_ERR_INVALID_ARGUMENT;
    }

    Node *curr = list->head;
    size_t index = 0;
    while (curr != NULL) {
        if (curr->value == value) {
            *out_position = index;
            return LIST_SUCCESS;
        }
        curr = curr->next;
        index++;
    }

    return LIST_ERR_NOT_FOUND;
}

void list_traverse(const List *list, ListCallback callback) {
    if (list == NULL || callback == NULL) {
        return;
    }

    Node *curr = list->head;
    while (curr != NULL) {
        callback(curr->value);
        curr = curr->next;
    }
}

size_t list_size(const List *list) {
    if (list == NULL) {
        return 0;
    }
    return list->size;
}

const char *list_error_to_string(ListErrorCode error) {
    switch (error) {
        case LIST_SUCCESS:
            return "Успешно (LIST_SUCCESS)";
        case LIST_ERR_INVALID_ARGUMENT:
            return "Некорректный аргумент (LIST_ERR_INVALID_ARGUMENT)";
        case LIST_ERR_NO_MEMORY:
            return "Недостаточно памяти (LIST_ERR_NO_MEMORY)";
        case LIST_ERR_NOT_FOUND:
            return "Элемент не найден (LIST_ERR_NOT_FOUND)";
        default:
            return "Неизвестная ошибка";
    }
}
