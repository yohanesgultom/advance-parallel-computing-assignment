// Author: Yohanes
// Matrix transpose using MPI_Alltoall

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int** create_matrix(int row, int col) {
    int i = 0;
    int **m = (int**) malloc(row * sizeof(int*));
    int *n = (int*) malloc(row * col * sizeof(int));
    for (i=0;i<row;i++) m[i] = &(n[i * col]);
    return m;
}

void init_matrix_rand(int **m, int row, int col, int max) {
    int i, j = 0;
    for (i=0;i<row;i++) {
        for (j=0;j<col;j++) {
            m[i][j] = rand() % (max+1);
        }
    }
}

int** create_rand_matrix(int world_size) {
    // due to the nature of MPI_Alltoall
    // the number of rows must be the same as the number of processors
    // to even more simplify the experiment we will use square matrix m[world_size x world_size]
    int **m = create_matrix(world_size, world_size);
    init_matrix_rand(m, world_size, world_size, 9);
    return m;
}

void print_matrix(int **m, int row, int col) {
    int i, j = 0;
    for (i=0;i<row;i++) {
        for (j=0;j<col;j++) {
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {

    int i, world_rank, world_size;
    double process_time, process_time_to_avg, comm_time = 0;
    int **rand_matrix, **trans_matrix = NULL;
    int *recv_buffer = (int*) malloc(world_size * sizeof(int));
    int *recv_buffer2 = (int*) malloc(world_size * sizeof(int));

    srand(time(NULL));
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // scatter rows
    if (world_rank == 0) {
        rand_matrix = create_rand_matrix(world_size);
        // print_matrix(rand_matrix, world_size, world_size);
        process_time -= MPI_Wtime();
        comm_time -= MPI_Wtime();
        MPI_Scatter(&(rand_matrix[0][0]), world_size, MPI_INT, &(recv_buffer[0]), world_size, MPI_INT, 0, MPI_COMM_WORLD);
        comm_time += MPI_Wtime();
        comm_time -= MPI_Wtime();
    } else {
        MPI_Scatter(NULL, 0, MPI_INT, &(recv_buffer[0]), world_size, MPI_INT, 0, MPI_COMM_WORLD);
    }
    // printf("Process #%d received: ", world_rank);
    // for (i = 0; i < world_size; i++) printf("%d", recv_buffer[i]);
    // printf("\n");

    // do transpose of scrattered rows
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Alltoall(recv_buffer, 1, MPI_INT, recv_buffer2, 1, MPI_INT, MPI_COMM_WORLD);
    // printf("Process #%d then received: ", world_rank);
    // for (i = 0; i < world_size; i++) printf("%d", recv_buffer2[i]);
    // printf("\n");

    // gather to form matrix
    //MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) {
        comm_time += MPI_Wtime();
        trans_matrix = create_matrix(world_size, world_size);
        comm_time -= MPI_Wtime();
        MPI_Gather(recv_buffer2, world_size, MPI_INT, &(trans_matrix[0][0]), world_size, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Gather(recv_buffer2, world_size, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }

    //MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) {
        comm_time += MPI_Wtime();
        process_time += MPI_Wtime();
        //print_matrix(trans_matrix, world_size, world_size);
        printf("%d\t%d\t%lf\t%lf\n", world_size, world_size * world_size, process_time, comm_time);

        free(rand_matrix[0]);
        free(trans_matrix[0]);
        free(rand_matrix);
        free(trans_matrix);
    }
    free(recv_buffer);
    free(recv_buffer2);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
