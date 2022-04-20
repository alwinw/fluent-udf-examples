/**
 * The following is an example of a serial UDF that has been parallelized, so that it
 * can run on any version of Ansys Fluent (host, node).
 * The UDF, named face_av, is defined using an adjust function, computes a global sum
 * of pressure on a specific face zone, and computes its area average.
 */

#include "udf.h"

DEFINE_ADJUST(face_av, domain)
{
  /* Variables used by host, node versions */
  int surface_thread_id = 0;
  real total_area = 0.0;
  real total_force = 0.0;

/* "Parallelized" Sections */
#if !RP_HOST /* Compile this section for computing processes only since \
        these variables are not available on the host */
  Thread *thread;
  face_t face;
  real area[ND_ND];
#endif /* !RP_HOST */

/* Get the value of the thread ID from a user-defined Scheme variable */
#if !RP_NODE
  surface_thread_id = RP_Get_Integer("pres_av/thread-id");
  Message("\nCalculating on Thread # %d\n", surface_thread_id);
#endif /* !RP_NODE */

  /* To set up this user Scheme variable in cortex type */
  /* (rp-var-define 'pres_av/thread-id 2 'integer #f) */
  /* After set up you can change it to another thread's ID using : */
  /* (rpsetvar 'pres_av/thread-id 7) */
  /* Send the ID value to all the nodes */
  host_to_node_int_1(surface_thread_id);

#if RP_NODE Message("\nNode %d is calculating on thread # %d\n", myid,
      surface_thread_id);
#endif /* RP_NODE */

#if !RP_HOST
      /* thread is only used on compute processes */
      thread = Lookup_Thread(domain, surface_thread_id);
      begin_f_loop(face, thread)

          /* If this is the node to which face "officially" belongs,*/
          /* get the area vector and pressure and increment   */
          /* the total area and total force values for this node */
          if (PRINCIPAL_FACE_P(face, thread))
      {
        F_AREA(area, face, thread);
        total_area += NV_MAG(area);
        total_force += NV_MAG(area) * F_P(face, thread);
      }

      end_f_loop(face, thread)
          Message("Total Area Before Summing %f\n", total_area);
      Message("Total Normal Force Before Summing %f\n", total_force);

#if RP_NODE /* Perform node synchronized actions here */
      total_area = PRF_GRSUM1(total_area);
      total_force = PRF_GRSUM1(total_force);
#endif /* RP_NODE */

#endif /* !RP_HOST */

      /* Pass the node's total area and pressure to the Host for averaging */
      node_to_host_real_2(total_area, total_force);

#if !RP_NODE
      Message("Total Area After Summing: %f (m2)\n", total_area);
      Message("Total Normal Force After Summing %f (N)\n", total_force);
      Message("Average pressure on Surface %d is %f (Pa)\n",
              surface_thread_id, (total_force / total_area));
#endif /* !RP_NODE */
}