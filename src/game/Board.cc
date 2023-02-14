// Copyright 2023 Andrew Hughes
// jace/src/Board.cc

#include <jace/include/game/Board.h>

#include <iostream>

namespace jace {

namespace game {

Board::Board() {
    board_bitboard_ = (Bitboard*) malloc(sizeof(Bitboard) * 12);
    board_mailbox_ = (Piece*) malloc(sizeof(Piece) * 64);
    history_ = (HistoryInfo*) malloc(sizeof(HistoryInfo) * 256);

    board_bitboard_[kWhiteRook] = 0x81;
    board_bitboard_[kWhiteKnight] = 0x42;
    board_bitboard_[kWhiteBishop] = 0x24;
    board_bitboard_[kWhiteQueen] = 0x8;
    board_bitboard_[kWhiteKing] = 0x10;
    board_bitboard_[kWhitePawn] = 0xff00;
    board_bitboard_[kBlackRook]  = 0x8100000000000000;
    board_bitboard_[kBlackKnight] = 0x4200000000000000;
    board_bitboard_[kBlackBishop] = 0x2400000000000000;
    board_bitboard_[kBlackQueen] = 0x800000000000000;
    board_bitboard_[kBlackKing] = 0x1000000000000000;
    board_bitboard_[kBlackPawn]  = 0xff000000000000;
    for (size_t index = 0; index < 64; index++) {
        board_mailbox_[index] = kDefaultBoard[index];
    }
    current_player_ = Color::kWhite;
}

Piece FenHelper(char c) {
    switch (c) {
        case 'p': return Piece::kBlackPawn;
        case 'b': return Piece::kBlackBishop;
        case 'r': return Piece::kBlackRook;
        case 'n': return Piece::kBlackKnight;
        case 'q': return Piece::kBlackQueen;
        case 'k': return Piece::kBlackKing;
        case 'P': return Piece::kWhitePawn;
        case 'B': return Piece::kWhiteBishop;
        case 'R': return Piece::kWhiteRook;
        case 'N': return Piece::kWhiteKnight;
        case 'Q': return Piece::kWhiteQueen;
        case 'K': return Piece::kWhiteKing;
        default: return Piece::kNoPiece;
    }
}

Board::Board(std::string fen) {
    board_bitboard_ = (Bitboard*) malloc(sizeof(Bitboard) * 12);
    board_mailbox_ = (Piece*) malloc(sizeof(Piece) * 64);
    history_ = (HistoryInfo*) malloc(sizeof(HistoryInfo) * 256);
    game_ply_ = 0;

    board_bitboard_[kWhiteRook] = 0;
    board_bitboard_[kWhiteKnight] = 0;
    board_bitboard_[kWhiteBishop] = 0;
    board_bitboard_[kWhiteQueen] = 0;
    board_bitboard_[kWhiteKing] = 0;
    board_bitboard_[kWhitePawn] = 0;
    board_bitboard_[kBlackRook]  = 0;
    board_bitboard_[kBlackKnight] = 0;
    board_bitboard_[kBlackBishop] = 0;
    board_bitboard_[kBlackQueen] = 0;
    board_bitboard_[kBlackKing] = 0;
    board_bitboard_[kBlackPawn]  = 0;
    for (size_t index = 0; index < 64; index++) {
        board_mailbox_[index] = Piece::kNoPiece;
    }

    const size_t size = fen.size();
    size_t iter = 0;
    int x = 0;
    int y = 7;
    for (; (iter < size) && (fen[iter] != ' '); iter++) {
        if (fen[iter] == '/')
            continue;

        if (isdigit(fen[iter])) {
            x += fen[iter] - '0' - 1;
        } else {
            board_mailbox_[x + (y * 8)] = FenHelper(fen[iter]);
            board_bitboard_[FenHelper(fen[iter])] |= 1ULL << (x + (y * 8));
        }

        x++;
        if (x > 7) {
            x = 0;
            y--;
        }
    }

    history_[game_ply_] = HistoryInfo();
    history_[game_ply_].SetSquaresMoved(kSquares[e8]);

    current_player_ = Color::kWhite;
}

void Board::Display() {
    int x, y, piece;

    std::string kPieceRepresentation[13] = {
        "\u265C",
        "\u265E",
        "\u265D",
        "\u265B",
        "\u265A",
        "\u265F",
        "\u2656",
        "\u2658",
        "\u2657",
        "\u2655",
        "\u2654",
        "\u2659",
        " "
    };

    for (y = 7; y >= 0; y--)
    {
        std::cout << y + 1 << " |";
        for (x = 0; x < 8; x++)
        {
            std::cout << kPieceRepresentation[GetPiece(Square(y * 8 + x))];
            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "   ----------------\n";
    std::cout << "   a b c d e f g h\n";
}

void Board::PutPiece(Piece piece, Square square) {
    board_bitboard_[piece] |= 1ULL << square;
    board_mailbox_[square] = piece;
}

void Board::RemovePiece(Square square) {
    board_bitboard_[GetPiece(square)] &= ~(1ULL << square);
    board_mailbox_[square] = kNoPiece;
}

void Board::MovePiece(Square from, Square to) {
    Bitboard mask = (1ULL << from) | (1ULL << to);
    board_bitboard_[board_mailbox_[from]] ^= mask;
    board_bitboard_[board_mailbox_[to]] &= ~mask;
    board_mailbox_[to] = board_mailbox_[from];
    board_mailbox_[from] = kNoPiece;
}

void Board::MovePieceQuiet(Square from, Square to) {
    board_bitboard_[board_mailbox_[from]] ^= ((1ULL << from) | (1ULL << to));
    board_mailbox_[to] = board_mailbox_[from];
    board_mailbox_[from] = kNoPiece;
}

Piece Board::GetPiece(Square square) {
    return board_mailbox_[square];
}

void Board::MakeMove(Move move) {
    current_player_ = ~current_player_;
    game_ply_++;
    history_[game_ply_] = HistoryInfo(history_[game_ply_ - 1]);

    Square from = move.GetFrom();
    Square to = move.GetTo();
    MoveFlags type = move.GetFlags();

    history_[game_ply_].SetSquaresMoved(history_[game_ply_].GetSquaresMoved() | (1ULL << from) | (1ULL << to));

    switch (type) {
        case MoveFlags::kQuietMove:
            MovePieceQuiet(from, to);
            break;
        case MoveFlags::kDoublePawnPush:
            MovePieceQuiet(from, to);
            history_[game_ply_].SetEnPassentSquare(from + RelativeDirection(~current_player_, kNorth));
            break;
        case MoveFlags::kKingCastle:
            if (current_player_ == Color::kBlack) {
                MovePieceQuiet(e1, g1);
                MovePieceQuiet(h1, f1);
            } else {
                MovePieceQuiet(e8, g8);
                MovePieceQuiet(h8, f8);
            }
            break;
        case MoveFlags::kQueenCastle:
            if (current_player_ == Color::kBlack) {
                MovePieceQuiet(e1, c1);
                MovePieceQuiet(a1, d1);
            } else {
                MovePieceQuiet(e8, c8);
                MovePieceQuiet(a8, d8);
            }
            break;
        case MoveFlags::kCapture:
            history_[game_ply_].SetCaptured(GetPiece(to));
            MovePiece(from, to);
            break;
        case MoveFlags::kEnPassent:
            MovePieceQuiet(from, to);
            RemovePiece(to + RelativeDirection(~current_player_, kSouth));
            break;
        case MoveFlags::kKnightPromotion:
            RemovePiece(from);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteKnight, to);
            } else {
                PutPiece(kBlackKnight, to);
            }
            break;
        case MoveFlags::kBishopPromotion:
            RemovePiece(from);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteBishop, to);
            } else {
                PutPiece(kBlackBishop, to);
            }
            break;
        case MoveFlags::kRookPromotion:
            RemovePiece(from);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteRook, to);
            } else {
                PutPiece(kBlackRook, to);
            }
            break;
        case MoveFlags::kQueenPromotion:
            RemovePiece(from);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteQueen, to);
            } else {
                PutPiece(kBlackQueen, to);
            }
            break;
        case MoveFlags::kKnightPromotionCapture:
            RemovePiece(from);
            history_[game_ply_].SetCaptured(GetPiece(to));
            RemovePiece(to);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteKnight, to);
            } else {
                PutPiece(kBlackKnight, to);
            }
            break;
        case MoveFlags::kBishopPromotionCapture:
            RemovePiece(from);
            history_[game_ply_].SetCaptured(GetPiece(to));
            RemovePiece(to);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteBishop, to);
            } else {
                PutPiece(kBlackBishop, to);
            }
            break;
        case MoveFlags::kRookPromotionCapture:
            RemovePiece(from);
            history_[game_ply_].SetCaptured(GetPiece(to));
            RemovePiece(to);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteRook, to);
            } else {
                PutPiece(kBlackRook, to);
            }
            break;
        case MoveFlags::kQueenPromotionCapture:
            RemovePiece(from);
            history_[game_ply_].SetCaptured(GetPiece(to));
            RemovePiece(to);
            if (~current_player_ == Color::kWhite) {
                PutPiece(kWhiteQueen, to);
            } else {
                PutPiece(kBlackQueen, to);
            }
            break;
        default:
            break;
    }
}

