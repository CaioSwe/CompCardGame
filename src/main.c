#pragma region "Structs"

#include <animatedButton.h>
#include <imageObject.h>
#include <textObject.h>
#include <lista.h>
#include <fileHandler.h>

#include <utils.h>

#include "card.h"

#pragma endregion "Structs"

Lista listaAllCards;
Lista listaCardsStored;
Lista listaCardsBox;
Lista listaCardsDealed;

Lista stack;
Lista stack2;

#pragma region "FuncoesUteis"

bool compararCard(const void* item, const void* comparar){
    const Card* card = (const Card*)item;
    const Card* comp = (const Card*)comparar;

    if(card == comp){
        return true;
    }
    return false;
}

void throwAwayCard(const void* item){
    Card* card = (Card*)item;

    inserirInicio(stack2, card);
}

void RefreshAllCards(){
    percorrerLista(listaCardsDealed, runExtra, throwAwayCard);
}

void DealCards(Lista listaCardsBox, int amount){
    static float i = 0;

    for(int j = 0; j <= amount; j++){
        if(!isListaVazia(listaCardsBox)){
            Card card = removerInicio(listaCardsBox);
            inserirInicio(listaCardsDealed, card);

            float totalCards = listaTamanho(listaCardsBox) + listaTamanho(listaCardsDealed);

            float next = i + (1.0f/(float)totalCards);

            float cardwidth = Card_GetWidth(card);
            float screenwidth = GetScreenWidth();

            float targetX = lerp(screenwidth*1/4 - cardwidth/2, screenwidth*3/4 + cardwidth/2, (i + next) / 2);
            float targetY = GetScreenHeight()/3;

            Card_MoveTo(card, (Vector2){targetX, targetY}, 1.0f);
            
            i = next;
            if(i > 1.0f) i = 1.0f;
        }
        else i = 0;
    }
}

void StoreCards(Rectangle storeArea, Card* card, bool insert){
    Vector2 centerArea = (Vector2){storeArea.x + storeArea.width/2, storeArea.y + storeArea.height/2};

    Vector2 startPoint = (Vector2){centerArea.x - storeArea.width*3/8, centerArea.y};

    Vector2 endPoint = (Vector2){centerArea.x + storeArea.width*3/8, centerArea.y};

    if(insert){
        inserirFim(listaCardsStored, card);
        CardMinimize(card);
    }
    else{
        remover(listaCardsStored, compararCard, card);
    }

    int tam = listaTamanho(listaCardsStored);
    for (int k = 0; k < tam; k++) {
        float t = (((float)k / (float)tam) + (((float)k+1) / (float)tam))/2;
        float targetX = Slerp(startPoint.x, endPoint.x, t);
        float targetY = centerArea.y;

        Card* c = (Card*)getItemLista(listaCardsStored, k);
        MoveCardTo(c, (Vector2){targetX, targetY}, 0.5f);
    }
}

void atualizarCarta(Card card, float deltaTime){
    Card_UpdateAnim(card, deltaTime, true);
    Card_UpdateSize(card, deltaTime);
    
    Card_Move(card, true);

    Rectangle handZone = {GetScreenWidth()/2 - GetScreenWidth()*0.8/2, GetScreenHeight() - GetScreenHeight()/4, GetScreenWidth()*0.8, GetScreenHeight()/4};

    bool cardGrabbed = Card_isGrabbed(card);
    bool cardStored = isInLista(listaCardsStored, compararCard, card);

    if(CheckCollisionRecs(Card_GetRect(card), handZone)){
        if(cardGrabbed){
            Card_Wiggle(card);
        }
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !cardStored){
            remover(listaCardsDealed, compararCard, card);
            StoreCards(handZone, card, true);
            printf("\n | Entered");
        }
    }
    else{
        Card_SetRotation(card, 0.0f);
    }

    if(cardGrabbed && cardStored){
        inserirInicio(stack, card);
    }
}

