#include "GAME.h"
Game::Game(int pixels, char* file):pixelsPerUnit(pixels){
    ///Initializeaza variabile
    ///Citeste din fisier daca exista un best score
    pixelsPerUnit /= 12;
    strcpy(this->saveFile, file);
    std::fstream saveDataFile = std::fstream(file, std::ios::in | std::ios::out | std::ios::app);
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

bool Game::LoadTextures(char* tMap, char* tSnake, char* tFruit, char* tMenu, char* tScore, char* tNumbers){
    ///Incarca texturile din memorie
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

bool Game::Start(){
    ///Aceasta metoda da start la aplicatia in sine
    ///Aici se proceseaza inputurile pentru meniuri, ce se afiseaza,
    ///ce se prelucreaza si ce se updateaza
    this->buttonSelected = 0;
    this->inStartingMenu = true;
    this->inGame = false;
    this->inPauseMenu = false;
    this->inGameOverMenu = false;

    this->window.create(sf::VideoMode(this->pixelsPerUnit*12,this->pixelsPerUnit*13),"SNAKE GAME");

    MyInput input;

    while (this->window.isOpen())
    {
        input.GetInput();

        sf::Event event;
        while (this->window.pollEvent(event))
        {
            ///"close requested" event: we close the window;
            if (event.type == sf::Event::Closed)
                this->window.close();
        }

        ///Reseteaza ecranul, il face negru
        this->window.clear(sf::Color::Black);

        ///Afiseaza meniurile
        if(this->inGameOverMenu)
            this->ShowGameOverMenu();
        if(this->inPauseMenu)
            this->ShowPauseMenu();
        if(this->inStartingMenu)
            this->ShowMainMenu();
        ///Proceseaza input-urile pentru meniuri
        if(input.onceEsc)
            this->PauseGame();
        if(this->inPauseMenu || this->inStartingMenu || this->inGameOverMenu)
        {
            ///Mareste FPS-urile pentru o sensibilitate a controlului mai buna
            this->window.setFramerateLimit(60);
            if(this->inPauseMenu)
            {
                ///preia inputul pentru selectarea butoanelor (3)
                /// 0 - Continue
                /// 1 - Restart
                /// 2 - Menu
                if(input.onceDown && buttonSelected<2)
                    this->buttonSelected += 1;
                if(input.onceUp && buttonSelected>0)
                    this->buttonSelected -= 1;
            }
            if(this->inStartingMenu || this->inGameOverMenu)
            {
                ///preia inputul pentru selectarea butoanelor (2)
                /// 0 - Restart
                /// 1 - Menu
                if(input.onceDown)
                    this->buttonSelected = 1;
                if(input.onceUp)
                    this->buttonSelected = 0;
            }
            ///Proceseaza meniul
            if(input.onceEnter == true)
            {
                ///Play sau continue
                if(this->buttonSelected == 0){
                    if(this->inStartingMenu)
                        ///Butonul de Play
                        this->StartGame();
                    else
                        if(this->inPauseMenu)
                            ///Butonul de continue
                            this->ContinueGame();
                        else 
                            ///Butonul de restart
                            this->RestartGame();
                }
                if(this->buttonSelected == 1){
                    if(this->inStartingMenu)
                        ///Butonul exit
                        this->Exit();
                    else
                        if(this->inPauseMenu)
                            ///Butonul de restart
                            this->RestartGame();
                        else
                            ///Butonul de menu
                            this->ReturnToMenu();
                }
                if(this->buttonSelected == 2)
                    ///Butonul de menu
                    this->ReturnToMenu();
            }
        }

        ///Updateaza jocul daca ruleaza si nu este in meniuri
        if(this->inGame && !this->inStartingMenu && !this->inPauseMenu && !this->inGameOverMenu)
        {
            ///Limiteaza FPS-urile pentru a rula jocul la o viteza normala
            this->window.setFramerateLimit(3);

            ///Daca returneaza fals, inseamna ca jocul sa terminat
            ///In caz contrar jocul continua
            if(!this->UpdateGame())
                this->GameOver();
        }

        ///Termina frame-ul curent
        this->window.display();
    }
    return true;
};

bool Game::UpdateGame(){
    ///Culege input-ul
    MyInput input;
    input.GetInput();

    ///Proceseaza input-ul
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

    ///Verifica daca noua directie este eligibila
    if(newDirection != sf::Vector2f(0.0f,0.0f) && this->direction+newDirection != sf::Vector2f(0.0f,0.0f))
    {
        this->direction = newDirection;
    }

    ///Misca sarpele si updateaza mapa si afisarea sarpelui
    if(this->direction != sf::Vector2f(0.0f,0.0f))
    {
        ///Verificare daca sarpele iese din dimenisunea matricei
        this->snake->MoveSnake(this->direction);

        ///Verifica daca au fost ciocniri, daca da, atungi GameOver
        if(!this->map->UpdateMap(*(this->snake)))
            return false;
        this->map->UpdateMap(*(this->fruit));
    }

    ///Buton special pentru incrementarea sarpelui
    if(input.onceIncrement && input.onceEnter)
    {
        if(!this->snake->IncreaseSnake())
            return false;
        else
            this->score++;
    }

    ///Verifica daca sarpele a mancat fructul
    if(this->fruit->GetPosition() == this->snake->GetSnakeBodyPosition(0))
    {
        ///Verifica daca dimensiunea sarpelui este maxima
        ///Daca da, jocul s-a terminat
        ///In caz contrar, mareste scorul si lungimea sarpelui
        if(!this->snake->IncreaseSnake())
            return false;
        else
            this->score++;

        ///Verifica daca mai este spatiu pe mapa
        ///pentru a genera o noua pozitie, daca nu mai
        ///este spatiu atunci jocul s-a terminat
        this->map->UpdateMap(*(this->snake));
        if(this->map->IsFull())
            return false;
        this->fruit->UpdatePosition(this->map->RandomFreePosition());
        this->map->UpdateMap(*(this->fruit));
    }

    ///Afiseaza elementele grafice ale jocului
    this->ShowInGameScore();
    this->map->DrawMap(this->window);
    this->fruit->DrawFruit(this->window, pixelsPerUnit);
    this->snake->DrawSnake(this->window, pixelsPerUnit);

    ///Returneaza true pentru a continua updatarea jocului
    return true;
};

bool Game::ContinueGame(){
    this->inPauseMenu = false;
    return !this->inPauseMenu;
};

bool Game::RestartGame(){
    ///Reseteaza Jocul
    this->inGame = false;
    return this->StartGame();
};

bool Game::ReturnToMenu(){
    ///Seteaza aplicatia pe Meniul principal
    this->buttonSelected = 0;
    this->inPauseMenu = false;
    this->inGameOverMenu = false;
    this->inGame = false;
    this->inStartingMenu = true;
    return true;
};

bool Game::PauseGame(){
    ///Seteaza aplicatia pe Pauza, daca este permisa operatia
     if(this->inStartingMenu || this->inGameOverMenu)
        return false;
    this->buttonSelected = 0;
    this->inPauseMenu = true;
    return true;
};

bool Game::StartGame()
{
    ///Seteaza starea aplicatiei pe Joc
    this->inStartingMenu = false;
    this->inPauseMenu = false;
    this->inGameOverMenu = false;
    this->inGame = true;

    ///Reseteaza scorul
    this->score = 0;

    ///Sterge obiectele instantiate anterior, daca exista
    if(this->snake!=NULL)
        delete this->snake;
    if(this->fruit!=NULL)
        delete this->fruit;
    if(this->map!=NULL)
        delete this->map;

    ///Genereaza noi elemente: mapa, fruct,sarpe
    this->map = new Map(this->mapTexture, this->pixelsPerUnit);
    this->fruit = new Fruit(this->fruitTexture, this->pixelsPerUnit);
    this->snake = new Snake(this->snakeTexture);

    ///Updateaza matricea mapei
    this->map->UpdateMap(*(this->snake));
    this->fruit->UpdatePosition(this->map->RandomFreePosition());
    this->map->UpdateMap(*(this->fruit));

    ///Seteaza directia de mers in jos
    this->direction = sf::Vector2f(0.0f,1.0f);

    return true;
};

bool Game::GameOver(){
    ///Seteaza aplicatia pe GameOver
    this->inGame = false;
    this->inPauseMenu = false;
    this->inStartingMenu = false;
    this->inGameOverMenu = true;

    ///Daca noul scor este mai mare decat cel mai bun scor
    ///atunci seteaza si salveaza noul scor ca fiind Best
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

bool Game::ShowMainMenu(){
    ///Afiseaza elementele corespunzatoare Meniului principal
    ///Seteaza texturile, pozitia si dimensiunile corespunzatoare
    ///butonului selectat
    float scaleFactor = (this->pixelsPerUnit * 2.0f) / (float)yMSize;
    sf::Vector2f scaleVect = sf::Vector2f(scaleFactor, scaleFactor);
    sf::Vector2f scaleSelectedVect = sf::Vector2f(1.25f * scaleFactor, 1.25f * scaleFactor);
    sf::Vector2i size(xMSize, yMSize);

    sf::Sprite sPlay(this->menuTexture, sf::IntRect(sf::Vector2i(0, mPlay * yMSize), size));
    sf::Sprite sExit(this->menuTexture, sf::IntRect(sf::Vector2i(0, mExit * yMSize), size));

    sPlay.setScale(this->buttonSelected == 0 ? scaleSelectedVect:scaleVect);
    sExit.setScale(this->buttonSelected == 1 ? scaleSelectedVect:scaleVect);

    sPlay.setPosition(sf::Vector2f(3 * this->pixelsPerUnit, 3 * this->pixelsPerUnit));
    sExit.setPosition(sf::Vector2f(3 * this->pixelsPerUnit, 6 * this->pixelsPerUnit));

    this->window.draw(sPlay);
    this->window.draw(sExit);

    return true;
};

bool Game::ShowPauseMenu(){
    ///Afiseaza elementele corespunzatoare meniului de pauza
    ///Seteaza texturile, pozitia si dimensiunile corespunzatoare
    ///butonului selectat
    float scaleFactor = (this->pixelsPerUnit * 2.0f) / (float)yMSize;
    sf::Vector2f scaleVect=sf::Vector2f(scaleFactor, scaleFactor);
    sf::Vector2f scaleSelectedVect=sf::Vector2f(1.15f * scaleFactor, 1.15f * scaleFactor);
    sf::Vector2i size(xMSize, yMSize);

    sf::Sprite sContinue(this->menuTexture, sf::IntRect(sf::Vector2i(0, mContinue * yMSize), size));
    sf::Sprite sRestart(this->menuTexture, sf::IntRect(sf::Vector2i(0, mRestart * yMSize), size));
    sf::Sprite sMenu(this->menuTexture, sf::IntRect(sf::Vector2i(0, mMenu * yMSize), size));

    sContinue.setScale(this->buttonSelected == 0 ? scaleSelectedVect:scaleVect);
    sRestart.setScale(this->buttonSelected == 1 ? scaleSelectedVect:scaleVect);
    sMenu.setScale(this->buttonSelected == 2 ? scaleSelectedVect:scaleVect);

    sContinue.setPosition(sf::Vector2f(0.5f * this->pixelsPerUnit, 3 * this->pixelsPerUnit));
    sRestart.setPosition(sf::Vector2f(0.5f * this->pixelsPerUnit, 6 * this->pixelsPerUnit));
    sMenu.setPosition(sf::Vector2f(0.5f * this->pixelsPerUnit, 9 * this->pixelsPerUnit));

    this->window.draw(sContinue);
    this->window.draw(sRestart);
    this->window.draw(sMenu);

    return true;
};

bool Game::ShowGameOverMenu(){
    ///Afiseaza elementele corespunzatoare Mmeniului de GameOver
    ///Seteaza texturile, pozitia si dimensiunile corespunzatoare
    ///butonului selectat
    float scaleFactor = (this->pixelsPerUnit * 2.0f) / (float)yMSize;
    sf::Vector2f scaleVect = sf::Vector2f(scaleFactor, scaleFactor);
    sf::Vector2f scaleSelectedVect = sf::Vector2f(1.15f * scaleFactor, 1.15f * scaleFactor);
    sf::Vector2i size(xMSize, yMSize);

    sf::Sprite sRestart(this->menuTexture, sf::IntRect(sf::Vector2i(0, mRestart * yMSize), size));
    sf::Sprite sMenu(this->menuTexture, sf::IntRect(sf::Vector2i(0, mMenu * yMSize), size));

    sRestart.setScale(this->buttonSelected == 0 ? scaleSelectedVect:scaleVect);
    sMenu.setScale(this->buttonSelected == 1 ? scaleSelectedVect:scaleVect);

    sRestart.setPosition(sf::Vector2f(0.5f * this->pixelsPerUnit, 6 * this->pixelsPerUnit));
    sMenu.setPosition(sf::Vector2f(0.5f * this->pixelsPerUnit, 9 * this->pixelsPerUnit));

    this->window.draw(sRestart);
    this->window.draw(sMenu);

    ///Afiseaza noul scor si cel mai bun scor
    ///Seteaza texturile, pozitia si dimensiunile corespunzatoare
    scaleVect = sf::Vector2f((this->pixelsPerUnit / 64.0f) * 1.1f, (this->pixelsPerUnit / 64.0f) * 1.1f);
    size = sf::Vector2i(192, 64);

    sf::Sprite sScore(this->scoreTexture, sf::IntRect(sf::Vector2i(0, 0), size));
    sf::Sprite sBest(this->scoreTexture, sf::IntRect(sf::Vector2i(0, 64), size));

    sScore.setScale(scaleVect);
    sBest.setScale(scaleVect);

    sScore.setPosition(sf::Vector2f(0.5f * this->pixelsPerUnit, 1 * this->pixelsPerUnit));
    sBest.setPosition(sf::Vector2f(0.5f * this->pixelsPerUnit, 3 * this->pixelsPerUnit));

    this->DrawNumber(sScore.getPosition() + sf::Vector2f(192.0f * (this->pixelsPerUnit / 64.0f), 0.0f), this->score);
    this->DrawNumber(sBest.getPosition() + sf::Vector2f(192.0f * (this->pixelsPerUnit / 64.0f), 0.0f), this->bestScore);

    this->window.draw(sScore);
    this->window.draw(sBest);

    return true;
};

bool Game::ShowInGameScore(){
    ///Afiseaza scorul in timpul jocului
    sf::Sprite sScore(this->scoreTexture, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(192, 64)));
    sScore.setScale(sf::Vector2f(this->pixelsPerUnit / 64.0f, this->pixelsPerUnit / 64.0f));
    sScore.setPosition(sf::Vector2f(1.0f * this->pixelsPerUnit, 12.0f * this->pixelsPerUnit));

    ///Afiseaza valoarea scorului
    this->DrawNumber(sf::Vector2f(6.0f * this->pixelsPerUnit, 12.0f * this->pixelsPerUnit),this->score);

    this->window.draw(sScore);
    return true;
};

bool Game::DrawNumber(sf::Vector2f position,int number){
    ///Afiseaza numarul daca se incadreaza in limitele corespunzatoare
    if(number>=0 && number<=999)
    {
        ///In n salvam lungimea numarului
        int n = 0;
        if(number <= 999)
            n = 3;
        if(number <= 99)
            n = 2;
        if(number <= 9)
            n = 1;
        for(int i = 0; i < n; i++)
        {
            ///Datorita faptului ca afisam numarul de la dreapta la stanga
            ///trebuie ca sa afisam imaginea numarului corespunzator, la un offset
            ///fata de pozitia pe ecran de la care incepe afisarea
            int val=number%10;
            sf::Sprite numberSprite(this->numbersTexture, sf::IntRect(sf::Vector2i(val * 64, 0), sf::Vector2i(64, 64)));
            numberSprite.scale(this->pixelsPerUnit / 64.0f, this->pixelsPerUnit / 64.0f);
            numberSprite.setPosition(position + sf::Vector2f((n - i - 1) * this->pixelsPerUnit / 1.5f, 0));
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

Game::~Game(){
    ///Sterge obiectele jocului cand se inchide aplicatia
    if(this->snake!=NULL)
        delete this->snake;
    if(this->fruit!=NULL)
        delete this->fruit;
    if(this->map!=NULL)
        delete this->map;
    std::cout<<"Sesiune terminata\n";
};

///Implementarea clasei pentru input
MyInput::MyInput(){};

void MyInput::GetInput()
{
    ///Variabile locale care preiau inputul
    ///true - cand sunt apasate
    ///false - cand nu sunt apasate
    bool pressedUp, pressedDown, pressedLeft, pressedRight, pressedEsc, pressedEnter, pressedIncrement;
    pressedDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    pressedLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    pressedRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    pressedUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    pressedIncrement = sf::Keyboard::isKeyPressed(sf::Keyboard::I);
    pressedEnter = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
    pressedEsc = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    ///Prelucrarea inputurilor astfel incat in
    ///variabilele "once[var]" se salveaza true doar in primul
    ///frame cand este tastatura apasata, si poate deveni true doar
    ///cand tastatura este ridicata si apasata
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

    ///Variabile care sunt true cat timp tastatura este apasata
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
MyInput::~MyInput(){};