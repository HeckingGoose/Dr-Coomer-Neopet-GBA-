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
#include "bn_sprite_items_buy.h"
#include "bn_sprite_items_buy_highlighted.h"
#include "bn_sprite_items_buy_pressed.h"
#include "bn_sprite_items_back.h"
#include "bn_sprite_items_back_pressed.h"
#include "bn_sprite_items_buttona.h"
#include "bn_sprite_items_buttona_pressed.h"
#include "bn_sprite_items_buttonb.h"
#include "bn_sprite_items_buttonb_pressed.h"
// Backgrounds
#include "bn_regular_bg_items_background.h"
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
const unsigned short EXIT_TIMER_GOAL = FRAMERATE * 0.15;

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

    // Reset values
    _aPressed = false;
    _bPressed = false;

    // Initialise background image
    bn::regular_bg_ptr background = bn::regular_bg_items::background.create_bg(0, 0);

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

    // Declare exit timer
    bool exitTimerRunning = false;
    unsigned short exitTicker = 0;

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
            exitTimerRunning = true;
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

        // Check if we are exitting
        if (exitTimerRunning)
        {
            // If we are

            // Are we ready to leave?
            if (exitTicker > EXIT_TIMER_GOAL)
            {
                // Then exit
                goingToShop = true;
            }

            // Increment ticker
            exitTicker++;
        }
    }

    // When the variables fall out of scope, butano cleans everything up, so that's nice
}

// Shop Const
const short SHOPITEMS_POS_ROOT_X = -80;
const short SHOPITEMS_POS_ROOT_Y = 0;
const short SHOPITEMS_POS_Y_GAP = 45;
const short SHOPITEMS_POS_TITLE_X_OFFSET = 12;
const short SHOPITEMS_POS_TITLE_Y_OFFSET = 20;
const short SHOPITEMS_POS_STAT_X_OFFSET = 20;
const short SHOPITEMS_POS_STAT_Y_GAP = 5;
const short BACKBUTTON_POS_X = 75;
const short BACKBUTTON_POS_Y = 65;
const short BACKBUTTON_POS_B_OFFSET = 25;
const short MENUBUTTONS_COUNT = 3;
const short BUYBUTTON_OFFSET = 120;
const short PLAYCOINCOST_PIZZA = 5;
const short PLAYCOINCOST_SODA = 5;
const short PLAYCOINCOST_GUN = 5;
const short PLAYCOIN_TEXT_X = -100;
const short PLAYCOIN_TEXT_Y = 68;
const short STATGAIN_PIZZA = 15;
const short STATGAIN_SODA = 15;
const short STATGAIN_GUN = 15;

