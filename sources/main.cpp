#include "GAME.h"

#define tMAP ".//assets/textures/map.png"
#define tSNAKE ".//assets/textures/snake.png"
#define tFRUIT ".//assets/textures/fruit.png"
#define tMenu ".//assets/textures/menu.png"
#define tScore ".//assets/textures/score.png"
#define tNumbers ".//assets/textures/numbers.png"
#define fSave ".//save/score.txt"
enum ScreenSize {
    small=192, // 16 pixeli pe unitate
    medium=384,// 32 pixeli pe unitate
    big=768    // 64 pixeli pe unitate
};

int main()
{
    try{
        Game game(big,(char*)fSave); //12 blocuri * X pixeli pe unitate
        game.LoadTextures((char*)tMAP,(char*)tSNAKE,(char*)tFRUIT,(char*)tMenu, (char*)tScore, (char*)tNumbers);
        game.Start();
    }
    catch(Exception exception){
        exception.Print();
    }
    return 0;
}