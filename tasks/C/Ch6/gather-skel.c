/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the High Performance           *
 * Computing Centre Stuttgart (HLRS).                           *
 * The examples are based on the examples in the MPI course of  *
 * the Edinburgh Parallel Computing Centre (EPCC).              *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * HLRS and EPCC take no responsibility for the use of the      *
 * enclosed teaching material.                                  *
 *                                                              *
 * Authors: Joel Malard, Alan Simpson, (EPCC)                   *
 *          Rolf Rabenseifner          (HLRS)                   *
 *                                                              *
 * Contact: rabenseifner@hlrs.de                                *
 *                                                              *
 * Purpose: Gathering data from all processes                   *
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char *argv[])
{
  int n;  double result;  // application-related data
  double *result_array;
  int my_rank, num_procs, rank; // MPI-related data

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  // doing some application work in each process, e.g.:
  result = 100.0 + 1.0 * my_rank;
  printf("I am process %i out of %i, result=%f \n", 
                       my_rank, num_procs, result);
  if (my_rank == 0)
  { result_array = malloc(sizeof(double) * num_procs);
  }

  // ----------- the following gathering of the results should --------------
  // -----------   be substituted by one call to MPI_Gather    --------------
  if (my_rank != 0)  // in all processes, except "root" process 0
  { // sending some results from all processes (except 0) to process 0:
    MPI_Send(&result, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
  } else  // only in "root" process 0
  { result_array[0] = result; // process 0's own result
    // receiving all these messages
    for (rank=1; rank<num_procs; rank++)
    { // result of processes 1, 2, ...
      MPI_Recv(&result_array[rank], 1, MPI_DOUBLE, rank, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
  // ------------------ end of the gathering algorithm ----------------------

  if (my_rank == 0)
  { for (rank=0; rank<num_procs; rank++)
      printf("I'm proc 0: result of process %i is %f \n", rank, result_array[rank]); 
  }

  MPI_Finalize();
}
