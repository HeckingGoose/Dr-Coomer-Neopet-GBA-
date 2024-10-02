#ifndef COOMER_H
#define COOMER_H

// Includes
#include "bn_core.h"
#include "bn_sound_items.h"
#include "LargeSprite.h"

// Coomer class definition
class Coomer
{
    // Public Members
    public:
    // Methods
    Coomer(
        LargeSprite closedSprite,
        LargeSprite openSprite,
        unsigned int mouthTimerMax
    );
    bn::sound_handle* _soundEffect;
    void update();

    // Private Members
    private:
    // Variables
    LargeSprite _closedSprite;
    LargeSprite _openSprite;
    bool _mouthState;
    unsigned int _mouthTimer;
    unsigned int _mouthTimerMax;

    // Methods
    void swapState();
};

#endif