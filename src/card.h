#ifndef __Card_H__
#define __Card_H__

#include <raylib.h>
#include <stdbool.h>

typedef void* Card;

/**
 * @brief Inicializa uma estrutura de Card (carta).
 * @param rect Retangulo de dimensoes para a carta.
 * @param txr Textura ja' carregada na memo'ria para referenciacao na carta.
 * @return Retorna um ponteiro para a estrutura de carta criada.
 */
Card Card_Init(Rectangle rect, Texture2D txr);

Rectangle Card_GetRect(Card card);

int Card_GetId(Card card);

bool Card_isHovered(Card card, Vector2 point);

bool Card_isGrabbed(Card card, bool isDown);

void Card_SetRotation(Card card, float rotation);

void Card_Move(Card card, Vector2 delta);

void Card_MoveTo(Card card, Vector2 finalPoint, float duration);

void Card_Wiggle(Card card, float deltaTime);

void Card_SetScaleRatio(Card card, float scaleRatio);

void Card_Minimize(Card card, float duration);

void Card_Maximize(Card card, float duration);

void Card_Update(Card card, float deltaTime);

void Card_Draw(Card card);

Texture2D Card_free(Card card);

#endif