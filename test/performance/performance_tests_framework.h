#ifndef PHYSICS_ENGINE_PERFORMANCE_TESTS_FRAMEWORK_H
#define PHYSICS_ENGINE_PERFORMANCE_TESTS_FRAMEWORK_H

#include "physics/acceleration_calculation_factory.h"

namespace PerformanceTestFramework {
	float generateRandomFloat(float inclusiveMin, float exclusiveMax);

	void generateNRandomBodies(size_t n, const physics::Bodies<float, float, float> &bodies);

	void performTest(const physics::AccelerationCalculationImplementation &implementation, size_t n);
}

#endif //PHYSICS_ENGINE_PERFORMANCE_TESTS_FRAMEWORK_H
