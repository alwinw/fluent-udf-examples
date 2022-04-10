/**********************************************************************
 This file contains two UDFs: an execute on loading UDF that reserves three UDMs
 for libudf and renames the UDMs to enhance postprocessing,
 and an on-demand UDF that sets the initial value of the UDMs.
 **********************************************************************/

/**
 * The following source code contains two UDFs. The first UDF is an `EXECUTE_ON_LOADING`
 * function that is used to reserve three UDMs (using `Reserve_User_Memory_Vars`) for a
 * library and set unique names for the UDM locations (using `Set_User_Memory_Name`).
 * The second UDF is an `ON_DEMAND` function that is used to set the values of the UDM
 * locations after the solution has been initialized. The `ON_DEMAND` UDF sets the
 * initial values of the UDM locations using `udm_offset`, which is defined in the
 * on-loading UDF.
 * Note that the on demand UDF must be executed *after* the solution is initialized to
 * reset the initial values for the UDMs.
 * See [Reserving UDM Variables Using `Reserve_User_Memory_Vars` ](https://ansyshelp.ansys.com/Views/Secured/corp/v221/en/flu_udf/flu_udf_sec_udm_macros.html#flu_udf_sec_reserve_udm "3.2.12.7. Reserving UDM Variables Using Reserve_User_Memory_Vars")
 * and [`Set_User_Memory_Name` ](https://ansyshelp.ansys.com/Views/Secured/corp/v221/en/flu_udf/flu_udf_sec_udm_macros.html#flu_udf_set_udm_variable_names "3.2.12.1. Set_User_Memory_Name")
 * for more information on reserving and naming UDMs.
 */

#include "udf.h"

#define NUM_UDM 3
static int udm_offset = UDM_UNRESERVED;

DEFINE_EXECUTE_ON_LOADING(on_loading, libname)
{
  if (udm_offset == UDM_UNRESERVED)
    udm_offset =
        Reserve_User_Memory_Vars(NUM_UDM);

  if (udm_offset == UDM_UNRESERVED)
    Message("\nYou need to define up to %d extra UDMs in GUI and "
            "then reload current library %s\n",
            NUM_UDM, libname);
  else
  {
    Message("%d UDMs have been reserved by the current "
            "library %s\n",
            NUM_UDM, libname);

    Set_User_Memory_Name(udm_offset, "lib1-UDM-0");
    Set_User_Memory_Name(udm_offset + 1, "lib1-UDM-1");
    Set_User_Memory_Name(udm_offset + 2, "lib1-UDM-2");
  }
  Message("\nUDM Offset for Current Loaded Library = %d", udm_offset);
}

DEFINE_ON_DEMAND(set_udms)
{
  Domain *d;
  Thread *ct;
  cell_t c;
  int i;

  d = Get_Domain(1);

  if (udm_offset != UDM_UNRESERVED)
  {
    Message("Setting UDMs\n");

    for (i = 0; i < NUM_UDM; i++)
    {
      thread_loop_c(ct, d)
      {
        begin_c_loop(c, ct)
        {
          C_UDMI(c, ct, udm_offset + i) = 3.0 + i / 10.0;
        }
        end_c_loop(c, ct)
      }
    }
  }
  else
    Message("UDMs have not yet been reserved for library 1\n");
}
