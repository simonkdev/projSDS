//GCC includes these so we can use them for stuff like fixed-width types
#include <stdint.h>
#include <stddef.h>

//basic checks to make sure GCC works properly

#if definded(__linux__)
#error "This code must be compiled with a cross-compiler"

#elif !defined(__i386__)
#error "This code must be compiled with an x86-elf compiler"

#endif

//This is x86´s VGA textmode buffer. To display text, data is written to 
//this memory location.

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

//by default, the VGA textmode buffer has a size of 80x25 characters

const int VGA_COLS = 80;
const int VGA_ROWS = 25;

//we start displaying text at the top-left of the screen (column = 0, row = 0)

int term_col = 0;
int term_row = 0;

uint8_t term_color = 0x0F;
//black bg, white text

//initialize terminal by clearing it

void term_init()
{
    for (int col = 0; col < VGA_COLS; col++)
    {
        for (int row = 0; row < VGA_ROWS; row++)
        {
           //VGA textmode buffer with size (VGA_COLS * VGA_ROWS).
            const size_t index = (VGA_COLS * row) + col;    //moves the "cursor" in the form of a constant to the current space
            //Entries in VGA buffer have binary form BBBBFFFFCCCCCCCC, where:
            // - B is the background color
            // - F is the foreground color
            // - C is the ASCII character
            //meaning: B declares what color the background of that space has, F declares the color of the character,
            //and C is the character within the space.(in ASCII)
            vga_buffer[index] = ((uint16_t)term_color << 8) | ' '; //we set the current space to empty (blank)
            //example: vga_buffer[0] = 0x0F20; 
            //0x0F is the color, 20 is the ASCII code for space
        }
    }
}

//put one char on the screen
void term_putc(char c)
{
    switch(c)           //basically a logic gate, checks for defined cases and if nothing is found, default is executed
    {
        case '\n':  //newline character puts column to 0 and increments row
            {
                term_col = 0;
                term_row++;
                break;
            }
        default:
            {
                const size_t index = (VGA_COLS * term_row) + term_col; //calculate current index
                vga_buffer[index] = ((uint16_t)term_color << 8 | c;
                term_col ++; //increment column
                break;
            } 
        //in case we get past last column
        if (term_col >= VGA_COLS)
            {
                term_col = 0;
                term_row++;
            }
        
        //in case we get past last row
        if (term_row >= VGA_ROWS)
            {
                term_col = 0;
                term_row = 0;
            }

    }    
}

void term_print(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i++) //keep placing until we reach null-terminating character ('\0)
    {
        term_putc(str[i]);
        term_putc('\n');
    }
}

//main function, called by start.s
void kernel_main()
{
    term_init();
    term_print("Hello World!")
    term_print("This is a kernel. Welcome to the project!")
    term_print("Developed by: simonkdev")
}