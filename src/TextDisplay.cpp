// Includes
#include "TextDisplay.h"

// Const
short DEFAULT_MIN = 0;
short DEFAULT_MAX = 100;

// Implementations
// -- Constructors --
TextDisplay::TextDisplay(
    bn::string_view displayText,
    short value,
    short x,
    short y,
    short minValue,
    short maxValue,
    bn::sprite_text_generator* textGenHandle
    )
    {
        init(
            displayText,
            value,
            x,
            y,
            minValue,
            maxValue,
            textGenHandle
        );
    }
TextDisplay::TextDisplay(
    bn::string_view displayText,
    short value,
    short x,
    short y,
    bn::sprite_text_generator* textGenHandle
    )
    {
        init(
            displayText,
            value,
            x,
            y,
            DEFAULT_MIN,
            DEFAULT_MAX,
            textGenHandle
        );
    }
// -- Constructor Helper --
void TextDisplay::init(
    bn::string_view displayText,
    short value,
    short x,
    short y,
    short minValue,
    short maxValue,
    bn::sprite_text_generator* textGenHandle
    )
    {
        // Pass in values
        _displayText = displayText;
        _value = value;
        _minValue = minValue;
        _maxValue = maxValue;
        _position = bn::fixed_point(x, y);
        _textGenHandle = textGenHandle;

        // Set text gen alignment
        _textGenHandle->set_left_alignment();

        // Make text
        _textGenHandle->generate(_position, "DEFAULT", _textStorage);

        // Generate display text
        generateDisplay();
    }

// Methods
// -- Public --
void TextDisplay::decrementValue()
    {
        // Decrement
        _value--;

        // Check max
        if (_value < _minValue)
        {
            _value = _minValue;
        }

        // Update display
        generateDisplay();
    }
void TextDisplay::decrementValueBy(short value)
    {
        // Decrement by
        _value -= value;

        // Check max
        if (_value < _minValue)
        {
            _value = _minValue;
        }

        // Update display
        generateDisplay();
    }
void TextDisplay::incrementValue()
    {
        // Increment
        _value++;

        // Check max
        if (_value > _maxValue)
        {
            _value = _maxValue;
        }

        // Update display
        generateDisplay();
    }
void TextDisplay::incrementValueBy(short value)
    {
        // Increment by
        _value += value;

        // Check max
        if (_value > _maxValue)
        {
            _value = _maxValue;
        }

        // Update display
        generateDisplay();
    }
short TextDisplay::getValue()
    {
        // Return value
        return _value;
    }
void TextDisplay::setPosition(short x, short y)
    {
        // Create and set position
        _position = bn::fixed_point(x, y);
    }
bn::fixed_point TextDisplay::getPosition()
    {
        // Return position
        return _position;
    }

// -- Private --
void TextDisplay::generateDisplay()
    {
        // Clear out the current text display
        _textStorage.clear();

        // Now let's generate the text :)
        _textGenHandle->generate(_position, _displayText + bn::to_string<20, short>(_value), _textStorage);
    }