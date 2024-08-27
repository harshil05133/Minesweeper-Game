#include <cstdlib>
#include <iostream>
#include "Square.h"
#include <vector>
#include <string>


#include <SFML/Graphics.hpp>
using namespace std;


struct Board{
    //Dimensions[0] = height, Dimensions[1] = width
    vector<int> dimensions;
    vector<Square*> boardSquares;
    vector<string> images;
    int numMines;
    int numFlags;
    int time;
    string stateOfGame;

    Board(int height, int width, int c){
        time = 0;
        stateOfGame = "playing";
        dimensions.push_back(height);
        dimensions.push_back(width);
        numMines = c;
        numFlags = c;
        for(int i = 0; i < (dimensions[0] * dimensions[1]); i++)
        {
            boardSquares.push_back(new Square(false, (i%dimensions[1])*32, (i/dimensions[1])*32 ,i));
        }
    }

    void placeMines(){
        int remainingMines = numMines;
        int currentSquare;
        while(remainingMines > 0){
            currentSquare = rand() % (dimensions[0]*dimensions[1]);
            if(!boardSquares[currentSquare]->isMine){
                boardSquares[currentSquare]->isMine = true;
                 remainingMines--;
            }
            }

    }

    void drawMines(sf::RenderWindow& window){
        sf::Texture mineTexture;
        mineTexture.loadFromFile("files/images/mine.png");
        sf::Sprite mineSprite;
        mineSprite.setTexture(mineTexture);
        for(int i = 0; i < dimensions[0]*dimensions[1]; i++){
            if(boardSquares[i]->isMine){
                mineSprite.setPosition(boardSquares[i]->coordinates[0], boardSquares[i]->coordinates[1]);
                window.draw(mineSprite);
            }
        }
    }

    void fillNeighborVector(){
        Square* currentSquare;
        int numOfSquare;
        int row;
        int column;
        for (int i = 0; i < dimensions[0]*dimensions[1]; i++){
            currentSquare = boardSquares[i];
            numOfSquare = currentSquare->number;
            column = numOfSquare % dimensions[1];
            row = numOfSquare / dimensions[1];

            if(column > 0 && row > 0){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare - dimensions[1] - 1]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
            if(row > 0){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare - dimensions[1]]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
            if(column < dimensions[1] -1 && row > 0){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare - dimensions[1] + 1]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
            if(column > 0){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare - 1]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
            if(column < dimensions[1] - 1){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare + 1]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
            if(column > 0 && row < dimensions[0] - 1){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare + dimensions[1] - 1]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
            if(row < dimensions[0] - 1){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare + dimensions[1]]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
            if(column < dimensions[1] - 1 && row < dimensions[0] - 1){
                currentSquare->neighbors.push_back(boardSquares[numOfSquare + dimensions[1] + 1]);
            }
            else{
                currentSquare->neighbors.push_back(nullptr);
            }
        }
    }

    void loadImages(){
        images.push_back("files/images/number_1.png");
        images.push_back("files/images/number_2.png");
        images.push_back("files/images/number_3.png");
        images.push_back("files/images/number_4.png");
        images.push_back("files/images/number_5.png");
        images.push_back("files/images/number_6.png");
        images.push_back("files/images/number_7.png");
        images.push_back("files/images/number_8.png");
        images.push_back("files/images/face_happy.png");
        images.push_back("files/images/face_win.png");
        images.push_back("files/images/face_lose.png");
    }

    void placeAFlag(Square* square){
        if(square->isFlagged){
            numFlags++;
        }
        else{
            numFlags--;
        }
        square->changeFlagging();
    }

    void drawBoard(sf::RenderWindow& window){
        sf::Texture numberTex;
        sf::Sprite numberSprite;


        sf::Texture hiddenTile;
        hiddenTile.loadFromFile("files/images/tile_hidden.png");
        sf::Texture revealedTile;
        revealedTile.loadFromFile("files/images/tile_revealed.png");
        for(int i = 0; i < dimensions[0]*dimensions[1]; i++){
            if(boardSquares[i]->isRevealed){
                numberSprite.setTexture(revealedTile);
            }else{
                numberSprite.setTexture(hiddenTile);
            }
            numberSprite.setPosition(boardSquares[i]->coordinates[0], boardSquares[i]->coordinates[1]);
            window.draw(numberSprite);
        }

        sf::Texture flagTexture;
        flagTexture.loadFromFile("files/images/flag.png");
        for(int i = 0; i < dimensions[0]*dimensions[1]; i++){
            sf::Sprite flagSprite;
            if(boardSquares[i]->isFlagged){
                flagSprite.setTexture(flagTexture);
                flagSprite.setPosition(boardSquares[i]->coordinates[0], boardSquares[i]->coordinates[1]);
                window.draw(flagSprite);
            }
        }



        for(int i = 0; i < dimensions[0]*dimensions[1]; i++){
            if(boardSquares[i]->neighborCount > 0 && boardSquares[i]->isRevealed){
                numberTex.loadFromFile(images[boardSquares[i]->neighborCount-1]);
                numberSprite.setTexture(numberTex);
                numberSprite.setPosition(boardSquares[i]->coordinates[0], boardSquares[i]->coordinates[1]);
                window.draw(numberSprite);
            }
        }



    }