void freeStoreLista(const void* item){
    Card* card = (Card*)item;

    Rectangle handZone = {GetScreenWidth()/2 - GetScreenWidth()*0.8/2, GetScreenHeight() - GetScreenHeight()/4, GetScreenWidth()*0.8, GetScreenHeight()/4};

    inserirInicio(listaCardsDealed, card);
    StoreCards(handZone, card, false);
    printf("\n | Exited");
}

void freeStoreLista2(const void* item){
    Card* card = (Card*)item;

    Rectangle deckPos = {GetScreenWidth()*1/8, GetScreenWidth()*1/8, card->rect.width, card->rect.height};

    inserirInicio(listaCardsBox, card);

    if(isInLista(listaCardsDealed, compararCard, card)){
        remover(listaCardsDealed, compararCard, card);
    }

    CardMaximize(card);
    MoveCardTo(card, (Vector2){deckPos.x, deckPos.y}, 0.5f);
    
    printf("\n | Exited");
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
    int numCards = 8;

    stack = criaLista();
    stack2 = criaLista();
    listaAllCards = criaLista();
    listaCardsBox = criaLista();
    listaCardsStored = criaLista();
    listaCardsDealed = criaLista();

    Rectangle handZone = {GetScreenWidth()/2 - GetScreenWidth()*0.8/2, GetScreenHeight() - GetScreenHeight()/4, GetScreenWidth()*0.8, GetScreenHeight()/4};
    Rectangle deckPos = {GetScreenWidth()*1/8 - width/2, GetScreenWidth()*1/8 - height/2, width, height}; 

    char* path = "./sprites/cards/";

    // Abre o path para leitura dos nomes das cartas
    DIR* dir = openDirectory(path);
    char** nomes = readFileNames(dir);

    Texture2D* cardSprites = (Texture2D*)malloc(sizeof(Texture2D) * 5);

    // Carrega a textura das cartas
    for(int i = 0; i < 5; i++){
        cardSprites[i] = LoadTexture(TextFormat("%s%s", path, nomes[i]));
        printf("\nCard Loaded: %s", nomes[i]);
    }

    // Fecha o path de leitura
    closeDirectory(dir);

    // Gera cartas aleatórias a partir das cartas carregadas
    for(int i = 0; i < numCards; i++){
        int randomCard = (rand() % 5);

        Card* card = Card_Init(deckPos, cardSprites[randomCard]);
        inserirInicio(listaAllCards, card);
        inserirInicio(listaCardsBox, card);

        printf("\nCard Generated [%d]: %s", i, nomes[randomCard]);
    }

    Vector2 buttonPos = (Vector2){GetScreenWidth()/2, 0.0f};
    AnimatedButton refresh = AnimatedButton_Init("Refresh", 30, buttonPos, BLACK, WHITE, BLACK, WHITE, 0.5f, 1.0f);

    ImageObject background = Image_Init("sprites/background.png");
    
    Image_FitToScreenSize(background);
    Image_SetColor(background, (Color){255, 255, 255, 60});

    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousepos = GetMousePosition();
        
        if(CheckCollisionPointRec(mousepos, deckPos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            DealCards(listaCardsBox, listaTamanho(listaCardsBox));
        }

        percorrerLista(listaAllCards, runExtra, atualizarCarta);

        BeginDrawing();
            ClearBackground(BLACK);

            Image_Draw(background);

            Button_Draw(refresh, WHITE, WHITE, WHITE, RED);

            DrawText(TextFormat("%d", listaTamanho(listaCardsDealed)), 0, 0, 20, WHITE);
            DrawText(TextFormat("%d", listaTamanho(listaCardsStored)), 0, 25, 20, WHITE);

            DrawRectangleLinesEx(handZone, 10.0f, (Color){100, 100, 100, 30});
            imprimirLista(listaAllCards, runExtra, Card_Draw);

            DrawRectangleRec(deckPos, WHITE);
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