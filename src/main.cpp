// Butano includes
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_palette_ptr.h"

// Extra includes
#include "LargeSprite.h"
#include "HandyConstants.h"
#include "Coomer.h"
#include "TextDisplay.h"

// --Asset Includes--
// Sprites
#include "bn_sprite_items_cross.h"
#include "bn_sprite_items_pistol.h"
#include "bn_sprite_items_pizza.h"
#include "bn_sprite_items_playcoin.h"
#include "bn_sprite_items_soda.h"
#include "bn_sprite_items_shop.h"
#include "bn_sprite_items_shop_pressed.h"
#include "bn_sprite_items_buttona.h"
#include "bn_sprite_items_buttona_pressed.h"
#include "bn_sprite_items_buttonb.h"
#include "bn_sprite_items_buttonb_pressed.h"
// Backgrounds
#include "bn_regular_bg_items_back.h"
// WAV Audio
#include "bn_sound_items.h"

// Const
const short MAIN_ABUTTON_POS_X = 30;
const short MAIN_ABUTTON_POS_Y = 64;
const short MAIN_SHOP_POS_X = -85;
const short MAIN_SHOP_POS_Y = 30;
const short MAIN_B_OFFSET = 25;
const unsigned short COOMER_MOUTH_TIMER = 7;
const unsigned short FRAMERATE = 60;
const unsigned short TIME_PER_DECREMENT = FRAMERATE * 2;
const unsigned short DECREMENT_STATS_BY = 2;
const short STATS_ROOT_X = -115;
const short STATS_ROOT_Y = -50;
const unsigned short DEFAULT_STAT = 100;
const unsigned short LOW_STAT = 20;

// Var
bool _aPressed = false;
bool _bPressed = false;
bool _thirstyAvailable = true;
unsigned long _ticker;

// GAME VALUES
short _hungerValue = DEFAULT_STAT;
short _thirstValue = DEFAULT_STAT;
short _happinessValue = DEFAULT_STAT;
short _playcoinsValue = 0;

