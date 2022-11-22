#!/usr/bin/env python3

#################################################################
#                                                               #
#  This file has been written as a sample solution to an        #
#  exercise in a course given at the High Performance           #
#  Computing Centre Stuttgart (HLRS).                           #
#  The examples are based on the examples in the MPI course of  #
#  the Edinburgh Parallel Computing Centre (EPCC).              #
#  It is made freely available with the understanding that      #
#  every copy of this file must include this header and that    #
#  HLRS and EPCC take no responsibility for the use of the      #
#  enclosed teaching material.                                  #
#                                                               #
#  Authors: Joel Malard, Alan Simpson,            (EPCC)        #
#           Rolf Rabenseifner, Traugott Streicher,              #
#           Tobias Haas (HLRS)                                  #
#                                                               #
#  Contact: rabenseifner@hlrs.de                                #
#                                                               #
#  Purpose: A program with derived datatypes.                   #
#                                                               #
#  Contents: Python code, buffer send version (comm.Send)       #
#                                                               #
#################################################################

from mpi4py import MPI
import numpy as np

np_dtype = np.dtype([('i', np.intc), ('j', np.intc)])

snd_buf = np.empty((),dtype=np_dtype)
rcv_buf = np.empty_like(snd_buf)
sum = np.empty_like(snd_buf)

status = MPI.Status()

comm_world = MPI.COMM_WORLD
my_rank = comm_world.Get_rank()
size = comm_world.Get_size()

right = (my_rank+1)      % size
left  = (my_rank-1+size) % size

sum['i'] = 0;            sum['j'] = 0
snd_buf['i'] = my_rank;  snd_buf['j'] = 10*my_rank  # Step 1 = init

for i in range(size): 
   request = comm_world.Issend((snd_buf, 2, MPI.INT), right, 17)  # Step 2a
   comm_world.Recv((rcv_buf, 2, MPI.INT), left, 17, status)       # Step 3
   request.Wait(status)                                                  # Step 2b
   np.copyto(snd_buf,rcv_buf)                          # Step 4
   sum['i'] += rcv_buf['i'];  sum['j'] += rcv_buf['j'] # Step 5

print(f"PE{my_rank}:\tSum = {sum['i']}\t{sum['j']}")
