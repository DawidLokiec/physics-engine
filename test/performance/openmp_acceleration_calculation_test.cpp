#include <gtest/gtest.h>

#include "performance_tests_framework.h"

using namespace physics;

TEST(PerformanceTestOpenMPAccelerationCalculation, N100) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_MP, 100);
}

TEST(PerformanceTestOpenMPAccelerationCalculation, N1_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_MP, 1'000);
}

TEST(PerformanceTestOpenMPAccelerationCalculation, N10_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_MP, 10'000);
}

TEST(PerformanceTestOpenMPAccelerationCalculation, N100_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_MP, 100'000);
}

TEST(PerformanceTestOpenMPAccelerationCalculation, N1_000_000) {
	PerformanceTestFramework::performTest(AccelerationCalculationImplementation::OPEN_MP, 1'000'000);
}
