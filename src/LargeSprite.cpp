// Includes
#include "LargeSprite.h"

// LargeSprite class implementation
// Constructor
LargeSprite::LargeSprite(
    bn::fixed_point position,
    unsigned char spriteWidth,
    bn::sprite_ptr TL,
    bn::sprite_ptr TR,
    bn::sprite_ptr BL,
    bn::sprite_ptr BR
    ) : _spriteWidth(spriteWidth),
    _position(position),
    _sprite_TL(TL),
    _sprite_TR(TR),
    _sprite_BL(BL),
    _sprite_BR(BR)
{
    // Correct sprite positions
    setPosition(_position);
}

// Methods
void LargeSprite::setPosition(bn::fixed_point position)
{
    // Cache half of sprite width
    unsigned char spriteWidthHalf = _spriteWidth / 2;

    // Set position variable
    _position = position;

    // Push position to get to bottom right of sprite
    position -= bn::fixed_point(spriteWidthHalf, spriteWidthHalf);

    // Set top left sprite position
    _sprite_TL.set_position(position);

    // Offset pos to right
    position.set_x(position.x() + _spriteWidth);

    // Set top right sprite position
    _sprite_TR.set_position(position);

    // Offset pos down
    position.set_y(position.y() + _spriteWidth);

    // Set bottom right sprite position
    _sprite_BR.set_position(position);

    // Offset pos to left
    position.set_x(position.x() - _spriteWidth);

    // Set bottom left sprite position
    _sprite_BL.set_position(position);
}

bn::fixed_point LargeSprite::getPosition()
{
    // Return position directly
    return _position;
}

void LargeSprite::setSprites(
    bn::sprite_ptr TL,
    bn::sprite_ptr TR,
    bn::sprite_ptr BL,
    bn::sprite_ptr BR
    )
{
    // Pass in sprites directly
    _sprite_TL = TL;
    _sprite_TR = TR;
    _sprite_BL = BL;
    _sprite_BR = BR;
}

void LargeSprite::setSpriteWidth(unsigned char spriteWidth)
{
    // Pass in width directly
    _spriteWidth = spriteWidth;
}