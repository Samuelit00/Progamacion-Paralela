// src/decrypt_it.c
// Usage: ./decrypt_it_c <rotation> <phrase...>  (rotation is how many to shift BACK)
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

static char rotate_char(char c, int k) {
    if (c >= 'A' && c <= 'Z') return (char)('A' + mod((c - 'A') + k, 26));
    if (c >= 'a' && c <= 'z') return (char)('a' + mod((c - 'a') + k, 26));
    return c;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <rotation> <phrase...>\n", argv[0]);
        return 1;
    }
    int k = atoi(argv[1]);
    k = -k; // shift backward for decrypt
    k = mod(k, 26);

    size_t len = 0;
    for (int i = 2; i < argc; ++i) len += strlen(argv[i]) + 1;
    char *buf = (char*)malloc(len + 1);
    if (!buf) return 1;
    buf[0] = '\0';
    for (int i = 2; i < argc; ++i) {
        strcat(buf, argv[i]);
        if (i + 1 < argc) strcat(buf, " ");
    }

    for (char *p = buf; *p; ++p) *p = rotate_char(*p, k);
    printf("%s\n", buf);
    free(buf);
    return 0;
}
