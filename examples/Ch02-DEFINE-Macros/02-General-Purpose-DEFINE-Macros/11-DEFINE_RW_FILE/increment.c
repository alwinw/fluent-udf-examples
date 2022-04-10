/***********************************************************************
   UDFs that increment a variable, write it to a legacy data file
   and read it back in
 ************************************************************************/

/**
 * The following C source code listing contains examples of functions that write
 * information to a legacy data file and read it back. These functions are concatenated
 * into a single source file that can be interpreted or compiled in Ansys Fluent.
 */

#include "udf.h"

int kount = 0; /* define global variable kount */

DEFINE_ADJUST(demo_calc, d)
{
  kount++;
  printf("kount = %d\n", kount);
}

DEFINE_RW_FILE(writer, fp)
{
  printf("Writing UDF data to legacy data file...\n");
#if !RP_NODE
  fprintf(fp, "%d", kount); /* write out kount to legacy data file */
#endif
}

DEFINE_RW_FILE(reader, fp)
{
  printf("Reading UDF data from legacy data file...\n");
#if !RP_NODE
  fscanf(fp, "%d", &kount); /* read kount from legacy data file */
#endif
}
