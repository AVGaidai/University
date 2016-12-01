#include <stdio.h>

int main (void){

    unsigned int e[5] = {0};

    __asm__ __volatile__ (

    "movl $0x02, %%eax \n\t" "xorl %%ebx, %%ebx \n\t"
    "xorl %%ecx, %%ecx \n\t" "xorl %%edx, %%edx \n\t"
    "cpuid "
    : "=a"(e[0]), "=b"(e[1]), "=c"(e[2]), "=d"(e[3]) );


    if ((e[3] && 0xFF) == 0x4E)
         printf ("L2: 6MByte, 24-way set associative, 64 byte line size");

    return (0);
}
