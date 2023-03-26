// Reminder: Always include standard library and system headers before including your own headers.
#include <type_traits>
#include <gtest/gtest.h>

#include "physics/bodies.h"

using namespace physics;

TEST(BodiesTest, TemplateShouldBeInstantiable) {
	const size_t N = 1'000'000;
	Bodies<long double, double, double> bodies{new long double[N], new double[N], new double[N]};

	for (size_t i = 0; i < N; ++i) {
		bodies.masses[i] = static_cast<long double>(i);
		bodies.positions[i] = static_cast<double>(i);
		bodies.velocities[i] = static_cast<double>(i);
	}

	for (size_t i = 0; i < N; ++i) {
		ASSERT_EQ(bodies.masses[i], i);
		ASSERT_EQ(bodies.positions[i], i);
		ASSERT_EQ(bodies.velocities[i], i);
	}

	delete[] bodies.masses;
	delete[] bodies.positions;
	delete[] bodies.velocities;
}

TEST(BodiesTest, BodiesStructureShouldHasStandardLayout) {
	const auto is_standard_layout = std::is_standard_layout<Bodies<long double, double, double>>::value;
	ASSERT_TRUE(is_standard_layout);
}

TEST(BodiesTest, BodiesStructureShouldBeTrivial) {
	const auto is_trivial = std::is_trivial<Bodies<long double, double, double>>::value;
	ASSERT_TRUE(is_trivial);
}

