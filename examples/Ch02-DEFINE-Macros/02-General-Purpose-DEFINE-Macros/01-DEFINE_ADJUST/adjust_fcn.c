/********************************************************************
   UDF for defining user-defined scalars and their gradients
 *********************************************************************/

/**
 * The following UDF, named adjust_fcn, specifies a user-defined scalar as a function
 * of the gradient of another user-defined scalar, using DEFINE_ADJUST.
 * The function is called once every iteration. It is executed as a compiled UDF
 * in Ansys Fluent.
 */

#include "udf.h"

DEFINE_ADJUST(adjust_fcn, d)
{
  Thread *t;
  cell_t c;
  real K_EL = 1.0;

  /* Do nothing if gradient isn't allocated yet. */
  if (!Data_Valid_P())
    return;

  thread_loop_c(t, d)
  {
    if (FLUID_THREAD_P(t))
    {
      begin_c_loop_all(c, t)
      {
        C_UDSI(c, t, 1) +=
            K_EL * NV_MAG2(C_UDSI_G(c, t, 0)) * C_VOLUME(c, t);
      }
      end_c_loop_all(c, t)
    }
  }
}