void Run_Shop()
{
    // Setup shop scene
    bool goingToMain = false;

    // Reset values
    _aPressed = false;
    _bPressed = false;

    // Declare which menu option is currently highlighted
    short currentMenuOption = 0;

    // Declare exit timer
    bool exitTimerRunning = false;
    unsigned short exitTicker = 0;

    // Create shopjects
    bn::sprite_ptr pizzaSprite = bn::sprite_items::pizza.create_sprite(SHOPITEMS_POS_ROOT_X, SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP);
    bn::sprite_ptr sodaSprite = bn::sprite_items::soda.create_sprite(SHOPITEMS_POS_ROOT_X, SHOPITEMS_POS_ROOT_Y);
    bn::sprite_ptr gunSprite = bn::sprite_items::pistol.create_sprite(SHOPITEMS_POS_ROOT_X, SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP);

    // Create back button
    bn::sprite_ptr backSprite = bn::sprite_items::back.create_sprite(BACKBUTTON_POS_X, BACKBUTTON_POS_Y);
    bn::sprite_ptr bSprite = bn::sprite_items::buttonb.create_sprite(BACKBUTTON_POS_X + BACKBUTTON_POS_B_OFFSET, BACKBUTTON_POS_Y);

    // Create menu buttons
    bn::sprite_ptr buyPizza = bn::sprite_items::buy.create_sprite(
        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
        );
    bn::sprite_ptr buySoda = bn::sprite_items::buy.create_sprite(
        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
        SHOPITEMS_POS_ROOT_Y
        );
    bn::sprite_ptr buyGun = bn::sprite_items::buy.create_sprite(
        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
        );
    bn::sprite_ptr buttonA = bn::sprite_items::buttona.create_sprite(
        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
        );

    // Which one is currently selected?
    switch (currentMenuOption)
    {
        // Pizza
        case 0:
            // Replace
            buyPizza = bn::sprite_items::buy_highlighted.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                );
            buttonA = bn::sprite_items::buttona.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                );
            break;
        // Soda
        case 1:
            // Replace
            buySoda = bn::sprite_items::buy_highlighted.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y
                );
            buttonA = bn::sprite_items::buttona.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                );
            break;
        // Gun
        default:
            // Replace
            buyGun = bn::sprite_items::buy_highlighted.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                );
            buttonA = bn::sprite_items::buttona.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                );
            break;
    }

    // Make text generator
    bn::sprite_text_generator textGen(common::variable_8x16_sprite_font);
    textGen.set_left_alignment();

    // Create playcoins text
    bn::vector<bn::sprite_ptr, 32> playcoinStorage;
    textGen.generate(
        PLAYCOIN_TEXT_X,
        PLAYCOIN_TEXT_Y,
        bn::string_view("Playcoins: ") + bn::to_string<20, short>(_playcoinsValue),
        playcoinStorage
        );

    // Create titles
    bn::vector<bn::sprite_ptr, 32> titleStorage;
    textGen.generate(
        SHOPITEMS_POS_ROOT_X - SHOPITEMS_POS_TITLE_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_TITLE_Y_OFFSET - SHOPITEMS_POS_Y_GAP,
        "Pizza",
        titleStorage
        );
    textGen.generate(
        SHOPITEMS_POS_ROOT_X - SHOPITEMS_POS_TITLE_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_TITLE_Y_OFFSET,
        "Soda",
        titleStorage
        );
    textGen.generate(
        SHOPITEMS_POS_ROOT_X - SHOPITEMS_POS_TITLE_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_TITLE_Y_OFFSET + SHOPITEMS_POS_Y_GAP,
        "Kill a man",
        titleStorage
        );

    // Make text generator
    bn::sprite_text_generator textGen_sub(common::variable_8x8_sprite_font);
    textGen_sub.set_left_alignment();

    // Make stat descriptions
    bn::vector<bn::sprite_ptr, 64> statStorage;
    textGen_sub.generate(
        SHOPITEMS_POS_ROOT_X + SHOPITEMS_POS_STAT_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP - SHOPITEMS_POS_STAT_Y_GAP,
        "+15 Hunger",
        statStorage
        );
    textGen_sub.generate(
        SHOPITEMS_POS_ROOT_X + SHOPITEMS_POS_STAT_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP + SHOPITEMS_POS_STAT_Y_GAP,
        "-5 Playcoins",
        statStorage
        );
    textGen_sub.generate(
        SHOPITEMS_POS_ROOT_X + SHOPITEMS_POS_STAT_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_STAT_Y_GAP,
        "+15 Thirst",
        statStorage
        );
    textGen_sub.generate(
        SHOPITEMS_POS_ROOT_X + SHOPITEMS_POS_STAT_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_STAT_Y_GAP,
       "-5 Playcoins",
        statStorage
        );
    textGen_sub.generate(
        SHOPITEMS_POS_ROOT_X + SHOPITEMS_POS_STAT_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP - SHOPITEMS_POS_STAT_Y_GAP,
        "+15 Happiness",
        statStorage
        );
    textGen_sub.generate(
        SHOPITEMS_POS_ROOT_X + SHOPITEMS_POS_STAT_X_OFFSET,
        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP + SHOPITEMS_POS_STAT_Y_GAP,
        "-5 Playcoins",
        statStorage
        );

    // Loop until going to main
    while (!goingToMain)
    {
        // Check if a has been pressed
        if (bn::keypad::a_pressed())
        {
            // Log it
            _aPressed = true;

            // What do we have selected?
            switch (currentMenuOption)
            {
                // Pizza
                case 0:
                    // Replace
                    buyPizza = bn::sprite_items::buy_pressed.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona_pressed.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );

                    // Do we have enough playcoins?
                    if (_playcoinsValue >= PLAYCOINCOST_PIZZA)
                    {
                        // Then buy it
                        _playcoinsValue -= PLAYCOINCOST_PIZZA;

                        // Increment happiness
                        _hungerValue += STATGAIN_PIZZA;

                        // Update text
                        playcoinStorage.clear();
                        textGen.generate(
                            PLAYCOIN_TEXT_X,
                            PLAYCOIN_TEXT_Y,
                            bn::string_view("Playcoins: ") + bn::to_string<20, short>(_playcoinsValue),
                            playcoinStorage
                            );
                    }
                    break;
                // Soda
                case 1:
                    // Replace
                    buySoda = bn::sprite_items::buy_pressed.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    buttonA = bn::sprite_items::buttona_pressed.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    
                    // Do we have enough playcoins?
                    if (_playcoinsValue >= PLAYCOINCOST_SODA)
                    {
                        // Then buy it
                        _playcoinsValue -= PLAYCOINCOST_SODA;

                        // Increment happiness
                        _thirstValue += STATGAIN_SODA;

                        // Update text
                        playcoinStorage.clear();
                        textGen.generate(
                            PLAYCOIN_TEXT_X,
                            PLAYCOIN_TEXT_Y,
                            bn::string_view("Playcoins: ") + bn::to_string<20, short>(_playcoinsValue),
                            playcoinStorage
                            );
                    }
                    break;
                // Gun
                default:
                    // Replace
                    buyGun = bn::sprite_items::buy_pressed.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona_pressed.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );

                    // Do we have enough playcoins?
                    if (_playcoinsValue >= PLAYCOINCOST_GUN)
                    {
                        // Then buy it
                        _playcoinsValue -= PLAYCOINCOST_GUN;

                        // Increment happiness
                        _happinessValue += STATGAIN_GUN;

                        // Update text
                        playcoinStorage.clear();
                        textGen.generate(
                            PLAYCOIN_TEXT_X,
                            PLAYCOIN_TEXT_Y,
                            bn::string_view("Playcoins: ") + bn::to_string<20, short>(_playcoinsValue),
                            playcoinStorage
                            );
                    }
                    break;
            }
        }
        else if (bn::keypad::a_released() && _aPressed)
        {
            // Set it to false
            _aPressed = false;

            // Which one is currently selected?
            switch (currentMenuOption)
            {
                // Pizza
                case 0:
                    // Replace
                    buyPizza = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );
                    break;
                // Soda
                case 1:
                    // Replace
                    buySoda = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    break;
                // Gun
                default:
                    // Replace
                    buyGun = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );
                    break;
            }
        }

        // Check if b has just been pressed
        if (bn::keypad::b_pressed())
        {
            // Log that b has been pressed
            _bPressed = true;

            // Swap out sprites
            bSprite = bn::sprite_items::buttonb_pressed.create_sprite(BACKBUTTON_POS_X + BACKBUTTON_POS_B_OFFSET, BACKBUTTON_POS_Y);
            backSprite = bn::sprite_items::back_pressed.create_sprite(BACKBUTTON_POS_X, BACKBUTTON_POS_Y);

            // Switch to shop at some point
            exitTimerRunning = true;
        }
        // Otherwise if it's just been released
        else if (bn::keypad::b_released() && _bPressed)
        {
            // Set it to false
            _bPressed = false;

            // Swap out sprites
            bSprite = bn::sprite_items::buttonb.create_sprite(BACKBUTTON_POS_X + BACKBUTTON_POS_B_OFFSET, BACKBUTTON_POS_Y);
            backSprite = bn::sprite_items::back.create_sprite(BACKBUTTON_POS_X, BACKBUTTON_POS_Y);
        }

        // Check for up pressed (prevent while a held)
        if (bn::keypad::up_pressed() && !bn::keypad::a_held())
        {
            // Then decrement the option
            currentMenuOption--;

            // Check bounds
            if (currentMenuOption < 0)
            {
                // Wrap
                currentMenuOption = MENUBUTTONS_COUNT - 1;
            }

            // Update
            buyPizza = bn::sprite_items::buy.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                );
            buySoda = bn::sprite_items::buy.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y
                );
            buyGun = bn::sprite_items::buy.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                );

            // Which one is currently selected?
            switch (currentMenuOption)
            {
                // Pizza
                case 0:
                    // Replace
                    buyPizza = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );
                    break;
                // Soda
                case 1:
                    // Replace
                    buySoda = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    break;
                // Gun
                default:
                    // Replace
                    buyGun = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );
                    break;
            }
        }
        // (Also prevent while a held)
        else if (bn::keypad::down_pressed() && !bn::keypad::a_held())
        {
            // Then increment the option
            currentMenuOption++;

            // Check bounds
            if (currentMenuOption >= MENUBUTTONS_COUNT)
            {
                // Wrap
                currentMenuOption = 0;
            }

            // Update
            buyPizza = bn::sprite_items::buy.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                );
            buySoda = bn::sprite_items::buy.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y
                );
            buyGun = bn::sprite_items::buy.create_sprite(
                SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                );

            // Which one is currently selected?
            switch (currentMenuOption)
            {
                // Pizza
                case 0:
                    // Replace
                    buyPizza = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y - SHOPITEMS_POS_Y_GAP
                        );
                    break;
                // Soda
                case 1:
                    // Replace
                    buySoda = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y
                        );
                    break;
                // Gun
                default:
                    // Replace
                    buyGun = bn::sprite_items::buy_highlighted.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );
                    buttonA = bn::sprite_items::buttona.create_sprite(
                        SHOPITEMS_POS_ROOT_X + BUYBUTTON_OFFSET + BACKBUTTON_POS_B_OFFSET,
                        SHOPITEMS_POS_ROOT_Y + SHOPITEMS_POS_Y_GAP
                        );
                    break;
            }
        }

        // Window update
        bn::core::update();

        // Are we exitting?
        if (exitTimerRunning)
        {
            // We are

            // Are we ready to exit?
            if (exitTicker > EXIT_TIMER_GOAL)
            {
                // GTFO
                goingToMain = true;
            }

            // Increment ticker
            exitTicker++;
        }
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
