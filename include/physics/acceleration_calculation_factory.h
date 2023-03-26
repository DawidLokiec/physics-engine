#ifndef PHYSICS_ENGINE_ACCELERATION_CALCULATION_FACTORY_H
#define PHYSICS_ENGINE_ACCELERATION_CALCULATION_FACTORY_H

#include "acceleration_calculation.h"

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief Contains the constants to specify different implementations of an acceleration calculation.
	 */
	enum class AccelerationCalculationImplementation {

		/**
		 * The constant to specify the <strong>sequential</strong> implementation of the acceleration calculation.
		 */
		SEQUENTIAL,

		/**
		 * The constant to specify the <strong>OpenMP-accelerated</strong> implementation of the acceleration calculation.
		 */
		OPEN_MP,

		/**
		 * The constant to specify the <strong>OpenCL-accelerated</strong> implementation of the acceleration calculation.
		 */
		OPEN_CL,

		/**
 		 * The constant to specify the <strong>CUDA-accelerated</strong> implementation of the acceleration calculation.
 		 */
		CUDA
	};

	/**
	 * @brief Creates an acceleration calculation.
	 * @details The returned acceleration calculation should be destroyed with <code>delete</code> by the caller.
	 * @param implementation the specification of a concrete implementation to be created.
	 * @return the pointer to the implementation of the specified acceleration calculation.
	 */
	IAccelerationCalculation *
	createAccelerationCalculation(const AccelerationCalculationImplementation &implementation);
}

#endif //PHYSICS_ENGINE_ACCELERATION_CALCULATION_FACTORY_H
