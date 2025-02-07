#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

// Includes
#include "bn_core.h"
#include "bn_string.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_fixed_32x64_font.h"
#include "bn_sprite_items_variable_8x16_font_white.h"
#include "bn_sprite_text_generator.h"
#include "common_fixed_8x8_sprite_font.h"
#include "common_fixed_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

// Class for managing a text display with a stored unsigned value
class TextDisplay
{
    // Public members
    public:

    // Constructor
    TextDisplay(
        bn::string_view displayText,
        short value,
        short x,
        short y,
        short minValue,
        short maxValue,
        bn::sprite_text_generator* textGenHandle
    );
    TextDisplay(
        bn::string_view displayText,
        short value,
        short x,
        short y,
        bn::sprite_text_generator* textGenHandle
    );

    // Methods
    void decrementValue();
    void decrementValueBy(short value);
    void incrementValue();
    void incrementValueBy(short value);
    short getValue();
    void setPosition(short x, short y);
    bn::fixed_point getPosition();

    // Private members
    private:

    // Variables
    bn::string_view _displayText;
    short _value;
    short _minValue;
    short _maxValue;
    bn::vector<bn::sprite_ptr, 32> _textStorage;
    bn::fixed_point _position;
    bn::sprite_text_generator* _textGenHandle;

    // Methods
    void generateDisplay();
    void init(bn::string_view displayText, short value, short x, short y, short minValue, short maxValue, bn::sprite_text_generator* textGenHandle);
};

#endif