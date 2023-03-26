// Reminder: Always include standard library and system headers before including your own headers.
#include <gtest/gtest.h>

#include "commons/language.h"
#include "physics/acceleration_calculation_factory.h"
#include "../../src/sequential_acceleration_calculation.h"
#include "../../src/openmp_acceleration_calculation.h"
#include "../../src/opencl_acceleration_calculation.h"
#include "../../cuda-module/include/cudamodule/cuda_acceleration_calculation.h"

using namespace physics;

namespace {
	template<typename Expected>
	void assertReturnedTypeOfImplementationIs(const IAccelerationCalculation *const actual) {
		ASSERT_NE(nullptr, actual);
		const bool isCorrectSubtype = commons::isInstanceOf<IAccelerationCalculation, Expected>(actual);
		ASSERT_TRUE(isCorrectSubtype);
	}
}

TEST(AccelerationCalculationFactoryTest, ShouldCreateSequentialAcclerationImplementation) {
	// Stimulation
	const IAccelerationCalculation *const pAccelerationCalculation =
			createAccelerationCalculation(AccelerationCalculationImplementation::SEQUENTIAL);

	// Test
	assertReturnedTypeOfImplementationIs<SequentialAccelerationCalculationImpl>(pAccelerationCalculation);

	// Clean up
	delete pAccelerationCalculation;
}

TEST(AccelerationCalculationFactoryTest, ShouldCreateOpenMPAcclerationImplementation) {
	// Stimulation
	const IAccelerationCalculation *const pAccelerationCalculation =
			createAccelerationCalculation(AccelerationCalculationImplementation::OPEN_MP);

	// Test
	assertReturnedTypeOfImplementationIs<OpenMpAccelerationCalculationImpl>(pAccelerationCalculation);

	// Clean up
	delete pAccelerationCalculation;
}

TEST(AccelerationCalculationFactoryTest, ShouldCreateOpenCLAcclerationImplementation) {
	// Stimulation
	const IAccelerationCalculation *const pAccelerationCalculation =
			createAccelerationCalculation(AccelerationCalculationImplementation::OPEN_CL);

	// Test
	assertReturnedTypeOfImplementationIs<OpenClAccelerationCalculationImpl>(pAccelerationCalculation);

	// Clean up
	delete pAccelerationCalculation;
}

TEST(AccelerationCalculationFactoryTest, ShouldCreateCUDAAcclerationImplementation) {
	// Stimulation
	const IAccelerationCalculation *const pAccelerationCalculation =
			createAccelerationCalculation(AccelerationCalculationImplementation::CUDA);

	// Test
	assertReturnedTypeOfImplementationIs<CudaAccelerationCalculationImpl>(pAccelerationCalculation);

	// Clean up
	delete pAccelerationCalculation;
}