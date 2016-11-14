#include "AT91SAM7L128.h"
#include "lcd.h"

// LCD configuration
#define COMMON_NUMBER   9<<0   // 10 commons
#define SEGMENT_NUMBER  39<<8  // 40 segments

//We modified the lcd_put_char7 function so it would print arbitrary pixels
void pprint(uint8 com, uint8 seg, uint8 mode){
	
	//gets memory location of pixels and "mask"
	//gets the segement pixels memory
	AT91_REG *common_p = AT91C_SLCDC_MEM + (seg * 2);
	if (com > 31) common_p++;
	
    uint32 mask = ((uint32) 1) << (com & 0x1f);
    	
	if(mode)
		//print out the pixel
		*common_p = *common_p | mask;
	else
		//removes the pixel. no idea why. because XOR = ~OR
		*common_p = *common_p ^ mask;
}

//Prints pixels based on a normal coordinate system. 
//Since the last section of pixels, is rotated, we need this
//function to help translate from normal coodinates to how rows
//and cols are described in the lcd of the calculator
void lcd_print(uint8 column, uint8 row, uint8 mode){
	//error checking
	if(column<0 || row < 0 || column > 42 || row > 5) return;
	
	if(column <33){
		if(row < 4) pprint(column + 6, row + 6, mode);
		else pprint(column + 6, row - 4, mode);
	}
	else if (column < 41){
		pprint(5 - row, column - 31, mode);
	}
	else{
		pprint(5 - row, column - 41, mode);
	}
}

/* Segment/common map for the seven-segment display area

  Segment names:
    aaa
   f   b
   f   b
    ggg
   e   c
   e   c
    ddd
        dp   */
       
#define SC(s,c) ((uint8) (((s) - 6) | ((c-2) << 6)))
#define SEGMENT(x) (((x) & 0x3f) + 6)
#define COMMON(x) (((x) >> 6) + 2)
static uint8 const segcom[LCD_7_COLUMNS][8] = {
  //   a          b          c          d          e          f          g        dp
  {SC( 7, 2), SC( 8, 2), SC( 7, 4), SC( 6, 4), SC( 6, 3), SC( 6, 2), SC( 7, 3), SC( 8, 5) },
  {SC( 9, 2), SC(10, 3), SC(10, 4), SC( 9, 4), SC( 8, 4), SC( 8, 3), SC( 9, 3), SC(10, 5) },
  {SC(11, 2), SC(12, 2), SC(12, 3), SC(11, 5), SC(11, 4), SC(10, 2), SC(11, 3), SC(12, 4) },
  {SC(14, 2), SC(14, 3), SC(14, 4), SC(13, 5), SC(13, 4), SC(13, 2), SC(13, 3), SC(15, 4) },
  {SC(16, 2), SC(17, 2), SC(16, 4), SC(15, 5), SC(15, 3), SC(15, 2), SC(16, 3), SC(17, 5) },
  {SC(18, 2), SC(19, 3), SC(19, 4), SC(18, 4), SC(17, 4), SC(17, 3), SC(18, 3), SC(19, 5) },
  {SC(20, 2), SC(21, 2), SC(21, 3), SC(20, 5), SC(20, 4), SC(19, 2), SC(20, 3), SC(21, 4) },
  {SC(23, 2), SC(23, 3), SC(23, 4), SC(22, 5), SC(22, 4), SC(22, 2), SC(22, 3), SC(24, 4) },
  {SC(25, 2), SC(26, 2), SC(25, 4), SC(24, 5), SC(24, 3), SC(24, 2), SC(25, 3), SC(26, 5) },
  {SC(27, 2), SC(28, 3), SC(28, 4), SC(27, 4), SC(26, 4), SC(26, 3), SC(27, 3), SC(28, 5) },
  {SC(29, 2), SC(30, 2), SC(30, 3), SC(29, 5), SC(29, 4), SC(28, 2), SC(29, 3), SC(30, 4) },
  {SC(32, 2), SC(32, 3), SC(32, 4), SC(31, 5), SC(31, 4), SC(31, 2), SC(31, 3), SC(33, 5) },
  // Exponent digits							                 
  {SC(34, 2), SC(34, 3), SC(35, 5), SC(34, 5), SC(33, 4), SC(33, 2), SC(34, 4), SC(27, 5) },
  {SC(36, 2), SC(36, 3), SC(36, 4), SC(36, 5), SC(35, 4), SC(35, 2), SC(35, 3), SC(30, 5) },
  {SC(38, 3), SC(38, 4), SC(38, 5), SC(37, 5), SC(37, 4), SC(37, 2), SC(37, 3), SC(32, 5) },
  };

