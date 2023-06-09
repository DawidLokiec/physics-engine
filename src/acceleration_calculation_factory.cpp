// Reminder: Always include standard library and system headers before including your own headers.
#include <stdexcept>

#include "physics/acceleration_calculation_factory.h"
#include "sequential_acceleration_calculation.h"
#include "openmp_acceleration_calculation.h"
#include "opencl_acceleration_calculation.h"
#include "cudamodule/cuda_acceleration_calculation.h"

using namespace physics;

IAccelerationCalculation *
physics::createAccelerationCalculation(const AccelerationCalculationImplementation &implementation) {
	switch (implementation) {
		case AccelerationCalculationImplementation::SEQUENTIAL:
			return new SequentialAccelerationCalculationImpl();
		case AccelerationCalculationImplementation::OPEN_MP:
			return new OpenMpAccelerationCalculationImpl();
		case AccelerationCalculationImplementation::OPEN_CL:
			return new OpenClAccelerationCalculationImpl();
		case AccelerationCalculationImplementation::CUDA:
			return new CudaAccelerationCalculationImpl();
		default:
			// let it crash
			throw std::runtime_error("Unknown implementation of an acceleration calculation.");
	}
}