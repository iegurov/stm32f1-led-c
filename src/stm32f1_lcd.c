#include "stm32f1_lcd.h"

static Pin pinE, pinRS;
static Pin dataPins[4]; 
static uint8_t c_pos, c_line;

uint8_t lcd_get_c_pos() {return c_pos;}
uint8_t lcd_get_c_line() {return c_line;}

static void lcd_delay()
{
    for (volatile uint32_t i=0; i<LCD_DELAY_TIME; i++) __NOP();
}

//дергаем E пин
void lcd_pulse()
{
    PinSet(pinE);
    lcd_delay();
    PinReset(pinE);
    lcd_delay();
}

static void lcd_write(uint8_t data)
{
    uint8_t shift = 0x8;
    for(int8_t i = 3; i>-1; i--)
    {
        if(data & shift) PinSet(dataPins[i]);
        else PinReset(dataPins[i]);

        shift = shift >> 1;
    }
}

static void lcd_data(uint8_t data, uint8_t mode)
{
    if (mode==0) PinReset(pinRS);
    else PinSet(pinRS);
    lcd_write(data>>4); //старший полубайт
    lcd_pulse(); 
    lcd_write(data & 0xF); //младший полубайт
    lcd_pulse();
}

void lcd_cmd(uint8_t cmd){lcd_data(cmd, 0);}

void lcd_printch(uint8_t ch)
{
    if(c_pos==39) lcd_cursor(0, c_line); //если закончилось место в буфере строки, переместим курсор в ее начало
    else c_pos++;
    lcd_data(ch, 1);
}

//Нужно только для сброса + настройки LCD в режим 4-х разрядной шины
static void lcd_halfcmd(uint8_t cmd)
{
    PinReset(pinRS); // режим команд
    lcd_write(cmd>>4); //старший полубайт
    lcd_pulse(); 
}

//На 4-х строковом дисплее функция не проверялась!
void lcd_cursor(uint8_t pos, uint8_t line)
{
    static const uint8_t row_addr[] = {0x00, 0x40, 0x14, 0x54};
    lcd_cmd(0x80 | (row_addr[line] + pos));

    c_pos = pos;
    c_line = line;
}

void lcd_init(Pin E, Pin RS, Pin _dataPins[4])
{
    pinE = E;
    pinRS = RS;
    PinMode(pinE, OUT_PP_50MHz);
    PinMode(pinRS, OUT_PP_50MHz);
    PinReset(pinRS);

    for(uint8_t i = 0; i<4; i++)
    {
        dataPins[i] = _dataPins[i];
        PinMode(dataPins[i], OUT_PP_50MHz);
        PinReset(dataPins[i]);
    }

    //Согласно документации, отправляем 3 команды  0x30 с небольшим перерывом между ними для сброса lcd
    lcd_halfcmd(LCD_1LINE_8BUS);
    lcd_delay();
    lcd_halfcmd(LCD_1LINE_8BUS);
    lcd_delay();
    lcd_halfcmd(LCD_1LINE_8BUS);
    lcd_delay();

    //Настраиваем lcd на 4-разрядную шину и готовим к выводу символов
    lcd_halfcmd(LCD_1LINE_4BUS);
    lcd_cmd(LCD_2LINE_4BUS);
    lcd_cmd(LCD_ON);
    lcd_cmd(LCD_C_RIGHT_D_STAY);
    lcd_cmd(LCD_CLEAR);
    lcd_cursor(0,0);
}

void lcd_print(const char* data)
{
    while(*data)
    {
        lcd_printch(*data);
        data++;
    }
}

void lcd_newchar(const uint8_t data[8], uint8_t num)
{
    uint8_t prev_pos = c_pos;
    uint8_t prev_line = c_line; //Сохраняем позицию курсора 

    lcd_cmd(0x40 | num<<3); //Сообщаем lcd, что пишем в CGRAM
    for(uint8_t i = 0; i<8; i++)
    {
        lcd_printch(data[i]);
    }
    lcd_cursor(prev_pos, prev_line);  //Сообщаем lcd, что дальше пишем в DDRAM и восстанавливаем позицию курсора
}

