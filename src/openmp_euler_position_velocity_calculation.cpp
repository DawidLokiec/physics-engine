// Reminder: Always include standard library and system headers before including your own headers.
#include <cmath>
#include <omp.h>

#include "openmp_euler_position_velocity_calculation.h"

using namespace physics;

void OpenMpEulerPositionVelocityCalculationImpl::updatePositionAndVelocity(
		const Bodies<float, float, float> &bodies,
		const size_t numBodies,
		const float *accelerations,
		const float timeStep
) {
	// omp_get_num_procs seems to return the number of logical (!) cores
	omp_set_num_threads(std::min(static_cast<int>(numBodies), omp_get_num_procs()));
	// @formatter:off
	#pragma omp parallel for default(none) shared(bodies, numBodies, accelerations, timeStep)
	//@formatter:on
	// using signed (!) int for the loop counter variable is mandatory if you compile with visual c++ compiler !!!
	for (int i = 0; i < static_cast<long long>(numBodies); ++i) {
		const size_t xCoordinateIndex = i * 3;
		const size_t yCoordinateIndex = xCoordinateIndex + 1;
		const size_t zCoordinateIndex = xCoordinateIndex + 2;

		bodies.velocities[xCoordinateIndex] += (accelerations[xCoordinateIndex] * timeStep);
		bodies.velocities[yCoordinateIndex] += (accelerations[yCoordinateIndex] * timeStep);
		bodies.velocities[zCoordinateIndex] += (accelerations[zCoordinateIndex] * timeStep);

		bodies.positions[xCoordinateIndex] += (bodies.velocities[xCoordinateIndex] * timeStep);
		bodies.positions[yCoordinateIndex] += (bodies.velocities[yCoordinateIndex] * timeStep);
		bodies.positions[zCoordinateIndex] += (bodies.velocities[zCoordinateIndex] * timeStep);
	}
}