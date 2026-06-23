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

#include <raymath.h>

#include "card.h"

#pragma region "FuncoesUteis"

typedef struct Essentials{
    float deltaTime;
    Vector2 mousePos;
    Vector2 mouseDelta;

    int* grabbedCardId;
    int* maxId;
} Essentials;

static void percorrerCartas(Card c, Item extra){
    Essentials* e = (Essentials*)extra;

    int maxId = *e->maxId;
    int grabbedCardId = *e->grabbedCardId;
    int id = Card_GetId(c);

    bool mbDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if(!mbDown && grabbedCardId == id) grabbedCardId = -1;
    
    bool hovered = Card_isHovered(c, e->mousePos);
    bool canGrab = (grabbedCardId == -1 || grabbedCardId == id) && (maxId == -1 || maxId == id);
    bool isDragging = Card_isGrabbed(c, mbDown) && canGrab;
    
    if(isDragging){
        grabbedCardId = id;
        Card_Move(c, e->mouseDelta);
        Card_Maximize(c, 0.3f);
    }
    else if(canGrab && hovered){
        maxId = id;
        Card_Medianize(c, 0.3f);
    }
    else Card_Minimize(c, 0.3f);

    Card_Update(c, e->deltaTime);

    float rotation = Card_GetRotation(c);
    
    float max_card_rotation = 25.5f;
    float targetRotation = Clamp((Vector2Subtract(Card_GetPosition(c), Card_GetLastPosition(c))).x * 1.2f, -max_card_rotation, max_card_rotation);
    
    rotation = lerp(rotation, targetRotation, 12.0f * e->deltaTime);
    Card_SetRotation(c, rotation);

    Card_UpdateLastPosition(c);

    *e->maxId = maxId;
    *e->grabbedCardId = grabbedCardId;
}

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
    
    Rectangle c2Pos = (Rectangle){centerX + 200, centerY, width, height};

    // Inicializa uma carta
    Card c1 = Card_Init(centerCardPos, cardSprites[0]);
    Card_SetScaleRatio(c1, 0.80f);

    Card c2 = Card_Init(c2Pos, cardSprites[1]);
    Card_SetScaleRatio(c2, 0.80f);

    Lista cardsList = criaLista();

    inserirFim(cardsList, c1);
    inserirFim(cardsList, c2);

    int grabbedCardId = -1;
    int lastCardId = -1;
    int maxId = -1;

    // Tela
    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();

        Vector2 mousepos = GetMousePosition();
        Vector2 mouseDelta = GetMouseDelta();

        Essentials* e = &(Essentials){deltaTime, mousepos, mouseDelta, &grabbedCardId, &maxId};
        maxId = -1;

        percorrerListaReverso(cardsList, percorrerCartas, e);

        if(grabbedCardId != lastCardId && grabbedCardId != -1){
            lastCardId = grabbedCardId;

            Card c = remover(cardsList, compararCartas, &grabbedCardId);
            inserirFim(cardsList, c);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            percorrerLista(cardsList, runExtra, Card_Draw);
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