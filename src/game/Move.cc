#include <JACE/include/game/Move.h>

namespace jace {

namespace game {

Move::Move(Square from, Square to, MoveFlags flags) {
    move_ = (static_cast<int>(flags) << 12) | (to << 6) | from;
}

Square Move::GetFrom() {
    return (Square) (move_ & 0x3f);
}

Square Move::GetTo() {
    return (Square) ((move_ >> 6) & 0x3f);
}

MoveFlags Move::GetFlags() {
    return (MoveFlags) ((move_ >> 12) & 0xf);
}

uint16_t Move::AsShort() {
    return static_cast<uint16_t>(move_ & 0xffff);
}

std::ostream& operator<<(std::ostream& os, Move& move) {
  os << "from: " << (int) move.GetFrom() << '\n' << "to: " << (int) move.GetTo() << '\n' << "flags: " << static_cast<int>(move.GetFlags()) << '\n';
  return os;
}

}  // namespace game

}  // namespace jace
