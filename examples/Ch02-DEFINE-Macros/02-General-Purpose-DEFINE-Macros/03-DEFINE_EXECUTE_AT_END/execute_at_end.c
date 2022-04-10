/********************************************************************
   UDF for integrating turbulent dissipation and displaying it in the
   console at the end of the current iteration or time step
 *********************************************************************/

/**
 * The following UDF, named execute_at_end, integrates the turbulent dissipation over
 * the entire domain using DEFINE_EXECUTE_AT_END and displays it in the console at the
 * end of the current iteration or time step. It can be executed as an interpreted or
 * compiled UDF in Ansys Fluent.
 */

#include "udf.h"

DEFINE_EXECUTE_AT_END(execute_at_end)
{

  Domain *d;
  Thread *t;
  /* Integrate dissipation. */
  real sum_diss = 0.;
  cell_t c;
  d = Get_Domain(1); /* mixture domain if multiphase */

  thread_loop_c(t, d)
  {
    if (FLUID_THREAD_P(t))
    {
      begin_c_loop(c, t)
          sum_diss += C_D(c, t) * C_VOLUME(c, t);
      end_c_loop(c, t)
    }
  }

  printf("Volume integral of turbulent dissipation: %g\n", sum_diss);
  fflush(stdout);
}
