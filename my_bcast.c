# include <stdio.h>
# include <stdlib.h>
# include "mpi.h"

int main(int argc, char** argv) {
  int num_elements = 5000000;
  int num_trials = 100;

  // MPI_Init(NULL, NULL);

  double total_my_bcast_time = 0.0;
  double total_mpi_bcast_time = 0.0;
  int* data = (int*)malloc(sizeof(int) * num_elements);

  int i;
  for (i = 0; i < num_trials; i++) {
    // Time my_bcast
    // Synchronize before starting timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time -= MPI_Wtime();
    my_bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    // Synchronize again before obtaining final time
     MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time += MPI_Wtime();

    // Time MPI_Bcast
    printf("Calling MPI_BCast");
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time -= MPI_Wtime();
    MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time += MPI_Wtime();
  }

  printf("%.2f\n", total_my_bcast_time);
  printf("%.2f", total_mpi_bcast_time);
  free(data);
  // MPI_Finalize();
}

int my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
  printf("%s", "Called my_bcast");
  int rank;
  MPI_Comm_rank(communicator, &rank);
  int size;
  MPI_Comm_size(communicator, &size);

  MPI_Request request;
  MPI_Status status;



  if (rank == root) {
    MPI_Request sr;
    // If we are the root process, send our data to everyone
    int i;
    for (i = 0; i < size; i++) {
      MPI_Start(&request);
      MPI_Wait(&request, &status);
      if (i != rank) {
        MPI_Isend(data, count, datatype, i, 0, communicator, &request);
      }
    }
    MPI_Request_free(&request);
  } else {
    printf("Started MPI_Request");
    // If we are a receiver process, receive the data from the root
    MPI_Irecv(data, count, datatype, root, 0, communicator,
             &request);
  }
}
