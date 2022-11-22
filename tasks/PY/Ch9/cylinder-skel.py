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
#  Purpose: Creating a 2-dimensional topology.                  # 
#                                                               # 
#  Contents: Python code, buffer send version (comm.Send)       # 
#                                                               # 
#################################################################

from mpi4py import MPI
import numpy as np

max_dims = 2

rcv_buf = np.empty((),dtype=np.intc)

dims = [0]*max_dims
periods = [False]*max_dims
coords = np.empty((max_dims), dtype=np.intc)

status = MPI.Status()

# Get process info.
comm_world = MPI.COMM_WORLD
size = comm_world.Get_size()

# Set cartesian topology.
dims[0] = _____;          dims[1] = _____
periods[0] = True;    periods[1] = False
reorder = True
 
dims = MPI._____________(____, ____)
new_comm = comm_world.Create_cart(dims=dims, periods=periods, reorder=reorder)

# Get coords
my_rank = new_comm.Get_rank()
my_coords = _____________________________

# Calculate left and right neihbors' ranks based on my_coords,
#    use MPI_Cart_rank
# hint: MPI_Cart_rank allows out of bound for cyclic dimensions!
for i in range(max_dims):
   coords[i] = my_coords[i]

coords[0] = ________________; left = new_comm.Get_cart_rank(______)
coords[0] = ________________; right = new_comm.Get_cart_rank(______)

# Compute global sum.

sum = 0
snd_buf = np.array(my_rank, dtype=np.intc)
 
for i in range(_______):
   request = new_comm.Issend((snd_buf, 1, MPI.INT), right, 17)

   new_comm.Recv((rcv_buf, 1, MPI.INT), left,  17, status)

   request.Wait(status)

   np.copyto(snd_buf, rcv_buf)
   sum += rcv_buf

print("PE{:2d}, Coords=({},{}): Sum = {}".format( 
          my_rank, my_coords[0], my_coords[1], sum))
