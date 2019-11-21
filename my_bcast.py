from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

def my_bcast(data, root):
    rank = comm.rank
    size = comm.size

    assert()

    if rank == root:
        for i in range(0, size):
            if i == root:
                continue

            req = comm.isend(data, dest=i)
            req.wait()

    else:
        req = comm.irecv(source=root)
        data = req.wait()
