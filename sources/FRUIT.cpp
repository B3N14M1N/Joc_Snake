#include "FRUIT.h"

Fruit::Fruit(sf::Texture& texture, int pixelsPerUnit){
    ///Constructorul primeste textura si unitatea dupa care sa fie scalata.
    ///Creaza un obiect de tipul "sf::Sprite", il scaleaza si il pozitioneaza corespunzator.
    texture.setSmooth(true);
    this->sprite.setTexture(texture);
    sf::Vector2u size = texture.getSize();
    this->sprite.setScale((float)pixelsPerUnit / size.x, (float)pixelsPerUnit / size.y);
    this->sprite.setPosition(sf::Vector2f(-1.0f * pixelsPerUnit, -1.0f * pixelsPerUnit));
};
sf::Vector2f& Fruit::GetPosition(){
    return this->position;
};
void Fruit::UpdatePosition(sf::Vector2f newPosition){
    this->position = newPosition;
};

void Fruit::DrawFruit(sf::RenderWindow& window,int pixelsPerUnit){
    ///Deoarece pozitia salvata este conform pozitiilor matricii mapa,
    ///pozitia afisata trebuie sa fie conform cu pozitia ecranului,
    ///astfel se inmulteste pozitia curenta cu nr de pixeli pe unitate.
    this->sprite.setPosition(sf::Vector2f(this->position.x * pixelsPerUnit, this->position.y * pixelsPerUnit));
    window.draw(this->sprite);
};
Fruit::~Fruit(){
    std::cout<<"Fruit Object Destroyed\n";
};