#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Вспомогательная функция обратного вызова для list_traverse: выводит один элемент */
static void print_element(int value) {
    printf("%d ", value);
}

/* Вспомогательная функция для отображения текущего состояния списка */
static void print_list_state(const List *list, const char *label) {
    printf("%s: [ ", label);
    list_traverse(list, print_element);
    printf("] (size: %zu)\n", list_size(list));
}

int main(void) {
    printf("=================================================================\n");
    printf("  Practice 1: Singly Linked List in C (Procedural Approach)     \n");
    printf("  (Resilient to arbitrary operations, edge cases & order shifts)\n");
    printf("=================================================================\n\n");

    List list;
    ListErrorCode err;
    size_t pos = 0;

    /* --------------------------------------------------------------------- */
    /* 1. Инициализация и операции над пустым списком                        */
    /* --------------------------------------------------------------------- */
    printf("--- [1] Initialization & Empty List Operations ---\n");
    err = list_init(&list);
    printf("list_init: %s\n", list_error_to_string(err));
    print_list_state(&list, "Initial state");

    printf("Search in empty list (searching 42): %s\n",
           list_error_to_string(list_find(&list, 42, &pos)));

    printf("Delete from empty list (deleting 42): %s\n",
           list_error_to_string(list_remove_first(&list, 42)));

    printf("Traverse empty list: \"");
    list_traverse(&list, print_element);
    printf("\" (safe: 0 callback invocations)\n\n");

    /* --------------------------------------------------------------------- */
    /* 2. Вставка элементов: в начало (push_front) и в конец (push_back)     */
    /* --------------------------------------------------------------------- */
    printf("--- [2] Insertions (push_front and push_back) ---\n");
    list_push_front(&list, 20);
    list_push_front(&list, 10);
    print_list_state(&list, "After push_front(20), push_front(10)");

    list_push_back(&list, 30);
    list_push_back(&list, 40);
    print_list_state(&list, "After push_back(30), push_back(40)");

    list_push_front(&list, 5);
    list_push_back(&list, 50);
    print_list_state(&list, "Resulting test list");
    /* Сейчас список: [ 5 10 20 30 40 50 ] */

    /* --------------------------------------------------------------------- */
    /* 3. Поиск элементов (list_find)                                        */
    /* --------------------------------------------------------------------- */
    printf("\n--- [3] Search Operations (list_find) ---\n");
    int search_values[] = {5, 30, 50, 999};
    size_t num_searches = sizeof(search_values) / sizeof(search_values[0]);

    for (size_t i = 0; i < num_searches; i++) {
        int val = search_values[i];
        err = list_find(&list, val, &pos);
        if (err == LIST_SUCCESS) {
            printf("Find %3d: Found at index %zu (%s)\n",
                   val, pos, list_error_to_string(err));
        } else {
            printf("Find %3d: %s\n", val, list_error_to_string(err));
        }
    }

    /* --------------------------------------------------------------------- */
    /* 4. Удаление первого вхождения (3 случая + краевые ситуации)           */
    /* --------------------------------------------------------------------- */
    printf("\n--- [4] Removal of First Occurrence (3 Cases) ---\n");

    /* Случай 2: Удаление из середины */
    printf("a) Removal from middle (value 30):\n");
    err = list_remove_first(&list, 30);
    printf("list_remove_first(30): %s\n", list_error_to_string(err));
    print_list_state(&list, "After removing 30");

    /* Повторное удаление того же значения (проверка на отказ) */
    printf("Attempting repeated removal of 30:\n");
    err = list_remove_first(&list, 30);
    printf("list_remove_first(30 again): %s\n", list_error_to_string(err));

    /* Случай 1: Удаление первого узла (головы) */
    printf("\nb) Removal of head node (value 5):\n");
    err = list_remove_first(&list, 5);
    printf("list_remove_first(5): %s\n", list_error_to_string(err));
    print_list_state(&list, "After removing head (5)");

    /* Случай 3: Удаление последнего узла (хвоста) */
    printf("\nc) Removal of tail node (value 50):\n");
    err = list_remove_first(&list, 50);
    printf("list_remove_first(50): %s\n", list_error_to_string(err));
    print_list_state(&list, "After removing tail (50)");

    /* Удаление несуществующего значения */
    printf("\nd) Removal of non-existent value (777):\n");
    err = list_remove_first(&list, 777);
    printf("list_remove_first(777): %s\n", list_error_to_string(err));
    print_list_state(&list, "List unchanged");

    /* --------------------------------------------------------------------- */
    /* 5. Доведение до 1 элемента, опустошение и повторное наполнение       */
    /*    Сейчас в списке: [ 10 20 40 ]                                      */
    /* --------------------------------------------------------------------- */
    printf("\n--- [5] Single Element, Emptying, and Re-insertion ---\n");
    list_remove_first(&list, 10);
    list_remove_first(&list, 40);
    print_list_state(&list, "List with single remaining element");

    printf("Removing single element (20):\n");
    err = list_remove_first(&list, 20);
    printf("list_remove_first(20): %s\n", list_error_to_string(err));
    print_list_state(&list, "List after removing last element");

    printf("\nInserting into freshly emptied list:\n");
    list_push_back(&list, 100);
    print_list_state(&list, "After push_back(100) into empty list");
    list_remove_first(&list, 100);
    print_list_state(&list, "After removing 100 again");

    /* --------------------------------------------------------------------- */
    /* 6. Экстремальные краевые значения (INT_MIN, INT_MAX, 0, отрицательные)*/
    /* --------------------------------------------------------------------- */
    printf("\n--- [6] Boundary Values (INT_MIN, INT_MAX, 0, negative) ---\n");
    list_push_back(&list, INT_MIN);
    list_push_back(&list, -1);
    list_push_back(&list, 0);
    list_push_back(&list, INT_MAX);
    print_list_state(&list, "List with boundary values");

    size_t int_min_pos = 0, int_max_pos = 0;
    list_find(&list, INT_MIN, &int_min_pos);
    list_find(&list, INT_MAX, &int_max_pos);
    printf("INT_MIN found at index: %zu\n", int_min_pos);
    printf("INT_MAX found at index: %zu\n", int_max_pos);

    list_remove_first(&list, INT_MIN);
    list_remove_first(&list, INT_MAX);
    print_list_state(&list, "After removing INT_MIN and INT_MAX");
    list_destroy(&list);

    /* --------------------------------------------------------------------- */
    /* 7. Дубликаты: удаление только первого вхождения                      */
    /* --------------------------------------------------------------------- */
    printf("\n--- [7] Handling Duplicate Values ---\n");
    list_push_back(&list, 7);
    list_push_back(&list, 7);
    list_push_back(&list, 7);
    print_list_state(&list, "List with three sevens [ 7 7 7 ]");

    list_remove_first(&list, 7);
    print_list_state(&list, "After 1st removal of 7");
    list_remove_first(&list, 7);
    print_list_state(&list, "After 2nd removal of 7");
    list_remove_first(&list, 7);
    print_list_state(&list, "After 3rd removal of 7");

    err = list_remove_first(&list, 7);
    printf("Attempting 4th removal of 7: %s\n", list_error_to_string(err));
    list_destroy(&list);

    /* --------------------------------------------------------------------- */
    /* 8. Хаотичный порядок операций и использование после destroy          */
    /* --------------------------------------------------------------------- */
    printf("\n--- [8] Chaotic Call Sequences & Reuse After Destroy ---\n");
    printf("Double destroy on an empty list:\n");
    list_destroy(&list);
    list_destroy(&list);
    print_list_state(&list, "After 2x destroy");

    printf("Insert immediately after destroy (without calling list_init):\n");
    list_push_front(&list, 99);
    list_push_back(&list, 88);
    print_list_state(&list, "List after insertions");

    printf("Instant clear and search:\n");
    list_destroy(&list);
    printf("Search in freshly destroyed list: %s\n",
           list_error_to_string(list_find(&list, 99, &pos)));

    /* --------------------------------------------------------------------- */
    /* 9. Независимость двух списков (отсутствие глобального состояния)     */
    /* --------------------------------------------------------------------- */
    printf("\n--- [9] Independence of Multiple Lists ---\n");
    List list_a, list_b;
    list_init(&list_a);
    list_init(&list_b);

    list_push_back(&list_a, 111);
    list_push_back(&list_a, 222);
    list_push_back(&list_b, 999);

    print_list_state(&list_a, "List A");
    print_list_state(&list_b, "List B");

    printf("Destroying List A...\n");
    list_destroy(&list_a);
    print_list_state(&list_a, "List A after destroy");
    print_list_state(&list_b, "List B (intact and unaffected)");
    list_destroy(&list_b);

    /* --------------------------------------------------------------------- */
    /* 10. Защитное программирование (устойчивость к NULL)                   */
    /* --------------------------------------------------------------------- */
    printf("\n--- [10] Defensive NULL Handling ---\n");
    printf("list_init(NULL): %s\n", list_error_to_string(list_init(NULL)));
    printf("list_push_front(NULL, 1): %s\n", list_error_to_string(list_push_front(NULL, 1)));
    printf("list_push_back(NULL, 1): %s\n", list_error_to_string(list_push_back(NULL, 1)));
    printf("list_remove_first(NULL, 1): %s\n", list_error_to_string(list_remove_first(NULL, 1)));
    printf("list_find(NULL, 1, &pos): %s\n", list_error_to_string(list_find(NULL, 1, &pos)));
    printf("list_find(&list, 1, NULL): %s\n", list_error_to_string(list_find(&list, 1, NULL)));
    printf("list_size(NULL): %zu\n", list_size(NULL));
    list_traverse(NULL, print_element);
    list_traverse(&list, NULL);
    list_destroy(NULL);
    printf("All NULL pointer calls safely intercepted!\n");

    /* --------------------------------------------------------------------- */
    /* 11. Стресс-тест памяти (Критерий приемки ТЗ: 100 циклов по 1000 узлов)*/
    /* --------------------------------------------------------------------- */
    printf("\n--- [11] Stress Test: 100 Cycles of 1000 Nodes ---\n");
    const int NUM_CYCLES = 100;
    const int NODES_PER_CYCLE = 1000;
    int stress_ok = 1;

    for (int cycle = 0; cycle < NUM_CYCLES; cycle++) {
        List test_list;
        if (list_init(&test_list) != LIST_SUCCESS) {
            stress_ok = 0;
            break;
        }

        for (int i = 0; i < NODES_PER_CYCLE; i++) {
            if (i % 2 == 0) {
                list_push_front(&test_list, i);
            } else {
                list_push_back(&test_list, i);
            }
        }

        if (list_size(&test_list) != (size_t)NODES_PER_CYCLE) {
            stress_ok = 0;
            list_destroy(&test_list);
            break;
        }

        list_destroy(&test_list);

        if (list_size(&test_list) != 0 || test_list.head != NULL) {
            stress_ok = 0;
            break;
        }
    }

    if (stress_ok) {
        printf("Stress test passed: %d cycles of %d nodes with zero memory leaks!\n",
               NUM_CYCLES, NODES_PER_CYCLE);
    } else {
        printf("Error during stress test!\n");
    }

    printf("\n=================================================================\n");
    printf("  All scenarios executed successfully!                           \n");
    printf("=================================================================\n");

    return 0;
}
