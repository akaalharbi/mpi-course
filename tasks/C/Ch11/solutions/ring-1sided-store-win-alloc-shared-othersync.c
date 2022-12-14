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
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/


#include <stdio.h>
#include <mpi.h>


int main (int argc, char *argv[])
{
  int my_rank_world, size_world;
  int my_rank_sm,    size_sm;
  MPI_Comm comm_sm;
  int snd_buf;
  int *rcv_buf_ptr;
  int right, left;
  int sum, i;

  MPI_Win     win;

  MPI_Request rq;
  MPI_Status status;
  int snd_dummy, rcv_dummy;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank_world);
  MPI_Comm_size(MPI_COMM_WORLD, &size_world);

  MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &comm_sm);
  MPI_Comm_rank(comm_sm, &my_rank_sm);
  MPI_Comm_size(comm_sm, &size_sm);
  if (my_rank_sm == 0)
  { if (size_sm == size_world) 
    {  printf("MPI_COMM_WORLD consists of only one shared memory region\n");
    }else
    { printf("MPI_COMM_WORLD is split into 2 or more shared memory islands\n");
  } }

  right = (my_rank_sm+1)         % size_sm;
  left  = (my_rank_sm-1+size_sm) % size_sm;

  /* Allocate the window. */
  MPI_Win_allocate_shared((MPI_Aint) sizeof(int), sizeof(int), MPI_INFO_NULL, comm_sm, &rcv_buf_ptr, &win);
  MPI_Win_lock_all(MPI_MODE_NOCHECK, win);

  sum = 0;
  snd_buf = my_rank_sm;

  for( i = 0; i < size_sm; i++) 
  {
/*  ... The local Win_syncs are needed to sync the processor and real memory. */
/*  ... The following pair of syncs is needed that the read-write-rule is fulfilled. */
    MPI_Win_sync(win);

/*    ... tag=17: posting to left that rcv_buf is exposed to left, i.e.,
                  the left process is now allowed to store data into the local rcv_buf  */
    MPI_Irecv(&rcv_dummy, 0, MPI_INT, right, 17, comm_sm, &rq);
    MPI_Send (&snd_dummy, 0, MPI_INT, left,  17, comm_sm);
    MPI_Wait(&rq, &status);

    MPI_Win_sync(win);


    /* MPI_Put(&snd_buf, 1, MPI_INT, right, (MPI_Aint) 0, 1, MPI_INT, win); */
    /*   ... is substited by (with offset "right-my_rank" to store into right neigbor's rcv_buf): */
    *(rcv_buf_ptr+(right-my_rank_sm)) = snd_buf;


/*  ... The following pair of syncs is needed that the write-read-rule is fulfilled. */
    MPI_Win_sync(win);

/*  ... The following communication synchronizes the processors in the way */
/*      that the origin processor has finished the store */
/*      before the target processor starts to load the data.   */
/*  ... tag=18: posting to right that rcv_buf was stored from left */
    MPI_Irecv(&rcv_dummy, 0, MPI_INT, left,  18, comm_sm, &rq);
    MPI_Send (&snd_dummy, 0, MPI_INT, right, 18, comm_sm);
    MPI_Wait(&rq, &status);

    MPI_Win_sync(win);

    
    snd_buf = *rcv_buf_ptr;
    sum += *rcv_buf_ptr;
  }

  printf ("World: %i of %i \tcomm_sm: %i of %i \tSum = %i\n", 
          my_rank_world, size_world, my_rank_sm, size_sm, sum);

  MPI_Win_unlock_all(win);
  MPI_Win_free(&win);

  MPI_Finalize();
}
