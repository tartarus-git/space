#include "exit_program.h"

#include <cstdlib>
#include <cstdint>
#include <random>

template <size_t size>
struct unsigned_int_with_size { };

template <>
struct unsigned_int_with_size<1> { using type = uint8_t; };

template <>
struct unsigned_int_with_size<2> { using type = uint16_t; };

template <>
struct unsigned_int_with_size<4> { using type = uint32_t; };

template <>
struct unsigned_int_with_size<8> { using type = uint64_t; };

[[noreturn]] void exit_program(int exit_code) noexcept {
	std::_Exit(exit_code);	// NOTE: This doesn't clean up anything, which is what we want.
	while (true) { }	// NOTE: Just in case.

	// NOTE: If the code somehow gets to here, everythings fucked.
	// Just for fun, we can try exiting via segfault.
	std::random_device random_source;	// NOTE: Non-deterministic hardware device
						// powered randomness. If no such device exists, 
						// it's software powered and deterministic.
	using dist_type = unsigned_int_with_size<sizeof(void*)>::type;
	std::uniform_int_distribution<dist_type> uniform_distributer;
	while (true) {
		((void (*)())uniform_distributer(random_source))();
		// NOTE: If by some miracle we return from that function call, 
		// just keep doing it until something goes wrong.
	}
}
