/*********************************************************
   UDF called from a user-defined GUI dialog box to reset
   all user-defined memory locations
 **********************************************************/

/**
 * The following UDF, named reset_udm, resets all user-defined memory (UDM) values
 * when a reset button on a user-defined GUI dialog box is clicked.
 * The clicking of the button is represented by 0, which is passed to the UDF by the
 * Ansys Fluent solver.
 */

#include "udf.h"

DEFINE_EXECUTE_FROM_GUI(reset_udm, myudflib, mode)
{
  Domain *domain = Get_Domain(1); /* Get domain pointer */
  Thread *t;
  cell_t c;
  int i;

  /* Return if mode is not zero */
  if (mode != 0)
    return;

  /* Return if no User-Defined Memory is defined in Ansys Fluent */
  if (n_udm == 0)
    return;

  /* Loop over all cell threads in domain */
  thread_loop_c(t, domain)
  {
    /* Loop over all cells */
    begin_c_loop(c, t)
    {
      /* Set all UDMs to zero */
      for (i = 0; i < n_udm; i++)
      {
        C_UDMI(c, t, i) = 0.0;
      }
    }
    end_c_loop(c, t);
  }
}
