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

np_dtype = np.dtype([('i', np.intc), ('f', np.single)])
snd_buf = np.empty((),dtype=np_dtype)
rcv_buf = np.empty_like(snd_buf)
sum = np.empty_like(snd_buf)

array_of_blocklengths = [None]*2
array_of_displacements = [None]*2
array_of_types = [None]*2

status = MPI.Status()

comm_world = MPI.COMM_WORLD
my_rank = comm_world.Get_rank()
size = comm_world.Get_size()

right = (my_rank+1)      % size
left  = (my_rank-1+size) % size

# Set MPI datatypes for sending and receiving partial sums.
array_of_blocklengths[0] = ____     # PLEASE SUBSTITUTE ALL SKELETON CODE: ____
array_of_blocklengths[1] = ____

first_var_address = MPI.Get_address(snd_buf['i'])
second_var_address = MPI.Get_address(snd_buf['f'])

array_of_displacements[0] = 0
array_of_displacements[1] = ____ 

array_of_types[0] = ____
array_of_types[1] = ____

send_recv_type = MPI.Datatype.Create_struct(___ ... ___)
send_recv_type.____()

sum['i'] = 0;            sum['f'] = 0
snd_buf['i'] = my_rank;  snd_buf['f'] = 10*my_rank  # Step 1 = init

# ---------- original source code from MPI/tasks/PY/Ch4/ring.py - PLEASE MODIFY WHERE NEEDED: 
for i in range(size):
   request = comm_world.Issend((snd_buf, 1, MPI.INT), dest=right, tag=17)
   comm_world.Recv  ((rcv_buf, 1, MPI.INT), source=left,  tag=17, status=status)
   request.Wait(status)
   np.copyto(snd_buf, rcv_buf)
   sum += rcv_buf

print(f"PE{my_rank}:\tSum = {sum['i']}\t{sum['f']}")
