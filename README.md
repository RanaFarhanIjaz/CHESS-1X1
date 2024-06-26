Chess Game Project
This project is an implementation of a chess game using C++ and SFML (Simple and Fast Multimedia Library). It allows two players to play a game of chess on a graphical interface.

Features
Graphical Interface: Visual representation of the chessboard and pieces using SFML.
Rules Implementation: Implements standard chess rules including piece movements, capturing, castling, pawn promotion, and check/checkmate detection.
Piece Classes: Each chess piece (pawn, knight, bishop, rook, queen, king) is represented by a C++ class inheriting from a base ChessPiece class.
Validation: Validates moves based on the rules of chess including piece-specific movements and capturing rules.
Check and Checkmate: Detects when a king is in check or checkmate condition.
Piece Capturing: Handles capturing of opponent pieces and removes them from the board.
End Game Condition: Displays a message when one player achieves checkmate.
Components
Main Program: main.cpp initializes the game window, loads textures for pieces, sets up the chessboard, handles player inputs, and manages game logic.
Piece Classes: Each chess piece type (Pawn, Knight, Bishop, Rook, Queen, King) is implemented as a subclass of ChessPiece, each with its own movement logic.
Textures: Piece textures are loaded from image files (*.png format) for visual representation.
Board Representation: Uses a 2D array to represent the current state of the chessboard and tracks positions of pieces.
SFML Library: Utilizes SFML for graphics rendering, window management, and event handling.
Usage
Compile: Compile the project using a C++ compiler that supports C++11 or higher and link with SFML.
Run: Execute the compiled executable to start the chess game.
Gameplay: Click on a piece to select it, then click on a valid square to move the piece. Follow standard chess rules for gameplay.
Installation
SFML: Ensure SFML library is installed and properly linked with the project.
Image Files: Place all piece image files (*.png) in the same directory as the executable.
Known Issues
No known issues at the moment.
Future Enhancements
Implement more advanced chess rules like en passant and pawn promotion choices.
Add an AI opponent for single-player mode.
Improve UI/UX features such as highlighting valid moves and displaying game status.
Contributing
Contributions are welcome. For major changes, please open an issue first to discuss potential changes or enhancements.

License
This project is licensed under the Unilicense - see the LICENSE file for details.