void Board::UnmakeMove(Move move) {
    Square from = move.GetFrom();
    Square to = move.GetTo();
    MoveFlags type = move.GetFlags();

    switch (type) {
        case MoveFlags::kQuietMove:
            MovePieceQuiet(to, from);
            break;
        case MoveFlags::kDoublePawnPush:
            MovePieceQuiet(to, from);
            break;
        case MoveFlags::kKingCastle:
            if (current_player_ == Color::kBlack) {
                MovePieceQuiet(g1, e1);
                MovePieceQuiet(f1, h1);
            } else {
                MovePieceQuiet(g8, e8);
                MovePieceQuiet(f8, h8);
            }
            break;
        case MoveFlags::kQueenCastle:
            if (current_player_ == Color::kBlack) {
                MovePieceQuiet(c1, e1);
                MovePieceQuiet(d1, a1);
            } else {
                MovePieceQuiet(c8, e8);
                MovePieceQuiet(d8, a8);
            }
            break;
        case MoveFlags::kCapture:
            MovePieceQuiet(to, from);
            PutPiece(history_[game_ply_].GetCaptured(), to);
            break;
        case MoveFlags::kEnPassent:
            MovePieceQuiet(to, from);
            PutPiece(~current_player_ == Color::kWhite ? kBlackPawn : kWhitePawn, to + RelativeDirection(~current_player_, kSouth));
            break;
        case MoveFlags::kKnightPromotion:
        case MoveFlags::kBishopPromotion:
        case MoveFlags::kRookPromotion:
        case MoveFlags::kQueenPromotion:
            RemovePiece(to);
            PutPiece(~current_player_ == Color::kWhite ? kWhitePawn : kBlackPawn, from);
            break;
        case MoveFlags::kKnightPromotionCapture:
        case MoveFlags::kBishopPromotionCapture:
        case MoveFlags::kRookPromotionCapture:
        case MoveFlags::kQueenPromotionCapture:
            RemovePiece(to);
            PutPiece(~current_player_ == Color::kWhite ? kWhitePawn : kBlackPawn, from);
            PutPiece(history_[game_ply_].GetCaptured(), to);
            break;
    }
    current_player_ = ~current_player_;
    --game_ply_;
}

