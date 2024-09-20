// Main includes
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_palette_ptr.h"

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

int main()
{
    // Begin
    bn::core::init();

    // Main loop
    while(true)
    {
        // Update window
        bn::core::update();
    }
}
