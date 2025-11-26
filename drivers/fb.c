#include "fb.h"
#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Framebuffer memory address and dimensions */
#define FB_ADDRESS              0x000B8000

/* Current cursor position */
static unsigned short cursor_x = 0;
static unsigned short cursor_y = 0;

/* Pointer to framebuffer memory */
static char *fb = (char *) FB_ADDRESS;

/** fb_write_cell:
 * Writes a character with the given foreground and background to position i
 * in the framebuffer.
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i * 2] = c;
    fb[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

/** fb_move:
 * Moves the cursor to the given x,y position
 */
void fb_move(unsigned short x, unsigned short y)
{
    cursor_x = x;
    cursor_y = y;
    unsigned short pos = y * FB_WIDTH + x;
    fb_move_cursor(pos);
}

/** fb_clear:
 * Clears the entire framebuffer
 */
void fb_clear(void)
{
    unsigned int i;
    for (i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fb_write_cell(i, ' ', FB_BLACK, FB_BLACK);
    }
    fb_move(0, 0);
}

/** fb_putc:
 * Writes a single character at the current cursor position
 */
void fb_putc(char c)
{
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        unsigned short pos = cursor_y * FB_WIDTH + cursor_x;
        fb_write_cell(pos, c, FB_WHITE, FB_BLACK);
        cursor_x++;
        if (cursor_x >= FB_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    
    if (cursor_y >= FB_HEIGHT) {
        cursor_y = 0;
    }
    
    fb_move(cursor_x, cursor_y);
}

/** fb_puts:
 * Writes a null-terminated string at the current cursor position
 */
void fb_puts(char *str)
{
    unsigned int i = 0;
    while (str[i] != '\0') {
        fb_putc(str[i]);
        i++;
    }
}

/** fb_write:
 * Writes the contents of the buffer buf of length len to the screen
 */
int fb_write(char *buf, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        fb_putc(buf[i]);
    }
    return 0;
}
