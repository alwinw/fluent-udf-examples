/*******************************************************************
   This function will write pressures and positions
    for a fluid zone to a file on the host machine
 ********************************************************************/

/**
 * Writing files in parallel is done in the following stages:
 * 1. The node_host process opens the file.
 * 2. Compute node-0 sends its data to the node_host.
 * 3. The other compute nodes send their data to compute node-0.
 * 4. Compute node-0 receives the data from the other compute nodes and sends it to the node_host.
 * 5. The node_host receives the data sent from all the compute nodes and writes it to the file.
 * 6. The node_host closes the file.
 * Since the NODE_HOST and NODE processes are performing different tasks, the example below appears long and utilizes a large number of compiler directives. If, however, as an exercise you make three copies of this example and in each copy delete the unused sections for either the NODE_HOST or NODE versions, then you will see that it is actually quite a simple routine.
 */

#include "udf.h"

#define FLUID_ID 2

DEFINE_ON_DEMAND(pressures_to_file)
{
  /* Different variables are needed on different nodes */
#if !RP_HOST
  Domain *domain = Get_Domain(1);
  Thread *thread;
  cell_t c;
#else
  int i;
#endif

#if !RP_NODE
  FILE *fp = NULL;
  char filename[] = "press_out.txt";
#endif

  int size; /* data passing variables */
  real *array;
  int pe;

#if !RP_HOST
  thread = Lookup_Thread(domain, FLUID_ID);
#endif
#if !RP_NODE
  if ((fp = fopen(filename, "w")) == NULL)
    Message("\n Warning: Unable to open %s for writing\n", filename);
  else
    Message("\nWriting Pressure to %s...", filename);
#endif
    /* UDF Now does 2 different things depending on NODE or HOST */

#if RP_NODE
  /* Each Node loads up its data passing array */
  size = THREAD_N_ELEMENTS_INT(thread);
  array = (real *)malloc(size * sizeof(real));
  begin_c_loop_int(c, thread)
      array[c] = C_P(c, thread);
  end_c_loop_int(c, thread)

      /* Set pe to destination node */
      /* If on node_0 send data to host */
      /* Else send to node_0 because */
      /*  compute nodes connect to node_0 & node_0 to host */
      pe = (I_AM_NODE_ZERO_P) ? node_host : node_zero;
  PRF_CSEND_INT(pe, &size, 1, myid);
  PRF_CSEND_REAL(pe, array, size, myid);
  free(array); /* free array on nodes after data sent */

  /* node_0 now collect data sent by other compute nodes */
  /*  and sends it straight on to the host */

  if (I_AM_NODE_ZERO_P)
    compute_node_loop_not_zero(pe)
    {
      PRF_CRECV_INT(pe, &size, 1, pe);
      array = (real *)malloc(size * sizeof(real));
      PRF_CRECV_REAL(pe, array, size, pe);
      PRF_CSEND_INT(node_host, &size, 1, myid);
      PRF_CSEND_REAL(node_host, array, size, myid);
      free((char *)array);
    }
#endif /* RP_NODE */

#if RP_HOST
  compute_node_loop(pe) /* only acts as a counter in this loop */
  {
    /* Receive data sent by each node and write it out to the file */
    PRF_CRECV_INT(node_zero, &size, 1, node_zero);
    array = (real *)malloc(size * sizeof(real));
    PRF_CRECV_REAL(node_zero, array, size, node_zero);

    for (i = 0; i < size; i++)
      fprintf(fp, "%g\n", array[i]);

    free(array);
  }
#endif /* RP_HOST */

#if !RP_NODE
  fclose(fp);
  Message("Done\n");
#endif
}