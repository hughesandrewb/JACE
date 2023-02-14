# JACE

Just Another Chess Engine

## Design

### Board

&nbsp;&nbsp;&nbsp;&nbsp;The board is represented by a combination of Bitboards and a piece list, this is down because Bitboard representations are efficient at move generation and validation while an 8x8 board representation is good at finding what piece is at a specific square.

### Move Generation

&nbsp;&nbsp;&nbsp;&nbsp;Move generation is done primarily with the Bitboard representation as it is the most efficient way. Sliding pieces will initially use the classic way to generate sliding piece moves but magic Bitboards would be an ideal next step once I have a working board representation to compare speeds with.

### Evaluation

tbd

## Classes

### Board

#### Variables

board_bitboard_: Bitboard[12]

board_mailbox_: Piece[64]

current_player_: Player

#### Methods

Board()

Board(String Fen)

### MoveGenerator

#### Variables

#### Methods
