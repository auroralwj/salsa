#include <gtest/gtest.h>
#include <ceres/ceres.h>

#include "multirotor_sim/simulator.h"
#include "multirotor_sim/controller.h"

#include "salsa/salsa.h"
#include "salsa/test_common.h"

using namespace salsa;

TEST (Salsa, MocapSimulation)
{
  Simulator sim(true);
  sim.load(imu_mocap());
  sim.tmax_ = 10;

  Salsa salsa;
  salsa.init(default_params("/tmp/Salsa/MocapSimulation/"));

  sim.register_estimator(&salsa);

  Logger true_state_log(salsa.log_prefix_ + "Truth.log");

  while (sim.run())
  {
    true_state_log.log(sim.t_);
    true_state_log.logVectors(sim.state().X.arr(), sim.state().v, sim.accel_bias_,
                              sim.gyro_bias_, Vector2d{sim.clock_bias_, sim.clock_bias_rate_});
  }
}

TEST (DISABLED_Salsa, RawGNSSSimulation)
{
    Simulator sim(true);
    sim.load("../lib/multirotor_sim/params/sim_params.yaml");
    sim.vo_enabled_ = false;
    sim.mocap_enabled_ = false;
    sim.alt_enabled_ = false;
    sim.gnss_enabled_ = false;
    sim.raw_gnss_enabled_ = true;

    Salsa salsa;
    salsa.init(default_params("/tmp/Salsa/RawGNSSSimulation/"));

    sim.register_estimator(&salsa);

    Logger true_state_log(salsa.log_prefix_ + "Truth.log");

    while (sim.run())
    {
        salsa.x_e2n_ = sim.X_e2n_;
        true_state_log.log(sim.t_);
        true_state_log.logVectors(sim.state().X.arr(), sim.state().v, sim.accel_bias_,
                                  sim.gyro_bias_, Vector2d{sim.clock_bias_, sim.clock_bias_rate_});
    }
}
