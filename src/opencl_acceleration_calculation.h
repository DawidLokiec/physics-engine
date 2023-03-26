#ifndef PHYSICS_ENGINE_OPENCL_ACCELERATION_CALCULATION_H
#define PHYSICS_ENGINE_OPENCL_ACCELERATION_CALCULATION_H

// Reminder: Always include standard library and system headers before including your own headers.
#include <atomic>

#include "physics/acceleration_calculation.h"
#include "opencl/program.h"
#include "opencl/command_queue.h"
#include "opencl/read_only_buffer.h"
#include "opencl/write_only_buffer.h"

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief An <strong>OpenCL-accelerated</strong> implementation of the calculation of gravitational accelerations of N bodies.
	 */
	class OpenClAccelerationCalculationImpl : public IAccelerationCalculation {

		private:
			/**
			 * The current used OpenCL context;
			 */
			OpenClToolkit::Context *pContext_;

			/**
			  * The pointer to the current used OpenCL command queue.
			  */
			OpenClToolkit::CommandQueue *pCommandQueue_;

			/**
			  * The pointer to the OpenCL program that calculates the gravitational accelerations of N bodies.
			  */
			OpenClToolkit::Program *pProgram_;

			/**
			   * A boolean flag to indicate if the device memory is already allocated.
			   */
			std::atomic<bool> deviceMemoryAlreadyAllocated_;

			/**
			  * The pointer to the read only buffer on the device for the masses of the bodies.
			  */
			OpenClToolkit::ReadOnlyBuffer *pMassesBuffer_;

			/**
			 * The pointer to the read only buffer on the device for the positions of the bodies.
			 */
			OpenClToolkit::ReadOnlyBuffer *pPositionsBuffer_;

			/**
			 * The pointer to the write only buffer on the device for the accelerations of the bodies.
			 */
			OpenClToolkit::WriteOnlyBuffer *pAccelerationsBuffer_;

		public:
			/**
			  * @brief The default constructor. Creates an new instance of this class.
			  */
			OpenClAccelerationCalculationImpl();

			/**
			 * @brief The destructor.
			 */
			~OpenClAccelerationCalculationImpl() override;

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

#endif //PHYSICS_ENGINE_OPENCL_ACCELERATION_CALCULATION_H
