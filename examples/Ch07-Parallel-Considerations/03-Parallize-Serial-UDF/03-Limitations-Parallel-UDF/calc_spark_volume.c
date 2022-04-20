/**
 * In the following example, the spark volume is calculated in the DEFINE_ADJUST
 * function and the value is stored in user-defined memory using C_UDMI.
 * The volume is then retrieved from user-defined memory and used in the DEFINE_SOURCE UDF.
 */

#include "udf.h"

/* These variables will be passed between the ADJUST and SOURCE UDFs */

static real spark_center[ND_ND] = {ND_VEC(20.0e-3, 1.0e-3, 0.0)};
static real spark_start_angle = 0.0, spark_end_angle = 0.0;
static real spark_energy_source = 0.0;
static real spark_radius = 0.0;
static real crank_angle = 0.0;

DEFINE_ADJUST(adjust, domain)
{
#if !RP_HOST

  const int FLUID_CHAMBER_ID = 2;

  real cen[ND_ND], dis[ND_ND];
  real crank_start_angle;
  real spark_duration, spark_energy;
  real spark_volume;
  real rpm;
  cell_t c;
  Thread *ct;

  rpm = RP_Get_Real("dynamesh/in-cyn/crank-rpm");
  crank_start_angle = RP_Get_Real("dynamesh/in-cyn/crank-start-angle");
  spark_start_angle = RP_Get_Real("spark/start-ca");
  spark_duration = RP_Get_Real("spark/duration");
  spark_radius = RP_Get_Real("spark/radius");
  spark_energy = RP_Get_Real("spark/energy");

  /* Set the global angle variables [deg] here for use in the SOURCE UDF */
  crank_angle = crank_start_angle + (rpm * CURRENT_TIME * 6.0);
  spark_end_angle = spark_start_angle + (rpm * spark_duration * 6.0);

  ct = Lookup_Thread(domain, FLUID_CHAMBER_ID);
  spark_volume = 0.0;

  begin_c_loop_int(c, ct)
  {
    C_CENTROID(cen, c, ct);
    NV_VV(dis, =, cen, -, spark_center);

    if (NV_MAG(dis) < spark_radius)
    {
      spark_volume += C_VOLUME(c, ct);
    }
  }
  end_c_loop_int(c, ct)

      spark_volume = PRF_GRSUM1(spark_volume);
  spark_energy_source = spark_energy / (spark_duration * spark_volume);

  Message0("\nSpark energy source = %g [W/m3].\n", spark_energy_source);
#endif
}

DEFINE_SOURCE(energy_source, c, ct, dS, eqn)
{
  /* Don't need to mark with #if !RP_HOST as DEFINE_SOURCE is only executed
     on nodes as indicated by the arguments "c" and "ct" */
  real cen[ND_ND], dis[ND_ND];

  if ((crank_angle >= spark_start_angle) &&
      (crank_angle < spark_end_angle))
  {
    C_CENTROID(cen, c, ct);
    NV_VV(dis, =, cen, -, spark_center);

    if (NV_MAG(dis) < spark_radius)
    {
      return spark_energy_source;
    }
  }

  /* Cell is not in spark zone or within time of spark discharge */

  return 0.0;
}