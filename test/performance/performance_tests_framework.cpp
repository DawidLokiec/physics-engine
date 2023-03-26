// Reminder: Always include standard library and system headers before including your own headers.
#include <chrono>
#include <iostream>
#include <random>

#include "performance_tests_framework.h"

using namespace physics;

float PerformanceTestFramework::generateRandomFloat(float inclusiveMin, float exclusiveMax) {
	if (exclusiveMax < inclusiveMin) {
		const float tmp = exclusiveMax;
		exclusiveMax = inclusiveMin;
		inclusiveMin = tmp;
	}

	static std::random_device rd;
	static std::default_random_engine engine(rd());
	static std::uniform_real_distribution<> distribution(inclusiveMin, exclusiveMax);
	return static_cast<float>(distribution(engine));
}

void PerformanceTestFramework::generateNRandomBodies(const size_t n, const Bodies<float, float, float> &bodies) {
	// @formatter:off
#pragma omp parallel for default(none) shared(n, bodies)
	// @formatter:on
	for (int i = 0; i < n; ++i) {
		const size_t xCoordinateIndex = i * 3;
		const size_t yCoordinateIndex = xCoordinateIndex + 1;
		const size_t zCoordinateIndex = xCoordinateIndex + 2;

		bodies.masses[i] = 1.0f;
		bodies.positions[xCoordinateIndex] = generateRandomFloat(-0.25f, 0.251f);
		bodies.positions[yCoordinateIndex] = generateRandomFloat(-0.25f, 0.251f);
		bodies.positions[zCoordinateIndex] = 0.0f;
		bodies.velocities[xCoordinateIndex] = 0.0f;
		bodies.velocities[yCoordinateIndex] = 0.0f;
		bodies.velocities[zCoordinateIndex] = 0.0f;
	}
}

void
PerformanceTestFramework::performTest(const AccelerationCalculationImplementation &implementation, const size_t n) {
	const size_t numCoordinates = n * 3;
	Bodies<float, float, float> bodies{new float[n], new float[numCoordinates], new float[numCoordinates]};

	generateNRandomBodies(n, bodies);

	IAccelerationCalculation *pAccelerationCalculation = createAccelerationCalculation(implementation);
	const float softeningFactorSquared = 0.01;
	auto *accelerations = new float[numCoordinates];

	auto tmp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::cout << "Test started at " << std::ctime(&tmp) << std::endl;
	if (n == 1'000'000) {
		tmp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + std::chrono::seconds(11702));
		std::cout << "Estimated time of arrival (ETA): " << std::ctime(&tmp) << std::endl;
	}
	auto startTimeInNanoSeconds = std::chrono::high_resolution_clock::now();
	{
		pAccelerationCalculation->calcAccelerations(bodies, n, accelerations, softeningFactorSquared);
	}
	auto endTimeInNanoSeconds = std::chrono::high_resolution_clock::now();
	std::cout.precision(17);
	std::cout << "Processing took: "
			  << static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
					  endTimeInNanoSeconds - startTimeInNanoSeconds).count()) / 1e+9
			  << " seconds for N = "
			  << n
			  << std::endl;

	delete pAccelerationCalculation;
	delete[] accelerations;
	delete[] bodies.masses;
	bodies.masses = nullptr;
	delete[] bodies.positions;
	bodies.positions = nullptr;
	delete[] bodies.velocities;
	bodies.velocities = nullptr;
}