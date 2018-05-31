   #include <stdio.h>
   #include <math.h>
   #include <mpi.h>

   #define PI 3.1415926535

   main(int argc, char **argv) 
   {
      int my_id, root_process, num_procs, ierr, num_intervals, i;
      double rect_width, area, sum, x_middle, partial_sum;
      MPI_Status status;
      root_process = 0;
      ierr = MPI_Init(&argc, &argv);
      ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
      ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

      if(my_id == root_process) {
         printf("Please enter the number of intervals to interpolate: ");
         scanf("%i", &num_intervals);
      }
      ierr = MPI_Bcast(&num_intervals, 1, MPI_INT, root_process, 
            MPI_COMM_WORLD);

      rect_width = PI / num_intervals;
      partial_sum = 0;
      for(i = my_id + 1; i <num_intervals + 1; i += num_procs) { 
         x_middle = (i - 0.5) * rect_width;
         area =  sin(x_middle) * rect_width; 
         partial_sum = partial_sum + area;
      } 
      printf("proc %i computes: %f\n", my_id, (float)partial_sum);
      ierr = MPI_Reduce(&partial_sum, &sum, 1, MPI_DOUBLE,
            MPI_SUM, root_process, MPI_COMM_WORLD);


      if(my_id == root_process) {
         printf("The integral is %f\n", (float)sum);

      } 


      ierr = MPI_Finalize();
   } 