/**
 * In the following example, the host process on Windows copies the file from its local
 * directory e:\udfs\test.bat to the directory /tmp on the remote Linux nodes.
 */

#include "udf.h"

DEFINE_ON_DEMAND(host_to_node_sync)
{
#if RP_HOST
  int total_bytes_copied = host_to_node_sync_file("e:\\udfs\\test.dat.h5");
#endif
#if RP_NODE
  int total_bytes_copied = host_to_node_sync_file("/tmp");
  /* The file /tmp/test.dat.h5 can be opened now */
#endif
  printf("Total number of bytes copied is %d\n", total_bytes_copied);
}