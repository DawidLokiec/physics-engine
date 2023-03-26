#ifndef PHYSICS_ENGINE_OPENMP_EULER_POSITION_VELOCITY_CALCULATION_H
#define PHYSICS_ENGINE_OPENMP_EULER_POSITION_VELOCITY_CALCULATION_H

#include "physics/position_velocity_calculation.h"

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief Implements the method for updating the positions and velocities of N bodies using the <em>Euler method</em>.
	 * @details The Euler method is known for its simplicity, but also for its lack of accuracy.
	 */
	class [[maybe_unused]] OpenMpEulerPositionVelocityCalculationImpl : public IPositionVelocityCalculation {
		public:
			/**
			 * @brief Updates the positions and velocities of the given bodies using the <em>Euler method</em>.
			 * @param bodies the bodies whose positions and velocities are to be updated.
			 * @param numBodies the number of bodies.
			 * @param accelerations the current accelerations of the bodies. The number of elements in
			 * 						<code>accelerations</code> parameter must be <code>numBodies * vector dimension</code>.
			 * @param timeStep the time step.
			 */
			void updatePositionAndVelocity(
					const Bodies<float, float, float> &bodies,
					size_t numBodies,
					const float *accelerations,
					float timeStep
			) override;
	};
}

#endif //PHYSICS_ENGINE_OPENMP_EULER_POSITION_VELOCITY_CALCULATION_H
