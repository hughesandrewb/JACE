#include <JACE/include/game/HistoryInfo.h>

namespace jace {

namespace game {

HistoryInfo::HistoryInfo() {
    captured_ = kNoPiece;
    squares_moved_ = 0ULL;
    en_passent_square_ = kNoSquare;
}

HistoryInfo::HistoryInfo(const HistoryInfo& prev) {
    captured_ = kNoPiece;
    squares_moved_ = prev.squares_moved_;
    en_passent_square_ = kNoSquare;
}

Piece HistoryInfo::GetCaptured() {
    return captured_;
}

Bitboard HistoryInfo::GetSquaresMoved() {
    return squares_moved_;
}

Square HistoryInfo::GetEnPassentSquare() {
    return en_passent_square_;
}

void HistoryInfo::SetCaptured(Piece captured) {
    captured_ = captured;
}

void HistoryInfo::SetSquaresMoved(Bitboard squares_moved) {
    squares_moved_ = squares_moved;
}

void HistoryInfo::SetEnPassentSquare(Square en_passent_square) {
    en_passent_square_ = en_passent_square;
}

}  // namespace game

}  // namespace jace