Bitboard Board::GetPieces(Color current_player_) {
    return board_bitboard_[kWhiteRook + static_cast<int>(current_player_) * 6] | 
        board_bitboard_[kWhiteKnight + static_cast<int>(current_player_) * 6] | 
        board_bitboard_[kWhiteBishop + static_cast<int>(current_player_) * 6] | 
        board_bitboard_[kWhiteQueen + static_cast<int>(current_player_) * 6] | 
        board_bitboard_[kWhiteKing + static_cast<int>(current_player_) * 6] | 
        board_bitboard_[kWhitePawn + static_cast<int>(current_player_) * 6];
}

Bitboard Board::GetDiagonalSlidingPieces(Color color) {
    return board_bitboard_[kWhiteQueen + static_cast<int>(color) * 6] | board_bitboard_[kWhiteBishop + static_cast<int>(color) * 6];
}

Bitboard Board::GetOrthogonalSlidingPieces(Color color) {
    return board_bitboard_[kWhiteQueen + static_cast<int>(color) * 6] | board_bitboard_[kWhiteRook + static_cast<int>(color) * 6];
}

Square Board::GetIndexOfKing(Color current_player_) {
    return Square(arch::BitScanForward(board_bitboard_[kWhiteKing + 6 * static_cast<int>(current_player_)]));
}

