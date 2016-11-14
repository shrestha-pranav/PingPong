#ifndef _LCD_H
#  define _LCD_H

#  define LCD_7_COLUMNS 15

extern void lcd_init(void);
extern void lcd_put_char7(char ch, int col);
extern void lcd_print7(const char *s);
extern void lcd_print_int_neg(int, unsigned int);
extern void lcd_print_int(int);
extern void lcd_print(uint8 column, uint8 row, uint8 mode);
extern void pprint(uint8 com, uint8 seg, uint8 mode);

#endif
