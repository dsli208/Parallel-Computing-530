#!/bin/bash
#
#SBATCH --job-name=test
#SBATCH --output=res2a.txt
#SBATCH --ntasks-per-node=40
#SBATCH --nodes=1
#SBATCH -p short-40core

module load shared
module load anaconda/2
module load mvapich2/gcc/64/2.2rc1
module load intel/compiler/64/2017/17.0.0
module load intel/mkl/64/2017/0.098
module load intel/mpi/64/2017/0.098


mpirun python my_cannon.py 1536
