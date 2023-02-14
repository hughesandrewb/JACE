#ifndef JACE_INCLUDE_GAME_RNG_H_
#define JACE_INCLUDE_GAME_RNG_H_

#include <cstdint>

namespace jace {

namespace game {

//A psuedorandom number generator
//Source: Stockfish
class PRNG {
	uint64_t s;

	constexpr uint64_t rand64() {
		s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
		return s * 2685821657736338717LL;
	}

public:
	PRNG(uint64_t seed) : s(seed) {}

	//Generate psuedorandom number
	template<typename T> constexpr T rand() { return T(rand64()); }

	//Generate psuedorandom number with only a few set bits
	template<typename T> 
	constexpr T sparse_rand() {
		return T(rand64() & rand64() & rand64());
	}
};

}
}
#endif