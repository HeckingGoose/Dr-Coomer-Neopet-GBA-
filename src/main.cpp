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
// Backgrounds
// WAV Audio
#include "bn_sound_items.h"

int main()
{
    // Begin
    bn::core::init();

    bn::sound_items::hellogordan.play();

    // Main loop
    while(true)
    {
        // Update window
        bn::core::update();
    }
}
