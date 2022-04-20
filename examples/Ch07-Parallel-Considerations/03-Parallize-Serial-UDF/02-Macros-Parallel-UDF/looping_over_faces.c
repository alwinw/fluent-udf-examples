
#include "udf.h"

begin_f_loop(f, tf);
/* each compute node checks whether or not it is the principal compute
 node with respect to the given face and thread  */

if PRINCIPAL_FACE_P (f, tf)
/* face is on the principal compute node, so get the area and pressure
vectors, and compute the total area and pressure for the thread
from the magnitudes */
{
  F_AREA(area, f, tf);
  total_area += NV_MAG(area);
  total_pres_a += NV_MAG(area) * F_P(f, tf);
}
end_f_loop(f, tf);

total_area = PRF_GRSUM1(total_area);
total_pres_a = PRF_GRSUM1(total_pres_a);