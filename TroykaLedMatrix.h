//#ifndef TROYKA_LED_MATRIX_H
//#define TROYKA_LED_MATRIX_H

#include <Arduino.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include "font8x8/font8x8.h"

#define I2C_ADDR_BASE               0b01100000
#define I2C_ADDR_ALT0               0b01100000
#define I2C_ADDR_ALT1               0b01100001
#define I2C_ADDR_ALT2               0b01100010
#define I2C_ADDR_ALT3               0b01100011
#define I2C_ADDR_MASK               0b00000011

#define MATRIX_SIZE_8X8             0b0000
#define MATRIX_SIZE_7X9             0b0001
#define MATRIX_SIZE_6X10            0b0010
#define MATRIX_SIZE_5X11            0b0011
#define MATRIX_SIZE_MASK            0b0011

#define MATRIX_MIN_ROWS             8
#define MATRIX_MAX_ROWS             11
#define MATRIX_MIN_COLS             5
#define MATRIX_MAX_COLS             8

#define AUDIO_GAIN_0DB              0b0000
#define AUDIO_GAIN_3DB              0b0001
#define AUDIO_GAIN_6DB              0b0010
#define AUDIO_GAIN_9DB              0b0011
#define AUDIO_GAIN_12DB             0b0100
#define AUDIO_GAIN_15DB             0b0101
#define AUDIO_GAIN_18DB             0b0110
#define AUDIO_GAIN_M6DB             0b0111
#define AUDIO_GAIN_MASK             0b0111

#define ROW_CURRENT_40MA            0b0000
#define ROW_CURRENT_45MA            0b0001
#define ROW_CURRENT_50MA            0b0010
#define ROW_CURRENT_55MA            0b0011
#define ROW_CURRENT_60MA            0b0100
#define ROW_CURRENT_65MA            0b0101
#define ROW_CURRENT_70MA            0b0110
#define ROW_CURRENT_75MA            0b0111
#define ROW_CURRENT_05MA            0b1000
#define ROW_CURRENT_10MA            0b1001
#define ROW_CURRENT_15MA            0b1010
#define ROW_CURRENT_20MA            0b1011
#define ROW_CURRENT_25MA            0b1100
#define ROW_CURRENT_30MA            0b1101
#define ROW_CURRENT_35MA            0b1110
#define ROW_CURRENT_XXMA            0b1111
#define ROW_CURRENT_MASK            0b1111

#define REG_ADDR_CONFIGURATION      0x00
#define REG_ADDR_COLUMN_DATA        0x01
#define REG_ADDR_UPDATE_COLUMN      0x0C
#define REG_ADDR_LIGHTING_EFFECT    0x0D
#define REG_ADDR_AUDIO_EQUALIZER    0x0F

#define BIT_CONFIG_SSD              7
#define BIT_CONFIG_AUDIO_EN         2
#define BIT_CONFIG_ADM              0

#define BIT_EFFECT_AUDIO_GAIN       4
#define BIT_EFFECT_ROW_CURRENT      0

#define BIT_AUDIO_EQ_EN             6

class IS32FL3728
{
public:
    IS32FL3728();
    void clear();
    void clearPixel(const uint8_t x, const uint8_t y);
    void drawPixel(const uint8_t x, const uint8_t y);
    void setFont(const uint8_t* font, const uint8_t countChars = 1, const uint8_t countRaws = 8);
    void drawCharacter(const uint8_t c);
    void drawBitmap(const uint8_t* data, const uint8_t countRaws = 8);
    uint8_t getRow(const uint8_t y);
private:
    uint8_t *_font;
    uint8_t _fontSize;
    uint8_t _fontHeight;
    uint8_t _data[MATRIX_MAX_ROWS];
};

class TroykaLedMatrix
{
public:
    TroykaLedMatrix();
    TroykaLedMatrix(const uint8_t addr);
    void begin();
    void enableDisplay();
    void disableDisplay();
    void enableAudioInput();
    void disableAudioInput();
    void getMetrics(uint8_t &width, uint8_t &height);
    void setMatrixSize(const uint8_t value);
    void updateDisplay();
    void setAudioGain(const uint8_t value);
    void setCurrentLimit(const uint8_t value);
    void enableEqualizer();
    void disableEqualizer();
    IS32FL3728 &frameBuffer() { return _frameBuffer; }
    // функции вызова класса IS32FL3728
    void clear();
    void setFont(const uint8_t* font, const uint8_t countChars = 1, const uint8_t countRaws = 8);
    void clearPixel(const uint8_t x, const uint8_t y);
    void drawPixel(const uint8_t x, const uint8_t y);
    void drawCharacter(const uint8_t c);
    void drawBitmap(const uint8_t* data, const uint8_t countRaws = 8);
    uint8_t getRow(const uint8_t y);
private:
    uint8_t readReg(const uint8_t addr);
    void writeReg(const uint8_t addr, const uint8_t data);
    uint8_t makeConfigReg();
    uint8_t makeEffectReg();
    uint8_t _addr;
    bool _shutDown;
    bool _audioInput;
    uint8_t _matrixSize;
    uint8_t _audioInputGain;
    uint8_t _currentLimit;
    bool _audioEqualizer;
    IS32FL3728 _frameBuffer;
};

//#endif //TROYKA_LED_MATRIX
