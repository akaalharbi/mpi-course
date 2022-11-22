PROGRAM ring

!==============================================================!
!                                                              !
! This file has been written as a sample solution to an        !
! exercise in a course given at the High Performance           !
! Computing Centre Stuttgart (HLRS).                           !
! The examples are based on the examples in the MPI course of  !
! the Edinburgh Parallel Computing Centre (EPCC).              !
! It is made freely available with the understanding that      !
! every copy of this file must include this header and that    !
! HLRS and EPCC take no responsibility for the use of the      !
! enclosed teaching material.                                  !
!                                                              !
! Authors: Joel Malard, Alan Simpson,            (EPCC)        !
!          Rolf Rabenseifner, Traugott Streicher (HLRS)        !
!                                                              !
! Contact: rabenseifner@hlrs.de                                !
!                                                              !
! Purpose: A program to try out one-sided communication        !
!          with window=rcv_buf and MPI_PUT to put              !
!          local snd_buf value into remote window (rcv_buf).   !
!                                                              !
! Contents: F-Source                                           !
!                                                              !
!==============================================================!

  USE mpi_f08
  IMPLICIT NONE

  INTEGER :: my_rank, size
  INTEGER :: right, left
  INTEGER :: i, sum
  INTEGER, ASYNCHRONOUS :: snd_buf, rcv_buf
  TYPE(MPI_Win) :: win 
  INTEGER :: disp_unit
  INTEGER(KIND=MPI_ADDRESS_KIND) :: integer_size, lb
  INTEGER(KIND=MPI_ADDRESS_KIND) :: rcv_buf_size, target_disp

  CALL MPI_Init()
  CALL MPI_Comm_rank(MPI_COMM_WORLD, my_rank)
  CALL MPI_Comm_size(MPI_COMM_WORLD, size)

  right = mod(my_rank+1,      size)
  left  = mod(my_rank-1+size, size)

! CREATE THE WINDOW.
  CALL MPI_Type_get_extent(MPI_INTEGER, lb, integer_size)
  rcv_buf_size = 1 * integer_size
  disp_unit = integer_size
  CALL MPI_Win_create(rcv_buf, rcv_buf_size, disp_unit, MPI_INFO_NULL, MPI_COMM_WORLD, win)

  sum = 0
  snd_buf = my_rank

  DO i = 1, size

!    ... The compiler may move the read access to rcv_buf
!        in the previous loop iteration after the following 
!        1-sided MPI calls, because the compiler has no chance
!        to see, that rcv_buf will be modified by the following 
!        1-sided MPI calls.  Therefore a dummy routine must be 
!        called with rcv_buf as argument:
 
     IF (.NOT.MPI_ASYNC_PROTECTS_NONBLOCKING) CALL MPI_F_sync_reg(rcv_buf)
 
!    ... Now, the compiler expects that rcv_buf was modified,
!        because the compiler cannot see that MPI_F_SYNC_REG
!        did nothing. Therefore the compiler cannot move any
!        access to rcv_buf across this dummy call. 
 
     CALL MPI_Win_fence(MPI_MODE_NOSTORE + MPI_MODE_NOPRECEDE, win)

     target_disp = 0
     CALL MPI_Put(snd_buf, 1, MPI_INTEGER, right, target_disp, 1, MPI_INTEGER, win)

     CALL MPI_Win_fence(MPI_MODE_NOSTORE + MPI_MODE_NOPUT + MPI_MODE_NOSUCCEED, win)
 
!    ... The compiler has no chance to see, that rcv_buf was
!        modified. Therefore a dummy routine must be called
!        with rcv_buf as argument:
 
     IF (.NOT.MPI_ASYNC_PROTECTS_NONBLOCKING) CALL MPI_F_sync_reg(rcv_buf)
 
!    ... Now, the compiler expects that rcv_buf was modified,
!        because the compiler cannot see that MPI_F_SYNC_REG
!        did nothing. Therefore the compiler will use the new
!        value on the memory, instead of some old value in a
!        register.

     IF (.NOT.MPI_ASYNC_PROTECTS_NONBLOCKING) CALL MPI_F_sync_reg(snd_buf)
!    ... This dummy call with snd_buf in the argument list prevents
!        the following store access to snd_buf may be moved by the
!        compiler to a position between PUT(snd_buf...) and FENCE.

     snd_buf = rcv_buf
     sum = sum + rcv_buf

  END DO

  WRITE(*,*) "PE", my_rank, ": Sum =", sum

  CALL MPI_Win_free(win)

  CALL MPI_Finalize()

END PROGRAM
