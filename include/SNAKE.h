#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <iostream>

enum SnakeBody{
    HeadUp=3,
    HeadLeft=4,
    HeadDown=9,
    HeadRight=8,
    TailDown=13,
    TailRight=14,
    TailUp=19,
    TailLeft=18,
    LeftDown=0,
    Horisontal=1,
    RightDown=2,
    LeftUp=5,
    Vertical=7,
    RightUp=12
};

class Snake{
private:
    const int tilePixelSize=64;
    const int tilesPerRow=5;
    const int snakeMaxLenght=144;

    sf::Texture snakeTexture;

    int lenght;
    int *bodyParts;
    sf::Vector2f *body;

    sf::Vector2f direction;
public:
    ///Constructor
    Snake(sf::Texture&);

    ///Metode
    int GetSnakeLenght();
    sf::Vector2f GetSnakeBodyPosition(int);
    void MoveSnake(sf::Vector2f&);
    void DrawSnake(sf::RenderTarget &, int);
    bool IncreaseSnake();

    ///Deconstructor
    ~Snake();
};
#endif