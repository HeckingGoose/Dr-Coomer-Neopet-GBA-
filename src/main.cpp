// Butano includes
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_palette_ptr.h"

// Extra includes
#include "LargeSprite.h"
#include "HandyConstants.h"
#include "Coomer.h"

// --Asset Includes--
// Sprites
#include "bn_sprite_items_cross.h"
#include "bn_sprite_items_pistol.h"
#include "bn_sprite_items_pizza.h"
#include "bn_sprite_items_playcoin.h"
#include "bn_sprite_items_soda.h"
#include "bn_sprite_items_coomerclosed_tl.h"
#include "bn_sprite_items_coomerclosed_tr.h"
#include "bn_sprite_items_coomerclosed_bl.h"
#include "bn_sprite_items_coomerclosed_br.h"
#include "bn_sprite_items_coomeropen_tl.h"
#include "bn_sprite_items_coomeropen_tr.h"
#include "bn_sprite_items_coomeropen_bl.h"
#include "bn_sprite_items_coomeropen_br.h"
#include "bn_sprite_items_buttona.h"
#include "bn_sprite_items_buttona_pressed.h"
// Backgrounds
#include "bn_regular_bg_items_back.h"
// WAV Audio
#include "bn_sound_items.h"

// Const
const unsigned char COOMER_SPRITEWIDTH = 64;

// Var
unsigned int _playcoins = 0;

int main()
{
    // Begin
    bn::core::init();

    // Initialise background image
    bn::regular_bg_ptr background = bn::regular_bg_items::back.create_bg(0, 0);

    // Initialise Coomer (as closed)
    Coomer coomer = Coomer(
        LargeSprite(
            bn::fixed_point(0,-10),
            COOMER_SPRITEWIDTH,
            bn::sprite_items::coomerclosed_tl.create_sprite(0,0),
            bn::sprite_items::coomerclosed_tr.create_sprite(0,0),
            bn::sprite_items::coomerclosed_bl.create_sprite(0,0),
            bn::sprite_items::coomerclosed_br.create_sprite(0,0)
            ),
        LargeSprite(
            bn::fixed_point(-300, -300),
            COOMER_SPRITEWIDTH,
            bn::sprite_items::coomeropen_tl.create_sprite(0,0),
            bn::sprite_items::coomeropen_tr.create_sprite(0,0),
            bn::sprite_items::coomeropen_bl.create_sprite(0,0),
            bn::sprite_items::coomeropen_br.create_sprite(0,0)
            ),
            7
        );

    // Play hello gordan welcome audio and pass sound reference to coomer
    bn::sound_handle soundHandle = bn::sound_items::hellogordan.play();
    coomer._soundEffect = &soundHandle;

    // Create A button
    bn::sprite_ptr buttonASprite = bn::sprite_items::buttona.create_sprite(0, 50);

    // Main loop
    while(true)
    {
        // Run Coomer update method
        coomer.update();

        // Check if A is held
        if (bn::keypad::a_held())
        {
            // If this is the first frame
            if (bn::keypad::a_pressed())
            {
                // Increment total playcoin count
                _playcoins++;
            }

            // Swap the 'A' sprite to be held
            buttonASprite = bn::sprite_items::buttona_pressed.create_sprite(0, 64);
        }
        // Otherwise
        else
        {
            // Swap the 'A' sprite to be released
            buttonASprite = bn::sprite_items::buttona.create_sprite(0, 64);
        }

        // Update window
        bn::core::update();
    }
}
