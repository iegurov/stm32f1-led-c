#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1_gpio.h"

/* КОМАНДЫ НАСТРОЙКИ (FUNCTION SET) */
#define LCD_1LINE_8BUS 0x30 //1 строка, 8-разрядная шина (Также используется как команда сброса)
#define LCD_2LINE_8BUS 0x38 
#define LCD_1LINE_4BUS 0x20 //1 строка, 4-разрядная шина (Также используется как команда инициализации 4-разрядного режима)
#define LCD_2LINE_4BUS 0x28


/* УПРАВЛЕНИЕ ОТОБРАЖЕНИЕМ */
#define LCD_OFF         0x08
#define LCD_ON          0x0C
#define LCD_CURSOR_ON   0x0E
#define LCD_BLINK_ON    0x0D
#define LCD_CURBLINK_ON 0x0F //Включить курсор и мигание
#define LCD_CLEAR       0x01 //очистка экрана
#define LCD_SCROLL_L    0x18 //прокрутка влево
#define LCD_SCROLL_R    0x1C //прокрутка вправо
#define LCD_C_RIGHT_D_STAY 0x06 //При выводе символа курсор вправо, дисплей без сдвига


#define LCD_DELAY_TIME 750 //Необходимо подбирать значение индивидуально для каждой частоты ядра для корректной работы дисплея

void lcd_init(Pin E, Pin RS, Pin dataPins[4]);
void lcd_pulse(); //Импульс на E пин
void lcd_cmd(uint8_t cmd);
void lcd_print(const char* data);
void lcd_printch(uint8_t ch);
void lcd_cursor(uint8_t pos, uint8_t line);
void lcd_newchar(const uint8_t data[8], uint8_t num);
uint8_t lcd_get_c_pos();
uint8_t lcd_get_c_line();

#ifdef __cplusplus
} //extern "C"
#endif