/**
 * UDF that writes a complete dataset to a CFF file, sets an attribute for the dataset
 * and then reads it back.  These demonstrate the use of the following helper functions:
 *    Write_Complete_User_Dataset
 *    Write_User_Attributes
 *    Read_Complete_User_Dataset
 *    Read_User_Attributes
 */

#include "udf.h"
#include "hdfio.h"

#define ELEM_COUNT 50

DEFINE_RW_HDF_FILE(write_complete_dataset, filename)
{
  size_t i, nelems = ELEM_COUNT;
  real *ptr = NULL;
  char *path = "/test/complete_data";

  /**
   * Assign equal number of elements in all the nodes
   * and fill with some values. Write a dataset with
   * all the values at the end. Also write an attribute
   * with total number of elements.
   */
#if RP_NODE
  ptr = (real *)CX_Malloc(sizeof(real) * nelems);
  for (i = 0; i < nelems; ++i)
  {
    ptr[i] = (real)((myid + 1) * i);
  }
#endif
  Write_Complete_User_Dataset(filename, path, ptr, nelems);
  Write_User_Attributes(filename, path,
                        "totalElems", MPT_SIZE_T, (size_t)(nelems * compute_node_count),
                        NULL);
  if (NNULLP(ptr))
  {
    CX_Free(ptr);
  }
}

DEFINE_RW_HDF_FILE(read_complete_dataset, filename)
{
  size_t nelems = 0, totalElems = 0;
  real *ptr = NULL;
  char *path = "/test/complete_data";

  /* Read complete dataset and check the elements. */
  Read_User_Attributes(filename, path,
                       "totalElems", MPT_SIZE_T, &totalElems,
                       NULL);
#if RP_NODE
  nelems = totalElems / compute_node_count;
  ptr = (real *)CX_Malloc(sizeof(real) * nelems);
#endif
  Read_Complete_User_Dataset(filename, path, ptr, nelems);
  if (NNULLP(ptr))
  {
    CX_Free(ptr);
  }
}
