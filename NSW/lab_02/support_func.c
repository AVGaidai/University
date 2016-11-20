#include <sys/socket.h>
#include <sys/types.h>

#include <sys/time.h>

void set_recv_timer (int sockfd, time_t tv_sec)
{
    struct timeval tv;

    tv.tv_sec = tv_sec;
    tv.tv_usec = 0;
    setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, (socklen_t) sizeof (tv));
}
