#ifndef PHYSICS_ENGINE_BODIES_H
#define PHYSICS_ENGINE_BODIES_H

/**
 * @brief Namespace for physics-related functions and classes.
 */
namespace physics {

	/**
	 * @brief This structure of arrays (SoA) contains physical bodies. Each body has a mass, position and velocity in a space.
	 * @tparam TMass the data type of the mass.
	 * @tparam TPosition the data type of the position.
	 * @tparam TVelocity the data type of the velocity.
	 */
	template<typename TMass, typename TPosition, typename TVelocity>
	struct Bodies {

		/**
		 * The masses of the bodies.
		 */
		TMass *masses;

		/**
		 * The positions of the bodies.
		 */
		TPosition *positions;

		/**
		 * The velocities of the bodies.
		 */
		TVelocity *velocities;
	};
}

#endif //PHYSICS_ENGINE_BODIES_H
