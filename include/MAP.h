#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include "FRUIT.h"
#include "SNAKE.h"
class Map{
private:
    sf::Sprite sprite;

    // constante de identificare a spatiilor din board
    const int EMPTY = 0;
    const int WALL = 1;
    const int FRUIT = 2;
    
    const int height=12, width=12; //dimensiunile mapei
    int **map;

    void CreateMapData();

public:
    ///Constructor
    Map(sf::Texture&, int);

    ///Metode
    void SetInfo(int, int, int);
    int GetInfo(int, int);
    bool UpdateMap(Snake&);
    void UpdateMap(Fruit&);
    sf::Vector2f RandomFreePosition();
    bool IsFull();
    void PrintInfoToConsole();
    void DrawMap(sf::RenderWindow&);

    ///Deconstructor
    ~Map();
};

#endif