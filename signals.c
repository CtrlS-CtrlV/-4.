
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static volatile sig_atomic_t sigusr1_count = 0;
static volatile sig_atomic_t running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    const char *msg = "\n[SIGINT] Перехоплено Ctrl+C! Ігнорую...\n";
    write(STDOUT_FILENO, msg, strlen(msg));
}

void handle_sigusr1(int sig)
{
    (void)sig;
    sigusr1_count++;
    char msg[64];
    int len = snprintf(msg, sizeof(msg), "[SIGUSR1] Отримано користувацький сигнал #%d\n", (int)sigusr1_count);
    write(STDOUT_FILENO, msg, len);
}

void handle_sigterm(int sig)
{
    (void)sig;
    const char *msg = "[SIGTERM] Отримано запит на завершення\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    running = 0;
}

int main(void)
{
    printf("=== Демонстрація обробки сигналів ===\n");
    printf("PID процесу: %d\n", getpid());
    printf("Надішліть сигнали з іншого терміналу:\n");
    printf("  kill -SIGUSR1 %d\n", getpid());
    printf("  kill -SIGINT  %d\n", getpid());
    printf("  kill -SIGTERM %d\n", getpid());
    printf("Очікую сигнали...\n\n");

    /* Встановлення обробників через signal() */
    signal(SIGINT, handle_sigint);
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGTERM, handle_sigterm);

    /* Головний цикл очікування сигналів */
    while (running) {
        pause();  /* Блокує до отримання сигналу */
        printf("  (повернення з pause(), sigusr1_count = %d)\n", (int)sigusr1_count);
    }

    printf("\nПрограма завершується коректно.\n");
    printf("Всього отримано SIGUSR1: %d\n", (int)sigusr1_count);

    return EXIT_SUCCESS;
}
