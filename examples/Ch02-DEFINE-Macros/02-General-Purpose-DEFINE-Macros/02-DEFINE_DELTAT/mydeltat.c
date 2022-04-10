/*********************************************************************
   UDF that changes the time step value for a time-dependent solution
 **********************************************************************/

/**
 * The following UDF, named `mydeltat`, is a simple function that shows how you can use
 *  `DEFINE_DELTAT` to change the value of the time step in a simulation.
 * First, `CURRENT_TIME` is used to get the value of the current simulation time
 * (which is assigned to the variable `flow_time`). Then, for the first `0.5` seconds
 * of the calculation, a time step of `0.1` is set. A time step of `0.2` is set for
 * the remainder of the simulation. The time step variable is then returned to the solver.
 * See [Time-Dependent Macros](https://ansyshelp.ansys.com/Views/Secured/corp/v221/en/flu_udf/flu_udf_RPVariables.html "3.5. Time-Dependent Macros")
 * for details on `CURRENT_TIME`.
 */

#include "udf.h"

DEFINE_DELTAT(mydeltat, d)
{
  real time_step;
  real flow_time = CURRENT_TIME;
  if (flow_time < 0.5)
    time_step = 0.1;
  else
    time_step = 0.2;
  return time_step;
}
