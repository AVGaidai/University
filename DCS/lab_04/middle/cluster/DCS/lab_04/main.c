#include <mpi.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[])
{
    MPI_Init (NULL, NULL);

    int world_size;

    MPI_Comm_size (MPI_COMM_WORLD, &world_size);

    int world_rank;

    MPI_Comm_rank (MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name (processor_name, &name_len);

    MPI_Barrier (MPI_COMM_WORLD);
    for (int i = 0; i <= world_size; ++i) {
        if (world_rank != i) usleep (1000);
        printf ( "Hello world from processor %s, rank %d"
                 " out of %d processors\n",
                 processor_name, world_rank, world_size   );
        printf ("pid:  %d\n", getpid ());
        printf ("ppid: %d\n", getppid ());
        printf ("uid:  %d\n", getuid ());
        printf ("euid: %d\n", geteuid ());
        printf ("gid:  %d\n", getgid ());
        printf ("egid: %d\n", getegid ());
        printf ("pgrp: %d\n", getpgrp ());
        printf ("sid:  %d\n\n", getsid (getpid ()));
        printf ("=============================================\n\n");
        break;
    }

    MPI_Finalize ();

    return 0;
}
