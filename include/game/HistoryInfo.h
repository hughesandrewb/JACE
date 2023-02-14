#ifndef JACE_INCLUDE_GAME_HISTORYINFO_H_
#define JACE_INCLUDE_GAME_HISTORYINFO_H_

#include <JACE/include/game/Piece.h>
#include <JACE/include/game/Square.h>
#include <JACE/include/game/Bitboard.h>

namespace jace {

namespace game {

class HistoryInfo {
 public:
  HistoryInfo();
  HistoryInfo(const HistoryInfo&);
  Piece GetCaptured();
  Bitboard GetSquaresMoved();
  Square GetEnPassentSquare();
  void SetCaptured(Piece);
  void SetSquaresMoved(Bitboard);
  void SetEnPassentSquare(Square);
  
 private:
  Piece captured_;
  Bitboard squares_moved_;
  Square en_passent_square_;
};

}  // namespace game

}  // namespace jace

#endif  // JACE_INCLUDE_GAME_HISTORYINFO_H_