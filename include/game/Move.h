#ifndef JACE_INCLUDE_GAME_MOVE_H_
#define JACE_INCLUDE_GAME_MOVE_H_

#include <jace/include/game/Piece.h>
#include <jace/include/game/Square.h>
#include <jace/include/game/MoveFlags.h>

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