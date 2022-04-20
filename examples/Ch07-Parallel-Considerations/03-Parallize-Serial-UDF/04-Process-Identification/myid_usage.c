/**
 * myid is commonly used in conditional-if statements in parallel UDF code.
 * Below is some sample code that uses the global variable myid. In this example, the
 * total number of faces in a face thread is first computed by accumulation.
 * Then, if myid is not compute node-0, the number of faces is passed from all of the
 * compute nodes to compute node-0 using the message passing macro PRF_CSEND_INT.
 */

#include "udf.h"

int noface = 0;
begin_f_loop(f, tf) /* loops over faces in a face thread and
      computes number of faces */
{
  noface++;
}
end_f_loop(f, tf)

/* Pass the number of faces from node 1,2, ... to node 0 */

#if RP_NODE if (myid != node_zero)
{
  PRF_CSEND_INT(node_zero, &noface, 1, myid);
}
#endif