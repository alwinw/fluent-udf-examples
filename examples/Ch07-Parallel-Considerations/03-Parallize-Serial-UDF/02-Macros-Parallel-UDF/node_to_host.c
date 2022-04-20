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

/* integer and real variables passed from compute node-0 to host */
node_to_host_int_1(count);
node_to_host_real_7(len1, len2, width1, width2, breadth1, breadth2, vol);

/* string and array variables passed from compute node-0 to host */
char *string;
int string_length;
real vel[ND_ND];

node_to_host_string(string, string_length);
node_to_host_real(vel, ND_ND);