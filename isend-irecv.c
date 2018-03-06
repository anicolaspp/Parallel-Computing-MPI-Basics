#include <stdio.h>
#include <mpi.h>

void send_to_both(int p, int id, MPI_Request const *requests, float from_left, float from_right);

void send_to_right(int p, int id, MPI_Request const *requests, float from_left);

void send_to_left(int id, MPI_Request const *requests, float from_right);

int main(int argc, char *argv[]) {

    int p, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (p <= 1) {
        printf("0\n0\n");
    } else if (id == 0) {

        MPI_Request requests[4];
        MPI_Status statuses[4];
        int index[4];

        float to_first, to_last = 0;
        MPI_Isend(&to_first, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&to_last, 1, MPI_FLOAT, p - 1, 0, MPI_COMM_WORLD, &requests[1]);

        int out_count;
        MPI_Waitsome(2, requests, &out_count, index, statuses);

        float data_from_one;
        MPI_Irecv(&data_from_one, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &requests[2]);

        float data_from_last;
        MPI_Irecv(&data_from_last, 1, MPI_FLOAT, p - 1, 0, MPI_COMM_WORLD, &requests[3]);

        MPI_Waitall(4, requests, statuses);

        printf("%f\n", data_from_one);
        printf("%f\n", data_from_last);

    } else {

        MPI_Request requests[4];
        MPI_Status statuses[4];
        int index[4];

        float from_left;
        MPI_Irecv(&from_left, 1, MPI_FLOAT, id - 1, 0, MPI_COMM_WORLD, &requests[0]);

        float from_right;
        MPI_Irecv(&from_right, 1, MPI_FLOAT, (id + 1) % p, 0, MPI_COMM_WORLD, &requests[1]);

        int out_count;
        MPI_Waitsome(2, requests, &out_count, index, statuses);

        if (out_count == 2) {
            send_to_both(p, id, requests, from_left, from_right);

        } else {
            if (index[0] == 0) {
                send_to_right(p, id, requests, from_left);

                MPI_Waitsome(2, requests, &out_count, index, statuses);

                send_to_left(id, requests, from_right);
            } else {
                send_to_left(id, requests, from_right);

                MPI_Waitsome(2, requests, &out_count, index, statuses);

                send_to_right(p, id, requests, from_left);
            }
        }
    }

    MPI_Finalize();

    return 0;
}

void send_to_left(int id, MPI_Request const *requests, float from_right) {
    float to_left = from_right - id;
    MPI_Isend(&to_left, 1, MPI_FLOAT, id - 1, 0, MPI_COMM_WORLD, &requests[3]);
}

void send_to_right(int p, int id, MPI_Request const *requests, float from_left) {
    float to_right = from_left + id;
    MPI_Isend(&to_right, 1, MPI_FLOAT, (id + 1) % p, 0, MPI_COMM_WORLD, &requests[2]);
}

void send_to_both(int p, int id, MPI_Request const *requests, float from_left, float from_right) {
    send_to_right(p, id, requests, from_left);
    send_to_left(id, requests, from_right);
}

