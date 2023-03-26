// Reminder: Always include standard library and system headers before including your own headers.
#include <stdexcept>
#include <cassert>

#include "opencl_acceleration_calculation.h"
#include "opencl/device_manager.h"
#include "commons/text_file_reading.h"
#include "config.h"

using namespace physics;

OpenClAccelerationCalculationImpl::OpenClAccelerationCalculationImpl() :
		pContext_(nullptr),
		pCommandQueue_(nullptr),
		pProgram_(nullptr),
		deviceMemoryAlreadyAllocated_(false),
		pMassesBuffer_(nullptr),
		pPositionsBuffer_(nullptr),
		pAccelerationsBuffer_(nullptr) {

	const OpenClToolkit::DeviceManager &deviceManager = OpenClToolkit::DeviceManager::getInstance();
	cl_device_id device;

	// Prefer to use the best GPU
	if (deviceManager.isOpenClCompatibleGpuAvailable()) {
		device = deviceManager.getDeviceWithMostComputeUnits();
	} else if (deviceManager.isDefaultDeviceAvailable()) { // else use the default device, if available
		device = deviceManager.getDefaultOpenClDevice();
	} else {
		// let it crash
		throw std::runtime_error(
				"No OpenCL-compatible processors found on this system\n" +
				deviceManager.constructDebugInfoAboutAvailableOpenClDevicesOnCurrentSystem()
		);
	}

	pContext_ = new OpenClToolkit::Context(device);
	pCommandQueue_ = new OpenClToolkit::CommandQueue(*pContext_, device);

	const std::string kernelSourceCode = commons::io::readTextFile(RESOURCES_FOLDER_PATH"calc_accelerations_kernel.cl");
	pProgram_ = new OpenClToolkit::Program(kernelSourceCode.c_str(), "calcAccelerations", *pContext_, device);
}

OpenClAccelerationCalculationImpl::~OpenClAccelerationCalculationImpl() {
	delete pCommandQueue_;
	pCommandQueue_ = nullptr;
	delete pProgram_;
	pProgram_ = nullptr;
	delete pMassesBuffer_;
	pMassesBuffer_ = nullptr;
	delete pPositionsBuffer_;
	pPositionsBuffer_ = nullptr;
	delete pAccelerationsBuffer_;
	pAccelerationsBuffer_ = nullptr;
	delete pContext_;
	pContext_ = nullptr;
}

void OpenClAccelerationCalculationImpl::calcAccelerations(
		const Bodies<float, float, float> &bodies,
		const size_t numBodies,
		float *accelerations,
		const float squaredSofteningFactor
) {
	const size_t floatScalarBufferSize = sizeof(cl_float) * numBodies;
	const size_t float3dVectorBufferSize = floatScalarBufferSize * 3; // reuse floatScalarBufferSize in this calculation
	bool expected = false;
	if (deviceMemoryAlreadyAllocated_.compare_exchange_strong(expected, true)) {
		// allocate memory on the device
		pMassesBuffer_ = new OpenClToolkit::ReadOnlyBuffer(*pContext_, floatScalarBufferSize);
		pPositionsBuffer_ = new OpenClToolkit::ReadOnlyBuffer(*pContext_, float3dVectorBufferSize);
		pAccelerationsBuffer_ = new OpenClToolkit::WriteOnlyBuffer(*pContext_, float3dVectorBufferSize);

		// set the arguments of the kernel function:
		pProgram_->setKernelArg(0, sizeof(cl_mem), *pMassesBuffer_);
		pProgram_->setKernelArg(1, sizeof(cl_mem), *pPositionsBuffer_);
		pProgram_->setKernelArg(2, sizeof(cl_mem), *pAccelerationsBuffer_);
		pProgram_->setKernelArg(3, sizeof(cl_ulong), &numBodies);
		pProgram_->setKernelArg(4, sizeof(cl_float), &squaredSofteningFactor);
	}

	// transfer data into the device memory
	try {
		pCommandQueue_->enqueueCommandCopyBytesFromHostMemoryIntoDeviceMemory(
				bodies.masses,
				*pMassesBuffer_,
				floatScalarBufferSize
		);
		pCommandQueue_->enqueueCommandCopyBytesFromHostMemoryIntoDeviceMemory(
				bodies.positions,
				*pPositionsBuffer_,
				float3dVectorBufferSize
		);
	} catch (const std::runtime_error &error) {
		pProgram_->printDeviceMemoryInfo(true);
		// rethrow
		throw error;
	}

	// execute the program on the device
	pCommandQueue_->enqueueCommandExecuteProgramOnDevice(*pProgram_, numBodies);

	// get data back from the device memory
	try {
		pCommandQueue_->enqueueCommandCopyBytesFromDeviceMemoryIntoHostMemory(
				*pAccelerationsBuffer_,
				accelerations,
				float3dVectorBufferSize
		);
	} catch (const std::runtime_error &error) {
		pProgram_->printDeviceMemoryInfo(true);
		// rethrow
		throw error;
	}
}
