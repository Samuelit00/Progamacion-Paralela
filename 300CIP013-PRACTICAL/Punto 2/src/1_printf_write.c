#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Line 1 ..\n");          // stdio (buffered)
    write(1, "Line 2 ", 7);          // unbuffered write to fd=1 (stdout)
    return 0;
}
