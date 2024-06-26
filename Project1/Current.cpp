#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <string>

const float squareSize = 504 / 8.0f;
bool turn = true;
bool isInsideBoard(int x, int y);

class ChessPiece {
public:
    ChessPiece(const sf::Texture& texture, const sf::Vector2f position, bool isWhite)
        : sprite(texture), currentPosition(position), curr(position), isWhite(isWhite) {}

    virtual bool isValidMove(int targetX, int targetY, const std::string(&boardLayout)[8][8]) const = 0;

    bool getWhite()
    {
        return isWhite;
    }
    virtual void move(const sf::Vector2f& newPosition, bool turn) {
        if (isWhite == turn)
        {
            curr = newPosition;
            sprite.setPosition(newPosition);
        }
    }

    virtual void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

    const sf::Vector2f& getPosition() const {
        return currentPosition;
    }

    bool isSameColor(bool otherIsWhite) const {
        return isWhite == otherIsWhite;
    }
    void setPos()
    {
        currentPosition = curr;
    }

public:
    sf::Sprite sprite;
    sf::Vector2f currentPosition, curr;
    bool isWhite;
};


class Pawn : public ChessPiece {
public:
    Pawn(const sf::Texture& texture, const sf::Vector2f& position, bool isWhite)
        : ChessPiece(texture, position, isWhite), firstMove(true) {}


    bool isValidMove(int targetX, int targetY, const std::string(&boardLayout)[8][8]) const override {
        // Calculate the delta values
        int deltaX = targetX - static_cast<int>(currentPosition.x / squareSize);
        int deltaY = targetY - static_cast<int>(currentPosition.y / squareSize);

        // Check if the move is within board bounds
        if (!isInsideBoard(targetX, targetY)) {
            return false;
        }

        // Check if the target square is occupied by a piece of the same color
        if (!boardLayout[targetY][targetX].empty() && isSameColor(boardLayout[targetY][targetX][0] == 'w')) {
            return false;
        }

        // Moving forward one square
        if (deltaX == 0 && deltaY == (isWhite ? -1 : 1) && boardLayout[targetY][targetX].empty()) {
            return true;
        }

        // Moving forward two squares at the start
        if (deltaX == 0 && deltaY == (isWhite ? -2 : 2) && boardLayout[targetY][targetX].empty() && (static_cast<int>(currentPosition.y / squareSize) == 6 || static_cast<int>(currentPosition.y / squareSize) == 1)) {
            // Check if the square in between is empty
            int intermediateY = currentPosition.y / squareSize + (isWhite ? -1 : 1);
            if (boardLayout[intermediateY][targetX].empty()) {
                return true;
            }
        }

        // Capturing diagonally
        if (std::abs(deltaX) == 1 && deltaY == (isWhite ? -1 : 1) &&
            !boardLayout[targetY][targetX].empty() && !isSameColor(boardLayout[targetY][targetX][0] == 'w')) {
            return true;
        }


        return false;
    }




    void move(const sf::Vector2f& newPosition, bool turn) override {
        ChessPiece::move(newPosition, turn);
        firstMove = false;
    }



private:
    bool firstMove;
};


class Knight : public ChessPiece {
public:
    Knight(const sf::Texture& texture, const sf::Vector2f& position, bool isWhite)
        : ChessPiece(texture, position, isWhite) {}

    bool isValidMove(int targetX, int targetY, const std::string(&boardLayout)[8][8]) const override {


        int deltaX = targetX - static_cast<int>(currentPosition.x / squareSize);
        int deltaY = targetY - static_cast<int>(currentPosition.y / squareSize);
  

        // Check if move is within board bounds
        if (!isInsideBoard(targetX, targetY)) {
            return false;
        }

        // Check if destination square is occupied by a piece of the same color
        if (!boardLayout[targetY][targetX].empty() && isSameColor(boardLayout[targetY][targetX][0] == 'w')) {
            return false;
        }

        // Check all 8 possible knight moves based on color
        if ((isWhite && ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1) ||
            (deltaX == 1 && deltaY == -2) || (deltaX == 2 && deltaY == -1) ||
            (deltaX == -1 && deltaY == 2) || (deltaX == -2 && deltaY == 1) ||
            (deltaX == -1 && deltaY == -2) || (deltaX == -2 && deltaY == -1))) ||
            (!isWhite && ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1) ||
                (deltaX == 1 && deltaY == -2) || (deltaX == 2 && deltaY == -1) ||
                (deltaX == -1 && deltaY == 2) || (deltaX == -2 && deltaY == 1) ||
                (deltaX == -1 && deltaY == -2) || (deltaX == -2 && deltaY == -1)))) {
            return true; // Valid knight move
        }

        return false; // Invalid move
    }

};

