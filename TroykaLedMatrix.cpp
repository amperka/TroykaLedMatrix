#include "TroykaLedMatrix.h"

TroykaLedMatrix::TroykaLedMatrix() {
    _addr = I2C_ADDR_BASE | (I2C_ADDR_ALT0 & I2C_ADDR_MASK);
}

TroykaLedMatrix::TroykaLedMatrix(const uint8_t addr) {
    _addr = I2C_ADDR_BASE | (addr & I2C_ADDR_MASK);
}

void TroykaLedMatrix::begin() {
    _shutDown = false;
    _audioInput = false;
    _matrixSize = MATRIX_SIZE_8X8;
    _audioEqualizer = false;
    _audioInputGain = AUDIO_GAIN_0DB;
    _currentLimit = ROW_CURRENT_40MA;
    Wire.begin();
    writeReg(REG_ADDR_CONFIGURATION, makeConfigReg());
    writeReg(REG_ADDR_LIGHTING_EFFECT, makeEffectReg());
    disableEqualizer();
}

uint8_t TroykaLedMatrix::readReg(const uint8_t addr) {
    Wire.beginTransmission(_addr);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.requestFrom(_addr, 1);
    while (!Wire.available());
    uint8_t data = Wire.read();
    return data; 
}

void TroykaLedMatrix::writeReg(const uint8_t addr, const uint8_t data) {
    Wire.beginTransmission(_addr);
    Wire.write(addr);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t TroykaLedMatrix::makeConfigReg() {
    uint8_t data = 0;
    if (_shutDown) {
        data |= _BV(BIT_CONFIG_SSD);
    }
    if (_audioInput) {
        data |= _BV(BIT_CONFIG_AUDIO_EN);
    }
    data |= _matrixSize << BIT_CONFIG_ADM;
    return data;
}

void TroykaLedMatrix::enableDisplay() {
    _shutDown = false;
    uint8_t data = makeConfigReg();
    writeReg(REG_ADDR_CONFIGURATION, data);
}

void TroykaLedMatrix::disableDisplay() {
    _shutDown = true;
    uint8_t data = makeConfigReg();
    writeReg(REG_ADDR_CONFIGURATION, data);
}

void TroykaLedMatrix::enableAudioInput() {
    _audioInput = true;
    uint8_t data = makeConfigReg();
    writeReg(REG_ADDR_CONFIGURATION, data);
}

void TroykaLedMatrix::disableAudioInput() {
    _audioInput = false;
    uint8_t data = makeConfigReg();
    writeReg(REG_ADDR_CONFIGURATION, data);
}

void TroykaLedMatrix::getMetrics(uint8_t &width, uint8_t &height) {
    switch (_matrixSize) {
        case MATRIX_SIZE_8X8: { 
            width = 8;
            height = 8;
            break; 
        }
        case MATRIX_SIZE_7X9: {
            width = 7;
            height = 9;
            break;
        }
        case MATRIX_SIZE_6X10: {
            width = 6;
            height = 10;
            break; 
        }
        case MATRIX_SIZE_5X11: { 
            width = 5;
            height = 11;
            break;
        }
    }
}

void TroykaLedMatrix::setMatrixSize(const uint8_t value) {
    _matrixSize = (value & MATRIX_SIZE_MASK);
    uint8_t data = makeConfigReg();
    writeReg(REG_ADDR_CONFIGURATION, data);
}

void TroykaLedMatrix::updateDisplay() {
    uint8_t w, h;
    getMetrics(w, h);
    for (uint8_t i = 0; i < h; i++) {
        uint8_t data = _frameBuffer.getRow(i);
        writeReg(REG_ADDR_COLUMN_DATA + i, data);
    }
    writeReg(REG_ADDR_UPDATE_COLUMN, 0xff);
}

uint8_t TroykaLedMatrix::makeEffectReg() {
    uint8_t data = (_audioInputGain << BIT_EFFECT_AUDIO_GAIN) |
                   (_currentLimit << BIT_EFFECT_ROW_CURRENT);
    return data;
}

void TroykaLedMatrix::setAudioGain(const uint8_t value) {
    _audioInputGain = value & AUDIO_GAIN_MASK;
    uint8_t data = makeEffectReg();
    writeReg(REG_ADDR_LIGHTING_EFFECT, data);
}

void TroykaLedMatrix::setCurrentLimit(const uint8_t value) {
    _currentLimit = value & ROW_CURRENT_MASK;
    uint8_t data = makeEffectReg();
    writeReg(REG_ADDR_LIGHTING_EFFECT, data);
}

void TroykaLedMatrix::enableEqualizer() {
    _audioEqualizer = true;
    writeReg(REG_ADDR_AUDIO_EQUALIZER, _BV(BIT_AUDIO_EQ_EN));
}

void TroykaLedMatrix::disableEqualizer() {
    _audioEqualizer = false;
    writeReg(REG_ADDR_AUDIO_EQUALIZER, 0);
}

void TroykaLedMatrix::clear() {
    frameBuffer().clear();
    updateDisplay();
}

void TroykaLedMatrix::clearPixel(const uint8_t x, const uint8_t y) {
    frameBuffer().clearPixel(x, y);
    updateDisplay();
}

void TroykaLedMatrix::drawPixel(const uint8_t x, const uint8_t y) {
    frameBuffer().drawPixel(x, y);
    updateDisplay();
}

void TroykaLedMatrix::setFont(const uint8_t* font, const uint8_t countChars = 1, const uint8_t countRaws = 8) {
    frameBuffer().setFont(font, countChars, countRaws);
}

void TroykaLedMatrix::drawCharacter(const uint8_t c) {
    frameBuffer().drawCharacter(c);
    updateDisplay();
}

void TroykaLedMatrix::drawBitmap(const uint8_t* data, const uint8_t countRaws = 8) {
    frameBuffer().drawBitmap(data, countRaws);
    updateDisplay();
}

uint8_t TroykaLedMatrix::getRow(const uint8_t y) {
    frameBuffer().getRow(y);
}

IS32FL3728::IS32FL3728() {
    _font = 0;
    _fontSize = 1;
    _fontHeight = 8;
    clear();
}

void IS32FL3728::clear() {
    for (uint8_t i = 0; i < MATRIX_MAX_ROWS; i++) {
        _data[i] = 0;
    }
}

void IS32FL3728::clearPixel(const uint8_t x, const uint8_t y) {
    uint8_t i = x % 8;
    uint8_t j = y % MATRIX_MAX_ROWS;
    _data[j] = _data[j] & ~_BV(i);
}

void IS32FL3728::drawPixel(const uint8_t x, const uint8_t y) {
    uint8_t i = x % 8;
    uint8_t j = y % MATRIX_MAX_ROWS;
    _data[j] = _data[j] | _BV(i);
}

void IS32FL3728::setFont(const uint8_t* font, const uint8_t countChars, const uint8_t countRaws) {
    _font = font;
    _fontSize = countChars;
    _fontHeight = countRaws;
}

void IS32FL3728::drawCharacter(const uint8_t c) {
    if (_font) {
        if (c < _fontSize) {
            drawBitmap(&_font[c * _fontHeight], _fontHeight);
        }
    }
}

void IS32FL3728::drawBitmap(const uint8_t* data, const uint8_t countRaws) {
    uint8_t n = min(countRaws, MATRIX_MAX_ROWS);
    for (uint8_t i = 0; i < n; i++) {
        _data[i] = data[i];
    }
}

uint8_t IS32FL3728::getRow(const uint8_t y) {
    return _data[y % MATRIX_MAX_ROWS];
}