size_t Board::GenerateLegalMoves(Move *moves) {
    size_t n_moves = 0;

    Bitboard friend_pieces = GetPieces(current_player_);
    Bitboard enemy_pieces = GetPieces(~current_player_);
    Bitboard all_pieces = friend_pieces | enemy_pieces;

        // generate pawn moves

    Bitboard pawn_pushes = (current_player_ == Color::kWhite ? ((board_bitboard_[kWhitePawn] & friend_pieces) << 8) : ((board_bitboard_[kBlackPawn] & friend_pieces) >> 8)) & ~all_pieces;
    Bitboard pawn_double_pushes = (current_player_ == Color::kWhite ? ((pawn_pushes & 0xff0000) << 8) : ((pawn_pushes & 0xff0000000000) >> 8)) & ~all_pieces;

    while (pawn_pushes) {
        uint16_t next_push = arch::PopBitScanForward(&pawn_pushes);
        if (kSquares[next_push] & 0xff000000000000ffULL) {
            *moves++ = Move(Square(next_push - (current_player_ == Color::kWhite ? 8 : -8)), Square(next_push), MoveFlags::kBishopPromotion);
            *moves++ = Move(Square(next_push - (current_player_ == Color::kWhite ? 8 : -8)), Square(next_push), MoveFlags::kRookPromotion);
            *moves++ = Move(Square(next_push - (current_player_ == Color::kWhite ? 8 : -8)), Square(next_push), MoveFlags::kKnightPromotion);
            *moves++ = Move(Square(next_push - (current_player_ == Color::kWhite ? 8 : -8)), Square(next_push), MoveFlags::kQueenPromotion);
            n_moves += 4;
        } else {
            *moves++ = Move(Square(next_push - (current_player_ == Color::kWhite ? 8 : -8)), Square(next_push), MoveFlags::kQuietMove);
            n_moves++;
        }
    }
    while (pawn_double_pushes) {
        uint16_t next_double_push = arch::PopBitScanForward(&pawn_double_pushes);
        *moves++ = Move(Square(next_double_push - (current_player_ == Color::kWhite ? 16 : -16)), Square(next_double_push), MoveFlags::kDoublePawnPush);
        n_moves++;
    }

    Bitboard pawns = board_bitboard_[current_player_ == Color::kWhite ? kWhitePawn : kBlackPawn];
    Bitboard pawn_attacks = 0;
    Bitboard pawn_push = 0;
    while (pawns) {
        uint16_t next_pawn = arch::PopBitScanForward(&pawns);
        Bitboard pawn_bb = 1ULL << next_pawn;

        Bitboard pawn_attacks = kPawnAttacks[static_cast<int>(current_player_)][next_pawn] & enemy_pieces;
        while (pawn_attacks) {
            Square next_pawn_attack = Square(arch::PopBitScanForward(&pawn_attacks));
            if (kSquares[next_pawn_attack] & 0xff000000000000ffULL) {
                *moves++ = Move(Square(next_pawn), next_pawn_attack, MoveFlags::kBishopPromotionCapture);
                *moves++ = Move(Square(next_pawn), next_pawn_attack, MoveFlags::kRookPromotionCapture);
                *moves++ = Move(Square(next_pawn), next_pawn_attack, MoveFlags::kKnightPromotionCapture);
                *moves++ = Move(Square(next_pawn), next_pawn_attack, MoveFlags::kQueenPromotionCapture);
                n_moves += 4;
            } else {
                *moves++ = Move(Square(next_pawn), next_pawn_attack, MoveFlags::kCapture);
                n_moves++;
            }
        }


        Bitboard en_passent_attacks = (kSquares[history_[game_ply_].GetEnPassentSquare()]) & (current_player_ == Color::kWhite ? 0xff0000000000 : 0xff0000) & kPawnAttacks[static_cast<int>(current_player_)][next_pawn];
        if (en_passent_attacks) {
            *moves++ = Move(Square(next_pawn), history_[game_ply_].GetEnPassentSquare(), MoveFlags::kEnPassent);
            n_moves++;
        }
    }

    Bitboard rooks = board_bitboard_[current_player_ == Color::kWhite ? kWhiteRook : kBlackRook] & friend_pieces;
    while (rooks) {
        uint16_t next_rook = arch::PopBitScanForward(&rooks);
        
        Bitboard rook_attacks = RookAttack(all_pieces, Square(next_rook));

        Bitboard attacks = rook_attacks & enemy_pieces;
        Bitboard rook_moves = rook_attacks & ~all_pieces;

        while (attacks) {
            uint16_t next_attack = arch::PopBitScanForward(&attacks);
            *moves++ = Move(Square(next_rook), Square(next_attack), MoveFlags::kCapture);
            n_moves++;
        }
        while (rook_moves) {
            uint16_t next_move = arch::PopBitScanForward(&rook_moves);
            *moves++ = Move(Square(next_rook), Square(next_move), MoveFlags::kQuietMove);
            n_moves++;
        }
    }

    // generate bishop moves

    Bitboard bishops = board_bitboard_[current_player_ == Color::kWhite ? kWhiteBishop : kBlackBishop] & friend_pieces;
    while (bishops) {
        uint16_t next_bishop = arch::PopBitScanForward(&bishops);
        
        Bitboard bishop_attacks = BishopAttack(all_pieces, Square(next_bishop));

        Bitboard attacks = bishop_attacks & enemy_pieces;
        Bitboard bishop_moves = bishop_attacks & ~all_pieces;

        while (attacks) {
            uint16_t next_attack = arch::PopBitScanForward(&attacks);
            *moves++ = Move(Square(next_bishop), Square(next_attack), MoveFlags::kCapture);
            n_moves++;
        }
        while (bishop_moves) {
            uint16_t next_move = arch::PopBitScanForward(&bishop_moves);
            *moves++ = Move(Square(next_bishop), Square(next_move), MoveFlags::kQuietMove);
            n_moves++;
        }
    }

    // generate queen moves

    Bitboard queens = board_bitboard_[current_player_ == Color::kWhite ? kWhiteQueen : kBlackQueen] & friend_pieces;
    while (queens) {
        uint16_t next_queen = arch::PopBitScanForward(&queens);
        
        Bitboard queen_attacks = BishopAttack(all_pieces, Square(next_queen));

        queen_attacks |= RookAttack(all_pieces, Square(next_queen));

        Bitboard attacks = queen_attacks & enemy_pieces;
        Bitboard queen_moves = queen_attacks & ~all_pieces;

        while (attacks) {
            uint16_t next_attack = arch::PopBitScanForward(&attacks);
            *moves++ = Move(Square(next_queen), Square(next_attack), MoveFlags::kCapture);
            n_moves++;
        }
        while (queen_moves) {
            uint16_t next_move = arch::PopBitScanForward(&queen_moves);
            *moves++ = Move(Square(next_queen), Square(next_move), MoveFlags::kQuietMove);
            n_moves++;
        }
    }

    // generate knight moves

    Bitboard knights = board_bitboard_[current_player_ == Color::kWhite ? kWhiteKnight : kBlackKnight] & friend_pieces;
    while (knights) {
        uint16_t next_knight = arch::PopBitScanForward(&knights);
        Bitboard knight_attacks = kKnightMoves[next_knight] & enemy_pieces;
        Bitboard knight_moves = kKnightMoves[next_knight] & ~all_pieces;

        while (knight_attacks) {
            uint16_t next_attack = arch::PopBitScanForward(&knight_attacks);
            *moves++ = Move(Square(next_knight), Square(next_attack), MoveFlags::kCapture);
            n_moves++;
        }

        while (knight_moves) {
            uint16_t next_move = arch::PopBitScanForward(&knight_moves);
            *moves++ = Move(Square(next_knight), Square(next_move), MoveFlags::kQuietMove);
            n_moves++;
        }
        
    }



    // generate king moves

    Bitboard king_attacks = kKingMoves[GetIndexOfKing(current_player_)] & enemy_pieces;
    Bitboard king_moves = kKingMoves[GetIndexOfKing(current_player_)] & ~all_pieces;
    while (king_attacks) {
        uint16_t next_king_attack = arch::PopBitScanForward(&king_attacks);

        *moves++ = Move(Square(GetIndexOfKing(current_player_)), Square(next_king_attack), MoveFlags::kCapture);
        n_moves++;
    }

    while (king_moves) {
        uint16_t next_king_move = arch::PopBitScanForward(&king_moves);

        *moves++ = Move(Square(GetIndexOfKing(current_player_)), Square(next_king_move), MoveFlags::kQuietMove);
        n_moves++;
    }

    // castling

    if (current_player_ == Color::kWhite && HasCastlingRights(0)) {
        Bitboard all_pieces = GetPieces(Color::kBlack) | GetPieces(Color::kWhite);
        if ((kSquares[g1] & ~all_pieces) && (kSquares[f1] & ~all_pieces) && !SquareAttacked(e1) && !SquareAttacked(f1)) {
            *moves++ = Move(e1, g1, MoveFlags::kKingCastle);
            n_moves++;
        }
    }
    if (current_player_ == Color::kWhite && HasCastlingRights(1)) {
        Bitboard all_pieces = GetPieces(Color::kBlack) | GetPieces(Color::kWhite);
        if ((kSquares[d1] & ~all_pieces) && (kSquares[c1] & ~all_pieces)&& (kSquares[b1] & ~all_pieces) && !SquareAttacked(e1) && !SquareAttacked(d1)) {
            *moves++ = Move(e1, c1, MoveFlags::kQueenCastle);
            n_moves++;
        }
    }
    if (current_player_ == Color::kBlack && HasCastlingRights(2)) {
        Bitboard all_pieces = GetPieces(Color::kBlack) | GetPieces(Color::kWhite);
        if ((kSquares[g8] & ~all_pieces) && (kSquares[f8] & ~all_pieces) && !SquareAttacked(e8) && !SquareAttacked(f8)) {
            *moves++ = Move(e8, g8, MoveFlags::kKingCastle);
            n_moves++;
        }
    }
    if (current_player_ == Color::kBlack && HasCastlingRights(3)) {
        Bitboard all_pieces = GetPieces(Color::kBlack) | GetPieces(Color::kWhite);
        if ((kSquares[d8] & ~all_pieces) && (kSquares[c8] & ~all_pieces )&& (kSquares[b8] & ~all_pieces) && !SquareAttacked(e8) && !SquareAttacked(d8)) {
            *moves++ = Move(e8, c8, MoveFlags::kQueenCastle);
            n_moves++;
        }
    }

    return n_moves;
}

