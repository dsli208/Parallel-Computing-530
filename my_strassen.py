from mpi4py import MPI
from random import random
import numpy as np
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

size = comm.size

def my_strassen(n):
    init_t = MPI.Wtime()
    size_n = int(pow(2, n))
    # print(size)

    A = np.array([[random() for i in range(n)] for j in range(n)])
    B = np.array([[random() for i in range(n)] for j in range(n)])

    # Split submatrices
    half_index = int(n / 2)

    A_11 = A[0:half_index, 0:half_index]
    A_12 = A[half_index:, 0:half_index]
    A_21 = A[0:half_index, half_index:]
    A_22 = A[half_index:, half_index:]

    B_11 = B[0:half_index, 0:half_index]
    B_12 = B[half_index:, 0:half_index]
    B_21 = B[0:half_index, half_index:]
    B_22 = B[half_index:, half_index:]

    # import pdb; pdb.set_trace()

    S_1 = np.subtract(B_12, B_22)
    S_2 = np.add(A_11, A_12)
    S_3 = np.add(A_21, A_22)
    S_4 = np.subtract(B_21, B_11)
    S_5 = np.add(A_11, A_22)
    S_6 = np.add(B_11, B_22)
    S_7 = np.subtract(A_12, A_22)
    S_8 = np.add(B_21, B_22)
    S_9 = np.subtract(A_11, A_21)
    S_10 = np.add(B_11, B_12)

    P_1 = np.dot(A_11, S_1)
    P_2 = np.dot(S_2, B_22)
    P_3 = np.dot(S_3, B_11)
    P_4 = np.dot(A_22, S_4)
    P_5 = np.dot(S_5, S_6)
    P_6 = np.dot(S_7, S_8)
    P_7 = np.dot(S_9, S_10)

    C_11 = np.add(np.subtract(np.add(P_5, P_4), P_2), P_6)
    C_12 = np.add(P_1, P_2)
    C_21 = np.add(P_3, P_4)
    C_22 = np.subtract(np.subtract(np.add(P_5, P_1), P_3), P_7)

    # print(C_11)
    # print(C_12)
    # print(C_21)
    C1 = np.concatenate((C_11, C_12), axis=1)
    C2 = np.concatenate((C_21, C_22), axis=1)
    C = np.concatenate((C1, C2), axis=0)

    # print(C)
    finit_t = MPI.Wtime()
    print("Strassen run time:")
    print(finit_t - init_t)

x = int(sys.argv[1])
my_strassen(x)
