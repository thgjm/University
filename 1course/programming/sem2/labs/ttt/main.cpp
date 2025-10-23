#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const int WINDOW_SIZE = 600;
const int BOARD_SIZE = 3;
const int CELL_SIZE = WINDOW_SIZE / BOARD_SIZE;

enum class Player { None, X, O };

class TicTacToe {
private:
    std::vector<std::vector<Player>> board;
    Player currentPlayer;
    bool gameOver;
    sf::Font font;
    sf::Text gameOverText;

public:
    TicTacToe() : board(BOARD_SIZE, std::vector<Player>(BOARD_SIZE, Player::None)), currentPlayer(Player::X), gameOver(false) {
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("");
        gameOverText.setPosition(50, WINDOW_SIZE / 2);
    }

    void draw(sf::RenderWindow& window) {
        window.clear(sf::Color::White);

        // Draw board
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(i * CELL_SIZE, j * CELL_SIZE);
                cell.setOutlineThickness(2);
                cell.setOutlineColor(sf::Color::Black);
                window.draw(cell);

                if (board[i][j] == Player::X) {
                    sf::Text xText("X", font, 50);
                    xText.setPosition(i * CELL_SIZE + 20, j * CELL_SIZE + 10);
                    window.draw(xText);
                } else if (board[i][j] == Player::O) {
                    sf::Text oText("O", font, 50);
                    oText.setPosition(i * CELL_SIZE + 20, j * CELL_SIZE + 10);
                    window.draw(oText);
                }
            }
        }

        window.draw(gameOverText);
        window.display();
    }

    void handleInput(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed && !gameOver) {
            sf::Vector2i mousePos = sf::Mouse::getPosition();
            int row = mousePos.x / CELL_SIZE;
            int col = mousePos.y / CELL_SIZE;

            if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == Player::None) {
                board[row][col] = currentPlayer;
                if (checkWin(row, col)) {
                    gameOverText.setString("Player " + std::to_string(static_cast<int>(currentPlayer)) + " wins!");
                    gameOver = true;
                } else if (checkDraw()) {
                    gameOverText.setString("It's a draw!");
                    gameOver = true;
                } else {
                    currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
                    // Uncomment the line below for playing against AI
                    // makeAIMove();
                }
            }
        }
    }

    bool checkWin(int row, int col) {
        // Implement your winning condition check logic here
        return false;
    }

    bool checkDraw() {
        // Implement your draw condition check logic here
        return false;
    }

    // Implement your AI logic here using alpha-beta pruning
    void makeAIMove() {
    int bestScore = -1000;
    std::pair<int, int> bestMove;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == Player::None) {
                board[i][j] = currentPlayer;
                int score = minimax(board, 0, false, -1000, 1000);
                board[i][j] = Player::None;

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }

    board[bestMove.first][bestMove.second] = currentPlayer;
    currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
}

int evaluate(std::vector<std::vector<Player>>& board) {
    // Оцінка поточної позиції на дошці
    // Наприклад, можна додати бали за кількість рядків, стовпців і діагоналей з X або O
    return 0;
}

int minimax(std::vector<std::vector<Player>>& board, int depth, bool isMaximizingPlayer, int alpha, int beta) {
    // Перевірка кінцевих станів гри або глибини пошуку
    int score = evaluate(board);
    if (score == 10) return score - depth; // Гравець X переміг
    if (score == -10) return score + depth; // Гравець O переміг
    if (depth == 0) return 0; // Досягнута максимальна глибина

    // Логіка мінімаксу зі зрізанням альфа-бета
    if (isMaximizingPlayer) {
        int bestScore = -1000;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == Player::None) {
                    board[i][j] = Player::X;
                    int currentScore = minimax(board, depth - 1, false, alpha, beta);
                    bestScore = std::max(bestScore, currentScore);
                    alpha = std::max(alpha, currentScore);
                    board[i][j] = Player::None;
                    if (beta <= alpha)
                        break; // Зрізання альфа-бета
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == Player::None) {
                    board[i][j] = Player::O;
                    int currentScore = minimax(board, depth - 1, true, alpha, beta);
                    bestScore = std::min(bestScore, currentScore);
                    beta = std::min(beta, currentScore);
                    board[i][j] = Player::None;
                    if (beta <= alpha)
                        break; // Зрізання альфа-бета
                }
            }
        }
        return bestScore;
    }
}
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Tic Tac Toe");

    TicTacToe game;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            game.handleInput(event);
        }

        game.draw(window);
    }

    return 0;
}
