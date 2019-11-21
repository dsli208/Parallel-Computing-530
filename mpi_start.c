#include "mpi.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    MPI_Request r;
    MPI_Status s;
    int flag;
    int buf[10];
    int rbuf[10];
    int tag = 27;
    int dest = 0;
    int rank, size, i;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    /* Create a persistent send request */
    MPI_Send_init( buf, 10, MPI_INT, dest, tag, MPI_COMM_WORLD, &r );

    /* Use that request */
    if (rank == 0) {
        int i;
        MPI_Request *rr = (MPI_Request *)malloc(size * sizeof(MPI_Request));
        for (i=0; i<size; i++) {
            MPI_Irecv( rbuf, 10, MPI_INT, i, tag, MPI_COMM_WORLD, &rr[i] );
        }
        MPI_Start( &r );
        MPI_Wait( &r, &s );
        MPI_Waitall( size, rr, MPI_STATUSES_IGNORE );
        free(rr);
    }
    else {
        MPI_Start( &r );
        MPI_Wait( &r, &s );
    }

    MPI_Request_free( &r );


    if (rank == 0)
    {
        MPI_Request sr;
        /* Create a persistent receive request */
        MPI_Recv_init( rbuf, 10, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &r );
        MPI_Isend( buf, 10, MPI_INT, 0, tag, MPI_COMM_WORLD, &sr );
        for (i=0; i<size; i++) {
            MPI_Start( &r );
            MPI_Wait( &r, &s );
        }
        MPI_Wait( &sr, &s );
        MPI_Request_free( &r );
    }
    else {
        MPI_Send( buf, 10, MPI_INT, 0, tag, MPI_COMM_WORLD );
    }

    MPI_Finalize();
    return 0;
}
