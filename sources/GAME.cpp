#include "GAME.h"
Game::Game(int _unitPixelSize, char* _saveFile):unitPixelSize(_unitPixelSize){
    unitPixelSize /= 12;
    strcpy(this->saveFile,_saveFile);
    std::fstream saveDataFile=std::fstream(_saveFile, std::ios::in | std::ios::out | std::ios::app);
    if(!saveDataFile)
    {  
        throw Exception((char*)"reading file", (char*)"open file error");
    }
    else
    {
        if(saveDataFile.peek() != std::ifstream::traits_type::eof())
        saveDataFile>>this->bestScore;
        saveDataFile.close();
    }
};

/*
    Incarca Texturile din memorie
*/
bool Game::LoadTextures(char* tMap, char* tSnake, char* tFruit, char* tMenu, char* tScore, char* tNumbers){
    if (!this->mapTexture.loadFromFile(tMap))
    {
        Exception exceptie((char*)"loading texture",(char*)"map.png not found");
        throw exceptie;
        return false;
    }
    if (!this->snakeTexture.loadFromFile(tSnake))
    {
        Exception exceptie((char*)"loading texture",(char*)"snake.png not found");
        throw exceptie;
        return false;
    }
    if (!this->fruitTexture.loadFromFile(tFruit))
    {
        Exception exceptie((char*)"loading texture",(char*)"fruit.png not found");
        throw exceptie;
        return false;
    }
    if (!this->menuTexture.loadFromFile(tMenu))
    {
        Exception exceptie((char*)"loading texture",(char*)"menu.png not found");
        throw exceptie;
        return false;
    }
    if (!this->scoreTexture.loadFromFile(tScore))
    {
        Exception exceptie((char*)"loading texture",(char*)"score.png not found");
        throw exceptie;
        return false;
    }
    if (!this->numbersTexture.loadFromFile(tNumbers))
    {
        Exception exceptie((char*)"loading texture",(char*)"numbers.png not found");
        throw exceptie;
        return false;
    }
    return true;
};
/*
    Initializeaza ecranul si obiectele din joc, cum ar fi:
    mapa, sarpele, fructele, meniuri etc..
*/
bool Game::Start(){
    this->buttonSelected = 0;
    this->inStartingMenu = true;
    this->inGame = false;
    this->inPauseMenu = false;
    this->inGameOverMenu = false;
    this->window.create(sf::VideoMode(this->unitPixelSize*12,this->unitPixelSize*13),"SNAKE GAME");
    MyInput input;

    while (this->window.isOpen())
    {
        input.GetInput();
        sf::Event event;
        while (this->window.pollEvent(event))
        {
            // "close requested" event: we close the window;
            if (event.type == sf::Event::Closed)
                this->window.close();
        }
        // clear the window with black color;
        this->window.clear(sf::Color::Black);
        //std::system("clear");
        //Afiseaza meniurile
        if(this->inGameOverMenu)
            this->ShowGameOverMenu();
        if(this->inPauseMenu)
            this->ShowPauseMenu();
        if(this->inStartingMenu)
            this->ShowMainMenu();
        //Proceseaza input-urile pentru meniuri
        if(input.onceEsc)
            this->PauseGame();
        if(this->inPauseMenu || this->inStartingMenu || this->inGameOverMenu)
        {
            //mareste FPS-urile pentru o sensibilitate a controlului mai buna
            this->window.setFramerateLimit(60);
            if(this->inPauseMenu)
            {
                //preia inputul pentru selectarea butoanelor (3)
                // 0 - Continue
                // 1 - Restart
                // 2 - Menu
                if(input.onceDown && buttonSelected<2)
                    this->buttonSelected += 1;
                if(input.onceUp && buttonSelected>0)
                    this->buttonSelected -= 1;
            }
            if(inStartingMenu || inGameOverMenu)
            {
                //preia inputul pentru selectarea butoanelor (2)
                // 0 - Restart
                // 1 - Menu
                if(input.onceDown)
                    this->buttonSelected = 1;
                if(input.onceUp)
                    this->buttonSelected = 0;
            }
            //proceseaza meniul
            if(input.onceEnter == true)
            {
                //Play sau continue
                if(this->buttonSelected == 0){
                    if(this->inStartingMenu)
                        this->StartGame();
                    else
                        if(this->inPauseMenu)
                            this->ContinueGame();
                        else //inGameOverMenu
                            this->RestartGame();
                }
                if(this->buttonSelected == 1){
                    if(this->inStartingMenu)
                        //Butonul exit
                        this->Exit();
                    else
                        if(this->inPauseMenu)
                            this->RestartGame();
                        else //inGameOVerMenu
                            this->ReturnToMenu();
                }
                if(this->buttonSelected == 2)
                    this->ReturnToMenu();
            }
        }
        /*PROCESARE APLICATIE*/
        if(this->inGame && !this->inStartingMenu && !this->inPauseMenu && !this->inGameOverMenu)
        {
            this->window.setFramerateLimit(3);
            if(!this->UpdateGame())
                this->GameOver();
        }
        // end the current frame
        this->window.display();
    }
    return true;
};
bool Game::UpdateGame(){
    //culege input-ul
    MyInput input;
    input.GetInput();
    // //proceseaza input-ul
    sf::Vector2f newDirection(0.0f,0.0f);
    if(input.down){
        newDirection = sf::Vector2f(0.0f,0.0f);
        newDirection.y = 1;
    }
    if(input.up){
        newDirection = sf::Vector2f(0.0f,0.0f);
        newDirection.y =- 1;
    }
    if(input.right){
        newDirection = sf::Vector2f(0.0f,0.0f);
        newDirection.x = 1;
    }
    if(input.left){
        newDirection = sf::Vector2f(0.0f,0.0f);
        newDirection.x = -1;
    }
    //updateaza jocul
    if(newDirection != sf::Vector2f(0.0f,0.0f) && this->direction+newDirection != sf::Vector2f(0.0f,0.0f))
    {
        this->direction = newDirection;
    }
    if(this->direction != sf::Vector2f(0.0f,0.0f))
    {
        //verificare daca sarpele iese din dimenisunea matricei
        this->snake->MoveSnake(this->direction);
        //verifica daca au fost ciocniri, daca da, atungi GameOver
        if(!this->map->UpdateMap(*(this->snake)))
            return false;
        this->map->UpdateMap(*(this->fruit));
    }
    if(input.onceIncrement)
    {
        if(!this->snake->IncreaseSnake())
            return false;
        else
            this->score++;
    }
    if(this->fruit->GetPosition() == this->snake->GetSnakeBodyPosition(0))
    {
        //if full- Game Over
        if(!this->snake->IncreaseSnake())
            return false;
        else
            this->score++;
        //verificare daca sarpele iese din dimenisunea matricei
        this->map->UpdateMap(*(this->snake));
        if(this->map->IsFull())
            return false;//Game Over
        this->fruit->UpdatePosition(this->map->RandomFreePosition());
        this->map->UpdateMap(*(this->fruit));
    }
    //afiseaza jocul
    this->ShowInGameScore();
    this->map->DrawMap(this->window);
    this->fruit->DrawFruit(this->window, unitPixelSize);
    this->snake->DrawSnake(this->window, unitPixelSize);
    //this->map->PrintInfoToConsole();
    return true;
};
bool Game::ContinueGame(){
    this->inPauseMenu = false;
    return !this->inPauseMenu;
};
bool Game::RestartGame(){
    this->direction = sf::Vector2f(0.0f,0.0f);
    this->inGame = false;
    return this->StartGame();
};
bool Game::ReturnToMenu(){
    this->buttonSelected = 0;
    this->inPauseMenu = false;
    this->inGameOverMenu = false;
    this->inGame = false;
    this->inStartingMenu = true;
    return true;
};
bool Game::PauseGame(){
     if(this->inStartingMenu || this->inGameOverMenu)
        return false;
    this->buttonSelected = 0;
    this->inPauseMenu = true;
    return true;
};
bool Game::StartGame()
{
    this->inStartingMenu = false;
    this->inPauseMenu = false;
    this->inGameOverMenu = false;
    this->score = 0;
    if(this->snake!=NULL)
        delete this->snake;
    if(this->fruit!=NULL)
        delete this->fruit;
    if(this->map!=NULL)
        delete this->map;
    //genereaza noui elemente: mapa, fruct,sarpe
    this->map = new Map(this->mapTexture, this->unitPixelSize);
    this->fruit = new Fruit(this->fruitTexture, this->unitPixelSize);
    this->snake = new Snake(this->snakeTexture);
    this->map->UpdateMap(*(this->snake));
    this->fruit->UpdatePosition(this->map->RandomFreePosition());
    this->map->UpdateMap(*(this->fruit));
    //reseteaza directia
    this->direction = sf::Vector2f(0.0f,1.0f);
    this->inGame = true;
    return this->inGame;
}
bool Game::GameOver(){
    this->inGame = false;
    this->inPauseMenu = false;
    this->inStartingMenu = false;
    this->inGameOverMenu = true;
    if(this->score > this->bestScore){
        this->bestScore = this->score;

        std::fstream saveDataFile=std::fstream(this->saveFile, std::ios::out | std::ios::trunc);
        if(!saveDataFile)
        {  
            throw Exception((char*)"reading file", (char*)"open file error");
        }
        else
        {
            saveDataFile<<this->bestScore;
            saveDataFile.close();
        }
    };
    return this->inGameOverMenu;
};
/*
    Updateaza ecranul, preia datele de intrare, updateaza evenimentele;
*/
bool Game::ShowMainMenu(){
    float scaleFactor = (this->unitPixelSize * 2.0f) / (float)yMSize;
    sf::Vector2f scaleVect = sf::Vector2f(scaleFactor, scaleFactor);
    sf::Vector2f scaleSelectedVect = sf::Vector2f(1.25f * scaleFactor, 1.25f * scaleFactor);
    sf::Vector2i size(xMSize, yMSize);

    sf::Sprite sPlay(this->menuTexture, sf::IntRect(sf::Vector2i(0, mPlay * yMSize), size));
    sf::Sprite sExit(this->menuTexture, sf::IntRect(sf::Vector2i(0, mExit * yMSize), size));

    sPlay.setScale(this->buttonSelected == 0 ? scaleSelectedVect:scaleVect);
    sExit.setScale(this->buttonSelected == 1 ? scaleSelectedVect:scaleVect);

    sPlay.setPosition(sf::Vector2f(3 * this->unitPixelSize, 3 * this->unitPixelSize));
    sExit.setPosition(sf::Vector2f(3 * this->unitPixelSize, 6 * this->unitPixelSize));

    this->window.draw(sPlay);
    this->window.draw(sExit);
    return true;
};
bool Game::ShowPauseMenu(){
    float scaleFactor = (this->unitPixelSize * 2.0f) / (float)yMSize;
    sf::Vector2f scaleVect=sf::Vector2f(scaleFactor, scaleFactor);
    sf::Vector2f scaleSelectedVect=sf::Vector2f(1.15f * scaleFactor, 1.15f * scaleFactor);
    sf::Vector2i size(xMSize, yMSize);

    sf::Sprite sContinue(this->menuTexture, sf::IntRect(sf::Vector2i(0, mContinue * yMSize), size));
    sf::Sprite sRestart(this->menuTexture, sf::IntRect(sf::Vector2i(0, mRestart * yMSize), size));
    sf::Sprite sMenu(this->menuTexture, sf::IntRect(sf::Vector2i(0, mMenu * yMSize), size));

    sContinue.setScale(this->buttonSelected == 0 ? scaleSelectedVect:scaleVect);
    sRestart.setScale(this->buttonSelected == 1 ? scaleSelectedVect:scaleVect);
    sMenu.setScale(this->buttonSelected == 2 ? scaleSelectedVect:scaleVect);

    sContinue.setPosition(sf::Vector2f(0.5f * this->unitPixelSize, 3 * this->unitPixelSize));
    sRestart.setPosition(sf::Vector2f(0.5f * this->unitPixelSize, 6 * this->unitPixelSize));
    sMenu.setPosition(sf::Vector2f(0.5f * this->unitPixelSize, 9 * this->unitPixelSize));

    this->window.draw(sContinue);
    this->window.draw(sRestart);
    this->window.draw(sMenu);
    return true;
};
bool Game::ShowGameOverMenu(){
    //draw Game over menu.
    float scaleFactor = (this->unitPixelSize * 2.0f) / (float)yMSize;
    sf::Vector2f scaleVect = sf::Vector2f(scaleFactor, scaleFactor);
    sf::Vector2f scaleSelectedVect = sf::Vector2f(1.15f * scaleFactor, 1.15f * scaleFactor);
    sf::Vector2i size(xMSize, yMSize);

    sf::Sprite sRestart(this->menuTexture, sf::IntRect(sf::Vector2i(0, mRestart * yMSize), size));
    sf::Sprite sMenu(this->menuTexture, sf::IntRect(sf::Vector2i(0, mMenu * yMSize), size));

    sRestart.setScale(this->buttonSelected == 0 ? scaleSelectedVect:scaleVect);
    sMenu.setScale(this->buttonSelected == 1 ? scaleSelectedVect:scaleVect);

    sRestart.setPosition(sf::Vector2f(0.5f * this->unitPixelSize, 6 * this->unitPixelSize));
    sMenu.setPosition(sf::Vector2f(0.5f * this->unitPixelSize, 9 * this->unitPixelSize));

    this->window.draw(sRestart);
    this->window.draw(sMenu);
    //draw score, best
    scaleVect = sf::Vector2f((this->unitPixelSize / 64.0f) * 1.1f, (this->unitPixelSize / 64.0f) * 1.1f);
    size = sf::Vector2i(192, 64);

    sf::Sprite sScore(this->scoreTexture, sf::IntRect(sf::Vector2i(0, 0), size));
    sf::Sprite sBest(this->scoreTexture, sf::IntRect(sf::Vector2i(0, 64), size));

    sScore.setScale(scaleVect);
    sBest.setScale(scaleVect);

    sScore.setPosition(sf::Vector2f(0.5f * this->unitPixelSize, 1 * this->unitPixelSize));
    sBest.setPosition(sf::Vector2f(0.5f * this->unitPixelSize, 3 * this->unitPixelSize));

    this->DrawNumber(sScore.getPosition() + sf::Vector2f(192.0f * (this->unitPixelSize / 64.0f), 0.0f), this->score);
    this->DrawNumber(sBest.getPosition() + sf::Vector2f(192.0f * (this->unitPixelSize / 64.0f), 0.0f), this->bestScore);

    this->window.draw(sScore);
    this->window.draw(sBest);

    return true;
};
bool Game::ShowInGameScore(){
    sf::Sprite sScore(this->scoreTexture, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(192, 64)));
    sScore.setScale(sf::Vector2f(this->unitPixelSize / 64.0f, this->unitPixelSize / 64.0f));
    sScore.setPosition(sf::Vector2f(1.0f * this->unitPixelSize, 12.0f * this->unitPixelSize));
    this->DrawNumber(sf::Vector2f(6.0f * this->unitPixelSize, 12.0f * this->unitPixelSize),this->score);
    this->window.draw(sScore);
    return true;
};
bool Game::DrawNumber(sf::Vector2f position,int number){
    if(number>=0 && number<=999)
    {
        int n = 0;
        if(number <= 999)
            n =3 ;
        if(number <= 99)
            n = 2;
        if(number <= 9)
            n = 1;
        for(int i = 0; i < n; i++)
        {
            int val=number%10;
            sf::Sprite numberSprite(this->numbersTexture, sf::IntRect(sf::Vector2i(val * 64, 0), sf::Vector2i(64, 64)));
            numberSprite.scale(this->unitPixelSize / 64.0f, this->unitPixelSize / 64.0f);
            numberSprite.setPosition(position + sf::Vector2f((n - i - 1) * this->unitPixelSize / 1.5f, 0));
            this->window.draw(numberSprite);
            number/=10;
        }
        return true;
    }
    return false;
};
bool Game::Exit(){
    this->window.clear();
    this->window.close();
    return true;
};
/*
    Sterge obiectele instantiate;
*/
Game::~Game(){
    if(this->snake!=NULL)
        delete this->snake;
    if(this->fruit!=NULL)
        delete this->fruit;
    if(this->map!=NULL)
        delete this->map;
    std::cout<<"Sesiune terminata\n";
};




