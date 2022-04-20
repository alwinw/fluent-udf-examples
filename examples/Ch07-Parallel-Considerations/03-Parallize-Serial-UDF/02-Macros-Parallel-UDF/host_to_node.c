/**
 * To send data from the host process to all the node processes (indirectly via compute
 * node-0), macros of the following form is used
 *
 * Note that these host_to_node communication macros do not need to be "protected" by
 * compiler directives for parallel UDFs, because all of these macros automatically do
 * the following:
 * - send the variable value if compiled as the host version
 * - receive and then set the local variable if compiled as a compute node version
 */

#include "udf.h"

/* integer and real variables passed from host to nodes */
host_to_node_int_1(count);
host_to_node_real_7(len1, len2, width1, width2, breadth1, breadth2, vol);

/* string and array variables passed from host to nodes */
char wall_name[] = "wall-17";
int thread_ids[10] = {1, 29, 5, 32, 18, 2, 55, 21, 72, 14};

host_to_node_string(wall_name, 8); /* remember terminating NUL character */
host_to_node_int(thread_ids, 10);