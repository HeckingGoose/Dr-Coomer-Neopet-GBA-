// Includes
#include "Coomer.h"

// Implementations
Coomer::Coomer(
    LargeSprite sprite
    ) : _sprite(sprite)
{
    // Set current sound handle to be null
    _soundEffect = nullptr;
}