class Bishop : public ChessPiece {
public:
    Bishop(const sf::Texture& texture, const sf::Vector2f& position, bool isWhite)
        : ChessPiece(texture, position, isWhite) {}

    bool isValidMove(int targetX, int targetY, const std::string(&boardLayout)[8][8]) const override {
        int deltaX = targetX - static_cast<int>(currentPosition.x / squareSize);
        int deltaY = targetY - static_cast<int>(currentPosition.y / squareSize);

        // Check if move is within board bounds
        if (!isInsideBoard(targetX, targetY)) {
            return false;
        }

        // Check if destination square is occupied by a piece of the same color
        if (!boardLayout[targetY][targetX].empty() && isSameColor(boardLayout[targetY][targetX][0] == 'w')) {
            return false;
        }

        // Check if the move is diagonal
        if (std::abs(deltaX) == std::abs(deltaY)) {
            // Check if the path is clear up to the destination square
            int stepX = (deltaX > 0) ? 1 : -1;
            int stepY = (deltaY > 0) ? 1 : -1;
            int x = static_cast<int>(currentPosition.x / squareSize) + stepX;
            int y = static_cast<int>(currentPosition.y / squareSize) + stepY;

            while (x != targetX && y != targetY) {
                if (!boardLayout[y][x].empty()) {
                    return false; // Path is blocked
                }
                x += stepX;
                y += stepY;
            }

            return true; // Valid bishop move
        }

        return false; // Invalid move
    }

};

class Rook : public ChessPiece {
public:
    Rook(const sf::Texture& texture, const sf::Vector2f& position, bool isWhite)
        : ChessPiece(texture, position, isWhite) {}

    bool isValidMove(int targetX, int targetY, const std::string(&boardLayout)[8][8]) const override {
        int deltaX = targetX - static_cast<int>(currentPosition.x / squareSize);
        int deltaY = targetY - static_cast<int>(currentPosition.y / squareSize);

        // Check if move is within board bounds
        if (!isInsideBoard(targetX, targetY)) {
            return false;
        }

        // Check if destination square is occupied by a piece of the same color
        if (!boardLayout[targetY][targetX].empty() && isSameColor(boardLayout[targetY][targetX][0] == 'w')) {
            return false;
        }

        // Check if the move is vertical or horizontal
        if ((deltaX == 0 && deltaY != 0) || (deltaX != 0 && deltaY == 0)) {
            // Determine step direction based on move direction
            int stepX = (deltaX > 0) ? 1 : (deltaX < 0) ? -1 : 0;
            int stepY = (deltaY > 0) ? 1 : (deltaY < 0) ? -1 : 0;

            // Check if the path is clear up to the destination square
            int x = static_cast<int>(currentPosition.x / squareSize) + stepX;
            int y = static_cast<int>(currentPosition.y / squareSize) + stepY;

            while (x != targetX || y != targetY) {
                if (!boardLayout[y][x].empty()) {
                    return false; // Path is blocked
                }
                x += stepX;
                y += stepY;
            }

            return true; // Valid rook move
        }

        return false; // Invalid move
    }

};

class Queen : public ChessPiece {
public:
    Queen(const sf::Texture& texture, const sf::Vector2f& position, bool isWhite)
        : ChessPiece(texture, position, isWhite) {}

