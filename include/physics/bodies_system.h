#ifndef PHYSICS_ENGINE_BODIES_SYSTEM_H
#define PHYSICS_ENGINE_BODIES_SYSTEM_H

#include "bodies.h"
#include "acceleration_calculation.h"
#include "position_velocity_calculation.h"

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief A system of bodies.
	 */
	class BodiesSystem {

		private:
			/**
			 * The current system's bodies.
			 */
			Bodies<float, float, float> bodies_;

			/**
			 * The number of bodies in the current system.
			 */
			size_t numBodies_;

			/**
			 * The pointer to the acceleration calculation used by the current system.
			 */
			IAccelerationCalculation *pAccelerationCalculation_;

			/**
			 * The pointer to the position and velocity calculation used by the current system.
			 */
			IPositionVelocityCalculation *pPositionVelocityCalculation_;

			/**
			 * The squared softening factor in order to avoid division by zero.
			 */
			float squaredSofteningFactor_;

			/**
			 * The accelerations.
			 */
			float *accelerations_;

		public:
			/**
			 * @brief The parameterized Constructor. Creates a new instance of this class by the parameters.
			 * @param bodies the bodies of the system to be created.
			 * @param numBodies the number of bodies.
			 * @param pAccelerationCalculation the pointer to the acceleration calculation to be used by the system.
			 * @param pPositionVelocityCalculation the pointer to the position and velocity calculation to be used by the system.
			 * @param softeningFactor the squared softening factor in order to avoid division by zero.
			 */
			BodiesSystem(
					const Bodies<float, float, float> &bodies,
					size_t numBodies,
					IAccelerationCalculation *pAccelerationCalculation,
					IPositionVelocityCalculation *pPositionVelocityCalculation,
					float softeningFactor
			);

			/**
			 * @brief The destructor.
			 */
			~BodiesSystem();

			/**
			 * @brief Returns the current system's bodies.
			 * @return the current system's bodies.
			 */
			[[nodiscard]] inline Bodies<float, float, float> getBodies() const {
				return bodies_;
			}

			/**
			 * @brief Returns the number of bodies in the current system.
			 * @return the number of bodies in the current system.
			 */
			[[nodiscard]] inline size_t getNumBodies() const {
				return numBodies_;
			}

			/**
			 * @brief Updates the current system's bodies.
			 * @param timeStep the optional time step used to calculate the bodies positions, accelerations, velocities.
			 * If not specified 0.1 is used.
			 */
			void update(float timeStep = 0.1f);
	};
}

#endif //PHYSICS_ENGINE_BODIES_SYSTEM_H
