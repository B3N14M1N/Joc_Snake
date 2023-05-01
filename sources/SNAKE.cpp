#include "SNAKE.h"

Snake::Snake(sf::Texture& _texture){
    this->snakeTexture=_texture;
    this->lenght=3;
    this->body= new sf::Vector2f[this->lenght];
    this->body[0]=sf::Vector2f(6,4);
    this->body[1]=sf::Vector2f(6,3);
    this->body[2]=sf::Vector2f(6,2);
    this->bodyParts=new int[3]{HeadDown,Vertical,TailUp};
    this->bodyParts=new int[this->lenght]{HeadDown,Vertical,TailUp};
    direction=sf::Vector2f(0.0f,0.0f);
};

void Snake::MoveSnake(sf::Vector2f& newDirection){
    //Updateaza Corpul de la sfarsit pana la al doilea segment (fara cap si gat)
    for(int i=this->lenght-1;i>1;i--)
    {
        this->body[i]=this->body[i-1];
        this->bodyParts[i]=this->bodyParts[i-1];
    }
    //Updateaza al doilea segment adica gatul in functie de noua miscare
    this->body[1]=this->body[0];
    if(newDirection==sf::Vector2f(0.0f,1.0f) && this->direction==sf::Vector2f(-1.0f,0.0f))
        this->bodyParts[1]=LeftDown;
    if(newDirection==sf::Vector2f(1.0f,0.0f) && this->direction==sf::Vector2f(0.0f,-1.0f))
        this->bodyParts[1]=LeftDown;
    if(newDirection==sf::Vector2f(-1.0f,0.0f) && this->direction==sf::Vector2f(0.0f,-1.0f))
        this->bodyParts[1]=RightDown;
    if(newDirection==sf::Vector2f(0.0f,1.0f) && this->direction==sf::Vector2f(1.0f,0.0f))
        this->bodyParts[1]=RightDown;
    if(newDirection==sf::Vector2f(0.0f,-1.0f) && this->direction==sf::Vector2f(-1.0f,0.0f))
        this->bodyParts[1]=LeftUp;
    if(newDirection==sf::Vector2f(1.0f,0.0f) && this->direction==sf::Vector2f(0.0f,1.0f))
        this->bodyParts[1]=LeftUp;
    if(newDirection==sf::Vector2f(0.0f,-1.0f) && this->direction==sf::Vector2f(1.0f,0.0f))
        this->bodyParts[1]=RightUp;
    if(newDirection==sf::Vector2f(-1.0f,0.0f) && this->direction==sf::Vector2f(0.0f,1.0f))
        this->bodyParts[1]=RightUp;
    if(newDirection==this->direction && newDirection.y==0)
        this->bodyParts[1]=Horisontal;
    if(newDirection==this->direction && newDirection.x==0)
        this->bodyParts[1]=Vertical;

    //Updateaza coada in functie de noua miscare
    sf::Vector2f dir=this->body[this->lenght-1]-this->body[this->lenght-2];
    if(dir.x==1.0f)
        this->bodyParts[this->lenght-1]=TailLeft;
    if(dir.x==-1.0f)
        this->bodyParts[this->lenght-1]=TailRight;
    if(dir.y==1.0f)
        this->bodyParts[this->lenght-1]=TailDown;
    if(dir.y==-1.0f)
        this->bodyParts[this->lenght-1]=TailUp;
    //Updateaza primul segment adica capul
    this->body[0]+=newDirection;
    if(newDirection.x==1)
        this->bodyParts[0]=HeadLeft;
    if(newDirection.x==-1)
        this->bodyParts[0]=HeadRight;
    if(newDirection.y==1)
        this->bodyParts[0]=HeadDown;
    if(newDirection.y==-1)
        this->bodyParts[0]=HeadUp;
    //Updateaza directia
    this->direction=newDirection;
};
void Snake::DrawSnake(sf::RenderTarget& window, int pixelPerUnit){
    for(int i=0;i<this->lenght;i++)
    {
        /*
         *  Aceasta secventa de cod genereaza partile grafice ale sarpelui.
         *  Mai exact, imparte textura principala, sub forma matriceala si 
         *  selecteaza partea de sarpe corespunzatoare valorii din "this->bodyParts[i]",
         *  apoi scaleaza si pozitioneaza imaginea corespunzator pozitiei din mapa.
         */
        int j=this->bodyParts[i]/tilesPerRow;
        sf::Vector2i pos(int((this->bodyParts[i]-(j*tilesPerRow))*tilePixelSize),j*tilePixelSize);
        sf::Vector2i size(tilePixelSize,tilePixelSize);
        sf::IntRect rectangle(pos,size);
        sf::Sprite bodyPart(snakeTexture,rectangle);
        bodyPart.setScale(sf::Vector2f((float)pixelPerUnit/(float)tilePixelSize,(float)pixelPerUnit/(float)tilePixelSize));
        bodyPart.setPosition(sf::Vector2f(this->body[i].x*pixelPerUnit,this->body[i].y*pixelPerUnit));
        window.draw(bodyPart);
    }
};
bool Snake::IncreaseSnake(){
    if(this->lenght>=this->snakeMaxLenght)
        return false;
    else
    {
        this->lenght++;
        sf::Vector2f* NewBody= new sf::Vector2f[this->lenght];
        int *NewBodyParts=new int[this->lenght];
        for(int i=0;i<this->lenght-1;i++)
        {
            NewBody[i]=this->body[i];
            NewBodyParts[i]=this->bodyParts[i];
        }
        NewBodyParts[this->lenght-1]=NewBodyParts[this->lenght-2];
        NewBody[this->lenght-1]=NewBody[this->lenght-2];
        // delete []this->body;
        // delete []this->bodyParts;
        this->body=NewBody;
        this->bodyParts=NewBodyParts;
        //delete []NewBody;
        //delete []NewBodyParts;
    }
    return true;
};
sf::Vector2f Snake::GetSnakeBodyPosition(int index){
    return this->body[index];
};
int Snake::GetSnakeLenght(){
    return this->lenght;
};
Snake::~Snake(){
    // delete [] this->body;
    // delete [] this->bodyParts;
    std::cout<<"Snake Object Destroyed\n";
};