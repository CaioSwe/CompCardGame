#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <time.h>

#include <animatedButton.h>
#include <imageObject.h>
#include <textObject.h>
#include <lista.h>
#include <fileHandler.h>
#include <utils.h>

#include "card.h"

#pragma region "FuncoesUteis"



#pragma endregion "FuncoesUteis"

int main(){
    // Inicializa a tela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Teste");
    SetTargetFPS(60);
    srand(time(NULL));

    // Pega o caminho da aplicação (para usar nos paths futuros)
    _chdir(GetApplicationDirectory());

    // Tamanho de cada carta
    float width = 120;
    float height = 200;
    
    // Número de cartas a serem geradas
    // int numCards = 8;

    // Caminho do direto'rio dos sprites das cartas
    char* path = "./sprites/cards/";

    // Abre o path para leitura dos nomes das cartas
    DIR* dir = Directory_Open(path);
    
    FilesInfo finfo = Directory_ReadFiles(dir);
    char** nomes = Directory_GetFileNames(finfo);
    int nFiles = Directory_GetNumFiles(finfo);

    Texture2D* cardSprites = (Texture2D*)malloc(sizeof(Texture2D) * nFiles);

    // Carrega a textura das cartas
    for(int i = 0; i < 5; i++){
        cardSprites[i] = LoadTexture(TextFormat("%s%s", path, nomes[i]));
        printf("\nCard Loaded: %s", nomes[i]);
    }

    // Fecha o path de leitura
    Directory_FreeFilesInfo(finfo);
    Directory_Close(dir);
    
    // Coloca a carta no centro da tela
    float centerX = CENTER.x - width/2;
    float centerY = CENTER.y - height/2;
    Rectangle centerCardPos = (Rectangle){centerX, centerY, width, height};
    
    // Inicializa uma carta
    Card c1 = Card_Init(centerCardPos, cardSprites[0]);
    Card_SetScaleRatio(c1, 0.5f);

    // Tela
    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousepos = GetMousePosition();
        
        if(IsKeyPressed(KEY_SPACE)) Card_MoveTo(c1, mousepos, 0.5f);
        
        if(Card_isGrabbed(c1, IsMouseButtonDown(MOUSE_BUTTON_LEFT))) Card_Maximize(c1, 0.5f);
        else Card_Minimize(c1, 0.5f);
        
        Card_Update(c1, deltaTime);
        
        BeginDrawing();
            ClearBackground(BLACK);

            Card_Draw(c1);
        EndDrawing();
    }

    return 0;
}

/*

        percorrerLista(listaAllCards, atualizarCarta);

        percorrerLista(stack, freeStoreLista);
        limparLista(stack);

        percorrerLista(stack2, freeStoreLista2);
        limparLista(stack2);

        if(Button_IsPressed(refresh, mousepos)){
            RefreshAllCards();
        }

        BeginDrawing();
            ClearBackground(BLACK);

            Image_Draw(background);

            Button_Draw(refresh, WHITE, WHITE, WHITE, RED);

            DrawText(TextFormat("%d", listaTamanho(listaCardsDealed)), 0, 0, 20, WHITE);
            DrawText(TextFormat("%d", listaTamanho(listaCardsStored)), 0, 25, 20, WHITE);

            DrawRectangleLinesEx(handZone, 10.0f, (Color){100, 100, 100, 30});
            imprimirLista(listaAllCards, imprimirCard);

            DrawRectangleRec(deckPos, WHITE);
        EndDrawing();
    }

    limparLista(listaAllCards);

    return 0;
}

*/