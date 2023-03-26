#include "physics/bodies_system.h"

using namespace physics;

BodiesSystem::BodiesSystem(
		const Bodies<float, float, float> &bodies,
		const size_t numBodies,
		IAccelerationCalculation *pAccelerationCalculation,
		IPositionVelocityCalculation *pPositionVelocityCalculation,
		float softeningFactor
) : bodies_(bodies),
	numBodies_(numBodies),
	pAccelerationCalculation_(pAccelerationCalculation),
	pPositionVelocityCalculation_(pPositionVelocityCalculation),
	squaredSofteningFactor_(softeningFactor * softeningFactor),
	accelerations_(new float[numBodies * 3]) {

}

BodiesSystem::~BodiesSystem() {
	delete[] accelerations_;
	accelerations_ = nullptr;
}

void BodiesSystem::update(const float timeStep) {
	// 1. calc accelerations
	pAccelerationCalculation_->calcAccelerations(
			bodies_,
			numBodies_,
			accelerations_,
			squaredSofteningFactor_
	);
	// 2. apply accelerations
	pPositionVelocityCalculation_->updatePositionAndVelocity(
			bodies_,
			numBodies_,
			accelerations_,
			timeStep
	);
}