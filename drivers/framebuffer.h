#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

/* Framebuffer colors */
#define FB_BLACK         0
#define FB_BLUE          1
#define FB_GREEN         2
#define FB_CYAN          3
#define FB_RED           4
#define FB_MAGENTA       5
#define FB_BROWN         6
#define FB_LIGHT_GREY    7
#define FB_DARK_GREY     8
#define FB_LIGHT_BLUE    9
#define FB_LIGHT_GREEN   10
#define FB_LIGHT_CYAN    11
#define FB_LIGHT_RED     12
#define FB_LIGHT_MAGENTA 13
#define FB_YELLOW        14   
#define FB_WHITE         15

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos);

/** fb_write:
 *  Writes the contents of the buffer buf of length len to the screen
 *
 *  @param buf The buffer to write
 *  @param len The length of the buffer
 */
int fb_write(char *buf, unsigned int len);

/** fb_move:
 *  Moves the cursor to the specified x, y position
 *
 *  @param x The column (0-79)
 *  @param y The row (0-24)
 */
void fb_move(unsigned short x, unsigned short y);

/** fb_write_string:
 *  Writes a null-terminated string to the screen
 *
 *  @param str The string to write
 */
void fb_write_string(char *str);

/** fb_write_num:
 *  Writes a number to the screen
 *
 *  @param num The number to write
 */
void fb_write_num(int num);

/** fb_clear:
 *  Clears the entire screen
 */
void fb_clear(void);

/** fb_set_color:
 *  Sets the default foreground and background colors
 *
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_set_color(unsigned char fg, unsigned char bg);

#endif /* INCLUDE_FRAMEBUFFER_H */
