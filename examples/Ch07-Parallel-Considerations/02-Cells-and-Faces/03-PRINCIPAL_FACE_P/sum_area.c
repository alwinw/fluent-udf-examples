/**
 * You can use PRINCIPAL_FACE_P to test whether a given face is the principal face,
 * before including it in a face loop summation. In the sample source code below,
 * the area of a face is added to the total area only if it is the principal face.
 *
 * Important: PRINCIPAL_FACE_P can be used only in compiled UDFs.
 */

#include "udf.h"

begin_f_loop(f, t);
if PRINCIPAL_FACE_P (f, t) /* tests if the face is the principal face
               FOR COMPILED UDFs ONLY */
{
  F_AREA(area, f, t);         /* computes area of each face */
  total_area += NV_MAG(area); /* computes total face area by
                 accumulating magnitude of each
                 face's area */
}
end_f_loop(f, t)