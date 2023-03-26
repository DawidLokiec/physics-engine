// Reminder: Always include standard library and system headers before including your own headers.
#include <cmath>

#include "sequential_acceleration_calculation.h"
#include "physics/astronomical_algorithms.h"

using namespace physics;

void SequentialAccelerationCalculationImpl::calcAccelerations(
		const Bodies<float, float, float> &bodies,
		const size_t numBodies,
		float *const accelerations,
		const float squaredSofteningFactor
) {
	for (size_t i = 0; i < numBodies; ++i) {
		const size_t xCoordinateIndexBody1 = i * 3;
		const size_t yCoordinateIndexBody1 = xCoordinateIndexBody1 + 1;
		const size_t zCoordinateIndexBody1 = xCoordinateIndexBody1 + 2;
		for (size_t j = i + 1; j < numBodies; ++j) {
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
			const float tmp = bodies.masses[j] / distanceSquared;
			accelerations[xCoordinateIndexBody1] += (tmp * normalizedDistanceVectorXCoordinate);
			accelerations[yCoordinateIndexBody1] += (tmp * normalizedDistanceVectorYCoordinate);
			accelerations[zCoordinateIndexBody1] += (tmp * normalizedDistanceVectorZCoordinate);

			// Newton's third law
			const float tmp2 = bodies.masses[i] / distanceSquared;
			accelerations[xCoordinateIndexBody2] -= (tmp2 * normalizedDistanceVectorXCoordinate);
			accelerations[yCoordinateIndexBody2] -= (tmp2 * normalizedDistanceVectorYCoordinate);
			accelerations[zCoordinateIndexBody2] -= (tmp2 * normalizedDistanceVectorZCoordinate);
		}
		accelerations[xCoordinateIndexBody1] =
				static_cast<float>(accelerations[xCoordinateIndexBody1] * GRAVITATIONAL_CONSTANT);
		accelerations[yCoordinateIndexBody1] =
				static_cast<float>(accelerations[yCoordinateIndexBody1] * GRAVITATIONAL_CONSTANT);
		accelerations[zCoordinateIndexBody1] =
				static_cast<float>(accelerations[zCoordinateIndexBody1] * GRAVITATIONAL_CONSTANT);
	}
}