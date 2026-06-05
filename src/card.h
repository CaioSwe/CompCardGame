#ifndef __Card_H__
#define __Card_H__

#include <raylib.h>
#include <stdbool.h>

typedef void* Card;

Card Card_Init(Rectangle rect, Texture2D path);

Rectangle Card_GetRect(Card card);

bool Card_isGrabbed(Card card);

void Card_SetRotation(Card card, float rotation);

void Card_Move(Card card, bool minimizeOnGrab);

void Card_MoveTo(Card card, Vector2 finalPoint, float time);

void Card_Wiggle(Card card);

void Card_Minimize(Card card);

void Card_Maximize(Card card);

void Card_UpdateAnim(Card card, float deltaTime, bool centerAligned);

void Card_UpdateSize(Card card, float deltaTime);

void Card_Draw(Card card);

void Card_free();

#endif