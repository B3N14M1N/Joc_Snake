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

int main(int argc, char *argv[])
{
    ScreenSize size=ScreenSize::big;
    if(argc > 1)
    {
        if(strcmp(argv[1],"small")==0)
            size=ScreenSize::small;
        if(strcmp(argv[1],"medium")==0)
            size=ScreenSize::medium;
        if(strcmp(argv[1],"big")==0)
            size=ScreenSize::big;
    }
    try{
        Game game(size,(char*)fSave); //12 blocuri * X pixeli pe unitate
        game.LoadTextures((char*)tMAP,(char*)tSNAKE,(char*)tFRUIT,(char*)tMenu, (char*)tScore, (char*)tNumbers);
        game.Start();
    }
    catch(Exception exception){
        exception.Print();
    }
    return 0;
}