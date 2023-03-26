// Reminder: Always include standard library and system headers before including your own headers.
#include <stdexcept>

#include "physics/acceleration_calculation_factory.h"
#include "sequential_acceleration_calculation.h"
#include "openmp_acceleration_calculation.h"
#include "opencl_acceleration_calculation.h"

using namespace physics;

IAccelerationCalculation *createAccelerationCalculation(const AccelerationCalculationImplementation &implementation) {
	switch (implementation) {
		case AccelerationCalculationImplementation::SEQUENTIAL:
			return new SequentialAccelerationCalculationImpl();
		case AccelerationCalculationImplementation::OPEN_MP:
			return new OpenMpAccelerationCalculationImpl();
		case AccelerationCalculationImplementation::OPEN_CL:
			return new OpenClAccelerationCalculationImpl();
		default:
			// let it crash
			throw std::runtime_error("Unknown implementation of an acceleration calculation.");
	}
}