    bool isValidMove(int targetX, int targetY, const std::string(&boardLayout)[8][8]) const override {
        int deltaX = targetX - static_cast<int>(currentPosition.x / squareSize);
        int deltaY = targetY - static_cast<int>(currentPosition.y / squareSize);

        if (deltaX == 0 && deltaY != 0) { // Vertical movement
            int stepY = (deltaY > 0) ? 1 : -1;
            for (int y = static_cast<int>(currentPosition.y / squareSize) + stepY; y != targetY; y += stepY) {
                if (!boardLayout[y][targetX].empty()) {
                    return false; // Obstacle in the path
                }
            }
            return true; // No obstacles, valid move
        }
        else if (deltaX != 0 && deltaY == 0) { // Horizontal movement
            int stepX = (deltaX > 0) ? 1 : -1;
            for (int x = static_cast<int>(currentPosition.x / squareSize) + stepX; x != targetX; x += stepX) {
                if (!boardLayout[targetY][x].empty()) {
                    return false; // Obstacle in the path
                }
            }
            return true; // No obstacles, valid move
        }
        else if (std::abs(deltaX) == std::abs(deltaY)) { // Diagonal movement
            int stepX = (deltaX > 0) ? 1 : -1;
            int stepY = (deltaY > 0) ? 1 : -1;
            for (int x = static_cast<int>(currentPosition.x / squareSize) + stepX, y = static_cast<int>(currentPosition.y / squareSize) + stepY;
                x != targetX && y != targetY; x += stepX, y += stepY) {
                if (!boardLayout[y][x].empty()) {
                    return false; // Obstacle in the path
                }
            }
            return true; // No obstacles, valid move
        }

        return false; // Invalid move
    }

};

class King : public ChessPiece {
public:
    King(const sf::Texture& texture, const sf::Vector2f& position, bool isWhite)
        : ChessPiece(texture, position, isWhite) {}
    bool isValidMove(int targetX, int targetY, const std::string(&boardLayout)[8][8]) const override {
        // Check if move is within board bounds
        if (!isInsideBoard(targetX, targetY)) {
            return false;
        }

        // Check if destination square is occupied by a piece of the same color
        if (!boardLayout[targetY][targetX].empty() && isSameColor(boardLayout[targetY][targetX][0] == 'w')) {
            return false;
        }

        // Check if the move is one square in any direction
        int deltaX = std::abs(targetX - static_cast<int>(currentPosition.x / squareSize));
        int deltaY = std::abs(targetY - static_cast<int>(currentPosition.y / squareSize));
        return (deltaX <= 1 && deltaY <= 1);
    }

};

bool isInsideBoard(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}





bool isKingCaptured(const std::string& pieceCode) {
    return pieceCode == "wk" || pieceCode == "bk"; // Check if the captured piece is a king
}


