
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Використання: %s <команда> [аргументи...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Помилка fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Дочірній процес */
        printf("Дочірній (PID=%d) запускає команду...\n", getpid());
        
        execvp(argv[1], &argv[1]);
        
        /* Якщо execvp повернувся, значить сталася помилка */
        perror("Помилка execvp");
        exit(EXIT_FAILURE);
    }

    printf("Батько (PID=%d) очікує дочірній (PID=%d)\n", getpid(), pid);
    
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        printf("Дочірній завершився з кодом %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Дочірній був завершений сигналом №%d\n", WTERMSIG(status));
    }

    return 0;
}
