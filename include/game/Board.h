#ifndef JACE_INCLUDE_GAME_BOARD_H_
#define JACE_INCLUDE_GAME_BOARD_H_

#include <JACE/include/game/Piece.h>
#include <JACE/include/game/Color.h>
#include <JACE/include/game/Square.h>
#include <JACE/include/game/Move.h>
#include <JACE/include/game/MoveFlags.h>
#include <JACE/include/game/Bitboard.h>
#include <JACE/include/game/HistoryInfo.h>
#include <JACE/include/arch/arm.h>
#include <JACE/include/game/Tables.h>

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

namespace jace {

namespace game {

const std::array<Piece, 64> kDefaultBoard {
    kWhiteRook, kWhiteKnight, kWhiteBishop, kWhiteQueen, kWhiteKing, kWhiteBishop, kWhiteKnight, kWhiteRook,
    kWhitePawn, kWhitePawn, kWhitePawn, kWhitePawn, kWhitePawn, kWhitePawn, kWhitePawn, kWhitePawn,
    kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece,
    kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece,
    kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece,
    kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece, kNoPiece,
    kBlackPawn, kBlackPawn, kBlackPawn, kBlackPawn, kBlackPawn, kBlackPawn, kBlackPawn, kBlackPawn,
    kBlackRook, kBlackKnight, kBlackBishop, kBlackQueen, kBlackKing, kBlackBishop, kBlackKnight, kBlackRook,
};

class Board {
 public:
  Board();
  Board(std::string);
  void Display();
  void PutPiece(Piece, Square);
  void RemovePiece(Square);
  void MovePiece(Square, Square);
  void MovePieceQuiet(Square, Square);
  Piece GetPiece(Square);
  void MakeMove(Move);
  void UnmakeMove(Move);
  Bitboard GetDiagonalSlidingPieces(Color);
  Bitboard GetOrthogonalSlidingPieces(Color);
  Bitboard GetPieces(Color);
  Square GetIndexOfKing(Color);
  size_t GenerateLegalMoves(Move *moves);
  bool HasCastlingRights(int);
  bool SquareAttacked(Square);
  bool IsInCheck(Color);
  Color current_player_;

 private:
  Bitboard *board_bitboard_;
  Piece *board_mailbox_;
  HistoryInfo *history_;
  uint16_t game_ply_;
};

}  // namespace game

}  // namespace jace

#endif  // JACE_INCLUDE_GAME_BOARD_H_
