//
// Created by Nicolas A Perez on 1/26/18.
//

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int p, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    int output;

    if (p <= 1) {
        printf("0\n0\n");
    } else if (id != 0) {

        MPI_Reduce(&id, &output, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        int negative_id = id * (-1);

        MPI_Reduce(&negative_id, &output, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    } else {

        int output;
        int data = 0;

        MPI_Reduce(&data, &output, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        int second_output;
        MPI_Reduce(&data, &second_output, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        printf("%d\n", output);
        printf("%d\n", second_output);
    }

    MPI_Finalize();

    return 0;
}