bool Board::HasCastlingRights(int side) {
    switch (side) {
        case 0:
            return !(history_[game_ply_].GetSquaresMoved() & 0x90ULL);
        case 1:
            return !(history_[game_ply_].GetSquaresMoved() & 0x11ULL);
        case 2:
            return !(history_[game_ply_].GetSquaresMoved() & 0x9000000000000000ULL);
        case 3:
            return !(history_[game_ply_].GetSquaresMoved() & 0x1100000000000000ULL);
        default: return false;
    }
}

bool Board::SquareAttacked(Square square) {
        Bitboard diagnol_checkers = 0;
    Bitboard orthogonal_checkers = 0;

    Bitboard friend_pieces = GetPieces(current_player_);
    Bitboard enemy_pieces = GetPieces(~current_player_);

    Bitboard enemy_orthogonal_sliding_pieces = GetOrthogonalSlidingPieces(~current_player_);
    Bitboard enemy_diagonal_sliding_pieces = GetDiagonalSlidingPieces(~current_player_);

    // checks bishop rook and queen

    diagnol_checkers |= kRays[kNorthEastRays][square];
    if (kRays[kNorthEastRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kNorthEastRays][square] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kNorthEastRays][blocker_index];
    }

    diagnol_checkers |= kRays[kNorthWestRays][square];
    if (kRays[kNorthWestRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kNorthWestRays][square] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kNorthWestRays][blocker_index];
    }

    diagnol_checkers |= kRays[kSouthWestRays][square];
    if (kRays[kSouthWestRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kSouthWestRays][square] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kSouthWestRays][blocker_index];
    }

    diagnol_checkers |= kRays[kSouthEastRays][square];
    if (kRays[kSouthEastRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kSouthEastRays][square] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kSouthEastRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kEastRays][square];
    if (kRays[kEastRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kEastRays][square] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kEastRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kNorthRays][square];
    if (kRays[kNorthRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kNorthRays][square] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kNorthRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kWestRays][square];
    if (kRays[kWestRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kWestRays][square] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kWestRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kSouthRays][square];
    if (kRays[kSouthRays][square] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kSouthRays][square] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kSouthRays][blocker_index];
    }

    if ((orthogonal_checkers & enemy_orthogonal_sliding_pieces) || (diagnol_checkers & enemy_diagonal_sliding_pieces))
        return true;

    // checks knight

    Bitboard knight_attacks = kKnightMoves[square];

    if (knight_attacks & board_bitboard_[kBlackKnight - static_cast<int>(current_player_) * 6])
        return true;

    // checks pawn

    Bitboard pawn_attacks = kPawnAttacks[static_cast<int>(current_player_)][square];

    if (pawn_attacks & board_bitboard_[kBlackPawn - static_cast<int>(current_player_) * 6])
        return true;

    // checks king

    Bitboard king_attacks = kKingMoves[square];

    if (king_attacks & board_bitboard_[kBlackKing - static_cast<int>(current_player_) * 6])
        return true;

    return false;
}

