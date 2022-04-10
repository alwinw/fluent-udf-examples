/***********************************************************************
   UDF for initializing flow field variables
 ************************************************************************/

/**
 * The following UDF, named my_init_func, initializes flow field variables in a solution.
 * It is executed once, at the beginning of the solution process.
 * The function can be executed as an interpreted or compiled UDF in Ansys Fluent.
 */

#include "udf.h"

DEFINE_INIT(my_init_func, d)
{
  cell_t c;
  Thread *t;
  real xc[ND_ND];

  /* loop over all cell threads in the domain */
  thread_loop_c(t, d)
  {

    /* loop over all cells */
    begin_c_loop_all(c, t)
    {
      C_CENTROID(xc, c, t);
      if (sqrt(ND_SUM(pow(xc[0] - 0.5, 2.),
                      pow(xc[1] - 0.5, 2.),
                      pow(xc[2] - 0.5, 2.))) < 0.25)
        C_T(c, t) = 400.;
      else
        C_T(c, t) = 300.;
    }
    end_c_loop_all(c, t)
  }
}
