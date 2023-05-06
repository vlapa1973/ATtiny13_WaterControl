/*
      Контролер перелива бака воды
      v.20220927-005
*/

#include <avr/io.h>
#include <util/delay.h>

#define PIN_OPEN PB3  //  открытие
#define PIN_CLOSE PB4 //  закрытие
#define PIN_BEEP PB2  //  выход beep/led
#define IN1 PB0       //  датчик 1
#define IN2 PB1       //  датчик 2

uint32_t PAUSE_OPEN = 300;     //  пауза до открытия вентиля, сек
uint32_t TIME_WORK_VALVE = 30; //  время работы клапана, сек

uint16_t pause = 0;
bool flagOpen = true;
bool flagClose = false;

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
int main(void)
{
  // инициализация порта светодиода
  DDRB = 0b00011100; //  порт: PB2-PB4 на выход; PB0-PB1 на вход
  // PORTB = 0b00001111; //  открытие вентиля
  // PORTB = 0b00010111; //  закрытие вентиля
  PORTB = 0b00000111;
  _delay_ms(1000);
  // flagOpen = true;

  while (1)
  {
    if (!((1 << IN1) & PINB) || !((1 << IN2) & PINB))
    {
      _delay_ms(100);
      if (!((1 << IN1) & PINB) || !((1 << IN2) & PINB))
      {
        if (flagClose)
          PORTB = 0b00010011; //  закрытие вентиля

        PORTB &= ~(1 << PIN_BEEP);
        _delay_ms(500);
        PORTB |= (1 << PIN_BEEP);

        _delay_ms(TIME_WORK_VALVE * 1000);
        PORTB = 0b00000111;
        _delay_ms(PAUSE_OPEN * 1000);
        flagOpen = true;
        flagClose = false;
      }
    }
    else
    {
      _delay_ms(100);
      if (((1 << IN1) & PINB) || ((1 << IN2) & PINB))
      {
        if (flagOpen)
        {
          uint16_t count = 0;
          while (count < TIME_WORK_VALVE)
          {
            PORTB = 0b00001111; //  открытие вентиля
            count++;
            _delay_ms(1000);
            if (!((1 << IN1) & PINB) || !((1 << IN2) & PINB))
              break;
          }
          flagOpen = false;
        }
        else
        {
          PORTB = 0b00000111;
        }
        flagClose = true;
      }
    }
  }
}