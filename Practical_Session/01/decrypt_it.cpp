#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void decrypt(char *text, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base - shift + 26) % 26 + base;
        }
        text[i] = c;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <shift> <text>\n", argv[0]);
        return 1;
    }
    int shift = atoi(argv[1]);
    char text[256];
    strcpy(text, argv[2]);

    decrypt(text, shift);
    printf("%s\n", text);
    return 0;
}
