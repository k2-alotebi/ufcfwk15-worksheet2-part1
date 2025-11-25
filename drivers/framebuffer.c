#include "framebuffer.h"
#include "../source/io.h"

/* Framebuffer memory address and size */
#define FB_START 0x000B8000
#define FB_COLS 80
#define FB_ROWS 25

/* I/O ports for cursor control */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* Commands for cursor control */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Current cursor position and color */
static unsigned short cursor_pos = 0;
static unsigned char current_fg = FB_WHITE;
static unsigned char current_bg = FB_BLACK;

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) FB_START;
    fb[i * 2] = c;
    fb[i * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

/** fb_write:
 *  Writes the contents of the buffer to the screen
 */
int fb_write(char *buf, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        if (buf[i] == '\n') {
            /* Move to next line */
            cursor_pos = (cursor_pos / FB_COLS + 1) * FB_COLS;
        } else {
            fb_write_cell(cursor_pos, buf[i], current_fg, current_bg);
            cursor_pos++;
        }
        
        /* Wrap to next line if at end */
        if (cursor_pos >= FB_COLS * FB_ROWS) {
            cursor_pos = 0;
        }
    }
    fb_move_cursor(cursor_pos);
    return len;
}

/** fb_move:
 *  Moves the cursor to the specified x, y position
 */
void fb_move(unsigned short x, unsigned short y)
{
    if (x < FB_COLS && y < FB_ROWS) {
        cursor_pos = y * FB_COLS + x;
        fb_move_cursor(cursor_pos);
    }
}

/** fb_write_string:
 *  Writes a null-terminated string to the screen
 */
void fb_write_string(char *str)
{
    unsigned int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            cursor_pos = (cursor_pos / FB_COLS + 1) * FB_COLS;
        } else {
            fb_write_cell(cursor_pos, str[i], current_fg, current_bg);
            cursor_pos++;
        }
        
        if (cursor_pos >= FB_COLS * FB_ROWS) {
            cursor_pos = 0;
        }
        i++;
    }
    fb_move_cursor(cursor_pos);
}

/** fb_write_num:
 *  Writes a number to the screen
 */
void fb_write_num(int num)
{
    char buf[12];
    int i = 0;
    int is_negative = 0;
    
    if (num == 0) {
        fb_write_cell(cursor_pos, '0', current_fg, current_bg);
        cursor_pos++;
        fb_move_cursor(cursor_pos);
        return;
    }
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    if (is_negative) {
        buf[i++] = '-';
    }
    
    /* Print in reverse order */
    while (i > 0) {
        fb_write_cell(cursor_pos, buf[--i], current_fg, current_bg);
        cursor_pos++;
    }
    fb_move_cursor(cursor_pos);
}

/** fb_clear:
 *  Clears the entire screen
 */
void fb_clear(void)
{
    unsigned int i;
    for (i = 0; i < FB_COLS * FB_ROWS; i++) {
        fb_write_cell(i, ' ', current_fg, current_bg);
    }
    cursor_pos = 0;
    fb_move_cursor(cursor_pos);
}

/** fb_set_color:
 *  Sets the default foreground and background colors
 */
void fb_set_color(unsigned char fg, unsigned char bg)
{
    current_fg = fg;
    current_bg = bg;
}
