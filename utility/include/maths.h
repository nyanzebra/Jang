#pragma once

#include <math.h>

namespace ftl {
namespace maths {

template<size_type N>
constexpr bool is_valid_precision() { return (N - 1) < N; }

template<typename T, size_type N>
struct precision {
	static_assert(is_valid_precision<N>(), "Significant figures must be larger than zero!");

	precision(T val) : val(val) {}
	precision(const precision&) = default;
	precision(precision&&) = default;
	~precision() = default;

	T val;
	T to_precise() { auto Q = pow(10, N - 1); return round(val * Q) / Q; }
};

}
}

