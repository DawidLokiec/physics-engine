#include <gtest/gtest.h>

#include "performance_tests_framework.h"

using namespace physics;

TEST(PerformanceTestOpenCLAccelerationCalculation, N100) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_CL, 100);
}

TEST(PerformanceTestOpenCLAccelerationCalculation, N1_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_CL, 1'000);
}

TEST(PerformanceTestOpenCLAccelerationCalculation, N10_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_CL, 10'000);
}

TEST(PerformanceTestOpenCLAccelerationCalculation, N100_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_CL, 100'000);
}

TEST(PerformanceTestOpenCLAccelerationCalculation, N1_000_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_CL, 1'000'000);
}
