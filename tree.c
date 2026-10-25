
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3

int main(void)
{
    printf("=== Дерево процесів ===\n");
    printf("Кореневий процес: PID = %d\n\n", getpid());

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t child = fork();
        if (child == -1) {
            perror("Помилка fork (дочірній)");
            exit(EXIT_FAILURE);
        }

        if (child == 0) {
            /* Дочірній процес рівня 1 */
            printf("  Дочірній [%d]: PID=%d, PPID=%d\n", i + 1, getpid(), getppid());

            /* Створюємо внуковий процес */
            pid_t grandchild = fork();
            if (grandchild == -1) {
                perror("Помилка fork (внуковий)");
                exit(EXIT_FAILURE);
            }

            if (grandchild == 0) {
                /* Внуковий процес */
                printf("    Внуковий [%d]: PID=%d, PPID=%d\n", i + 1, getpid(), getppid());
                sleep(5);
                exit(EXIT_SUCCESS);
            }

            /* Дочірній чекає на внукового */
            wait(NULL);
            sleep(5);
            exit(EXIT_SUCCESS);
        }
    }

    sleep(1);
    printf("\n[Підказка] Дерево запущено. Можете переглянути його в іншому терміналі через:\n");
    printf("pstree -p %d\n\n", getpid());

    /* Очікування всіх дочірніх процесів */
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    printf("Усі дочірні та внукові процеси завершили роботу.\n");
    return 0;
}
