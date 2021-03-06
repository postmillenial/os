#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

void hb(){
term_buf=(uint16_t*) 0xB8000;
char* s="\x68\x61\x70\x70\x79\x20\x62\x64\x61\x79\x20\x70\x68\x69\x6c\x0a";
size_t len = strlen(s);

for ( size_t i = 0; i < len; i++ )
     term_buf[i]=s[i];
}

uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;

}

uint16_t make_vgaentry(char c, uint8_t color)
{
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}

size_t strlen(const char* str)
{
    size_t ret = 0;
    while ( str[ret] != 0 )
        ret++;
    return ret;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void copy_display_to(uint16_t* device){
    for ( size_t y = 0; y < VGA_HEIGHT; y++ )
    {
        const size_t row = y * VGA_WIDTH;
        for ( size_t x = 0; x < VGA_WIDTH; x++ )
        {
            const size_t index = row + x;
            device[index] = terminal_buffer[index];
        }
    }
}

void terminal_initialize()
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for ( size_t y = 0; y < VGA_HEIGHT; y++ )
    {
        const size_t row = y * VGA_WIDTH;
        for ( size_t x = 0; x < VGA_WIDTH; x++ )
        {
            const size_t index = row + x;
            terminal_buffer[index] = make_vgaentry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    // this function is solely for writing to the buffer.
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
    // this is the function to actually do any logic for putting characters.
    if (c != '\n')
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    terminal_buffer = (uint16_t*) 0xB8000;

    if (c == '\n' || ++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;

        if ( ++terminal_row >= VGA_HEIGHT )
        {

            // So what this needs to do in order to scroll properly:
            copy_display_to((uint16_t*) (0xB8000 - (VGA_WIDTH*2)));
            terminal_row--;
            for(size_t i=0;i<VGA_WIDTH;i++){
                terminal_putentryat(' ', terminal_color, i, terminal_row);
            }
        }
    }
}

void terminal_writestring(const char* data)
{
    size_t datalen = strlen(data);

    for ( size_t i = 0; i < datalen; i++ )
        terminal_putchar(data[i]);
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main()
{
    terminal_initialize();
    terminal_setcolor(COLOR_BLUE);
    terminal_writestring("line 1\n");
    terminal_writestring("lines 2-4 Aimlessly Bill Carried Dolly Everywhere Forgetting George, His Iguana (Just Kidding). Laughing, Men, Not Obsession, Portrays Qualities, Revelations, Sides To U, Vindicated While Xylophoning Your Zebra.\n5\n\n\n\n9 ");
    terminal_writestring("Alas, badly crafted doglike edifices from Greece have injured Japanese kayakers, leaving many nearby ornaments perpetually quaking, ready soon to unseat very worried, xenophobic yellow zombies.\n12\n\n\n15\n");
    terminal_writestring("16-18 Angrily, brave cocky dozens encircled four grotesque, hairy imbeciles - jeering, \"Kill!\" like murderous, nefarious oafs, piking quickly, rounding swiftly, tacitly undermining various warriors, xiphoid yataghans zapping.\n\n\n\n");
    terminal_setcolor(COLOR_MAGENTA);
    terminal_writestring("22 colors!\n\n");
    terminal_setcolor(COLOR_WHITE);
    terminal_writestring("24 A banal culture does everything for generic, homogeneous intentions, just killing life's motile natural order, producing quantifiable results such that");
    terminal_writestring(" utterly verify why xenogenesis yields zeniths.\n");
    terminal_writestring("2 utterly verify why xenogenesis yields zeniths.\n");
    terminal_writestring("3 utterly verify why xenogenesis yields zeniths.\n");
    terminal_writestring("4 utterly verify why xenogenesis yields zeniths.\n");
    terminal_writestring("colors!\n");

//    terminal_writestring("where is thishjkhjh\n");
//    copy_to_display((uint16_t*) 0xB0000); // copies (VGA_WIDTH * VGA_HEIGHT) characters to the buffer specified here



}

