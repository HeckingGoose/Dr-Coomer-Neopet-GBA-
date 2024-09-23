#ifndef LARGESPRITE_H
#define LARGESPRITE_H

// Includes
#include "bn_core.h"
#include "bn_sprite_ptr.h"

// LargeSprite Class definition
class LargeSprite
{
    // Public members
    public:
    LargeSprite(
        bn::fixed_point position,
        unsigned char spriteWidth,
        bn::sprite_ptr TL,
        bn::sprite_ptr TR,
        bn::sprite_ptr BL,
        bn::sprite_ptr BR
        );
    bn::fixed_point getPosition();
    void setPosition(bn::fixed_point position);
    void setSprites(
        bn::sprite_ptr TL,
        bn::sprite_ptr TR,
        bn::sprite_ptr BL,
        bn::sprite_ptr BR
        );
    void setSpriteWidth(
        unsigned char spriteWidth
        );

    // Private members
    private:
    unsigned char _spriteWidth;
    bn::fixed_point _position;
    bn::sprite_ptr _sprite_TL;
    bn::sprite_ptr _sprite_TR;
    bn::sprite_ptr _sprite_BL;
    bn::sprite_ptr _sprite_BR;

};

#endif