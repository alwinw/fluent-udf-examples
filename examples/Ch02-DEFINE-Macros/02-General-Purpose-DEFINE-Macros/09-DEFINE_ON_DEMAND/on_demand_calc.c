/**********************************************************************
   UDF to calculate temperature field function and store in
   user-defined memory. Also print min, max, avg temperatures.
 ***********************************************************************/

/**
 * The following UDF, named on_demand_calc, computes and prints the minimum, maximum,
 * and average temperatures for the current data field. It then computes a temperature
 * function and stores it in user-defined memory location 0 (which is allocated as
 * described in Cell Macros).
 * After you hook the on-demand UDF (as described in Hooking DEFINE_ON_DEMAND UDFs),
 * the field values for f(T) will be available in drop-down lists in postprocessing
 * dialog boxes in Ansys Fluent. You can select this field by choosing User Memory 0
 * in the User-Defined Memory... category.
 * If you write a data file after executing the UDF, the user-defined memory field will
 * be saved to the data file.
 * This source code can be interpreted or compiled in Ansys Fluent.
 */

#include "udf.h"

DEFINE_ON_DEMAND(on_demand_calc)
{
  Domain *d; /* declare domain pointer since it is not passed as an
      argument to the DEFINE macro */
  real tavg = 0.;
  real tmax = 0.;
  real tmin = 0.;
  real temp, volume, vol_tot;
  Thread *t;
  cell_t c;
  d = Get_Domain(1); /* Get the domain using Ansys Fluent utility */

  /* Loop over all cell threads in the domain */
  thread_loop_c(t, d)
  {

    /* Compute max, min, volume-averaged temperature */

    /* Loop over all cells */
    begin_c_loop(c, t)
    {
      volume = C_VOLUME(c, t); /* get cell volume */
      temp = C_T(c, t);        /* get cell temperature */

      if (temp < tmin || tmin == 0.)
        tmin = temp;
      if (temp > tmax || tmax == 0.)
        tmax = temp;

      vol_tot += volume;
      tavg += temp * volume;
    }
    end_c_loop(c, t)

        tavg /= vol_tot;

    printf("\n Tmin = %g  Tmax = %g  Tavg = %g\n", tmin, tmax, tavg);

    /* Compute temperature function and store in user-defined memory*/
    /*(location index 0)                 */

    begin_c_loop(c, t)
    {
      temp = C_T(c, t);
      C_UDMI(c, t, 0) = (temp - tmin) / (tmax - tmin);
    }
    end_c_loop(c, t)
  }
}
