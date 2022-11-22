      PROGRAM ring

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                              C
C This file has been written as a sample solution to an        C 
C exercise in a course given at the High Performance           C
C Computing Centre Stuttgart (HLRS).                           C
C The examples are based on the examples in the MPI course of  C
C the Edinburgh Parallel Computing Centre (EPCC).              C
C It is made freely available with the understanding that      C 
C every copy of this file must include this header and that    C
C HLRS and EPCC take no responsibility for the use of the      C
C enclosed teaching material.                                  C
C                                                              C
C Authors: Joel Malard, Alan Simpson,            (EPCC)        C
C          Rolf Rabenseifner, Traugott Streicher (HLRS)        C
C                                                              C
C Contact: rabenseifner@hlrs.de                                C 
C                                                              C  
C Purpose: Creating a 2-dimensional topology.                  C
C                                                              C
C Contents: F-Source                                           C
C                                                              C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


      IMPLICIT NONE

      INCLUDE "mpif.h"

      INTEGER to_right
      PARAMETER(to_right=201)

      INTEGER max_dims
      PARAMETER(max_dims=2)

      INTEGER ierror, my_rank, size

      INTEGER right, left

      INTEGER i, sum

      INTEGER snd_buf, rcv_buf

      INTEGER status(MPI_STATUS_SIZE)

      INTEGER request

      INTEGER(KIND=MPI_ADDRESS_KIND) iadummy

      INTEGER new_comm
      INTEGER dims(max_dims)
      LOGICAL reorder, periods(max_dims)
      INTEGER coords(max_dims) 


      CALL MPI_INIT(ierror)

      CALL MPI_COMM_RANK(MPI_COMM_WORLD, my_rank, ierror)
      CALL MPI_COMM_SIZE(MPI_COMM_WORLD, size, ierror)

C Set two-dimensional cartesian topology.
      dims(1) = 0       
      dims(2) = 0
      periods(1) = .TRUE.
      periods(2) = .TRUE.
      reorder = .TRUE.

      CALL MPI_DIMS_CREATE(size, max_dims, dims, ierror)
      CALL MPI_CART_CREATE(MPI_COMM_WORLD, max_dims, dims, 
     &                     periods, reorder, new_comm, ierror)
      CALL MPI_COMM_RANK(new_comm, my_rank, ierror)
      CALL MPI_CART_COORDS(new_comm,my_rank, max_dims, 
     &                     coords, ierror) 

C Get nearest neighbour ranks.

      CALL MPI_CART_SHIFT(new_comm, 0, 1, left, right, ierror)

C Compute sum.

      sum = 0
      snd_buf = my_rank

      DO i = 1, dims(1)

         CALL MPI_ISSEND(snd_buf, 1, MPI_INTEGER, right, to_right, 
     &                   new_comm, request, ierror)

         CALL MPI_RECV(rcv_buf, 1, MPI_INTEGER, left, to_right,
     &                 new_comm, status, ierror)

         CALL MPI_WAIT(request, status, ierror)

         CALL MPI_GET_ADDRESS(snd_buf, iadummy, ierror)

         snd_buf = rcv_buf
         sum = sum + rcv_buf

      END DO

      WRITE(*,*) "PE", my_rank, ", coords = (", coords(1),
     &           ",", coords(2), "): Sum =", sum

      CALL MPI_FINALIZE(ierror)

      END
