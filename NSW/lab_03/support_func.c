#include <sys/socket.h>
#include <sys/types.h>

#include <sys/time.h>

#include <string.h>

void set_recv_timer (int sockfd, time_t tv_sec)
{
    struct timeval tv;

    tv.tv_sec = tv_sec;
    tv.tv_usec = 0;
    setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, (socklen_t) sizeof (tv));
}


/* reverse:  переворачиваем строку s на месте */
void reverse(char *s)
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


/* itoa:  конвертируем n в символы в s */
void itoa(int n, char *s)
{
    int i, sign;

    if ((sign = n) < 0)  /* записываем знак */
        n = -n;          /* делаем n положительным числом */
    i = 0;
    do {       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0';   /* берем следующую цифру */
    } while ((n /= 10) > 0);     /* удаляем */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