    void drawTimer(sf::RenderWindow &window){
        sf::Texture timerTexture;
        timerTexture.loadFromFile("files/images/digits.png");
        sf::Sprite numberSprite;
        numberSprite.setTexture(timerTexture);


        numberSprite.setTextureRect(sf::IntRect(21*((time / 60) / 10), 0, 21, 32));
        numberSprite.setPosition((dimensions[1]*32) - 97, 32 * (dimensions[0]+0.5) + 16);
        window.draw(numberSprite);

        numberSprite.setTextureRect(sf::IntRect(21*((time / 60) % 10), 0, 21, 32));
        numberSprite.setPosition((dimensions[1]*32) - 76, 32 * (dimensions[0]+0.5) + 16);
        window.draw(numberSprite);

        numberSprite.setTextureRect(sf::IntRect(21*((time % 60) / 10), 0, 21, 32));
        numberSprite.setPosition((dimensions[1]*32) - 54, 32 * (dimensions[0]+0.5) + 16);
        window.draw(numberSprite);

        numberSprite.setTextureRect(sf::IntRect(21*((time % 60) % 10), 0, 21, 32));
        numberSprite.setPosition((dimensions[1]*32) - 33, 32 * (dimensions[0]+0.5) + 16);
        window.draw(numberSprite);
    }



    void drawBar(sf::RenderWindow &window, bool gamePaused){
        sf::Texture numberTex;
        numberTex.loadFromFile("files/images/digits.png");
        sf::Sprite numberSprite;
        int flags = numFlags;
        numberSprite.setTexture(numberTex);
        if(numFlags < 0){
            flags = flags * -1;
            numberSprite.setTextureRect(sf::IntRect(210,0,21,32));
            numberSprite.setPosition(12, 32 * (dimensions[0]+0.5) + 16);
            window.draw(numberSprite);
        }

        numberSprite.setTextureRect(sf::IntRect(21*((flags)/100), 0, 21, 32));
        numberSprite.setPosition(33, 32 * (dimensions[0]+0.5) + 16);
        window.draw(numberSprite);

        numberSprite.setTextureRect(sf::IntRect(21*(((flags) % 100)/ 10), 0, 21, 32));
        numberSprite.setPosition(54, 32 * (dimensions[0]+0.5) + 16);
        window.draw(numberSprite);

        numberSprite.setTextureRect(sf::IntRect(21*((flags) % 10), 0, 21, 32));
        numberSprite.setPosition(75, 32 * (dimensions[0]+0.5) + 16);
        window.draw(numberSprite);




        //Face

        drawFace(window);




        //Buttons
        numberSprite.setTextureRect(sf::IntRect(0,0,64,64));

        sf::Texture debugTexture;
        debugTexture.loadFromFile("files/images/debug.png");
        sf::Texture playTexture;
        playTexture.loadFromFile("files/images/play.png");
        sf::Texture pauseTexture;
        pauseTexture.loadFromFile("files/images/pause.png");
        sf::Texture leaderboardTexture;
        leaderboardTexture.loadFromFile("files/images/leaderboard.png");

        numberSprite.setTexture(debugTexture);
        numberSprite.setPosition(dimensions[1]*32-304, (dimensions[0]+0.5)*32);
        window.draw(numberSprite);

        if(gamePaused){
            numberSprite.setTexture(playTexture);
        }else{
            numberSprite.setTexture(pauseTexture);
        }

        numberSprite.setPosition(dimensions[1]*32-240, (dimensions[0]+0.5)*32);
        window.draw(numberSprite);

        numberSprite.setTexture(leaderboardTexture);
        numberSprite.setPosition(dimensions[1]*32-176, (dimensions[0]+0.5)*32);
        window.draw(numberSprite);



    }

    bool checkIfWon(){
        int counter = 0;
        for (int i = 0; i < dimensions[0]*dimensions[1]; i++){
            if(boardSquares[i]->isRevealed){
                counter++;
            }
        }
        if(counter == dimensions[0]*dimensions[1] - numMines){
            return true;
        }
        else{
            return false;
        }
    }

    int convertClick(int x_coordinate, int y_coordinate){
        int numOfSquare = x_coordinate/32 + y_coordinate/32 * dimensions[1];
        return numOfSquare;
    }

    void drawFace(sf::RenderWindow &window){
        sf::Texture faceTex;
        if(stateOfGame == "playing"){
            faceTex.loadFromFile(images[8]);
        }
        else if(stateOfGame == "won"){
            faceTex.loadFromFile(images[9]);
        }
        else if(stateOfGame == "lost"){
            faceTex.loadFromFile(images[10]);
        }
        sf::Sprite faceSprite;
        faceSprite.setTexture(faceTex);
        faceSprite.setPosition((dimensions[1] / 2.0) * 32 - 32, 32*(dimensions[0] + 0.5));
        window.draw(faceSprite);
    }

};