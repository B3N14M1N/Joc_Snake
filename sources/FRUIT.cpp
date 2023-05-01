#include "FRUIT.h"

Fruit::Fruit(sf::Texture& texture, int _unitPixelSize){
    texture.setSmooth(true);
    this->sprite.setTexture(texture);
    sf::Vector2u size = texture.getSize();
    this->sprite.setScale((float)_unitPixelSize/size.x,(float)_unitPixelSize/size.y);
    this->sprite.setPosition(sf::Vector2f(-1.0f*_unitPixelSize,-1.0f*_unitPixelSize));
};
sf::Vector2f& Fruit::GetPosition(){
    return this->position;
};
void Fruit::UpdatePosition(sf::Vector2f& newPosition){
    this->position=newPosition;
};

void Fruit::DrawFruit(sf::RenderWindow& window,int _unitPixelSize){
    window.draw(this->sprite);
    this->sprite.setPosition(sf::Vector2f(this->position.x*_unitPixelSize,this->position.y*_unitPixelSize));
};
void Fruit::PrintPositionToConsole(){
    std::cout<<position.x<<' '<<position.y<<"\n";
};
Fruit::~Fruit(){
    std::cout<<"Fruit Object Destroyed\n";
};