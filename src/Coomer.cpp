// Includes
#include "Coomer.h"

// Implementations
Coomer::Coomer(
    LargeSprite sprite
    ) : _sprite(sprite)
{
    // Set current sound handle to be null
    _soundEffect = NULL;
}

void Coomer::setSoundEffect(bn::sound_handle handle)
{
    // Set value directly
    _soundEffect = &handle;
}

void Coomer::update()
{
    // Do stuff
}