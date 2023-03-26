#include <gtest/gtest.h>

#include "performance_tests_framework.h"

using namespace physics;

TEST(PerformanceTestSequentialAccelerationCalculation, N100) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::SEQUENTIAL, 100);
}

TEST(PerformanceTestSequentialAccelerationCalculation, N1_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::SEQUENTIAL, 1'000);
}

TEST(PerformanceTestSequentialAccelerationCalculation, N10_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::SEQUENTIAL, 10'000);
}

TEST(PerformanceTestSequentialAccelerationCalculation, N100_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::SEQUENTIAL, 100'000);
}

TEST(PerformanceTestSequentialAccelerationCalculation, N1_000_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::SEQUENTIAL, 1'000'000);
}
