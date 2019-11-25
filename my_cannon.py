from mpi4py import MPI
from random import random
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

size = comm.size

def my_cannon(n):
    init_t = MPI.Wtime()
    A = np.array([[random() for i in range(n)] for j in range(n)])
    B = np.array([[random() for i in range(n)] for j in range(n)])
    C = np.array([[0.0 for i in range(n)] for j in range(n)])

    # print(A)
    # print(B)
    # Skew the matrices
    # First by row
    for i in range(0, n):
        # use np.roll https://stackoverflow.com/questions/19878280/efficient-way-to-shift-2d-matrices-in-both-directions
        A[i] =  np.roll(A[i], -i, axis=0)

    # Then by column
    for i in range(0, n):
        B[:, i] = np.roll(B[:,i], -i, axis=0)

    # Now do the matrix multiplication
    # print(A)
    # print(B)

    for i in range(0, n):
        A = np.roll(A, -1, axis=0)
        B = np.roll(B, -2, axis=1)

        C = np.add(C, np.matmul(A, B))

    # print(C)
    finit_t = MPI.Wtime()
    print("Cannon run time:")
    print(finit_t - init_t)


my_cannon(4)
