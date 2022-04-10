/**
 * The following simple UDF named report_version, prints a message on the console
 * that contains the version and release number of the library being loaded.
 */

#include "udf.h"

static int version = 1;
static int release = 2;

DEFINE_EXECUTE_ON_LOADING(report_version, libname)
{
  Message("\nLoading %s version %d.%d\n", libname, version, release);
}
