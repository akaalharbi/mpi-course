#include <mpi.h>

#include "MPIX_interface_proposal.h"

/** Proposal for improvements of the MPI API related to application aware
 * hardware topology mapping.
 *
 * @author Christoph Niethammer and Rolf Rabenseifner, 
 * High Performance Computing Center Stuttgart (HLRS), 
 * University of Stuttgart, Germany, 2018.
 *
 * Copyright (c) 2019, HLRS, University of Stuttgart, Germany.
 * All rights reserved.
 *
 * This software is made available under the BSD 3-clause license ("BSD License 2.0"):
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its contributors
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER NOR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


int MPIX_Comm_ml_split_type(const MPI_Comm old_comm, const int ntype_levels, const int type_levels[], const MPI_Info info, MPI_Comm new_comms[]) {
  int level;
  MPI_Comm current_comm;
  current_comm = old_comm;
  for(level = 0; level < ntype_levels; ++level) {
    MPI_Comm_split_type(current_comm, type_levels[level], 0, info, &new_comms[level]);
  }
  return MPI_SUCCESS;
}
