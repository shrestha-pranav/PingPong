#include "AT91SAM7L128.h"
#include "keyboard.h"
#include "lcd.h"

#define NUM_COLUMNS 7
#define NUM_ROWS 6
#define KEYBOARD_COLUMNS 0x7f
#define KEYBOARD_ROWS 0x400fc00
#define DECIMAL_INT_MAX 100000000

const unsigned char keyboard_row_index[] = {11,12,13,14,15,26};

/* Character codes returned by keyboard_key */

const char keyboard_keys[NUM_COLUMNS][NUM_ROWS] = {
  {-1,  'I', 'P', 'M', 'F', 'A'},
  {'C',  'R', 'V', 'B', '%', 'L'},
  {'\r', '(', ')', '~', '\b', 0},
  {'\v', '7', '8', '9', '/',  0},
  {'\n', '4', '5', '6', '*',  0},
  {'S',  '1', '2', '3', '-',  0},
  { 0,   '0', '.', '=', '+',  0}};

void keyboard_init()
{
  // Initialize the keyboard: Columns are outputs, rows are inputs
  AT91C_BASE_PMC->PMC_PCER = (uint32) 1 << AT91C_ID_PIOC; // Turn on PIOC clock
  AT91C_BASE_PIOC->PIO_PER = KEYBOARD_ROWS | KEYBOARD_COLUMNS; // Enable control
  AT91C_BASE_PIOC->PIO_PPUDR = KEYBOARD_COLUMNS; // Disable pullups on columns
  AT91C_BASE_PIOC->PIO_OER = KEYBOARD_COLUMNS;   // Make columns outputs
  AT91C_BASE_PIOC->PIO_PPUER = KEYBOARD_ROWS;    // Enable pullups on rows
  AT91C_BASE_PIOC->PIO_ODR = KEYBOARD_ROWS;      // Make rows inputs

  AT91C_BASE_PIOC->PIO_SODR = KEYBOARD_COLUMNS;  // Drive all columns high

}

void keyboard_column_high(int column)
{
  AT91C_BASE_PIOC->PIO_SODR = 1 << column;
}

void keyboard_column_low(int column)
{
  AT91C_BASE_PIOC->PIO_CODR = 1 << column;
}

int keyboard_row_read(int row)
{
  return (AT91C_BASE_PIOC->PIO_PDSR) & (1 << keyboard_row_index[row]);
}

//Modified keyboard so that the amount of time it takes to 
//retrieve a key stays the same whether or not a key is pressed
//The return statement was moved from inside the loop to after the loop
int keyboard_key()
{
	int row, col;
	int retrow = 0; 
	int retcol = 0;
	for (col = 0 ; col < NUM_COLUMNS ; col++) {
		keyboard_column_low(col);
		for (row = 0 ; row < NUM_ROWS ; row++)
			if (!keyboard_row_read(row)) {
				keyboard_column_high(col);
				retrow = row;
				retcol = col;
			}
			keyboard_column_high(col);
		}
	return keyboard_keys[retcol][retrow];;
}

void keyboard_get_entry(struct entry *result)
{
  int key;
  unsigned int entry = INT_MAX;           // No number yet
  int negative = 0;                       // Positive by default
  for (;;) {
    while (keyboard_key()) ;              // Wait for release
    while (!(key = keyboard_key())) ;     // Wait for a key
    if ( key >= '0' && key <= '9') {
      if (entry == INT_MAX) entry = 0;    // first digit?
      if (entry < DECIMAL_INT_MAX)        // avoid overflow
	entry = entry * 10 + (key - '0'); // add digit to right
    } else if (key == '~') {              // +/- key
      if (entry == INT_MAX) entry = 0;    // First press gives "-0"
      negative = !negative;               // Toggle
    } else if (key == '\b' && entry != INT_MAX) // Backspace
      entry /= 10;
    else if (key == '\r' || key == '+' || key == '-' ||
	     key == '*' || key == '/') {
      result->number = negative ? -entry : entry;
      result->operation = key;
      return;
    }
    lcd_print_int_neg(negative, entry);
  }
}
