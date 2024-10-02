// Includes
#include "Coomer.h"

// Implementations
Coomer::Coomer(
    LargeSprite closedSprite,
    LargeSprite openSprite,
    unsigned int mouthTimerMax
    ) : 
    _closedSprite(closedSprite), 
    _openSprite(openSprite)
{
    // Set current sound handle to be null
    _soundEffect = NULL;

    // Set mouth max timer
    _mouthTimerMax = mouthTimerMax;

    // Force open sprite off screen
    _openSprite.setPosition(bn::fixed_point(-300, -300));
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