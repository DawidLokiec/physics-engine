// Reminder: Always include standard library and system headers before including your own headers.
#include <cmath>
#include <gtest/gtest.h>

#include "physics/acceleration_calculation.h"
#include "physics/acceleration_calculation_factory.h"
#include "commons/math.h"

namespace {
	float calc3dVectorLength(const float *vector3d) {
		const size_t xCoordinate = 0, yCoordinate = 1, zCoordinate = 2;
		return std::sqrt(
				commons::math::pow2(vector3d[xCoordinate]) +
				commons::math::pow2(vector3d[yCoordinate]) +
				commons::math::pow2(vector3d[zCoordinate])
		);
	}
}

using namespace physics;

TEST(AccelerationCalculationTest, SequentialAccelerationCalculationTest) {
	// Preparation
	// These values were pulled from NASA on 05/28/2022
	const float sunMass = 1.988409871326422e+21;
	const float sunPositionXCoordinate = 60764136.34568623 * 1000;
	const float sunPositionYCoordinate = 138876778.5691075 * 1000;
	const float sunPositionZCoordinate = -7392.035766117275 * 1000;
	const float sunVelocityXCoordinate = -26.81358403560408 * 1000;
	const float sunVelocityYCoordinate = 12.06331415757691 * 1000;
	const float sunVelocityZCoordinate = 0.000602317650384876 * 1000;

	const float venusMass = 4867305814842006.0;
	const float venusPositionXCoordinate = 155963686.5097929 * 1000;
	const float venusPositionYCoordinate = 86372916.2720451 * 1000;
	const float venusPositionZCoordinate = -6221383.90401521 * 1000;
	const float venusVelocityXCoordinate = -10.10767195510975 * 1000;
	const float venusVelocityYCoordinate = 42.58540771322825 * 1000;
	const float venusVelocityZCoordinate = -0.5443721325972781 * 1000;

	const float marsMass = 641690892138501.5;
	const float marsPositionXCoordinate = 220994088.6927211 * 1000;
	const float marsPositionYCoordinate = 7535624.027122181 * 1000;
	const float marsPositionZCoordinate = -6690421.407387457 * 1000;
	const float marsVelocityXCoordinate = -10.53562754024867 * 1000;
	const float marsVelocityYCoordinate = 32.87860971265692 * 1000;
	const float marsVelocityZCoordinate = 0.03755165281278394 * 1000;

	const float squaredSofteningFactor = 0.00f;

	IAccelerationCalculation *const pAccelerationCalculation =
			createAccelerationCalculation(AccelerationCalculationImplementation::SEQUENTIAL);

	// Test case 1: Sun<->Venus-Interaction
	{
		const size_t numBodies = 2;
		Bodies<float, float, float> bodies{new float[numBodies], new float[numBodies * 3],
										   new float[numBodies * 3]};
		bodies.masses[0] = sunMass;
		bodies.masses[1] = venusMass;
		bodies.positions[0] = sunPositionXCoordinate;
		bodies.positions[1] = sunPositionYCoordinate;
		bodies.positions[2] = sunPositionZCoordinate;
		bodies.positions[3] = venusPositionXCoordinate;
		bodies.positions[4] = venusPositionYCoordinate;
		bodies.positions[5] = venusPositionZCoordinate;
		bodies.velocities[0] = sunVelocityXCoordinate;
		bodies.velocities[1] = sunVelocityYCoordinate;
		bodies.velocities[2] = sunVelocityZCoordinate;
		bodies.velocities[3] = venusVelocityXCoordinate;
		bodies.velocities[4] = venusVelocityYCoordinate;
		bodies.velocities[5] = venusVelocityZCoordinate;

		auto *const accelerations = new float[numBodies * 3]();
		// Precondition check
		for (size_t i = 0; i < (numBodies * 3); ++i) {
			ASSERT_FLOAT_EQ(0.0, accelerations[i]);
		}

		// Stimulation
		pAccelerationCalculation->calcAccelerations(bodies, numBodies, accelerations, squaredSofteningFactor);

		// Tests
		// The expected values were calculated with https://www.sensorsone.com/force-and-mass-to-acceleration-calculator/
		ASSERT_NEAR(2.73954584e-17, calc3dVectorLength(&accelerations[0]), 1e-21);
		ASSERT_NEAR(1.11916946e-11, calc3dVectorLength(&accelerations[3]), 1e-16);

		delete[] bodies.masses;
		delete[] bodies.positions;
		delete[] bodies.velocities;
		delete[] accelerations;
	}

	// Test case 2: Sun<->Mars-Interaction
	{
		const size_t numBodies = 2;
		Bodies<float, float, float> bodies{new float[numBodies], new float[numBodies * 3],
										   new float[numBodies * 3]};
		bodies.masses[0] = sunMass;
		bodies.masses[1] = marsMass;
		bodies.positions[0] = sunPositionXCoordinate;
		bodies.positions[1] = sunPositionYCoordinate;
		bodies.positions[2] = sunPositionZCoordinate;
		bodies.positions[3] = marsPositionXCoordinate;
		bodies.positions[4] = marsPositionYCoordinate;
		bodies.positions[5] = marsPositionZCoordinate;
		bodies.velocities[0] = sunVelocityXCoordinate;
		bodies.velocities[1] = sunVelocityYCoordinate;
		bodies.velocities[2] = sunVelocityZCoordinate;
		bodies.velocities[3] = marsVelocityXCoordinate;
		bodies.velocities[4] = marsVelocityYCoordinate;
		bodies.velocities[5] = marsVelocityZCoordinate;

		auto *const accelerations = new float[numBodies * 3]();
		// Precondition check
		for (size_t i = 0; i < (numBodies * 3); ++i) {
			ASSERT_FLOAT_EQ(0.0, accelerations[i]);
		}

		// Stimulation
		pAccelerationCalculation->calcAccelerations(bodies, numBodies, accelerations, squaredSofteningFactor);

		// Tests
		// The expected values were calculated with https://www.sensorsone.com/force-and-mass-to-acceleration-calculator/
		ASSERT_NEAR(9.96733636e-19, calc3dVectorLength(&accelerations[0]), 1e-23);
		ASSERT_NEAR(3.0885821e-12, calc3dVectorLength(&accelerations[3]), 1e-17);

		delete[] bodies.masses;
		delete[] bodies.positions;
		delete[] bodies.velocities;
	}

	// Test case 3: Interaction between sun, venus and mars
	{
		const size_t numBodies = 3;
		Bodies<float, float, float> bodies{new float[numBodies], new float[numBodies * 3],
										   new float[numBodies * 3]};
		bodies.masses[0] = sunMass;
		bodies.masses[1] = venusMass;
		bodies.masses[2] = marsMass;
		bodies.positions[0] = sunPositionXCoordinate;
		bodies.positions[1] = sunPositionYCoordinate;
		bodies.positions[2] = sunPositionZCoordinate;
		bodies.positions[3] = venusPositionXCoordinate;
		bodies.positions[4] = venusPositionYCoordinate;
		bodies.positions[5] = venusPositionZCoordinate;
		bodies.positions[6] = marsPositionXCoordinate;
		bodies.positions[7] = marsPositionYCoordinate;
		bodies.positions[8] = marsPositionZCoordinate;
		bodies.velocities[0] = sunVelocityXCoordinate;
		bodies.velocities[1] = sunVelocityYCoordinate;
		bodies.velocities[2] = sunVelocityZCoordinate;
		bodies.velocities[3] = venusVelocityXCoordinate;
		bodies.velocities[4] = venusVelocityYCoordinate;
		bodies.velocities[5] = venusVelocityZCoordinate;
		bodies.velocities[6] = marsVelocityXCoordinate;
		bodies.velocities[7] = marsVelocityYCoordinate;
		bodies.velocities[8] = marsVelocityZCoordinate;

		auto *const accelerations = new float[numBodies * 3]();
		// Precondition check
		for (size_t i = 0; i < (numBodies * 3); ++i) {
			ASSERT_FLOAT_EQ(0.0, accelerations[i]);
		}

		// Stimulation
		pAccelerationCalculation->calcAccelerations(bodies, numBodies, accelerations, squaredSofteningFactor);

		// Tests
		// The expected values were calculated with https://www.sensorsone.com/force-and-mass-to-acceleration-calculator/
		ASSERT_NEAR(2.83921921e-17, calc3dVectorLength(&accelerations[0]), 1e-19);
		ASSERT_NEAR(1.11916987e-11, calc3dVectorLength(&accelerations[3]), 1e-15);
		ASSERT_NEAR(3.0886132e-12, calc3dVectorLength(&accelerations[6]), 1e-17);

		delete[] bodies.masses;
		delete[] bodies.positions;
		delete[] bodies.velocities;
		delete[] accelerations;
	}

	// Clean up
	delete pAccelerationCalculation;
}
