// Reminder: Always include standard library and system headers before including your own headers.
#include <cmath>
#include <omp.h>

#include "openmp_acceleration_calculation.h"
#include "physics/astronomical_algorithms.h"

using namespace physics;

void OpenMpAccelerationCalculationImpl::calcAccelerations(
		const Bodies<float, float, float> &bodies,
		const size_t numBodies,
		float *const accelerations,
		const float squaredSofteningFactor
) {
	if (1 < numBodies) {
		// omp_get_num_procs seems to return the number of logical (!) cores
		omp_set_num_threads(std::min(static_cast<int>(numBodies), omp_get_num_procs()));
		// @formatter:off
		#pragma omp parallel for default(none) shared(bodies, numBodies, accelerations, squaredSofteningFactor)
		//@formatter:on
		// using signed (!) int for the loop counter variable is mandatory if you compile with visual c++ compiler !!!
		for (int i = 0; i < static_cast<long long>(numBodies); ++i) {
			const size_t xCoordinateIndexBody1 = i * 3;
			const size_t yCoordinateIndexBody1 = xCoordinateIndexBody1 + 1;
			const size_t zCoordinateIndexBody1 = xCoordinateIndexBody1 + 2;
			float forceVector[3] = {0.0, 0.0, 0.0};
			for (int j = 0; j < static_cast<long long>(numBodies); ++j) {
				if (i != j) {
					const size_t xCoordinateIndexBody2 = j * 3;
					const size_t yCoordinateIndexBody2 = xCoordinateIndexBody2 + 1;
					const size_t zCoordinateIndexBody2 = xCoordinateIndexBody2 + 2;

					const float distanceVectorXCoordinate =
							bodies.positions[xCoordinateIndexBody1] - bodies.positions[xCoordinateIndexBody2];
					const float distanceVectorYCoordinate =
							bodies.positions[yCoordinateIndexBody1] - bodies.positions[yCoordinateIndexBody2];
					const float distanceVectorZCoordinate =
							bodies.positions[zCoordinateIndexBody1] - bodies.positions[zCoordinateIndexBody2];
					const float distance = std::sqrt(
							(distanceVectorXCoordinate * distanceVectorXCoordinate) +
							(distanceVectorYCoordinate * distanceVectorYCoordinate) +
							(distanceVectorZCoordinate * distanceVectorZCoordinate)
					) + squaredSofteningFactor; // to avoid zero in the following divisions
					const float normalizedDistanceVectorXCoordinate = distanceVectorXCoordinate / distance;
					const float normalizedDistanceVectorYCoordinate = distanceVectorYCoordinate / distance;
					const float normalizedDistanceVectorZCoordinate = distanceVectorZCoordinate / distance;
					const float distanceSquared = distance * distance;

					const float receivedForce = bodies.masses[j] / distanceSquared;
					// It is possible to write directly in to the accelerations array here, but the false sharing will
					// reduce in this loop significantly the performance
					forceVector[0] += (receivedForce * normalizedDistanceVectorXCoordinate);
					forceVector[1] += (receivedForce * normalizedDistanceVectorYCoordinate);
					forceVector[2] += (receivedForce * normalizedDistanceVectorZCoordinate);
				}
			}
			// false sharing is ok here
			accelerations[xCoordinateIndexBody1] = static_cast<float>(GRAVITATIONAL_CONSTANT * forceVector[0]);
			accelerations[yCoordinateIndexBody1] = static_cast<float>(GRAVITATIONAL_CONSTANT * forceVector[1]);
			accelerations[zCoordinateIndexBody1] = static_cast<float>(GRAVITATIONAL_CONSTANT * forceVector[2]);
		}
	}
}