static uint8 const afont7[] = {
#define segs 0
#define a | 0x1
#define b | 0x2
#define c | 0x4
#define d | 0x8
#define e | 0x10
#define f | 0x20
#define g | 0x40
#define dp | 0x80
  segs               , // space
  segs b dp          , // !
  segs f b           , // "
  segs e f b c       , // #
  segs a f g c d dp  , // $
  segs b g e dp      , // %
  segs f g e c d dp  , // &
  segs f             , // '
  segs a f e d       , // (
  segs a b c d       , // )
  segs f b g e c dp  , // *
  segs f g e         , // +
  segs c dp          , // ,
  segs g             , // -
  segs dp            , // .
  segs b g e         , // /
  segs a b c d e f   , // 0
  segs b c           , // 1
  segs a b g e d     , // 2
  segs a b g c d     , // 3
  segs f g b c       , // 4
  segs a f g c d     , // 5
  segs a f g e c d   , // 6
  segs a b c         , // 7
  segs a b c d e f g , // 8
  segs a b f g c d   , // 9
  segs f e dp        , // :
  segs b c dp        , // ;
  segs b g c         , // <
  segs g d           , // =
  segs f g e         , // >
  segs a b g e dp    , // ?
  segs a b c d e f g dp , // @
  segs a e f b c g   , // A
  segs f g e c d     , // b
  segs a f e d       , // C
  segs b g e c d     , // d
  segs a f g e d     , // E
  segs a f g e       , // F
  segs a f e d c g   , // g
  segs f e g b c     , // H
  segs e f           , // I
  segs b c d e       , // J
  segs f b g e c dp  , // K
  segs f e d         , // L
  segs e f a b c     , // m
  segs e g c         , // n
  segs a b c d e f   , // o
  segs f a b g e     , // P
  segs f a b g c dp  , // q
  segs g e           , // r
  segs a f g c d     , // S
  segs f g e d       , // t
  segs f e d c b     , // U
  segs e d c         , // v
  segs e d c a       , // w
  segs f b g e c dp  , // x
  segs f g b c d     , // y
  segs a b g e d dp  , // z
  segs a f e d dp    , // [
  segs f g c         , // backslash
  segs a b c d dp    , // ]
  segs f a b         , // ^
  segs d             , // _
  segs b             , // `
  segs a e f b c g   , // A
  segs f g e c d     , // b
  segs g e d         , // c
  segs b g e c d     , // d
  segs a f g e d     , // E
  segs a f g e       , // F
  segs a f b g c d   , // g
  segs f e g   c     , // h
  segs c a           , // i
  segs b c d         , // j
  segs f b g e c dp  , // K
  segs f e           , // l
  segs e f a b c     , // m
  segs e g c         , // n
  segs e g c d       , // o
  segs f a b g e     , // P
  segs f a b g c dp  , // q
  segs g e           , // r
  segs a f g c d     , // S
  segs f g e d       , // t
  segs f e d c b     , // U
  segs e d c         , // v
  segs e d c a       , // w
  segs f b g e c dp  , // x
  segs f g b c d     , // y
  segs a b g e d dp  , // z
  segs b g c dp      , // {
  segs f e dp        , // |
  segs f e g dp      , // }
  segs a g           , // ~
  segs a b c d e f g dp // DEL
#undef a
#undef b
#undef c
#undef d
#undef e
#undef f
#undef g
#undef dp
};

// Enable the LCD display: turn on its power supply and set various counters
void lcd_init()
{
  // Turn on LCD power supply
  AT91C_BASE_SUPC->SUPC_MR = (AT91C_BASE_SUPC->SUPC_MR&0xFFFFFFC0) |
    (0xA5 << 24) | AT91C_SUPC_LCDMODE_INTERNAL| 0x8;
  while (! (AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_LCDS) )
    ;

  AT91C_BASE_SLCDC->SLCDC_CR = AT91C_SLCDC_SWRST;  // Software reset of LCD
  AT91C_BASE_SLCDC->SLCDC_MR = COMMON_NUMBER | AT91C_SLCDC_BUFTIME_8_Tsclk |
    AT91C_SLCDC_BIAS_1_3 | SEGMENT_NUMBER; // Set number of pins
  AT91C_BASE_SLCDC->SLCDC_FRR = AT91C_SLCDC_PRESC_SCLK_16 | AT91C_SLCDC_DIV_2;
  AT91C_BASE_SLCDC->SLCDC_CR = AT91C_SLCDC_LCDEN; // Enable LCD
}

// Display an ASCII character in the given column on the 7-segment display
void lcd_put_char7(char ch, int col)
{
  if (col >= LCD_7_COLUMNS) return;
  uint8 segments;
  if (ch >= ' ' && ch <= 127) segments = afont7[ch - ' '];
  else segments = 0;

  uint8 const *segment_column = segcom[col];
  int i;
  for (i = 0 ; i < 8 ; i++) {
    uint8 common = COMMON(*segment_column);
    uint8 segment = SEGMENT(*segment_column);
	
    AT91_REG *common_p = AT91C_SLCDC_MEM + (common * 2);
    uint32 mask = ((uint32) 1) << (segment & 0x1f);
    if (segment & 0x20) common_p++;
    if (segments & 0x1)
      *common_p = *common_p | mask;
    else
      *common_p = *common_p & ~mask;
    ++segment_column, segments = segments >> 1;
  }
}

// Display a string on the 7-segment display starting from the leftmost column
void lcd_print7(const char *c)
{
  int column;
  char ch;
  for (column = 0 ; column < LCD_7_COLUMNS && (ch = *c++) ; ++column)
    lcd_put_char7(ch, column);
}

// Display a right-justified integer with an optional leading negative sign
void lcd_print_int_neg(int negative, unsigned int n)
{
  int column = LCD_7_COLUMNS - 4; // Start rightmost
  do {
    lcd_put_char7(n%10 + '0', column--);
    n /= 10;
  } while (n);
  if (negative) lcd_put_char7('-', column--);
  while (column >= 0) lcd_put_char7(' ', column--);
}

// Display a right-justified signed integer
void lcd_print_int(int n) {
  if (n < 0) lcd_print_int_neg(1, -n);
  else lcd_print_int_neg(0, n);
}
