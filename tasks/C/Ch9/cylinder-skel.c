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
 * Purpose: Creating a 2-dimensional topology.                  *
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/


#include <stdio.h>
#include <mpi.h>

#define max_dims 2


int main (int argc, char *argv[])
{
  int my_rank, size;
  int snd_buf, rcv_buf;
  int right, left;
  int sum, i;

  MPI_Comm    new_comm;
  int         dims[max_dims],
              periods[max_dims],
              reorder,
              my_coords[max_dims], coords[max_dims];

  MPI_Status  status;
  MPI_Request request;


  MPI_Init(&argc, &argv);

  /* Get process info. */
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* Set cartesian topology. */
  dims[0] = ____;    dims[1] = ____;
  periods[0] = 1;    periods[1] = 0;
  reorder = 1;
 
  MPI_Dims______________________________
  MPI_Cart_create(MPI_COMM_WORLD, max_dims, dims, periods,
                  reorder,&new_comm);

  /* Get coords */
  MPI_Comm_rank(new_comm, &my_rank);
  MPI________________________________________, my_coords); 

  /* Calculate left and right neihbors' ranks based on my_coords,
     use MPI_Cart_rank */
  /* hint: MPI_Cart_rank allows out of bound for cyclic dimensions! */
  for (i=0; i<max_dims; i++) coords[i]=my_coords[i];
  coords[0]=________________; MPI_Cart_rank(________________, &left);
  coords[0]=________________; MPI_Cart_rank(________________, &right);

  /* Compute global sum. */
 
  sum = 0;
  snd_buf = my_rank;

  for( i = 0; i < _______; i++) 
  {
    MPI_Issend(&snd_buf, 1, MPI_INT, right, 17, new_comm, &request);
    
    MPI_Recv  (&rcv_buf, 1, MPI_INT, left,  17, new_comm, &status);
    
    MPI_Wait(&request, &status);
    
    snd_buf = rcv_buf;
    sum += rcv_buf;
  }

  printf ("PE%2i, Coords=(%i,%i): Sum = %i\n", 
          my_rank, my_coords[0], my_coords[1], sum);

  MPI_Finalize();
}