/*
    Clasa Input preia datele de la tastatura sau introduse prin cod;
*/
MyInput::MyInput(){};

void MyInput::GetInput()
{
    bool pressedUp, pressedDown, pressedLeft, pressedRight, pressedEsc, pressedEnter, pressedIncrement;
    pressedDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    pressedLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    pressedRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    pressedUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    pressedIncrement = sf::Keyboard::isKeyPressed(sf::Keyboard::I);
    pressedEnter = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
    pressedEsc = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
    onceUp = false;
    if(!up && pressedUp)
        onceUp = true;
    onceIncrement = false;
    if(!increment && pressedIncrement)
        onceIncrement = true;
    onceEnter = false;
    if(!enter && pressedEnter)
        onceEnter = true;
    onceDown = false;
    if(!down && pressedDown)
        onceDown = true;
    onceEsc = false;
    if(!esc && pressedEsc)
        onceEsc = true;
    onceLeft = false;
    if(!left && pressedLeft)
        onceLeft = true;
    onceRight = false;
    if(!right && pressedRight)
        onceRight = true;
    increment = pressedIncrement;
    up = pressedUp;
    down = pressedDown;
    left = pressedLeft;
    right = pressedRight;
    enter = pressedEnter;
    esc = pressedEsc;;
};

void MyInput::SetUp(bool val){
    this->up = val;
};
void MyInput::SetDown(bool val){
    this->down = val;
};
void MyInput::SetLeft(bool val){
    this->left = val;
};
void MyInput::SetRight(bool val){
    this->right = val;
};
void MyInput::SetEnter(bool val){
    this->enter = val;
};
void MyInput::SetEsc(bool val){
    this->esc = val;
};
MyInput::~MyInput(){
    //nimic
};