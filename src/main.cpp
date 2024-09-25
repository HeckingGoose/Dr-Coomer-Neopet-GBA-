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
// Backgrounds
#include "bn_regular_bg_items_back.h"
// WAV Audio
#include "bn_sound_items.h"

// Constants
const unsigned char SCREEN_WIDTH = 240;
const unsigned char SCREEN_HEIGHT = 160;
const unsigned char COOMER_SPRITEWIDTH = 64;
const unsigned char COOMER_SPRITEWIDTH_HALF = COOMER_SPRITEWIDTH / 2;

int main()
{
    // Begin
    bn::core::init();

    // Initialise background image
    bn::regular_bg_ptr background = bn::regular_bg_items::back.create_bg(0, 0);

    // Initialise Coomer (as closed)
    LargeSprite coomer = LargeSprite(
        bn::fixed_point(0,0),
        COOMER_SPRITEWIDTH,
        bn::sprite_items::coomerclosed_tl.create_sprite(0,0),
        bn::sprite_items::coomerclosed_tr.create_sprite(0,0),
        bn::sprite_items::coomerclosed_bl.create_sprite(0,0),
        bn::sprite_items::coomerclosed_br.create_sprite(0,0)
        );


    // Play hello gordan welcome audio
    bn::sound_handle helloGordan = bn::sound_items::hellogordan.play();

    // helloGordan.active() <- true when playing (i think)

    // Main loop
    while(true)
    {
        // Update window
        bn::core::update();
    }
}
