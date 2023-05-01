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

    int unitPixelSize;
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

    sf::Vector2f direction;//directia in care merge sarpele

    //metode
    bool CheckClamp(sf::Vector2f&,float, float);
public:

    Game(int,char*);

    bool LoadTextures(char*, char*, char*, char*, char*, char*);

    bool Start();
    bool StartGame();
    bool UpdateGame();
    bool ContinueGame();
    bool PauseGame();
    bool RestartGame();
    bool ReturnToMenu();
    bool GameOver();
    bool Exit();

    bool DrawNumber(sf::Vector2f,int);
    bool ShowInGameScore();
    bool ShowMainMenu();
    bool ShowPauseMenu();
    bool ShowGameOverMenu();
    ~Game();
};

class MyInput{
public:
    bool onceUp,onceDown,onceLeft,onceRight,onceEnter,onceEsc,onceIncrement;
    bool up,down,left,right,enter,esc,increment;
    MyInput();
    void GetInput();
    void SetUp(bool);
    void SetDown(bool);
    void SetLeft(bool);
    void SetRight(bool);
    void SetEnter(bool);
    void SetEsc(bool);
    ~MyInput();
};
#endif