/********************************************************************
   UDF for integrating turbulent dissipation and displaying it in the
   console
 *********************************************************************/

/**
 * The following UDF, named my_adjust, integrates the turbulent dissipation over the
 * entire domain using DEFINE_ADJUST. This value is then displayed in the console.
 * The UDF is called once every iteration. It can be executed as an interpreted or
 * compiled UDF in Ansys Fluent.
 */

#include "udf.h"

DEFINE_ADJUST(my_adjust, d)
{
  Thread *t;
  /* Integrate dissipation. */
  real sum_diss = 0.;
  cell_t c;

  thread_loop_c(t, d)
  {
    begin_c_loop(c, t)
        sum_diss += C_D(c, t) *
                    C_VOLUME(c, t);
    end_c_loop(c, t)
  }

  printf("Volume integral of turbulent dissipation: %g\n", sum_diss);
}
