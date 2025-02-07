#ifndef COOMER_H
#define COOMER_H

// Includes
#include "bn_core.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_coomerclosed_tl.h"
#include "bn_sprite_items_coomerclosed_tr.h"
#include "bn_sprite_items_coomerclosed_bl.h"
#include "bn_sprite_items_coomerclosed_br.h"
#include "bn_sprite_items_coomeropen_tl.h"
#include "bn_sprite_items_coomeropen_tr.h"
#include "bn_sprite_items_coomeropen_bl.h"
#include "bn_sprite_items_coomeropen_br.h"
#include "LargeSprite.h"
#include "HandyConstants.h"

// Coomer class definition
class Coomer
{
    // Public Members
    public:
    // Methods
    Coomer(
        unsigned int mouthTimerMax
    );
    bn::sound_handle* _soundEffect;
    void update();

    // Private Members
    private:
    // Variables
    LargeSprite _sprite;
    bool _mouthState;
    unsigned int _mouthTimer;
    unsigned int _mouthTimerMax;

    // Methods
    void swapState();
};

#endif