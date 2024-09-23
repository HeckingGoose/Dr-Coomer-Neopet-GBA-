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
    bn::fixed_point getPosition();
    void setPosition(bn::fixed_point);

    // Private members
    private:
    bn::fixed_point _position;
    bn::sprite_ptr _sprite_TL;
    bn::sprite_ptr _sprite_TR;
    bn::sprite_ptr _sprite_BL;
    bn::sprite_ptr _sprite_BR;

};

#endif