int main() {
    sf::RenderWindow window(sf::VideoMode(504, 504), "Chess Game", sf::Style::Close);

    std::unordered_map<std::string, sf::Texture> pieceTextures;
    sf::Font sedan;
    sf::Text mate;
    pieceTextures["br1"].loadFromFile("br.png");
    pieceTextures["bn1"].loadFromFile("bn.png");
    pieceTextures["bb1"].loadFromFile("bb.png");
    pieceTextures["br2"].loadFromFile("br.png");
    pieceTextures["bn2"].loadFromFile("bn.png");
    pieceTextures["bb2"].loadFromFile("bb.png");
    pieceTextures["bq"].loadFromFile("bq.png");
    pieceTextures["bk"].loadFromFile("bk.png");
    pieceTextures["bp1"].loadFromFile("bp.png");
    pieceTextures["bp2"].loadFromFile("bp.png");
    pieceTextures["bp3"].loadFromFile("bp.png");
    pieceTextures["bp4"].loadFromFile("bp.png");
    pieceTextures["bp5"].loadFromFile("bp.png");
    pieceTextures["bp6"].loadFromFile("bp.png");
    pieceTextures["bp7"].loadFromFile("bp.png");
    pieceTextures["bp8"].loadFromFile("bp.png");

    sedan.loadFromFile("sedan-regular.ttf");
    mate.setFont(sedan);
    mate.setCharacterSize(40);
    mate.setPosition(16, 112);
    mate.setFillColor(sf::Color::White);
    mate.setString("Mate");

    pieceTextures["wr1"].loadFromFile("wr.png");
    pieceTextures["wn1"].loadFromFile("wn.png");
    pieceTextures["wb1"].loadFromFile("wb.png");
    pieceTextures["wr2"].loadFromFile("wr.png");
    pieceTextures["wn2"].loadFromFile("wn.png");
    pieceTextures["wb2"].loadFromFile("wb.png");
    pieceTextures["wq"].loadFromFile("wq.png");
    pieceTextures["wk"].loadFromFile("wk.png");
    pieceTextures["wp1"].loadFromFile("wp.png");
    pieceTextures["wp2"].loadFromFile("wp.png");
    pieceTextures["wp3"].loadFromFile("wp.png");
    pieceTextures["wp4"].loadFromFile("wp.png");
    pieceTextures["wp5"].loadFromFile("wp.png");
    pieceTextures["wp6"].loadFromFile("wp.png");
    pieceTextures["wp7"].loadFromFile("wp.png");
    pieceTextures["wp8"].loadFromFile("wp.png");




    sf::Color lightSquareColor(238, 238, 210);
    sf::Color darkSquareColor(118, 150, 86);

    std::string boardLayout[8][8] = {
        {"br1", "bn1", "bb1", "bq", "bk", "bb2", "bn2", "br2"},
        {"bp1", "bp2", "bp3", "bp4", "bp5", "bp6", "bp7", "bp8"},
        {"", "", "", "", "", "", "", ""},
        {"", "", "", "", "", "", "", ""},
        {"", "", "", "", "", "", "", ""},
        {"", "", "", "", "", "", "", ""},
        {"wp1", "wp2", "wp3", "wp4", "wp5", "wp6", "wp7", "wp8"},
        {"wr1", "wn1", "wb1", "wq", "wk", "wb2", "wn2", "wr2"}
    };



    std::unordered_map<std::string, std::vector<std::unique_ptr<ChessPiece>>> pieces;


    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::string pieceCode = boardLayout[i][j];
            if (!pieceCode.empty()) {
                std::unique_ptr<ChessPiece> piece;
                if (pieceCode[0] == 'b') {
                    if (pieceCode[1] == 'p') {
                        piece = std::make_unique<Pawn>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), false);
                    }
                    else if (pieceCode[1] == 'n') {
                        piece = std::make_unique<Knight>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), false);
                    }
                    else if (pieceCode[1] == 'b') {
                        piece = std::make_unique<Bishop>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), false);
                    }
                    else if (pieceCode[1] == 'r') {
                        piece = std::make_unique<Rook>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), false);
                    }
                    else if (pieceCode[1] == 'q') {
                        piece = std::make_unique<Queen>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), false);
                    }
                    else if (pieceCode[1] == 'k') {
                        piece = std::make_unique<King>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), false);
                    }
                }
                else if (pieceCode[0] == 'w') {
                    if (pieceCode[1] == 'p') {
                        piece = std::make_unique<Pawn>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), true);
                    }
                    else if (pieceCode[1] == 'n') {
                        piece = std::make_unique<Knight>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), true);
                    }
                    else if (pieceCode[1] == 'b') {
                        piece = std::make_unique<Bishop>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), true);
                    }
                    else if (pieceCode[1] == 'r') {
                        piece = std::make_unique<Rook>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), true);
                    }
                    else if (pieceCode[1] == 'q') {
                        piece = std::make_unique<Queen>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), true);
                    }
                    else if (pieceCode[1] == 'k') {
                        piece = std::make_unique<King>(pieceTextures[pieceCode], sf::Vector2f(j * squareSize, i * squareSize), true);
                    }
                }

                if (piece) {
                    pieces[pieceCode].emplace_back(std::move(piece));
                }
            }
        }
    }
    window.display();

    bool isMoving = false;
    sf::Vector2f offset;
    std::string selectedPiece;
    sf::Vector2f startPosition;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    int clickedX = static_cast<int>(mousePosition.x / squareSize);
                    int clickedY = static_cast<int>(mousePosition.y / squareSize);
                    std::string pieceCode = boardLayout[clickedY][clickedX];
                    if (!pieceCode.empty()) {
                        selectedPiece = pieceCode;
                        isMoving = true;
                        offset = pieces[selectedPiece][0]->getPosition() - mousePosition;
                        startPosition = pieces[selectedPiece][0]->getPosition();
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left && isMoving) {
                    isMoving = false;
                    sf::Vector2f dropPosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    int dropX = static_cast<int>(dropPosition.x / squareSize);
                    int dropY = static_cast<int>(dropPosition.y / squareSize);
  

                    if (isInsideBoard(dropX, dropY) && pieces[selectedPiece][0]->isValidMove(dropX, dropY, boardLayout) && (pieces[selectedPiece][0]->getWhite() == turn)) {
                        
 

                        if (boardLayout[dropY][dropX].empty() ) {

                            boardLayout[dropY][dropX] = selectedPiece;
                            boardLayout[static_cast<int>(startPosition.y / squareSize)][static_cast<int>(startPosition.x / squareSize)] = "";
                            pieces[selectedPiece][0]->move(sf::Vector2f(dropX * squareSize, dropY * squareSize), turn);
                            turn = !turn;
                            pieces[selectedPiece][0]->setPos();

                           

                            
                          

                        }
                        else {
                            std::string capturedPiece = boardLayout[dropY][dropX];
                            if (isKingCaptured(capturedPiece)) {
                                sf::RenderWindow result(sf::VideoMode(786, 400), "Result", sf::Style::Close);
                                while (result.isOpen()) {
                                    sf::Event e;
                                    while (result.pollEvent(e)) {
                                        if (e.type == sf::Event::Closed) {
                                            result.close();
                                            window.close();
                                        }



                                    }
                                    if (turn) {
                                        mate.setString("CONGRATULATIONS WHITE WINS");
                                    }
                                    else
                                    {
                                        mate.setString("CONGRATULATIONS BLACK WINS");
                                    };

                                    result.clear();
                                    result.draw(mate);
                                    result.display();




                                    window.close();
                                }
                            }
                            else {
                                // Capture the opponent's piece
                                pieces[capturedPiece].clear();
                                boardLayout[dropY][dropX] = selectedPiece;
                                boardLayout[static_cast<int>(startPosition.y / squareSize)][static_cast<int>(startPosition.x / squareSize)] = "";
                                pieces[selectedPiece][0]->move(sf::Vector2f(dropX * squareSize, dropY * squareSize), turn);
                                turn = !turn;
                                pieces[selectedPiece][0]->setPos();
                          
                   
                            }
                        }
                    }
                    else {
                        // Move the piece back to its original position if the move is invalid
                        pieces[selectedPiece][0]->move(startPosition, turn);
                    }
                }
            }


        }




        if (isMoving) {
            sf::Vector2f mousePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            if (mousePosition.x >= 0 && mousePosition.x < 8 * squareSize &&
                mousePosition.y >= 0 && mousePosition.y < 8 * squareSize) {
                pieces[selectedPiece][0]->move(mousePosition + offset, turn);
            }
        }

        window.clear();

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
                square.setPosition(j * squareSize, i * squareSize);
                if ((i + j) % 2 == 1) {
                    square.setFillColor(darkSquareColor);
                }
                else {
                    square.setFillColor(lightSquareColor);
                }
                window.draw(square);
            }
        }

        for (auto& piecePair : pieces) {
            for (auto& piece : piecePair.second) {
                const sf::Sprite& originalSprite = piece->sprite;
                sf::Sprite pieceSprite(originalSprite);
                float scale = squareSize / std::max(originalSprite.getLocalBounds().width, originalSprite.getLocalBounds().height);
                pieceSprite.setScale(scale, scale);
                float offsetX = (squareSize - originalSprite.getLocalBounds().width * scale) / 2;
                float offsetY = (squareSize - originalSprite.getLocalBounds().height * scale) / 2;
                pieceSprite.setPosition(piece->curr.x + offsetX, piece->curr.y + offsetY);
                window.draw(pieceSprite);
            }
        }
        window.display();
    }
    return 0;
}
