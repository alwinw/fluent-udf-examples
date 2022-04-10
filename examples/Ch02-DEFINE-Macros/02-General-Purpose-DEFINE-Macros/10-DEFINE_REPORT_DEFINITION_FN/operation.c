/**
 * This function computes the volumetric flow rate at an inlet, demonstrating the
 * ability to perform operations on an input variable and output the result for
 * plotting, printing, or writing to a file as part of a user-defined report definition.
 */

#include "udf.h"

DEFINE_REPORT_DEFINITION_FN(volume_flow_rate_inlet)
{
  real inlet_velocity = Get_Input_Parameter("vel_in");
  real inlet_area = 0.015607214;
  real volumeFlow = inlet_velocity * inlet_area;
  return volumeFlow;
}