// Methods
void Run_Main()
{
    // Start by initialising this scene
    bool goingToShop = false;

    // Initialise background image
    bn::regular_bg_ptr background = bn::regular_bg_items::back.create_bg(0, 0);

    // Create our text generator
    bn::sprite_text_generator textGen(common::variable_8x16_sprite_font);

    // Make text displays
    TextDisplay hungerText = TextDisplay(bn::string_view("Hunger: "), _hungerValue, STATS_ROOT_X, STATS_ROOT_Y, &textGen);
    TextDisplay thirstText = TextDisplay(bn::string_view("Thirst: "), _thirstValue, STATS_ROOT_X, STATS_ROOT_Y + 20, &textGen);
    TextDisplay happinessText = TextDisplay(bn::string_view("Happiness: "), _happinessValue, STATS_ROOT_X, STATS_ROOT_Y + 40, &textGen);
    TextDisplay playcoinsText = TextDisplay(bn::string_view("Playcoins: "), _playcoinsValue, STATS_ROOT_X, STATS_ROOT_Y + 60, 0, 20000, &textGen);

    // Initialise Coomer (as closed)
    Coomer coomer = Coomer(
            COOMER_MOUTH_TIMER
        );

    // Play hello gordan welcome audio and pass sound reference to coomer
    bn::sound_handle soundHandle = bn::sound_items::hellogordan.play();
    coomer._soundEffect = &soundHandle;

    // Create A button
    bn::sprite_ptr buttonASprite = bn::sprite_items::buttona.create_sprite(MAIN_ABUTTON_POS_X, MAIN_ABUTTON_POS_Y);

    // Create B button
    bn::sprite_ptr buttonBSprite = bn::sprite_items::buttonb.create_sprite(MAIN_SHOP_POS_X + MAIN_B_OFFSET, MAIN_SHOP_POS_Y);

    // Create shop button
    bn::sprite_ptr shopButtonSprite = bn::sprite_items::shop.create_sprite(MAIN_SHOP_POS_X, MAIN_SHOP_POS_Y);

    // Loop until we are going to shop
    while (!goingToShop)
    {
        // Run Coomer update method
        coomer.update();

        // Check if A has just been pressed
        if (bn::keypad::a_pressed())
        {
            // Increment total playcoin count
            playcoinsText.incrementValue();

            // Swap the 'A' sprite to be held
            buttonASprite = bn::sprite_items::buttona_pressed.create_sprite(MAIN_ABUTTON_POS_X, MAIN_ABUTTON_POS_Y);

            // Log that A has been pressed
            _aPressed = true;
        }
        // Otherwise, if A has just been released
        else if (bn::keypad::a_released() && _aPressed)
        {
            // Swap the 'A' sprite to be released
            buttonASprite = bn::sprite_items::buttona.create_sprite(MAIN_ABUTTON_POS_X, MAIN_ABUTTON_POS_Y);

            // Set A to have been released
            _aPressed = false;
        }

        // Check if b has just been pressed
        if (bn::keypad::b_pressed())
        {
            // Log that b has been pressed
            _bPressed = true;

            // Swap out sprites
            buttonBSprite = bn::sprite_items::buttonb_pressed.create_sprite(MAIN_SHOP_POS_X + MAIN_B_OFFSET, MAIN_SHOP_POS_Y);
            shopButtonSprite = bn::sprite_items::shop_pressed.create_sprite(MAIN_SHOP_POS_X, MAIN_SHOP_POS_Y);

            // Switch to shop at some point
            goingToShop = true;
        }
        // Otherwise if it's just been released
        else if (bn::keypad::b_released() && _bPressed)
        {
            // Set it to false
            _bPressed = false;

            // Swap out sprites
            buttonBSprite = bn::sprite_items::buttonb.create_sprite(MAIN_SHOP_POS_X + MAIN_B_OFFSET, MAIN_SHOP_POS_Y);
            shopButtonSprite = bn::sprite_items::shop.create_sprite(MAIN_SHOP_POS_X, MAIN_SHOP_POS_Y);
        }

        // Check thirst
        if (thirstText.getValue() < LOW_STAT && _thirstyAvailable)
        {
            // Play sound
            soundHandle = bn::sound_items::imthirsty.play();
            coomer._soundEffect = &soundHandle;

            // Mark thirsty as not being available
            _thirstyAvailable = false;
        }
        // If we are above threshold
        else if (thirstText.getValue() > LOW_STAT && !_thirstyAvailable) { _thirstyAvailable = true; }

        // Check for if we should decrement stats
        if (_ticker > TIME_PER_DECREMENT)
        {
            // Then reset the ticker
            _ticker = 0;

            // And decrement stats
            hungerText.decrementValueBy(DECREMENT_STATS_BY);
            thirstText.decrementValueBy(DECREMENT_STATS_BY);
            happinessText.decrementValueBy(DECREMENT_STATS_BY);
        }

        // Backup values
        _hungerValue = hungerText.getValue();
        _thirstValue = thirstText.getValue();
        _happinessValue = happinessText.getValue();
        _playcoinsValue = playcoinsText.getValue();

        // Update window
        bn::core::update();
        
        // Run ticker
        _ticker++;
    }

    // When the variables fall out of scope, butano cleans everything up, so that's nice
}

void Run_Shop()
{
    // Setup shop scene
    bool goingToMain = false;

    // Loop until going to main
    while (!goingToMain)
    {
        // Wait for a B press as part of this test
        if (bn::keypad::b_pressed())
        {
            goingToMain = true;
        }

        // Window update
        bn::core::update();
    }

    // Same cleanup as before
}

int main()
{
    // Begin
    bn::core::init();


    // Main loop
    while(true)
    {
        // Start in main scene
        Run_Main();

        // Then go to shop
        Run_Shop();

        // Loop back to main
    }
}
