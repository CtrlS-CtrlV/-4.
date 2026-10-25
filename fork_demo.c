/* fork_demo.c — створення дочірнього процесу через fork() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    printf("=== Демонстрація fork() ===\n");
    printf("Головний процес: PID = %d\n\n", getpid());

    pid_t pid = fork();

    if (pid == -1) {
        perror("Помилка fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Дочірній процес */
        printf("-> [Дочірній] PID: %d, PPID: %d\n", getpid(), getppid());
        printf("-> [Дочірній] Виконую sleep(1) для паралельного виконання...\n");
        sleep(1);
        printf("-> [Дочірній] Завершую роботу.\n");
        exit(EXIT_SUCCESS);
    } else {
        /* Батьківський процес */
        printf("[Батьківський] PID: %d, створено дочірній процес з PID: %d\n", getpid(), pid);
        printf("[Батьківський] Очікую завершення дочірнього процесу...\n");
        
        int status;
        wait(&status);
        
        printf("[Батьківський] Дочірній процес успішно завершив роботу.\n");
    }

    return 0;
}
