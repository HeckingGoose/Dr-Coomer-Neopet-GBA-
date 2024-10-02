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

void Coomer::swapState()
{
    // Cache closed and open mouth pos
    bn::fixed_point closedTemp = _closedSprite.getPosition();
    bn::fixed_point openTemp = _openSprite.getPosition();

    // Swap positions
    _closedSprite.setPosition(openTemp);
    _openSprite.setPosition(closedTemp);

    // Swap bool
    _mouthState = !_mouthState;
}

void Coomer::update()
{
    // Do stuff

    // Check if audio is playing
    if (_soundEffect != 0 && _soundEffect->active())
        {
        // Increment timer by 1 frame
        _mouthTimer++;

        // Check if mouthTimer has reached its destination
        if (_mouthTimer > _mouthTimerMax)
        {
            // Reset timer
            _mouthTimer = 0;

            // Swap sprites
            swapState();
        }
    }
    else
    {
        // Set mouth to closed and close mouth, if mouth is open
        if (_mouthState)
        {
            swapState();
        }
    }
}