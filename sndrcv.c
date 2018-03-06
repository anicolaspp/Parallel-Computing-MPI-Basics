//
// Created by Nicolas A Perez on 1/26/18.
//

#include <stdio.h>
#include <mpi.h>

void show_result(int id, int total);
void run_processe(int p, int *id);
void run_master(int p, int *id);

int main(int argc, char *argv[]) {

    int p, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (p <= 1) {
        show_result(0, 0);
    } else if (id != 0) {
        run_processe(p, &id);
    } else {
        run_master(p, &id);
    }

    MPI_Finalize();

    return 0;
}

void run_master(int p, int *id) {
    int reading_buffer;
    MPI_Status status;

    MPI_Sendrecv(id, 1, MPI_INT, 1, 0, &reading_buffer, 1, MPI_INT, p - 1, 0, MPI_COMM_WORLD, &status);

    int sum = (*id) + reading_buffer;
    MPI_Sendrecv(id, 1, MPI_INT, p - 1, 0, &reading_buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

    show_result((*id), sum + reading_buffer);
}

void run_processe(int p, int *id) {
    int reading_buffer;
    MPI_Status status;

    MPI_Sendrecv(id, 1, MPI_INT, (*id) - 1, 0, &reading_buffer, 1, MPI_INT, (*id) - 1, 0, MPI_COMM_WORLD, &status);

    int sum = (*id) + reading_buffer;
    MPI_Sendrecv(id, 1, MPI_INT, ((*id) + 1) % p, 0, &reading_buffer, 1, MPI_INT, ((*id) + 1) % p, 0, MPI_COMM_WORLD, &status);

    show_result((*id), sum + reading_buffer);
}


void show_result(int id, int total) {
    printf("%d:%d\n", id, total);
}
