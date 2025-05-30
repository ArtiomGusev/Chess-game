# Console Chess Game in C++

This is a console-based chess game written in **C++**. It supports full rules of chess, including castling, pawn promotion, legal move validation, and even **move suggestions** using a simple evaluation function.

---

## 🎮 Features

- Full 8x8 chessboard with text-based UI
- Move validation for all standard chess pieces
- Special moves: **castling**, **pawn promotion**, **check detection**
- Move suggestions (basic AI logic)
- Highlight available moves for selected piece
- Basic scoring evaluation and center control heuristics
- Command-line input: `'e2 e4'`, `'hint'`, `'moves e2'`, `'help'`, `'exit'`

---

## 🧱 Technologies

- Language: **C++**
- Platform: Windows console
- No external libraries required (uses `<windows.h>` for coloring)

---

## 🚀 How to Run

1. Compile using a C++ compiler (on Windows):

   ```bash
   g++ ChessGame.cpp -o chess
   ./chess


   📁 Status
✅ Finished prototype — ready for testing and further polishing

💡 Possible Improvements
Save/load game state

Improved AI for move suggestions

Graphical interface (SFML/SDL or Qt)

Multiplayer over LAN or network
