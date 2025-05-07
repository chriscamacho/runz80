#include <stdio.h>

extern int getch(void);

int main() {
    printf("Hello from Z80 with crt0 I/O!\r\n");
    printf("Enter a character: ");

    while(1) {
        int input = getch();
        printf("\r\nYou entered: %c\r\n", (char)input);
        printf("value = %i\r\n", (input & 0xff));
    }
//__asm 
//    halt
//__endasm;

    return 0;
}
