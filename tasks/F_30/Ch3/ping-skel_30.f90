PROGRAM ping

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
! Purpose: A program to try MPI_Ssend and MPI_Recv.            !
!                                                              !
! Contents: F-Source                                           !
!                                                              !
!==============================================================!

  USE mpi_f08

  IMPLICIT NONE

  TYPE(MPI_Status) :: status
  REAL :: buffer(1)
  INTEGER :: i, my_rank

  CALL MPI_Init()
  CALL MPI_Comm_rank(MPI_COMM_WORLD, my_rank)

        WRITE(*,*) 'I am ', my_rank, ' before send ping'

        WRITE(*,*) 'I am ', my_rank, ' after  recv ping'

  CALL MPI_Finalize()

END PROGRAM
