#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* Вспомогательная функция для вывода одного элемента через list_traverse */
static void print_element(int value) {
    printf("%d ", value);
}

/* Печать текущего состояния списка (обход + размер) */
static void print_list_state(const List *list, const char *label) {
    printf("%s: [ ", label);
    list_traverse(list, print_element);
    printf("] (размер: %zu)\n", list_size(list));
}

int main(void) {
    printf("=========================================================\n");
    printf("  Практическая работа №1: Односвязный список на языке C  \n");
    printf("=========================================================\n\n");

    List list;
    ListErrorCode err;
    size_t pos = 0;

    /* --------------------------------------------------------------------- */
    /* 1. Инициализация и операции над пустым списком                        */
    /* --------------------------------------------------------------------- */
    printf("--- [1] Создание и проверка пустого списка ---\n");
    err = list_init(&list);
    printf("list_init: %s\n", list_error_to_string(err));
    print_list_state(&list, "Начальное состояние");

    printf("\nПроверка поиска в пустом списке (ищем 42):\n");
    err = list_find(&list, 42, &pos);
    printf("list_find(42): %s\n", list_error_to_string(err));

    printf("\nПроверка удаления из пустого списка (удаляем 42):\n");
    err = list_remove_first(&list, 42);
    printf("list_remove_first(42): %s\n", list_error_to_string(err));

    printf("\nПроверка обхода пустого списка:\n");
    printf("list_traverse (пустой список): результат -> \"");
    list_traverse(&list, print_element);
    printf("\" (ничего не напечатано, вызовов не было)\n\n");

    /* --------------------------------------------------------------------- */
    /* 2. Вставка элементов: в начало и в конец                             */
    /* --------------------------------------------------------------------- */
    printf("--- [2] Вставка элементов (push_front и push_back) ---\n");

    printf("Добавляем в начало: 20, затем 10:\n");
    list_push_front(&list, 20);
    list_push_front(&list, 10);
    print_list_state(&list, "После push_front(20) и push_front(10)");

    printf("Добавляем в конец: 30, затем 40:\n");
    list_push_back(&list, 30);
    list_push_back(&list, 40);
    print_list_state(&list, "После push_back(30) и push_back(40)");

    printf("Добавляем в начало: 5, и в конец: 50:\n");
    list_push_front(&list, 5);
    list_push_back(&list, 50);
    print_list_state(&list, "Итоговый список для тестов");
    /* Сейчас список: [ 5 10 20 30 40 50 ] */

    /* --------------------------------------------------------------------- */
    /* 3. Поиск элементов (list_find)                                        */
    /* --------------------------------------------------------------------- */
    printf("\n--- [3] Поиск элементов в списке ---\n");

    int search_values[] = {5, 30, 50, 999};
    size_t num_searches = sizeof(search_values) / sizeof(search_values[0]);

    for (size_t i = 0; i < num_searches; i++) {
        int val = search_values[i];
        err = list_find(&list, val, &pos);
        if (err == LIST_SUCCESS) {
            printf("Поиск %3d: Найдено на позиции %zu (%s)\n",
                   val, pos, list_error_to_string(err));
        } else {
            printf("Поиск %3d: %s\n", val, list_error_to_string(err));
        }
    }

    /* --------------------------------------------------------------------- */
    /* 4. Удаление первого вхождения: все три случая + краевые               */
    /*    Текущий список: [ 5 10 20 30 40 50 ]                               */
    /* --------------------------------------------------------------------- */
    printf("\n--- [4] Удаление первого вхождения элементов (3 случая) ---\n");

    /* Случай 2: Удаление из середины (удаляем 30) */
    printf("а) Удаление из середины (значение 30):\n");
    err = list_remove_first(&list, 30);
    printf("list_remove_first(30): %s\n", list_error_to_string(err));
    print_list_state(&list, "Список после удаления 30");

    /* Повторное удаление уже удаленного элемента (проверка на отказ) */
    printf("Попытка повторного удаления 30:\n");
    err = list_remove_first(&list, 30);
    printf("list_remove_first(30 повторно): %s\n", list_error_to_string(err));

    /* Случай 1: Удаление первого элемента (головы, удаляем 5) */
    printf("\nб) Удаление первого узла (головы, значение 5):\n");
    err = list_remove_first(&list, 5);
    printf("list_remove_first(5): %s\n", list_error_to_string(err));
    print_list_state(&list, "Список после удаления головы 5");

    /* Случай 3: Удаление последнего элемента (хвоста, удаляем 50) */
    printf("\nв) Удаление последнего узла (хвоста, значение 50):\n");
    err = list_remove_first(&list, 50);
    printf("list_remove_first(50): %s\n", list_error_to_string(err));
    print_list_state(&list, "Список после удаления хвоста 50");

    /* Удаление отсутствующего элемента */
    printf("\nг) Удаление несуществующего значения (значение 777):\n");
    err = list_remove_first(&list, 777);
    printf("list_remove_first(777): %s\n", list_error_to_string(err));
    print_list_state(&list, "Список без изменений");

    /* --------------------------------------------------------------------- */
    /* 5. Доведение до единственного элемента и его удаление                 */
    /*    Сейчас в списке: [ 10 20 40 ]                                      */
    /* --------------------------------------------------------------------- */
    printf("\n--- [5] Единственный элемент и опустошение списка ---\n");
    list_remove_first(&list, 10);
    list_remove_first(&list, 40);
    print_list_state(&list, "Список с единственным оставшимся элементом");

    printf("Удаляем единственный элемент (20):\n");
    err = list_remove_first(&list, 20);
    printf("list_remove_first(20): %s\n", list_error_to_string(err));
    print_list_state(&list, "Список после удаления единственного элемента");

    printf("\nВставка в только что опустошенный список:\n");
    list_push_back(&list, 100);
    print_list_state(&list, "После push_back(100) в пустой список");
    list_remove_first(&list, 100);
    print_list_state(&list, "После повторного удаления 100");

    /* --------------------------------------------------------------------- */
    /* 6. Проверка устойчивости к некорректным аргументам (NULL)             */
    /* --------------------------------------------------------------------- */
    printf("\n--- [6] Устойчивость к некорректным аргументам (NULL) ---\n");
    printf("list_init(NULL): %s\n", list_error_to_string(list_init(NULL)));
    printf("list_push_front(NULL, 1): %s\n", list_error_to_string(list_push_front(NULL, 1)));
    printf("list_push_back(NULL, 1): %s\n", list_error_to_string(list_push_back(NULL, 1)));
    printf("list_remove_first(NULL, 1): %s\n", list_error_to_string(list_remove_first(NULL, 1)));
    printf("list_find(NULL, 1, &pos): %s\n", list_error_to_string(list_find(NULL, 1, &pos)));
    printf("list_find(&list, 1, NULL): %s\n", list_error_to_string(list_find(&list, 1, NULL)));
    printf("list_size(NULL): %zu\n", list_size(NULL));
    list_traverse(NULL, print_element); /* Не должно вызывать краш */
    list_traverse(&list, NULL);          /* Не должно вызывать краш */
    list_destroy(NULL);                  /* Не должно вызывать краш */
    printf("Все вызовы с NULL безопасно обработаны!\n");

    /* --------------------------------------------------------------------- */
    /* 7. Уничтожение списка и повторный вызов (идемпотентность)             */
    /* --------------------------------------------------------------------- */
    printf("\n--- [7] Уничтожение списка и повторный destroy ---\n");
    list_push_back(&list, 1);
    list_push_back(&list, 2);
    list_push_back(&list, 3);
    print_list_state(&list, "Перед уничтожением");

    list_destroy(&list);
    print_list_state(&list, "После первого list_destroy");

    list_destroy(&list);
    print_list_state(&list, "После повторного list_destroy (идемпотентность)");

    /* --------------------------------------------------------------------- */
    /* 8. Стресс-тест памяти (Критерий приемки ТЗ: 100 циклов по 1000 узлов) */
    /* --------------------------------------------------------------------- */
    printf("\n--- [8] Стресс-тест: 100 циклов создания и уничтожения 1000 узлов ---\n");
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
        printf("Стресс-тест успешно пройден: %d циклов по %d элементов без ошибок!\n",
               NUM_CYCLES, NODES_PER_CYCLE);
    } else {
        printf("Ошибка во время стресс-теста!\n");
    }

    printf("\n=========================================================\n");
    printf("  Все демонстрационные сценарии успешно завершены!       \n");
    printf("=========================================================\n");

    return 0;
}
