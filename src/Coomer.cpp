// Includes
#include "Coomer.h"

// Const
const unsigned char COOMER_SPRITEWIDTH = 64;
const short COOMER_POS_X = 30;
const short COOMER_POS_Y = -10;

// Implementations
Coomer::Coomer(
    unsigned int mouthTimerMax
    ) : 
    _sprite(LargeSprite(
            bn::fixed_point(COOMER_POS_X, COOMER_POS_Y),
            COOMER_SPRITEWIDTH,
            bn::sprite_items::coomerclosed_tl.create_sprite(0,0),
            bn::sprite_items::coomerclosed_tr.create_sprite(0,0),
            bn::sprite_items::coomerclosed_bl.create_sprite(0,0),
            bn::sprite_items::coomerclosed_br.create_sprite(0,0)
            ))
{
    // Set current sound handle to be null
    _soundEffect = NULL;

    // Set mouth max timer
    _mouthTimerMax = mouthTimerMax;

    // Set mouth to be closed
    _mouthState = false;
}

void Coomer::swapState()
{
    // Swap bool
    _mouthState = !_mouthState;

    // What state is the mouth in?
    switch (_mouthState)
    {
        // Mouth is open
        case true:
            // Replace sprite
            _sprite = LargeSprite(
                bn::fixed_point(COOMER_POS_X, COOMER_POS_Y),
                COOMER_SPRITEWIDTH,
                bn::sprite_items::coomeropen_tl.create_sprite(0,0),
                bn::sprite_items::coomeropen_tr.create_sprite(0,0),
                bn::sprite_items::coomeropen_bl.create_sprite(0,0),
                bn::sprite_items::coomeropen_br.create_sprite(0,0)
                );
            break;
        // Mouth is closed
        default:
            // Replace sprite
            _sprite = LargeSprite(
                bn::fixed_point(COOMER_POS_X, COOMER_POS_Y),
                COOMER_SPRITEWIDTH,
                bn::sprite_items::coomerclosed_tl.create_sprite(0,0),
                bn::sprite_items::coomerclosed_tr.create_sprite(0,0),
                bn::sprite_items::coomerclosed_bl.create_sprite(0,0),
                bn::sprite_items::coomerclosed_br.create_sprite(0,0)
                );
            break;
    }
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