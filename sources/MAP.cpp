#include "MAP.h"
/*
    Constructor
    Initializeaza dimensiunea texturii si textura;
    Scaleaza textura sa incapa in dimensiunea ecranului;
    Creeaza matricea datelor mapei, initializeaza toate elementele cu 0;
*/
Map::Map(sf::Texture& texture, int _unitPixelSize){
    this->sprite.setTexture(texture);
    sf::Vector2u size = texture.getSize();
    this->sprite.setScale(float(this->width*_unitPixelSize)/size.x,float(this->height*_unitPixelSize)/size.y);
    CreateMapData();
};
/*
    Creeaza matricea datelor mapei;
    Initializeaza toate elementele cu 0;
*/
void Map::CreateMapData(){
    this->map=new int*[this->height];
    for(int i=0;i<this->height;i++)
    {
        this->map[i]=new int[this->width];
        for(int j=0;j<this->width;j++)
            this->map[i][j]=Map::EMPTY;
    }
};

void Map::UpdateMap(Fruit& fruit){
    this->map[int(fruit.GetPosition().y)][int(fruit.GetPosition().x)]=Map::FRUIT;
};
bool Map::UpdateMap(Snake& snake){
    ///Reseteaza valorie de pe mapa
    for(int i=0;i<this->height;i++)
    {
        for(int j=0;j<this->width;j++)
            this->map[i][j]=Map::EMPTY;
    }
    ///Updateaza mapa cu "WALL" pe pozitiile unde se afla sarpele
    int n=snake.GetSnakeLenght();
    for(int i=n-1;i>0;i--)
    {
        sf::Vector2f snakePos=snake.GetSnakeBodyPosition(i);
        if(snakePos.x>=0 && snakePos.x<12 && snakePos.y>=0 && snakePos.y<12)
            this->SetInfo((int)snakePos.y,(int)snakePos.x,Map::WALL);
    }
    ///Verifica daca Capul atinge coada sau marginile mapei
    sf::Vector2f snakePos=snake.GetSnakeBodyPosition(0);
    if(snakePos.x>=0 && snakePos.x<12 && snakePos.y>=0 && snakePos.y<12){
        if(this->GetInfo((int)snakePos.y,(int)snakePos.x)==Map::WALL)
            return false;
        else{
            this->SetInfo((int)snakePos.y,(int)snakePos.x,Map::WALL);
            return true;
        }
    }
    return false;
};
sf::Vector2f& Map::RandomFreePosition(){
    sf::Vector2f* position= new sf::Vector2f;
    int i=0;
    int j=0;
    do{
        i=std::rand()%12;
        j=std::rand()%12;
    }while(this->GetInfo(i,j)!=EMPTY);
    position->x=(float)j;
    position->y=(float)i;
    return *position;
};

/*
    Afiseaza datele matricei in consola;
*/
void Map::PrintInfoToConsole(){
    for(int i=0;i<this->height;i++)
    {
        for(int j=0;j<this->width;j++)
        std::cout<<this->map[i][j]<<' ';
        std::cout<<'\n';
    }
}
/*
    Afiseaza Sprite-ul (Textura) pe ecran;
*/
void Map::DrawMap(sf::RenderWindow& window){
    window.draw(this->sprite);
};

/*
    Interogeaza matricea datelor sau le introduce pentru o anumita pozitie;
*/
int Map::GetInfo(int _x, int _y){
    return this->map[_x][_y];
};
bool Map::IsFull(){
    for(int i=0;i<this->height;i++)
        for(int j=0;j<this->width;j++)
            if(this->map[i][j]==Map::EMPTY)
                return false;
    return true;
};
void Map::SetInfo(int _x, int _y, int _Data){
    this->map[_x][_y]=_Data;
};

/*
    Distruge Matricea datelor;
*/
Map::~Map(){
    // for(int i=0;i<this->height;i++)
    //     delete []this->map[i];
    // delete []this->map;
    std::cout<<"Map Object Destroyed\n";
};