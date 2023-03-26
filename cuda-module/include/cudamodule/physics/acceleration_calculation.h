#ifndef PHYSICS_ENGINE_ACCELERATION_CALCULATION_H
#define PHYSICS_ENGINE_ACCELERATION_CALCULATION_H

// Reminder: Always include standard library and system headers before including your own headers.
#include <cstddef>

#include "bodies.h"

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief This functional interface declares the method for the calculation of accelerations of N bodies.
	 */
	class IAccelerationCalculation {

		public:
			/**
			 * @brief The default destructor.
			 */
			virtual ~IAccelerationCalculation() = default;

			/**
			 * @brief Calculates the accelerations of the given bodies.
			 * @param bodies the bodies whose accelerations are to be calculated.
			 * @param numBodies the number of bodies.
			 * @param[in, out] accelerations the accelerations of the passed bodies. The <code>accelerations</code>
			 * 					parameter must be a pointer to an allocated storage which is large enough to store
			 * 					<code>numBodies * vector dimension</code>.
			 * @param squaredSofteningFactor the squared softening factor, in order to avoid division by zero.
			 */
			virtual void calcAccelerations(
					const Bodies<float, float, float> &bodies,
					size_t numBodies,
					float *accelerations,
					float squaredSofteningFactor
			) = 0;
	};
}

#endif //PHYSICS_ENGINE_ACCELERATION_CALCULATION_H
