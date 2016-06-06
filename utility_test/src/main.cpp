#include "../../utility/include/ratio"
#include "../../utility/include/exception"
#include "../../utility/include/array"
#include "../../utility/include/abstract_data_types/sequences/contiguous_sequence"
#include "../../utility/include/allocator"
#include "../../utility/include/initializer_list"
#include <iostream>

template <typename T, typename AllocatorPolicy = jang::standard_based_allocator_policy<T>>
using my_vector = jang::sequence_container<T, jang::contiguous_sequence, 1, 2, jang::random_access_iterator, AllocatorPolicy>;


int main(int argc, char const *argv[]) {	
	
	try {
		std::cout << "create vector" << std::endl;
		my_vector<int> v;
		std::cout << "insert elements" << std::endl;
		v.push_back(0);
		std::cout << v[0] << std::endl;
		v.push_back(1);
		std::cout << v[1] << std::endl;
		v.push_back(2);
		std::cout << v[2] << std::endl;
		v.push_back(3);
		std::cout << v[3] << std::endl;

		std::cout << "iterate elements" << std::endl;
		for (auto it = v.begin(); it != v.end(); ++it) {
			std::cout << "vector has " << *it << std::endl;
 		}

		std::cout << "making an array" << std::endl;
		jang::array<int, 10> a;
		std::cout << "filling an array" << std::endl;

		for (jang::size_type i = 0; i < 10; ++i) {
			std::cout << "setting element: " << i << " as " << i << std::endl;
			a[i] = i;
		}

		a[5] = 32;
		std::cout << *(a.data() + 8) << *(a.end() - 1) << a[9] << std::endl;
		std::cout << "output time" << std::endl;
		std::cout << a.front() << a.at(5) << a.back() << std::endl;
		std::cout <<"job done" <<std::endl;
	} catch (jang::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	try {
		std::cout << "ratio time" << std::endl;
		jang::ratio q(5,4);
		jang::ratio p(2,3);
		jang::ratio o = q * p;
		std::cout << o <<std::endl;
		jang::ratio r = 5 * jang::ratio(3, 2);
		std::cout << "5 * (3/2) = ";
		std::cout << r << std::endl;
		std::cout << "(15/2) * (1/3) = ";
		r *= jang::ratio(1,3);
		std::cout << r << std::endl;
		std::cout << "(5/2) / (1/3) = ";
		r /= jang::ratio(1,3);
		std::cout << r <<std::endl;
		std::cout << "(15/2) + (1/3) = ";
		r += jang::ratio(1,3);
		std::cout << r <<std::endl;
		std::cout << "(47/6) - (1/3) = ";
		r -= jang::ratio(1,3);
		std::cout << r <<std::endl;

		std::cout << (jang::ratio(143, 765) < jang::ratio(912, 8578)) << std::endl;
		std::cout << (jang::ratio(765, 143) < jang::ratio(912, 8578)) << std::endl;
		std::cout << (jang::ratio(143, 765) < jang::ratio(8578, 912)) << std::endl;
		std::cout << (jang::ratio(765, 143) < jang::ratio(8578, 912)) << std::endl;

		std::cout << (jang::ratio(143, 765) > jang::ratio(912, 8578)) << std::endl;
		std::cout << (jang::ratio(765, 143) > jang::ratio(912, 8578)) << std::endl;
		std::cout << (jang::ratio(143, 765) > jang::ratio(8578, 912)) << std::endl;
		std::cout << (jang::ratio(765, 143) > jang::ratio(8578, 912)) << std::endl;

	} catch (jang::exception& ex) {
		std::cout << ex.what();
	}
	// try {
	// 	throw jang::out_of_range();
	// } catch (jang::exception& ex) {
	// 	std::cout << ex.what();
	// }
	//int a = jang::forward<int>(9);
	// long double a = 2.0_deci_metre_type;
	// std::cout << a;
	// std::cin >> a;
	//jang::test_registry::instance().run();

	//jang::io::console::output_line(DEFAULT_COLOR_SCHEME, jang::test_registry::instance().size());

	return 0;
}
