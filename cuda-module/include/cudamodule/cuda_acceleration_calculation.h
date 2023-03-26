#ifndef PHYSICS_ENGINE_CUDA_ACCELERATION_CALCULATION_H
#define PHYSICS_ENGINE_CUDA_ACCELERATION_CALCULATION_H

#include "physics/acceleration_calculation.h"

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief An <strong>CUDA-accelerated</strong> implementation of the calculation of gravitational accelerations of
	 * N bodies.
	 */
	class CudaAccelerationCalculationImpl : public IAccelerationCalculation {

		public:
			/**
			 * @brief Calculates the accelerations of the given bodies.
			 * @param bodies the bodies whose accelerations are to be calculated.
			 * @param numBodies the number of bodies.
			 * @param[in, out] accelerations the accelerations of the passed bodies. The <code>accelerations</code>
			 * 					parameter must be a pointer to an allocated storage which is large enough to store
			 * 					<code>numBodies * vector dimension</code>.
			 * @param squaredSofteningFactor the squared softening factor, in order to avoid division by zero.
			 */
			void calcAccelerations(
					const Bodies<float, float, float> &bodies,
					size_t numBodies,
					float *accelerations,
					float squaredSofteningFactor
			) override;
	};
}

#endif //PHYSICS_ENGINE_CUDA_ACCELERATION_CALCULATION_H
