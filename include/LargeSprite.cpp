// Includes
#include "LargeSprite.h"

// LargeSprite class implementation
// Constructor


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