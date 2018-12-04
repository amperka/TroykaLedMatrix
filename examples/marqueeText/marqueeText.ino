#include "TroykaLedMatrix.h"
 
TroykaLedMatrix matrix;
 
char Str[] = " Arduino";
uint16_t  len = sizeof(Str);
 
byte shift = 0;
 
void setup() {
  // Инициализируем I2C
  Wire.begin();
  // начало работы с матрицей
  matrix.begin();
  // для нестандартного Wire
  // matrix.begin(Wire1);
    matrix.selectFont(FONT_8X8_BASIC);
}
 
void loop() {
  delay(70);
  matrix.marqueeText(Str, len, shift++);
  if (shift == len * 8) {
    shift = 0;
  }
}

