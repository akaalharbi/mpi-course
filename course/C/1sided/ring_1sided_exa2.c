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
 * Authors: Joel Malard, Alan Simpson,            (EPCC)        *
 *          Rolf Rabenseifner, Traugott Streicher (HLRS)        *
 *                                                              *
 * Contact: rabenseifner@hlrs.de                                * 
 *                                                              *  
 * Purpose: A program to try out one-sided communication        *
 *          with window=rcv_buf and MPI_PUT to put              *
 *          local snd_buf value into remote window (rcv_buf).   *
 *          With Start-Post-Complete-Wait synchronization.      *
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/


#include <stdio.h>
#include <mpi.h>


int main (int argc, char *argv[])
{
  int my_rank, size;
  int snd_buf, rcv_buf;
  int right, left;
  int sum, i;

  MPI_Win     win;
  MPI_Group   grp_world, grp_left, grp_right;


  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  right = (my_rank+1)      % size;
  left  = (my_rank-1+size) % size;
/* ... this SPMD-style neighbor computation with modulo has the same meaning as: */
/* right = my_rank + 1;          */
/* if (right == size) right = 0; */
/* left = my_rank - 1;           */
/* if (left == -1) left = size-1;*/

  /* Create the window. */

  MPI_Win_create(&rcv_buf, (MPI_Aint) sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

  /* Get nearest neighbour rank as group-arguments. */
  MPI_Comm_group(MPI_COMM_WORLD, &grp_world);
  MPI_Group_incl(grp_world, 1, &left,  &grp_left);
  MPI_Group_incl(grp_world, 1, &right, &grp_right);
  MPI_Group_free(&grp_world);

  sum = 0;
  snd_buf = my_rank;

  for( i = 0; i < size; i++) 
  {
    MPI_Win_post (grp_left,  MPI_MODE_NOSTORE, win);
    MPI_Win_start(grp_right, 0, win);
    MPI_Put(&snd_buf, 1, MPI_INT, right, (MPI_Aint) 0, 1, MPI_INT, win);
    MPI_Win_complete(win);
    MPI_Win_wait    (win);
    
    snd_buf = rcv_buf;
    sum += rcv_buf;
  }

  printf ("PE%i:\tSum = %i\n", my_rank, sum);

  MPI_Finalize();
}
