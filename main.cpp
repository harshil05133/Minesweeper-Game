#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <chrono>


#define ENTER_KEY 13
#define DELETE_KEY 8
using namespace std;
using namespace std::chrono;

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void setName(string& somestring){
    if (somestring.length() != 0){
        somestring[0] = toupper(somestring[0]);
        for(int i = 1; i < somestring.length(); i++){
            somestring[i] = tolower(somestring[i]);
        }
    }
}


sf::Text checkUserName(sf::Text& textbox, char temp, string& username, int width, int height){
    if(temp == DELETE_KEY && !username.empty()){
        string tempText;
        for(int i = 0; i < username.length(); i++){
            tempText += username[i];
        }
        username = tempText;
        textbox.setString(username + "|");
        setText(textbox, (float)width/2, (float)height - 45);
        return textbox;
    }
    else if(isalpha(temp)){
        if(username.empty()){
            username += (char)toupper(temp);
            textbox.setString(username + "|");
            setText(textbox, (float)width, (float)height);
        }
        else if(username.length() < 10 && !username.empty()){
            username += (char)tolower(temp);
            textbox.setString(username + "|");
            setText(textbox, (float)width, (float)height);
        }
    }
    return textbox;
}

int main() {
//read the config file and store into variables
    ifstream configFile;
    configFile.open("files/config.cfg");
    string line;
    vector<string> configVector;
    if(!configFile.is_open()){
        cout << "hello" << endl;
    }
    while(getline(configFile, line)){
        configVector.push_back(line);
    }
    configFile.close();

    //convert into integers and convert to pixel amounts
    int numColumns = stoi(configVector[0]);
    int numRows = stoi(configVector[1]);
    int numMines = stoi(configVector[2]);
    int welcomeWindowPixelHeight = (numRows * 32) + 100;
    int welcomeWindowPixelWidth = numColumns * 32;
    int gameWindowPixelHeight = (numRows * 32) + 100;
    int gameWindowPixelWidth = numColumns * 32;
    int leaderboardWindowPixelHeight = (numRows * 16) + 50;
    int leaderboardWindowPixelWidth = numColumns * 16;

//create welcome window
    sf::RenderWindow welcomeWindow(sf::VideoMode(welcomeWindowPixelWidth, welcomeWindowPixelHeight), "Minesweeper", sf::Style::Close);
    bool notForcedClose = true;
    sf::Font font;
    font.loadFromFile("files/font.ttf");

    //create the welcome message
    sf::Text text1;
    text1.setFont(font);
    text1.setString("WELCOME TO MINESWEEPER!");
    text1.setCharacterSize(24);
    text1.setFillColor(sf::Color::White);
    text1.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(text1, welcomeWindowPixelWidth / 2, (welcomeWindowPixelHeight / 2) - 150);

    //create the name message
    sf::Text text2;
    text2.setFont(font);
    text2.setString("Enter your name: ");
    text2.setCharacterSize(20);
    text2.setFillColor(sf::Color::White);
    text2.setStyle(sf::Text::Bold);
    setText(text2, welcomeWindowPixelWidth / 2, (welcomeWindowPixelHeight / 2) - 75);

    //create the textbox for name entering
    sf::Text textbox;
    textbox.setFont(font);
    textbox.setString("|");
    textbox.setCharacterSize(18);
    textbox.setFillColor(sf::Color::Yellow);
    textbox.setStyle(sf::Text::Bold);
    setText(textbox, welcomeWindowPixelWidth / 2, (welcomeWindowPixelHeight / 2) - 45);
    string username;

    //Welcome window loop
    while(welcomeWindow.isOpen()){
        sf::Event event1;
        while(welcomeWindow.pollEvent(event1)){
            if(event1.type == sf::Event::Closed){
                notForcedClose = false;
                welcomeWindow.close();
                return 0;
            }
            else if (event1.type == sf::Event::TextEntered){
                if (event1.text.unicode < 128){
                    if (event1.text.unicode == 13 && username.length() > 0) {
                        welcomeWindow.close();
                        break;
                    }
                    if (event1.text.unicode == 8 && username.length() > 0) {
                        username.pop_back();
                    }
                    char temp = event1.text.unicode;
                    checkUserName(textbox, temp, username, welcomeWindowPixelWidth, welcomeWindowPixelHeight);
                    setText(textbox, welcomeWindowPixelWidth / 2, (welcomeWindowPixelHeight / 2) - 45);
                    textbox.setString(username + "|");
                }
            }
        }
        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(text1);
        welcomeWindow.draw(text2);
        welcomeWindow.draw(textbox);

        welcomeWindow.display();

    }

    sf::RenderWindow gameWindow(sf::VideoMode(gameWindowPixelWidth, gameWindowPixelHeight), "Minesweeper", sf::Style::Close);

    sf::Texture hiddenTileTex;
    hiddenTileTex.loadFromFile("files/images/tile_hidden.png");
    sf::Sprite hiddenTileSprite;
    hiddenTileSprite.setTexture(hiddenTileTex);
    sf::FloatRect bounds = hiddenTileSprite.getGlobalBounds();
    float width = bounds.width;
    float height = bounds.height;

    cout << "Width: " << width << endl <<"Height: " << height << endl;

    sf::Texture revealedTileTex;
    revealedTileTex.loadFromFile("files/images/tile_hidden.png");
    sf::Sprite revealedTileSprite;
    revealedTileSprite.setTexture(revealedTileTex);

    sf::Texture flagTex;
    flagTex.loadFromFile("files/images/flag.png");
    sf::Sprite flagSprite;
    flagSprite.setTexture(flagTex);

    sf::Texture mineTex;
    mineTex.loadFromFile("files/images/mine.png");
    sf::Sprite mineSprite;
    mineSprite.setTexture(mineTex);

    sf::Texture happyFaceTex;
    happyFaceTex.loadFromFile("files/images/face_happy.png");
    sf::Sprite happyFaceSprite;
    happyFaceSprite.setTexture(happyFaceTex);
    happyFaceSprite.setPosition((float((numColumns/2.0)*32)-32), float(32 * (numRows + 0.5)));

    sf::Texture debugTex;
    debugTex.loadFromFile("files/images/debug.png");
    sf::Sprite debugSprite;
    debugSprite.setTexture(debugTex);
    debugSprite.setPosition(float((numColumns*32)-304), float(32*(numRows+0.5)));


    sf::Texture pauseTex;
    pauseTex.loadFromFile("files/images/pause.png");
    sf::Sprite pauseSprite;
    pauseSprite.setTexture(pauseTex);
    pauseSprite.setPosition(float((numColumns*32)-240), float(32 * (numRows+0.5)));

    sf::Texture leaderboardTex;
    leaderboardTex.loadFromFile("files/images/leaderboard.png");
    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(leaderboardTex);
    leaderboardSprite.setPosition(float((numColumns*32)-176), float(32*(numRows+0.5)));

    bool debugMode = false;
    bool paused = false;



    if(notForcedClose) {

        Board game = Board(numRows, numColumns, numMines);
        game.loadImages();
        game.placeMines();
        game.fillNeighborVector();

        for(int i = 0; i < game.dimensions[0] * game.dimensions[1]; i++)
        {
            game.boardSquares[i]->setNeighborCount();

        }


        int timePaused = 0;
        auto startTime = high_resolution_clock::now();
        auto startPausedTime = high_resolution_clock::now();
        auto endTime = high_resolution_clock::now();
        auto endPausedTime = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(endTime - startTime);
        auto durationPaused = duration_cast<seconds>(endPausedTime - startPausedTime);


        Square *sq;
        bool showLeaderboard=  false;
        while (gameWindow.isOpen()) {
            sf::Event event2;
            while (gameWindow.pollEvent(event2)) {
                if (event2.type == sf::Event::Closed) {
                    gameWindow.close();
                    return 0;
                }
                else if (event2.type == sf::Event::MouseButtonPressed) {
                    if (event2.mouseButton.button == sf::Mouse::Left) {

                        if (event2.mouseButton.x < 32 * numColumns && event2.mouseButton.y < 32 * numRows && !paused && !showLeaderboard) {
                            sq = game.boardSquares[game.convertClick(event2.mouseButton.x, event2.mouseButton.y)];
                            if (sq->isMine) {
                                if (!sq->isFlagged) {
                                    game.stateOfGame = "lost";
                                }
                            }
                            else {
                                if (game.stateOfGame == "playing") {
                                    if (!sq->isFlagged) {
                                        sq->reveal();
                                        if (game.checkIfWon()) {
                                            game.stateOfGame = "won";
                                            showLeaderboard = true;
                                            for (int i = 0; i < numRows * numColumns; i++) {
                                                if (game.boardSquares[i]->isMine) {
                                                    game.placeAFlag(game.boardSquares[i]);
                                                }
                                            }
                                            game.drawBoard(gameWindow);
                                            game.drawFace(gameWindow);
                                            gameWindow.display();
                                        }
                                    }
                                }
                            }
                        }
                        else if (event2.mouseButton.x >= (numColumns * 32 - 304) &&
                                   event2.mouseButton.x <= (numColumns * 32 - 240)
                                   && event2.mouseButton.y >= (32 * (numRows + 0.5)) &&
                                   event2.mouseButton.y <= (32 * (numRows + 0.5) + 64) && !showLeaderboard) {
                            if (game.stateOfGame == "playing") {
                                if (debugMode) {
                                    debugMode = false;
                                } else {
                                    debugMode = true;
                                }
                            }
                        }
                        else if (event2.mouseButton.x >= (numColumns / 2 * 32 - 32) &&
                                   event2.mouseButton.x <= (numColumns / 2 * 32 + 32)
                                   && event2.mouseButton.y >= (32 * (numRows + 0.5)) &&
                                   event2.mouseButton.y <= (32 * (numRows + 0.5) + 64) && !showLeaderboard) {
                            debugMode = false;
                            startTime = high_resolution_clock::now();
                            timePaused = 0;

                            Board game2 = Board(numRows, numColumns, numMines);
                            game = game2;
                            game.loadImages();
                            game.placeMines();
                            game.fillNeighborVector();
                            for(int i = 0; i < game.dimensions[0] * game.dimensions[1]; i++)
                            {
                                game.boardSquares[i]->setNeighborCount();

                            }


                        }
                        else if (event2.mouseButton.x >= (numColumns * 32 - 240) &&
                                   event2.mouseButton.x <= (numColumns * 32 - 176)
                                   && event2.mouseButton.y >= (32 * (numRows + 0.5)) &&
                                   event2.mouseButton.y <= (32 * (numRows + 0.5) + 64) &&
                                   game.stateOfGame == "playing" && !showLeaderboard) {

                            if (!paused) {
                                startPausedTime = high_resolution_clock::now();
                            } else {
                                endPausedTime = high_resolution_clock::now();
                                durationPaused = duration_cast<seconds>(endPausedTime - startPausedTime);
                                timePaused += durationPaused.count();

                            }
                            paused = !paused;
                        }
                    }
                        else if (event2.mouseButton.button == sf::Mouse::Right && game.stateOfGame == "playing" &&
                                   !paused && !showLeaderboard) {
                            if (event2.mouseButton.x < (32 * numColumns) && event2.mouseButton.y < (32 * numRows)) {
                                game.placeAFlag(game.boardSquares[game.convertClick(event2.mouseButton.x,
                                                                                    event2.mouseButton.y)]);
                            }
                        }

                        else if (event2.mouseButton.x >= (numColumns * 32 - 176) && event2.mouseButton.x <= (numColumns * 32 - 112)
                                 && event2.mouseButton.y >= (32 * numRows + 0.5)
                                 && event2.mouseButton.y <= (32 * numRows + 0.5) + 64 && showLeaderboard){
                            showLeaderboard = true;
                            sf::Sprite backgroundLeaderboardSprite;
                            sf::Texture backgroundLeaderboardTex;
                            backgroundLeaderboardTex.loadFromFile("files/images/tile_revealed.png");

                            backgroundLeaderboardSprite.setTexture(backgroundLeaderboardTex);
                            for (int i = 0; i < game.dimensions[0]*game.dimensions[1]; i++) {
                                backgroundLeaderboardSprite.setPosition(game.boardSquares[i]->coordinates[0], game.boardSquares[i]->coordinates[1]);
                                gameWindow.draw(backgroundLeaderboardSprite);
                            }
                            gameWindow.display();

                            startPausedTime = high_resolution_clock::now();
                        }

                }

            }

            fstream leaderboardFile;
            leaderboardFile.open("leaderboard.txt");
            string line2;
            vector<string> names;

            string winners;
            string firstName;
            string secondName;
            string leaderBoardTime;
            string drop;
            vector<string> leaderboardWinners;
            vector<int> winnerTimes;
            bool isNewRecord = false;

            if(showLeaderboard){
                int rank = 5;
                leaderboardFile.clear();
                leaderboardFile.seekg(0);
                leaderboardWinners.clear();
                winnerTimes.clear();

                for(int i = 0; i < 5; i++)
                {
                    getline(leaderboardFile, winners);
                    leaderboardWinners.push_back(winners);
                }

                leaderboardFile.clear();
                leaderboardFile.seekg(0);

                if(game.stateOfGame == "won" && !isNewRecord)
                {
                    isNewRecord = true;
                    rank=5;

                    for(int i = 0; i < 5; i++)
                    {
                        getline(leaderboardFile, leaderBoardTime, ',');
                        getline(leaderboardFile, drop);
                        winnerTimes.push_back(stoi(leaderBoardTime.substr(0, 1)) * 600 +
                        stoi(leaderBoardTime.substr(1, 1)) * 60 +
                        stoi(leaderBoardTime.substr(3, 1)) * 10 + stoi(leaderBoardTime.substr(4, 1)));
                    }

                    while(game.time < winnerTimes[rank-1])
                    {
                        rank-=1;
                    }

                    if(rank<5)
                    {
                        ofstream leaderboardFile2("files/leaderboard.txt");
                        leaderboardWinners.insert(leaderboardWinners.begin() + rank, (to_string(game.time / 600) +
                        to_string(game.time / 60 % 10) + ":" + to_string(game.time % 60 / 10) +
                        to_string(game.time % 60 % 10) + ", " + firstName));
                        for(int i = 0; i < 5; i++) {
                            leaderboardFile2 << leaderboardWinners[i] << endl;
                        }
                    }
                }

                winners = "";
                leaderboardFile.clear();
                leaderboardFile.seekg(0);

                for (int i = 0; i < 5; i++) {
                    getline(leaderboardFile, firstName, ',');
                    getline(leaderboardFile, secondName);
                    winners.append(to_string(i) + ".\t\t" + firstName + "\t\t" + secondName);
                    if(i-1 == rank)
                    {
                        winners += "*";
                    }
                    winners+= "\n\n";
                }

                sf::Text winnerTex;
                winnerTex.setFont(font);
                winnerTex.setStyle(sf::Text::Bold);
                winnerTex.setCharacterSize(18);
                winnerTex.setString(winners);
                winnerTex.setFillColor(sf::Color::White);

                setText(winnerTex, numColumns * 16 / 2, (numRows * 16 + 50) / 2 + 20);

                sf::Text leaderboardWordTex;
                leaderboardWordTex.setFont(font);
                leaderboardWordTex.setStyle(sf::Text::Bold | sf::Text::Underlined);
                leaderboardWordTex.setCharacterSize(20);
                leaderboardWordTex.setFillColor(sf::Color::White);
                leaderboardWordTex.setString("LEADERBOARD");

                setText(leaderboardWordTex, numColumns * 16 / 2, (numRows * 16 + 50) / 2 - 120);

                //Render Leaderboard window, handle closing
                sf::RenderWindow leaderboardWindow(sf::VideoMode(leaderboardWindowPixelWidth, leaderboardWindowPixelHeight), "Minesweeper", sf::Style::Close);
                while (leaderboardWindow.isOpen()) {
                    sf::Event event3;
                    while (leaderboardWindow.pollEvent(event3)) {
                        if (event3.type == sf::Event::Closed) {
                            leaderboardWindow.close();
                            showLeaderboard = false;

                            endPausedTime = high_resolution_clock::now();
                            durationPaused = duration_cast<seconds>(endPausedTime - startPausedTime);

                        }
                    }

                    leaderboardWindow.clear(sf::Color::Blue);

                    leaderboardWindow.draw(winnerTex);
                    leaderboardWindow.draw(leaderboardWordTex);

                    leaderboardWindow.display();
                }
            }


            gameWindow.clear(sf::Color::White);

            if (game.stateOfGame == "playing") {
                if (paused == false) {
                    endTime = high_resolution_clock::now();
                    duration = duration_cast<seconds>(endTime - startTime);
                    game.time = duration.count();
                    game.time -= timePaused;
                }
            }


            game.drawBoard(gameWindow);
            if (debugMode == true) {
                game.drawMines(gameWindow);
            } else if (game.stateOfGame == "lost") {
                game.drawMines(gameWindow);
            }

            game.drawTimer(gameWindow);
            game.drawBar(gameWindow, paused);
            gameWindow.display();
        }
    }





    return 0;
}