#include <gtest/gtest.h>

#include "physics/bodies.h"

using namespace physics;

namespace {
	void doPerformanceTestBodiesAsSoAAndSequential(const size_t n) {
		Bodies<long double, double, double> bodies{new long double[n], new double[n * 3], new double[n * 3]};
		auto *accelerations = new double[n * 3];

		for (size_t i = 0; i < n; ++i) {
			bodies.masses[i] = static_cast<long double>(i);
			bodies.positions[(i * 3)] = static_cast<double>(i);
			bodies.positions[(i * 3) + 1] = static_cast<double>(i + 1);
			bodies.positions[(i * 3) + 2] = static_cast<double>(i + 2);
			bodies.velocities[(i * 3)] = static_cast<double>(i);
			bodies.velocities[(i * 3) + 1] = static_cast<double>(i + 1);
			bodies.velocities[(i * 3) + 2] = static_cast<double>(i + 2);
			accelerations[i] = 0;
		}

		const double timeStep = 0.1;

		auto start_nano_seconds = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < n; ++i) {
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
		auto end_nano_seconds = std::chrono::high_resolution_clock::now();
		std::cout.precision(17);
		std::cout << "Sequential processing of SoA took: " <<
				  static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
						  end_nano_seconds - start_nano_seconds).count()) / 1e+9 << " seconds for N = " << n
				  << std::endl;

		delete[] bodies.masses;
		delete[] bodies.positions;
		delete[] bodies.velocities;
		delete[] accelerations;
	}

	void doPerformanceTestBodiesAsSoAAndOpenMP(const size_t n) {
		Bodies<long double, double, double> bodies{new long double[n], new double[n * 3], new double[n * 3]};
		auto *accelerations = new double[n * 3];

		for (size_t i = 0; i < n; ++i) {
			bodies.masses[i] = static_cast<long double>(i);
			bodies.positions[(i * 3)] = static_cast<double>(i);
			bodies.positions[(i * 3) + 1] = static_cast<double>(i + 1);
			bodies.positions[(i * 3) + 2] = static_cast<double>(i + 2);
			bodies.velocities[(i * 3)] = static_cast<double>(i);
			bodies.velocities[(i * 3) + 1] = static_cast<double>(i + 1);
			bodies.velocities[(i * 3) + 2] = static_cast<double>(i + 2);
			accelerations[i] = 0;
		}

		const double timeStep = 0.1;

		auto start_nano_seconds = std::chrono::high_resolution_clock::now();
		#pragma omp parallel for default(none) shared(bodies, n, accelerations, timeStep)
		for (int i = 0; i < n; ++i) {
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
		auto end_nano_seconds = std::chrono::high_resolution_clock::now();
		std::cout.precision(17);
		std::cout << "OpenMP-accelerated processing of SoA took: " <<
				  static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
						  end_nano_seconds - start_nano_seconds).count()) / 1e+9 << " seconds for N = " << n
				  << std::endl;

		delete[] bodies.masses;
		delete[] bodies.positions;
		delete[] bodies.velocities;
		delete[] accelerations;
	}
}

TEST(PerformanceTest, PerformanceTest1MioBodiesAsSoAAndSequential) {
	doPerformanceTestBodiesAsSoAAndSequential(1'000'000);
}

TEST(PerformanceTest, PerformanceTest10MioBodiesAsSoAAndSequential) {
	doPerformanceTestBodiesAsSoAAndSequential(10'000'000);
}

TEST(PerformanceTest, PerformanceTest100MioBodiesAsSoAAndSequential) {
	doPerformanceTestBodiesAsSoAAndSequential(100'000'000);
}

TEST(PerformanceTest, PerformanceTest1MiaBodiesAsSoAAndSequential) {
	doPerformanceTestBodiesAsSoAAndSequential(1'000'000'000);
}

// -----------------------------------------------------------------

TEST(PerformanceTest, PerformanceTest1MioBodiesAsSoAAndOpenMP) {
	doPerformanceTestBodiesAsSoAAndSequential(1'000'000);
}

TEST(PerformanceTest, PerformanceTest10MioBodiesAsSoAAndOpenMP) {
	doPerformanceTestBodiesAsSoAAndOpenMP(10'000'000);
}

TEST(PerformanceTest, PerformanceTest100MioBodiesAsSoAAndOpenMP) {
	doPerformanceTestBodiesAsSoAAndOpenMP(100'000'000);
}

TEST(PerformanceTest, PerformanceTest1MiaBodiesAsSoAAndOpenMP) {
	doPerformanceTestBodiesAsSoAAndOpenMP(1'000'000'000);
}
