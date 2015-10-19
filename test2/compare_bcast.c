// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// https://github.com/wesleykendall/mpitutorial/blob/gh-pages/tutorials/mpi-broadcast-and-collective-communication/code/compare_bcast.c
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header in tact.
//
// Comparison of MPI_Bcast with the my_bcast function
//
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator, int world_size, int world_rank) {

  if (world_rank == root) {
    // If we are the root process, send our data to everyone
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(data, count, datatype, i, 0, communicator);
      }
    }
  } else {
    // If we are a receiver process, receive the data from the root
    MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: compare_bcast num_elements num_trials\n");
    exit(1);
  }

  int world_rank, world_size, i;
  int num_elements = atoi(argv[1]);
  int num_trials = atoi(argv[2]);
  double total_my_bcast_time, total_mpi_bcast_time = 0.0;
  int* data = (int*)malloc(sizeof(int) * num_elements);
  assert(data != NULL);

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  for (i = 0; i < num_trials; i++) {
    // Time my_bcast
    // Synchronize before starting timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time -= MPI_Wtime();
    my_bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD, world_size, world_rank);
    // Synchronize again before obtaining final time
    MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time += MPI_Wtime();

    // Time MPI_Bcast
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time -= MPI_Wtime();
    MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time += MPI_Wtime();
  }

  // Print off timing information
  if (world_rank == 0) {
    int data_size = num_elements * (int)sizeof(int);
    // printf("Data size = %d, Trials = %d\n", num_elements * (int)sizeof(int),
    //        num_trials);
    // printf("Avg my_bcast time = %lf\n", total_my_bcast_time / num_trials);
    // printf("Avg MPI_Bcast time = %lf\n", total_mpi_bcast_time / num_trials);
    printf("my_bcast\t%d\t%d\t%lf\n", world_size, data_size, total_my_bcast_time / num_trials);
    printf("MPI_Bcast\t%d\t%d\t%lf\n", world_size, data_size, total_mpi_bcast_time / num_trials);
  }

  MPI_Finalize();
}
