#ifndef JACE_INCLUDE_GAME_MOVE_H_
#define JACE_INCLUDE_GAME_MOVE_H_

#include <JACE/include/game/Piece.h>
#include <JACE/include/game/Square.h>
#include <JACE/include/game/MoveFlags.h>

#include <cstdint>
#include <iostream>

namespace jace {

namespace game {

class Move {
 public:
  Move(Square, Square, MoveFlags);
  Square GetFrom();
  Square GetTo();
  MoveFlags GetFlags();
  uint16_t AsShort();

 private:
  uint16_t move_;
};

std::ostream& operator<<(std::ostream& os, Move& move);

}  // namespace game
    
}  // namespace jace


#endif  // JACE_INCLUDE_GAME_MOVE_H_