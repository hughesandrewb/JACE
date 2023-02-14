#include <JACE/include/game/MagicEntry.h>

namespace jace {

namespace game {

MagicEntry::MagicEntry() {
    mask_ = 0;
    magic_number_ = 0;
    index_bits_ = 0;
}

MagicEntry::MagicEntry(Bitboard mask, uint64_t magic_number, uint8_t index_bits) {
    mask_ = mask;
    magic_number_ = magic_number;
    index_bits_ = 64 - __builtin_popcountll(mask);
}

}

}
