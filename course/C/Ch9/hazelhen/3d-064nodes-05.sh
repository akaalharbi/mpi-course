# cc -o halo_irecv_send_toggle_3dim_grid_solution.exe halo_irecv_send_toggle_3dim_grid_solution.c MPIX_?[ai]*.c
# qsub -lnodes=64:ppn=24,walltime=0:30:00 ./3d-064nodes-05.sh
date
cd MPI/00
time aprun -n 1536 -N 24 ./halo_irecv_send_toggle_3dim_grid_solution.exe < 3d-064nodes-05-inp.txt > 3d-064nodes-05-out.txt
date