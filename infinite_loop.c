#include <stdio.h>
#include <unistd.h>

int main() {
    while (1) {
        printf(".");
        fflush(stdout); // Assure que le point est immédiatement affiché
        sleep(1); // Pause d'une seconde
    }
    return 0;
}