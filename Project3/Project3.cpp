/*********************************************
*               Oliver Peralta               *
*           Project 3: Minesweeper           *
*           Instructor: Alex Gomez           *
*                  COP3503C                  *
*********************************************/


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Random.h"

#include <SFML/Graphics.hpp>

#include "Tile.h"
using namespace std;
vector<Tile*> findAdjacentTiles(vector<vector<Tile>>& board, int x, int y, int row, int col) {

    // This function is implemented to find the adjacent tiles to clear the board once
    // a tile with no mine and no adjacent mines is clicked. It is used with a Tile
    // vector that points to where it is.
    vector<Tile*> adj;
    if (x > 0 && y > 0 && (y != (row - 1)) && (x != (col - 1))) {
        // For tiles that aren't on the edges
        adj.resize(0);
        adj.push_back(&board[x - 1][y - 1]);
        adj.push_back(&board[x - 1][y]);
        adj.push_back(&board[x - 1][y + 1]);
        adj.push_back(&board[x][y - 1]);
        adj.push_back(&board[x][y + 1]);
        adj.push_back(&board[x + 1][y - 1]);
        adj.push_back(&board[x + 1][y]);
        adj.push_back(&board[x + 1][y + 1]);
    }
    else if (x == 0 && y == 0) {
        // For top left corner 
        adj.resize(0);
        adj.push_back(&board[x + 1][y]);
        adj.push_back(&board[x + 1][y + 1]);
        adj.push_back(&board[x][y + 1]);
    }   
    else if ((x == (col - 1)) && y == 0) {
        // For top right corner
        adj.resize(0);
        adj.push_back(&board[x - 1][y]);
        adj.push_back(&board[x][y + 1]);
        adj.push_back(&board[x - 1][y + 1]);
    }
    else if (x == 0 && (y == row - 1)) {
        // For bottom left corner
        adj.resize(0);  
        adj.push_back(&board[x + 1][y]);
        adj.push_back(&board[x][y - 1]);
        adj.push_back(&board[x + 1][y - 1]);
    }
    else if (x == (col - 1) && y == (row - 1)) {
        // For bottom right corner
        adj.resize(0);
        adj.push_back(&board[x - 1][y - 1]);
        adj.push_back(&board[x][y - 1]);
        adj.push_back(&board[x - 1][y]);
    }
    else if (x == (col - 1)) {
        // For rightmost edge
        adj.resize(0);
        adj.push_back(&board[x][y - 1]);
        adj.push_back(&board[x][y + 1]);
        adj.push_back(&board[x - 1][y - 1]);
        adj.push_back(&board[x - 1][y]);
        adj.push_back(&board[x - 1][y + 1]);
    }
    else if (x == 0) {
        // For leftmost edge
        adj.resize(0);
        adj.push_back(&board[x][y - 1]);
        adj.push_back(&board[x][y + 1]);
        adj.push_back(&board[x + 1][y - 1]);
        adj.push_back(&board[x + 1][y]);
        adj.push_back(&board[x + 1][y + 1]);
    }
    else if (y == (row - 1)) {
        // For bottom edge
        adj.resize(0);
        adj.push_back(&board[x - 1][y]);
        adj.push_back(&board[x + 1][y]);
        adj.push_back(&board[x - 1][y - 1]);
        adj.push_back(&board[x][y - 1]);
        adj.push_back(&board[x + 1][y - 1]);
    }
    else if (y == 0) {
        // For top edge
        adj.resize(0);
        adj.push_back(&board[x - 1][y]);
        adj.push_back(&board[x + 1][y]);
        adj.push_back(&board[x - 1][y + 1]);
        adj.push_back(&board[x][y + 1]);
        adj.push_back(&board[x + 1][y + 1]);
    }
    // Returns Tile pointer vector
    return adj;
}
void recursiveReveal(vector<vector<Tile>>& board, int x, int y, int row, int col, int& tileGoal) {
    if (!board[x][y].checkIfBomb()) {
        vector<Tile*> temp = findAdjacentTiles(board, x, y, row, col);
        int j = 0;
        for (int u = 0; u < temp.size(); u++) {
            if (temp.at(u)->checkIfBomb())
                j++;
        }
        if (j != 0 && !board[x][y].flaggedCheck()) {
            string temp = "number_" + to_string(j);
            board[x][y].toggleLeftClick();
            tileGoal--;
            board[x][y].changeUnderSprite("tile_revealed");
            board[x][y].changeSprite(temp);
            board[x][y].adjacentMineCount = j;
            
        }
        else if (j == 0 && !board[x][y].flaggedCheck()) {
            board[x][y].adjacentMineCount = j;
            board[x][y].changeSprite("tile_revealed");
            board[x][y].toggleLeftClick();
            tileGoal--;
            for (int i = 0; i < temp.size(); i++) {
                if (!temp.at(i)->leftClickCheck()) {
                    board[x][y].flagOff();
                    recursiveReveal(board, (temp.at(i)->getX()) / 32, (temp.at(i)->getY()) / 32, row, col, tileGoal);
                }
            }
        }
    }
}
void blankBoard(float row, float col, vector<vector<Tile>>& board) {
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (board[i][j].checkIfBomb()) {
                board[i][j].toggleBomb();
            }
            board[i][j].leftClickOff();
            board[i][j].flagOff();
        }
    }
}
void loadBoard(float row, float col, vector<vector<Tile>>& board, Tile& bombDig1, Tile& bombDig2, Tile& bombDig3, Tile& middleButton, Tile& bombView, Tile& testOne, Tile& testTwo, Tile& testThree, bool& makeNewBoard, int finalBombs, int& bombs, int& bombsOnBoard, int& tileGoal) {
    int x = 0;
    int y = 0;
    for (float i = 0; i < col; i++) {
        for (float j = 0; j < row; j++) {
            board[i][j].changeUnderSprite("tile_revealed");
            board[i][j].changeSprite("tile_hidden");
            board[i][j].getSprite().setPosition(i * 32, j * 32);
            board[i][j].getUnderTile().setPosition(i * 32, j * 32);
            board[i][j].getFlag().setPosition(i * 32, j * 32);
            board[i][j].setXY(i * 32, j * 32);
        }
    }
    bombs = 0;
    if (makeNewBoard) {
        while (bombs != finalBombs) {
            int randomX = Random::Int(0, (col * 32) - 1);
            int randomY = Random::Int(0, (row * 32) - 1);
            randomX /= 32;
            randomY /= 32;
            if (!board[randomX][randomY].checkIfBomb()) {
                board[randomX][randomY].toggleBomb();
                bombs++;
            }
        }
        makeNewBoard = false;
    }
    // Everything below here is to set the position of the sprites
    x = 20;
    y = (row * 32);
    bombDig1.getSprite().setPosition(x, y);
    bombDig1.setXY(x, y);
    x += 21;
    bombDig2.getSprite().setPosition(x, y);
    bombDig2.setXY(x, y);
    x += 21;
    bombDig3.getSprite().setPosition(x, y);
    bombDig3.setXY(x, y);


    x = ((col * 32) / 2) - 32;
    middleButton.changeSprite("face_happy");
    middleButton.getSprite().setPosition(x, y);
    middleButton.setXY(x, y);
    x += 128;

    bombView.getSprite().setPosition(x, y);
    bombView.setXY(x, y);
    x += 64;

    testOne.getSprite().setPosition(x, y);
    testOne.setXY(x, y);
    x += 64;

    testTwo.getSprite().setPosition(x, y);
    testTwo.setXY(x, y);
    x += 64;

    testThree.getSprite().setPosition(x, y);
    testThree.setXY(x, y);
    bombsOnBoard = bombs;
    tileGoal = (row * col) - bombsOnBoard;
}
void printBomb(vector<vector<Tile>>& board, int x, int y) {
    board[x][y].changeSprite("mine");
}
void debugReveal(vector<vector<Tile>>& board, float row, float col) {
    // Function to reveal the mines once the reveal button is clicked
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (board[i][j].checkIfBomb()) {
                printBomb(board, i, j);
            }
        }
    }
}
void loadWindow(sf::RenderWindow& window, float row, float col, vector<vector<Tile>>& board, Tile& bombDig1, Tile& bombDig2, Tile& bombDig3, Tile& middleButton, Tile& bombView, Tile& testOne, Tile& testTwo, Tile& testThree, bool debug, bool loss, int bombs, bool winCondition) {
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (board[i][j].flaggedCheck() && debug && board[i][j].checkIfBomb()) {
                board[i][j].changeSprite("mine");
                board[i][j].changeUnderSprite("tile_hidden");
                window.draw(board[i][j].getUnderTile());
                window.draw(board[i][j].getFlag());
                window.draw(board[i][j].getSprite());
            }
            else if (!board[i][j].flaggedCheck() && debug && board[i][j].checkIfBomb()) {
                board[i][j].changeSprite("mine");
                board[i][j].changeUnderSprite("tile_hidden");
                window.draw(board[i][j].getUnderTile());
                window.draw(board[i][j].getSprite());
            }
            else if (board[i][j].flaggedCheck() && !debug && !board[i][j].leftClickCheck() && !board[i][j].leftClickCheck()) {
                board[i][j].changeSprite("tile_hidden");
                window.draw(board[i][j].getSprite());
                window.draw(board[i][j].getFlag());
            }
            else if (debug && board[i][j].leftClickCheck() && board[i][j].flaggedCheck()) {
                board[i][j].flagCheck();
            }
            else if (debug && board[i][j].flaggedCheck()) {
                window.draw(board[i][j].getSprite());
                window.draw(board[i][j].getFlag());
            }

            else if (board[i][j].leftClickCheck() && !loss) {
                board[i][j].changeUnderSprite("tile_revealed");
                window.draw(board[i][j].getUnderTile());
                window.draw(board[i][j].getSprite());
            }
            else if (board[i][j].leftClickCheck() && loss) {
                board[i][j].changeUnderSprite("tile_revealed");
                window.draw(board[i][j].getUnderTile());
                window.draw(board[i][j].getSprite());
            }
            else if (!board[i][j].leftClickCheck()) {
                window.draw(board[i][j].getSprite());
            }
            if (winCondition) {
                if (board[i][j].checkIfBomb()) {
                    board[i][j].changeUnderSprite("tile_hidden");
                    window.draw(board[i][j].getUnderTile());
                    window.draw(board[i][j].getFlag());
                }
            }
            else if (debug) {
                debugReveal(board, row, col);
            }   
            else if (loss) {
                if (board[i][j].checkIfBomb()) {
                    board[i][j].changeSprite("mine");
                    board[i][j].changeUnderSprite("tile_revealed");
                    window.draw(board[i][j].getUnderTile());
                    window.draw(board[i][j].getSprite());
                }
            }
        }
    }
    
    if (bombs >= 100) {
        bombDig1.getSprite().setTextureRect(sf::IntRect((bombs / 100) * 21, 0, 21, 32));
        window.draw(bombDig1.getSprite());
        bombDig2.getSprite().setTextureRect(sf::IntRect(((bombs / 10) % 10) * 21, 0, 21, 32));
        window.draw(bombDig2.getSprite());
        bombDig3.getSprite().setTextureRect(sf::IntRect((bombs % 10) * 21, 0, 21, 32));
        window.draw(bombDig3.getSprite());
    }
    else if (bombs >= 0) {
        bombDig1.getSprite().setTextureRect(sf::IntRect(0, 0, 21, 32));
        window.draw(bombDig1.getSprite());
        bombDig2.getSprite().setTextureRect(sf::IntRect((bombs / 10) * 21, 0, 21, 32));
        window.draw(bombDig2.getSprite());
        bombDig3.getSprite().setTextureRect(sf::IntRect((bombs % 10) * 21, 0, 21, 32));
        window.draw(bombDig3.getSprite());
    }
    else {
        bombDig1.getSprite().setTextureRect(sf::IntRect(210, 0, 21, 32));
        window.draw(bombDig1.getSprite());
        bombDig2.getSprite().setTextureRect(sf::IntRect((bombs / 10) * -21, 0, 21, 32));
        window.draw(bombDig2.getSprite());
        bombDig3.getSprite().setTextureRect(sf::IntRect((bombs % 10) * -21, 0, 21, 32));
        window.draw(bombDig3.getSprite());
    }
    
    if (loss) {
        middleButton.changeSprite("face_lose");
        window.draw(middleButton.getSprite());
    }
    else if (winCondition) {
        middleButton.changeSprite("face_win");
        window.draw(middleButton.getSprite());
    }
    else {
        middleButton.changeSprite("face_happy");
        window.draw(middleButton.getSprite());
    }
    window.draw(bombView.getSprite());
    window.draw(testOne.getSprite());
    window.draw(testTwo.getSprite());
    window.draw(testThree.getSprite());

    window.display();
}
void loadTest1(vector<vector<Tile>>& board, ifstream& inFile, float row, float col, int& bombs) {
    char temp;
    vector<char> temps;
    bombs = 0;
    while (inFile.get(temp)) {
        if (temp == '1' || temp == '0')
            temps.push_back(temp);
    }
    int counter = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (temps.at(counter) == '1') {
                board[j][i].toggleBomb();
                bombs++;
            }
            counter++;
        }
    }
}
void loadTest2(vector<vector<Tile>>& board, ifstream& inFile, float row, float col, int& bombs) {
    char temp;
    vector<char> temps;
    bombs = 0;
    while (inFile.get(temp)) {
        if (temp == '1' || temp == '0')
            temps.push_back(temp);
    }
    int counter = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
           if (temps.at(counter) == '1') {
              board[j][i].toggleBomb();
              bombs++;
           }
           counter++;
        }
    }
}
void loadTest3(vector<vector<Tile>>& board, ifstream& inFile, float row, float col, int& bombs) {
    char temp;
    vector<char> temps;
    bombs = 0;
    while (inFile.get(temp)) {
        if (temp == '1' || temp == '0')
            temps.push_back(temp);
    }
    int counter = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (temps.at(counter) == '1') {
                bombs++;
                board[j][i].toggleBomb();
            }
            counter++;
        }
    }
}
void debugHide(vector<vector<Tile>>& board, float row, float col) {
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (!board[i][j].leftClickCheck()) {
                board[i][j].changeSprite("tile_hidden");
            }
        }
    }
}
int bombCount(vector<vector<Tile>>& board, int row, int col) {
    int counter = 0;
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (board[i][j].checkIfBomb()) {
                counter++;
            }
        }
    }
    return counter;
}
int main()
{
    cout << "Minesweeper by: Oliver C. Peralta" << endl << endl;
    ifstream inFile("boards/config.cfg");
    float col;
    inFile >> col;
    cout << "Columns: " << col << endl;
    float row;
    inFile >> row;
    cout << "Rows: " << row << endl;
    int finalBombs;
    inFile >> finalBombs;
    cout << "Mines: " << finalBombs << endl;
    Tile newTile;
    vector<vector<Tile>> board(col, vector<Tile>(row, newTile));
    Tile bombDig1 = Tile("digits");
    Tile bombDig2 = Tile("digits");
    Tile bombDig3 = Tile("digits");

    Tile middleButton = Tile("face_happy");
    Tile bombView = Tile("debug");
    Tile testOne = Tile("test_1");
    Tile testTwo = Tile("test_2");
    Tile testThree = Tile("test_3");

    int tileGoal = 0;
    
    bool debugMode = false;
    bool makeNewBombs = true;
    bool loss = false;
    bool winCondition = false;

    int bombs = finalBombs;
    int bombsOnBoard = bombs;
    sf::RenderWindow window(sf::VideoMode((col * 32), (row * 32) + 100), "Minesweeper");
    sf::Mouse mouse;
    loadBoard(row, col, board, bombDig1, bombDig2, bombDig3, middleButton, bombView, testOne, testTwo, testThree, makeNewBombs, finalBombs, bombs, bombsOnBoard, tileGoal);
    int bombCounter = 0;
    while (window.isOpen())
    {
        loadWindow(window, row, col, board, bombDig1, bombDig2, bombDig3, middleButton, bombView, testOne, testTwo, testThree, debugMode, loss, bombs, winCondition);
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i tempMousePos = mouse.getPosition(window);
            
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                int tempX = (tempMousePos.x / 32);
                int tempY = (tempMousePos.y / 32);
                if (event.mouseButton.button == mouse.Right && !loss && !winCondition) {
                    if (tempMousePos.y <= (row * 32) - 1 && !board[tempX][tempY].leftClickCheck()) { // Mouse is on board or not
                        board[tempX][tempY].flagCheck();
                        if (board[tempX][tempY].flaggedCheck())
                            bombs--;
                        else
                            bombs++;
                    }
                }
                if (event.mouseButton.button == mouse.Left) {
                    if (tempMousePos.y <= (row*32) - 1) { // Mouse is on board or not
                        if (loss || winCondition) {
                            continue;
                        }
                        if (board[tempX][tempY].checkIfBomb() && !board[tempX][tempY].flaggedCheck()) {
                            debugMode = false;
                            loss = true;
                        }
                        else if (!board[tempX][tempY].leftClickCheck()) {
                            recursiveReveal(board, tempX, tempY, row, col, tileGoal);
                            if (tileGoal == 0) {
                                bombs = 0;
                                winCondition = true;
                            }
                        }
                    }
                    else if (tempMousePos.x >= middleButton.getX() && tempMousePos.x <= middleButton.getX() + 64 && tempMousePos.y <= middleButton.getY() + 64) {
                        makeNewBombs = true;
                        winCondition = false;
                        loss = false;
                        blankBoard(row, col, board);
                        loadBoard(row, col, board, bombDig1, bombDig2, bombDig3, middleButton, bombView, testOne, testTwo, testThree, makeNewBombs, finalBombs, bombs, bombsOnBoard, tileGoal);
                    }
                    else if (tempMousePos.x >= bombView.getX() && tempMousePos.x <= bombView.getX() + 64 && tempMousePos.y <= bombView.getY() + 64) {
                        
                        if (!loss && !winCondition) {
                            if (debugMode) {
                                debugHide(board, row, col);
                                debugMode = false;
                            }
                            else {
                                debugReveal(board, row, col);
                                debugMode = true;
                            }
                        }
                                             
                    }
                    else if (tempMousePos.x >= testOne.getX() && tempMousePos.x <= testOne.getX() + 64 && tempMousePos.y <= testOne.getY() + 64) {
                        loss = false;
                        winCondition = false;
                        blankBoard(row, col, board);
                        loadBoard(row, col, board, bombDig1, bombDig2, bombDig3, middleButton, bombView, testOne, testTwo, testThree, makeNewBombs, finalBombs, bombs, bombsOnBoard, tileGoal);
                        ifstream inFile1("boards/testboard1.brd");
                        loadTest1(board, inFile1, row, col, bombs);
                        bombsOnBoard = bombs;
                        tileGoal = (row * col) - bombsOnBoard;
                    }
                    else if (tempMousePos.x >= testTwo.getX() && tempMousePos.x <= testTwo.getX() + 64 && tempMousePos.y <= testTwo.getY() + 64) {
                        loss = false;
                        winCondition = false;
                        blankBoard(row, col, board);
                        loadBoard(row, col, board, bombDig1, bombDig2, bombDig3, middleButton, bombView, testOne, testTwo, testThree, makeNewBombs, finalBombs, bombs, bombsOnBoard, tileGoal);
                        ifstream inFile2("boards/testboard2.brd");
                        loadTest2(board, inFile2, row, col, bombs);
                        bombsOnBoard = bombs;
                        tileGoal = (row * col) - bombsOnBoard;
                    }
                    else if (tempMousePos.x >= testThree.getX() && tempMousePos.x <= testThree.getX() + 64 && tempMousePos.y <= testThree.getY() + 64) {
                        loss = false;
                        winCondition = false;
                        blankBoard(row, col, board);
                        loadBoard(row, col, board, bombDig1, bombDig2, bombDig3, middleButton, bombView, testOne, testTwo, testThree, makeNewBombs, finalBombs, bombs, bombsOnBoard, tileGoal);
                        ifstream inFile3("boards/testboard3.brd");
                        loadTest3(board, inFile3, row, col, bombs);
                        bombsOnBoard = bombs;
                        tileGoal = (row * col) - bombsOnBoard;
                    }
                }
            }
        }
    }
    TextureManager::Clear();
    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file