/**
 * There are a number of macros available in parallel Ansys Fluent that expand to
 * logical tests.
 */

#include "udf.h"

/* predicate definitions from para.h header file */

#define MULTIPLE_COMPUTE_NODE_P (compute_node_count > 1)
#define ONE_COMPUTE_NODE_P (compute_node_count == 1)
#define ZERO_COMPUTE_NODE_P (compute_node_count == 0)

/* predicate definitions from para.h header file */

#define I_AM_NODE_HOST_P (myid == host)
#define I_AM_NODE_ZERO_P (myid == node_zero)
#define I_AM_NODE_ONE_P (myid == node_one)
#define I_AM_NODE_LAST_P (myid == node_last)
#define I_AM_NODE_SAME_P(n) (myid == (n))
#define I_AM_NODE_LESS_P(n) (myid < (n))
#define I_AM_NODE_MORE_P(n) (myid > (n))

/* predicate definitions from para.h header file */
#define PRINCIPAL_FACE_P(f, t) (!TWO_CELL_FACE_P(f, t) || \
                                PRINCIPAL_TWO_CELL_FACE_P(f, t))

#define PRINCIPAL_TWO_CELL_FACE_P(f, t)                    \
  (!(I_AM_NODE_MORE_P(C_PART(F_C0(f, t), THREAD_T0(t))) || \
     I_AM_NODE_MORE_P(C_PART(F_C1(f, t), THREAD_T1(t)))))