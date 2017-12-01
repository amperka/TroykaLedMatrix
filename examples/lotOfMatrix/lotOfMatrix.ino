// библиотека для работы I²C
#include "Wire.h"
// библиотека для работы со светодиодной матрицей
#include "TroykaLedMatrix.h"

// создаём объекты matrix для работы с матрицами
// для каждой матрицы передаём свой адрес
// подробнее читайте на:
// http://wiki.amperka.ru/продукты:troyka-led-matrix
TroykaLedMatrix matrix1(0b01100000);
TroykaLedMatrix matrix2(0b01100001);
TroykaLedMatrix matrix3(0b01100010);

void setup()
{
  // начало работы с матрицами
  matrix1.begin();
  matrix2.begin();
  matrix3.begin();
  // очищаем матрицы
  matrix1.clear();
  matrix2.clear();
  matrix3.clear();
  // устанавливаем шрифт
  matrix1.setFont((const uint8_t *)font8x8_basic, 128, 8);
  matrix2.setFont((const uint8_t *)font8x8_basic, 128, 8);
  matrix3.setFont((const uint8_t *)font8x8_basic, 128, 8);
  // выводим на каждую матрицу по символу из строки «C++»
  matrix1.drawCharacter('C');
  matrix2.drawCharacter('+');
  matrix3.drawCharacter('+');
}

void loop()
{

}
