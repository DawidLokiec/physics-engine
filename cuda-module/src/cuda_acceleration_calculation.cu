#include "cudamodule/cuda_acceleration_calculation.h"
#include "physics/astronomical_algorithms.h"

#include "cudatoolkit/gpu_facade.cuh"
#include "cudatoolkit/gpu_code_utilities.cuh"

__global__ void calcAccelerationsKernel(
		const float *masses,
		const float *positions,
		float *accelerations,
		const unsigned int numBodies,
		const float softeningFactorSquared
) {
	// the global thread ID, which is unique across all threads in the grid.
	const unsigned int globalThreadId = CudaToolkit::GpuCodeUtilities::getGlobalThreadId();
	const unsigned int xCoordinateIndexBody1 = globalThreadId * 3;
	const unsigned int yCoordinateIndexBody1 = xCoordinateIndexBody1 + 1;
	const unsigned int zCoordinateIndexBody1 = xCoordinateIndexBody1 + 2;

	float forceVector[3] = {0.0f, 0.0f, 0.0f};
	for (unsigned int i = 0; (i < numBodies) && (globalThreadId < numBodies); ++i) {
		if (globalThreadId != i) {
			const unsigned int xCoordinateIndexBody2 = i * 3;
			const unsigned int yCoordinateIndexBody2 = xCoordinateIndexBody2 + 1;
			const unsigned int zCoordinateIndexBody2 = xCoordinateIndexBody2 + 2;

			const float distanceVectorXCoordinate = positions[xCoordinateIndexBody1] - positions[xCoordinateIndexBody2];
			const float distanceVectorYCoordinate = positions[yCoordinateIndexBody1] - positions[yCoordinateIndexBody2];
			const float distanceVectorZCoordinate = positions[zCoordinateIndexBody1] - positions[zCoordinateIndexBody2];

			const float distance = sqrt(
					(distanceVectorXCoordinate * distanceVectorXCoordinate) +
					(distanceVectorYCoordinate * distanceVectorYCoordinate) +
					(distanceVectorZCoordinate * distanceVectorZCoordinate)
			) + softeningFactorSquared; // to avoid zero in the following divisions

			const float normalizedDistanceVectorXCoordinate = distanceVectorXCoordinate / distance;
			const float normalizedDistanceVectorYCoordinate = distanceVectorYCoordinate / distance;
			const float normalizedDistanceVectorZCoordinate = distanceVectorZCoordinate / distance;
			const float receivedForce = masses[i] / (distance * distance);
			// It is possible to write directly in to the accelerations array here, but the false sharing will
			// reduce in this loop significantly the performance
			forceVector[0] += (receivedForce * normalizedDistanceVectorXCoordinate);
			forceVector[1] += (receivedForce * normalizedDistanceVectorYCoordinate);
			forceVector[2] += (receivedForce * normalizedDistanceVectorZCoordinate);
		}
	}
	// false sharing is ok here
	accelerations[xCoordinateIndexBody1] =
			CudaToolkit::GpuCodeUtilities::castDoubleToFloat(GRAVITATIONAL_CONSTANT * forceVector[0]);
	accelerations[yCoordinateIndexBody1] =
			CudaToolkit::GpuCodeUtilities::castDoubleToFloat(GRAVITATIONAL_CONSTANT * forceVector[1]);
	accelerations[zCoordinateIndexBody1] =
			CudaToolkit::GpuCodeUtilities::castDoubleToFloat(GRAVITATIONAL_CONSTANT * forceVector[2]);
}

using namespace physics;

__host__ void CudaAccelerationCalculationImpl::calcAccelerations(
		const Bodies<float, float, float> &bodies,
		const size_t numBodies,
		float *accelerations,
		const float squaredSofteningFactor
) {
	const size_t floatScalarGpuBufferSize = sizeof(float) * numBodies;
	// reuse floatScalarGpuBufferSize in this calculation:
	const size_t float3dVectorGpuBufferSize = floatScalarGpuBufferSize * 3;

	CudaToolkit::GpuMemory<float> massesGpuBuffer =
			CudaToolkit::gpuFacade.allocateGpuMemory<float>(floatScalarGpuBufferSize);
	CudaToolkit::GpuMemory<float> positionsGpuBuffer =
			CudaToolkit::gpuFacade.allocateGpuMemory<float>(float3dVectorGpuBufferSize);
	CudaToolkit::GpuMemory<float> accelerationsGpuBuffer =
			CudaToolkit::gpuFacade.allocateGpuMemory<float>(float3dVectorGpuBufferSize);

	CudaToolkit::gpuFacade.copyDataFromCpuMemoryToGpuMemory<float>(bodies.masses, massesGpuBuffer,
																   floatScalarGpuBufferSize);
	CudaToolkit::gpuFacade.copyDataFromCpuMemoryToGpuMemory<float>(bodies.positions, positionsGpuBuffer,
																   float3dVectorGpuBufferSize);

	int maxSuggestedBlockSize = 0; // Suggested block size to achieve maximum occupancy.
	int unused = 0;
	cudaOccupancyMaxPotentialBlockSize(
			&unused,
			&maxSuggestedBlockSize,
			calcAccelerationsKernel,
			unused,
			static_cast<int>(numBodies)
	);

	const int numThreadsPerBlock = std::min(static_cast<int>(numBodies), maxSuggestedBlockSize);
	const int numBlocks = std::ceil(static_cast<float>(numBodies) / static_cast<float>(numThreadsPerBlock));
	calcAccelerationsKernel<<<numBlocks, numThreadsPerBlock>>>(
			massesGpuBuffer,
			positionsGpuBuffer,
			accelerationsGpuBuffer,
			numBodies,
			squaredSofteningFactor
	);

	// This copy blocks until the above kernel call returns
	CudaToolkit::gpuFacade.copyDataFromGpuMemoryToCpuMemory(
			accelerationsGpuBuffer,
			accelerations,
			float3dVectorGpuBufferSize
	);
}
