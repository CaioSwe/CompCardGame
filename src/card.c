#include "card.h"

#include <animation.h>

typedef struct CardStr {
    Texture2D img;
    Animation anim;

    Rectangle rect;
    Rectangle rectOriginal;
    
    bool hovered;
    bool attached;

    float rotation;

    Color cor;
} CardStr;

