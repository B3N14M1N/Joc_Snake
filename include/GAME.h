#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "EXCEPTIONS.h"
#include "MAP.h"
#include "SNAKE.h"
#include "FRUIT.h"

#define MAX_FILE_PATH 50

class MyInput{
public:
    bool onceUp,onceDown,onceLeft,onceRight,onceEnter,onceEsc,onceIncrement;
    bool up,down,left,right,enter,esc,increment;

    ///Constructor
    MyInput();

    ///Metode
    void GetInput();

    ///Deconstructor
    ~MyInput();
};

enum MenuTexts{
    yMSize=128,
    xMSize=643,
    mPlay=0,
    mExit=1,
    mContinue=2,
    mRestart=3,
    mMenu=4
};

class Game{
private:
    sf::RenderWindow window;

    int pixelsPerUnit;
    sf::Texture mapTexture;
    sf::Texture snakeTexture;
    sf::Texture fruitTexture;
    sf::Texture menuTexture;
    sf::Texture scoreTexture;
    sf::Texture numbersTexture;
    

    bool inStartingMenu;
    bool inGame;
    bool inPauseMenu;
    bool inGameOverMenu;

    int buttonSelected;

    char saveFile[MAX_FILE_PATH];
    int score;
    int bestScore;

    Map* map;
    Fruit* fruit;
    Snake* snake;

    ///directia in care merge sarpele
    sf::Vector2f direction;

    ///Metode private pentru afisarea grafica
    bool DrawNumber(sf::Vector2f,int);
    bool ShowInGameScore();
    bool ShowMainMenu();
    bool ShowPauseMenu();
    bool ShowGameOverMenu();
public:
    ///Constructor
    Game(int,char*);

    ///Metode
    bool LoadTextures(char*, char*, char*, char*, char*, char*);

    bool Start();
    bool StartGame();
    bool UpdateGame(MyInput&);
    bool ContinueGame();
    bool PauseGame();
    bool RestartGame();
    bool ReturnToMenu();
    bool GameOver();
    bool Exit();

    ///Deconstructor
    ~Game();
};

#endif