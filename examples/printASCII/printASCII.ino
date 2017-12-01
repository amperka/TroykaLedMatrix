// библиотека для работы I²C
#include "Wire.h"
// библиотека для работы со светодиодной матрицей
#include "TroykaLedMatrix.h"

// создаём объект matrix для работы с матрицей
TroykaLedMatrix matrix;

void setup()
{
  // начало работы с матрицей
  matrix.begin();
  // очищаем матрицу
  matrix.clear();
  // устанавливаем шрифт
  matrix.setFont((const uint8_t *)font8x8_basic, 128, 8);
}

void loop()
{
  // с помощью цикла по очереди выводим символы из таблицы ASCII
  for (int i = 32; i < 128; i++) {
    matrix.drawCharacter(i); 
    delay(500);
  }
}
