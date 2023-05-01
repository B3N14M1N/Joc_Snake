#ifndef FRUIT_H_INCLUDED
#define FRUIT_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <iostream>
class Fruit{
private:
    sf::Sprite sprite;
    sf::Vector2f position;
public:
    ///Constructor
    Fruit(sf::Texture&,int);

    ///Metode
    sf::Vector2f& GetPosition();
    void UpdatePosition(sf::Vector2f);
    void DrawFruit(sf::RenderWindow&, int);

    ///Deconstructor
    ~Fruit();
};
#endif