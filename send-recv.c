//
// Created by Nicolas A Perez on 1/25/18.
//

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int p, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (p <= 1) {
        printf("0\n0\n");
    } else if (id == 0) {

        float to_first, to_last = 0;

        MPI_Send(&to_first, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&to_last, 1, MPI_FLOAT, p - 1, 0, MPI_COMM_WORLD);

        float data_from_one;

        MPI_Recv(&data_from_one, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("%f\n", data_from_one);

        float data_from_last;
        MPI_Recv(&data_from_last, 1, MPI_FLOAT, p - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("%f\n", data_from_last);
    } else {

        float from_left;
        MPI_Recv(&from_left, 1, MPI_FLOAT, id - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        float to_right = from_left + id;
        MPI_Send(&to_right, 1, MPI_FLOAT, (id + 1) % p, 0, MPI_COMM_WORLD);

        float from_right;
        MPI_Recv(&from_right, 1, MPI_FLOAT, (id + 1) % p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        float to_left = from_right - id;
        MPI_Send(&to_left, 1, MPI_FLOAT, id - 1, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

