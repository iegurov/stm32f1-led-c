//C example

#include "stm32f1_gpio.h"
#include "stm32f1_lcd.h"


#define B12 (Pin){GPIOB, 12} //RS  
#define B13 (Pin){GPIOB, 13} //E

#define A8  (Pin){GPIOA,  8} //D4
#define A9  (Pin){GPIOA,  9} //D5
#define A10 (Pin){GPIOA, 10} //D6
#define A11 (Pin){GPIOA, 11} //D7

void createChars()
{
    uint8_t char0[] = {0x00, 0x04, 0x0e, 0x19, 0x11, 0x0a, 0x04, 0x00};
    uint8_t char1[] = {0x00, 0x04, 0x0e, 0x13, 0x11, 0x0a, 0x04, 0x00};
    uint8_t char2[] = {0x00, 0x04, 0x0a, 0x11, 0x13, 0x0e, 0x04, 0x00};
    uint8_t char3[] = {0x00, 0x04, 0x0a, 0x11, 0x19, 0x0e, 0x04, 0x00};

    lcd_newchar(char0, 0);
    lcd_newchar(char1, 1);
    lcd_newchar(char2, 2);
    lcd_newchar(char3, 3);
}

int main()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    
    Pin dataPins[4] = {A8, A9, A10, A11};

    lcd_init(B13, B12, dataPins);
    createChars();
    lcd_print("HELLO WORLD ");

    while(1)
    {
        lcd_printch(0);
        lcd_cursor(lcd_get_c_pos()-1, 0);
        for(int i = 0; i<400000; i++);

        lcd_printch(1);
        lcd_cursor(lcd_get_c_pos()-1, 0);
        for(int i = 0; i<400000; i++);

        lcd_printch(2);
        lcd_cursor(lcd_get_c_pos()-1, 0);
        for(int i = 0; i<400000; i++);

        lcd_printch(3);
        lcd_cursor(lcd_get_c_pos()-1, 0);
        for(int i = 0; i<400000; i++);
    }
    
    return 0;
}