/**
 * The following simple UDF named after_case and after_data, prints a message to the
 * console that contains the name of the library being loaded.
 */

#include "udf.h"

DEFINE_EXECUTE_AFTER_CASE(after_case, libname)
{
  Message("EXECUTE_AFTER_CASE called from $s\n", libname);
}

DEFINE_EXECUTE_AFTER_DATA(after_data, libname)
{
  Message("EXECUTE_AFTER_DATA called from $s\n", libname);
}
