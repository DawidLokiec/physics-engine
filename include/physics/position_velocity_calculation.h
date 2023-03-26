#ifndef PHYSICS_ENGINE_POSITION_VELOCITY_CALCULATION_H
#define PHYSICS_ENGINE_POSITION_VELOCITY_CALCULATION_H

// Reminder: Always include standard library and system headers before including your own headers.
#include <cstddef>

#include "bodies.h"

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief This functional interface declares the method for updating the positions and velocities of N bodies.
	 */
	class IPositionVelocityCalculation {

		public:
			/**
			  * @brief The default destructor.
			  */
			virtual ~IPositionVelocityCalculation() = default;

			/**
			 * @brief Updates the positions and velocities of the given bodies.
			 * @param bodies the bodies whose positions and velocities are to be updated.
			 * @param numBodies the number of bodies.
			 * @param accelerations the current accelerations of the bodies. The number of elements in
			 * 						<code>accelerations</code> parameter must be <code>numBodies * vector dimension</code>.
			 * @param timeStep the time step.
			 */
			virtual void updatePositionAndVelocity(
					const Bodies<float, float, float> &bodies,
					size_t numBodies,
					const float *accelerations,
					float timeStep
			) = 0;
	};
}

#endif //PHYSICS_ENGINE_POSITION_VELOCITY_CALCULATION_H
