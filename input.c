#include <stdio.h>
#include <string.h>
#include "input.h"

static void flushLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int readInt(const char *prompt) {
    int value;

    while (1) {
        if (prompt) printf("%s", prompt);

        if (scanf("%d", &value) == 1) {
            flushLine();
            return value;
        }

        flushLine();
        printf("Invalid input. Please enter a number.\n");
    }
}

void readTwoInts(const char *prompt, int *a, int *b) {
    while (1) {
        if (prompt) printf("%s", prompt);

        if (scanf("%d %d", a, b) == 2) {
            flushLine();
            return;
        }

        flushLine();
        printf("Invalid input. Please enter two numbers.\n");
    }
}

void readLine(char *str, size_t size) {
    if (fgets(str, size, stdin) == NULL) {
        str[0] = '\0';
        return;
    }
    str[strcspn(str, "\n")] = '\0';
}
