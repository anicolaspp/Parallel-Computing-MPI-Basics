# Parallel-Computing-MPI-Basics

# Objectives:
The goal of this homework assignment is to practice basic MPI programming, including both point-to-point operations and collective operations. 

# Details:
The homework consists of four parts:

# 1. Block Send and Receive Operations (30%)
The MPI program will run with p number of MPI processes, ranked from 0 to p-1, where p >= 1. Imagine the situation where processes are linked together in a circle. That is, process 0 is linked to process 1, and process 1 is linked to process 2, and so on. Process p-1 is linked to process 0. The processes are going to send and receive messages only to and from their immediate neighbors on this circle.

In this task, each process will send two values and receives two values: Process i (0 < i < p) receives a single floating point number from process i-1, adds its own rank (that is, i) to the value, and then sends the updated value to the next process (i+1)%p. In the different direction, process i (0 < i < p) receives a single floating point number from process (i+1)%p, subtracts its own rank (i) from the value, and then sends to updated value to process i-1. After completing the above operations in both directions, the process terminates.

Process 0 behaves differently from the other processes. It starts and ends the operations. It first sends value 0 to process 1 and to process p-1, separately. Then, it receives one number from process 1 and another number from process p-1, although not necessarily in this order. Once it receives a number from its neighbor process, it prints out the value. After receiving and printing both values, the process terminates. 

For this task, you must use MPI_Send() and MPI_Recv() functions.

Name this program as send-recv. Here are some examples of running the program (note that the ordering of the output may not be the same from each run):
```
> mpirun -np 4 ./send-recv 
6
-6
> mpirun -np 10 ./send-recv
-45
45
> mpirun -np 1 ./send-recv
0
0
```

# 2. Immediate Send and Receive Operations (25%)
Perform the same operations as in the previous task, however, using immediate send and receive functions: MPI_Isend() and MPI_Irecv(). In addition, you should use wait functions, such as MPI_Waitall() or MPI_Waitsome(), for the processes to wait for the completion of multiple requests. Do not use busy wait (that is, constantly checking the completion of send/receive requests in a loop). 

Since semantically we performance the same operations, the results should be the same as in the previous task. Note that for process 0, one needs to send before receive. For other processes, one needs to receive before send.

*Name the program as isend-irecv*.

# 3. Reduce Operations (20%)
The above tasks can be easily performed using the reduce operation. Perform the tasks using MPI_Reduce(). Each process should call the MPI_Reduce() function twice, one with its rank and the other with the negative value of its rank. Process 0 should be the root process to receive the final reduced results and print them. Reduce functions are blocking functions. You can be sure the ordering of the results are fixed from each run. 
Name this program as reduce.

# 4. Sendrecv Operations (25%)

Separate sends and receives may cause trouble. If not being careful, the program can result in deadlock. Using MPI_Sendrecv() can sometimes solve this problem for some specific type of communications; for example, the communication pattern must be regular, communication peers need to be fixed with only send and receive, and there's no dependency among the different send/receive rounds. Unfortunately the previous tasks do not apply here. 

Let's change the task in this step. Here, we let each process send its own rank to its two immediate neighbors on the circle. That is, process i will send value i to process i-1 and (i+1)%p, separately. Process 0 will send 0 to process p-1 and process 1. Thus, each process will also receive two values from its neighbors. After that, each process will add the two received values together with its own rank, and prints out the result. In this case, each process computes the sum of the ranks of the three local processes (itself and two neighbors). 

For this task, you must use MPI_SendRecv() function.

Name this program as sndrcv. In this program each process eventually outputs its rank, followed by a colon and the local sum. Here are some examples of running the program (note that the ordering of the output may not be the same from each run):

```
> mpirun -np 4 ./sndrcv
0:4
1:3
2:6
3:5
> mpirun -np 1 ./sndrcv
0:0
```