bool Board::IsInCheck(Color color) {
    uint16_t index_of_king = GetIndexOfKing(color);

    Bitboard diagnol_checkers = 0;
    Bitboard orthogonal_checkers = 0;

    Bitboard friend_pieces = GetPieces(color);
    Bitboard enemy_pieces = GetPieces(~color);

    Bitboard enemy_orthogonal_sliding_pieces = GetOrthogonalSlidingPieces(~color);
    Bitboard enemy_diagonal_sliding_pieces = GetDiagonalSlidingPieces(~color);

    // checks bishop rook and queen

    diagnol_checkers |= kRays[kNorthEastRays][index_of_king];
    if (kRays[kNorthEastRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kNorthEastRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kNorthEastRays][blocker_index];
    }

    diagnol_checkers |= kRays[kNorthWestRays][index_of_king];
    if (kRays[kNorthWestRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kNorthWestRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kNorthWestRays][blocker_index];
    }

    diagnol_checkers |= kRays[kSouthWestRays][index_of_king];
    if (kRays[kSouthWestRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kSouthWestRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kSouthWestRays][blocker_index];
    }

    diagnol_checkers |= kRays[kSouthEastRays][index_of_king];
    if (kRays[kSouthEastRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kSouthEastRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        diagnol_checkers &= ~kRays[kSouthEastRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kEastRays][index_of_king];
    if (kRays[kEastRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kEastRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kEastRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kNorthRays][index_of_king];
    if (kRays[kNorthRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanForward(kRays[kNorthRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kNorthRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kWestRays][index_of_king];
    if (kRays[kWestRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kWestRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kWestRays][blocker_index];
    }

    orthogonal_checkers |= kRays[kSouthRays][index_of_king];
    if (kRays[kSouthRays][index_of_king] & (friend_pieces | (enemy_pieces)))
    {
        int blocker_index = arch::BitScanReverse(kRays[kSouthRays][index_of_king] & (friend_pieces | (enemy_pieces)));
        orthogonal_checkers &= ~kRays[kSouthRays][blocker_index];
    }

    if ((orthogonal_checkers & enemy_orthogonal_sliding_pieces) || (diagnol_checkers & enemy_diagonal_sliding_pieces)) {
        return true;
    }

    // checks knight

    Bitboard knight_attacks = kKnightMoves[index_of_king];

    if (knight_attacks & board_bitboard_[kBlackKnight - static_cast<int>(color) * 6]) {
        return true;
    }

    // checks pawn

    Bitboard pawn_attacks = kPawnAttacks[static_cast<int>(color)][index_of_king];

    if (pawn_attacks & board_bitboard_[kBlackPawn - static_cast<int>(color) * 6]) {
        return true;
    }

    // checks kings

    Bitboard king_attacks = kKingMoves[index_of_king];

    if (king_attacks & board_bitboard_[kBlackKing - static_cast<int>(color) * 6])
        return true;

    return false;
}

}  // namespace game

}  // namespace jace
