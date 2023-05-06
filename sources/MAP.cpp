#include "MAP.h"

Map::Map(sf::Texture& texture, int pixelsPerUnit){
    ///Constructorul creaza sprite-ul bazat pe textura primita;
    ///Scaleaza textura sa incapa in dimensiunea ecranului;
    ///Creeaza matricea datelor mapei, initializeaza toate elementele cu 0;
    this->sprite.setTexture(texture);
    sf::Vector2u size = texture.getSize();
    this->sprite.setScale(float(this->width * pixelsPerUnit) / size.x,float(this->height * pixelsPerUnit) / size.y);
    CreateMapData();
};

void Map::CreateMapData(){
    ///Creaza matricea datelor mapei, initializate cu 0 / EMPTY
    this->map = new int*[this->height];
    for(int i = 0; i < this->height; i++)
    {
        this->map[i] = new int[this->width];
        for(int j = 0; j < this->width; j++)
            this->map[i][j] = Map::EMPTY;
    }
};

void Map::UpdateMap(Fruit& fruit){
    ///Updateaza pozitia fructului in matricea mapei
    this->map[int(fruit.GetPosition().y)][int(fruit.GetPosition().x)] = Map::FRUIT;
};
bool Map::UpdateMap(Snake& snake){
    ///Reseteaza valorie de pe mapa
    for(int i = 0; i < this->height; i++)
    {
        for(int j = 0; j < this->width; j++)
            this->map[i][j] = Map::EMPTY;
    }
    ///Updateaza mapa cu "WALL" pe pozitiile unde se afla sarpele
    int n = snake.GetSnakeLenght();
    for(int i = n-1; i > 0; i--)
    {
        sf::Vector2f snakePos = snake.GetSnakeBodyPosition(i);
        if(snakePos.x >= 0 && snakePos.x < 12 && snakePos.y >= 0 && snakePos.y < 12)
            this->SetInfo((int)snakePos.y, (int)snakePos.x, Map::WALL);
    }
    ///Verifica daca noua pozitie a capului atinge coada sau marginile mapei
    sf::Vector2f snakePos = snake.GetSnakeBodyPosition(0);
    if(snakePos.x >= 0 && snakePos.x < 12 && snakePos.y >= 0 && snakePos.y < 12){
        if(this->GetInfo((int)snakePos.y,(int)snakePos.x) == Map::WALL)
            return false;
        else{
            this->SetInfo((int)snakePos.y, (int)snakePos.x, Map::WALL);
            return true;
        }
    }
    return false;
};
sf::Vector2f Map::RandomFreePosition(){
    ///Genereaza aleatoriu si returneaza o pozitie libera din matricea mapa
    ///Pozitia va fi data fructului
    if(this->IsFull())
        return sf::Vector2f();
    int i = 0;
    int j = 0;
    sf::Vector2f position(j,i);
    do{
        ///Random de la 0 pana la 12 (exclusiv)
        ///Valoare [0,11]
        i = std::rand()%12;
        j = std::rand()%12;
    }while(this->GetInfo(i,j) != Map::EMPTY);
    position.x = (float)j;
    position.y = (float)i;
    return position;
};

void Map::PrintInfoToConsole(){
    for(int i = 0; i < this->height; i++)
    {
        for(int j = 0; j < this->width; j++)
        std::cout<<this->map[i][j]<<' ';
        std::cout<<'\n';
    }
}

void Map::DrawMap(sf::RenderWindow& window){
    window.draw(this->sprite);
};

int Map::GetInfo(int x, int y){
    return this->map[x][y];
};

bool Map::IsFull(){
    ///Verifica daca exista locuri EMPTY in matrice
    for(int i = 0; i < this->height; i++)
        for(int j = 0; j < this->width; j++)
            if(this->map[i][j] == Map::EMPTY)
                return false;
    return true;
};

void Map::SetInfo(int x, int y, int data){
    this->map[x][y] = data;
};

Map::~Map(){
    for(int i=0;i<this->height;i++)
        delete []this->map[i];
    delete []this->map;
    std::cout<<"Map Object Destroyed\n";
};