#ifndef _OLED_H
#define _OLED_H

//======================================
// include                                         
//======================================
#include "project.h"
#include "font.h"

//======================================
// oled dimensions                              
//======================================
#define OLED_WIDTH 128          // width of oled (in pixels)
#define OLED_HEIGHT 64          // height of oled (in pixels)

//======================================
// bitmap modes                          
//======================================
#define OLED_BM_NORMAL 0        // bitmap displayed "as is"
#define OLED_BM_INV 1           // bitmap displayed with pixels inverted
#define OLED_BM_WHITE_TRAN 2    // white pixels are displayed in the forground color, black pixels are transparent
#define OLED_BM_BLACK_TRAN 3    // black pixels are displayed in the forground color, white pixels are transparent 
#define OLED_BM_WHITE_FILL 4    // white pixels are displayed in the forground color, black pixels are displayed in the background color
#define OLED_BM_BLACK_FILL 5    // black pixels are displayed in the forground color, white pixels are displayed in the forground color

//======================================
// text modes                          
//======================================
#define OLED_TEXT_TRAN OLED_BM_WHITE_TRAN   // characters are displayed in the forground color, background is transparent
#define OLED_TEXT_FILL OLED_BM_WHITE_TRAN   // characters are displayed in the forground color, background is filled with the background color  

//======================================
// ssd1306 commands                               
//======================================
#define SSD1306_DISPLAYON 0xAF              // Set Display ON
#define SSD1306_MEMORYADDRESSINGMODE 0x20   // Set Memory Addressing Mode
#define SSD1306_COMSCANDIRECTION 0xC8       // Set COM Output Scan Direction
#define SSD1306_SEGREMAP 0xA0               // Set Segment Re-map
#define SSD1306_CHARGEPUMP 0x8D             // Charge Pump Setting  
#define SSD1306_SETCOLUMNADDRESS 0x21       // Set Column Address
#define SSD1306_SETPAGEADDRESS 0x22         // Set Page Address

//======================================
// oled struct                                 
//======================================
typedef struct {
    uint8 slaveAddr;                                    // oled I2C address
    uint8 (*SendStart)(uint8 slaveAddr, uint8 R_nW);    // function pointer to I2C_MasterSendStart(). Generates an I2C Start condition and sends the slave address with the read/write bit
    uint8 (*SendStop)(void);                            // function pointer to I2C_MasterSendStop(). Generates and I2C Stop condition
    uint8 (*WriteByte)(uint8 byte);                     // function pointer to I2C_MasterWriteByte(). Send one byte via I2C
    uint8 bkColor;                                      // background color
    uint8 color;                                        // foreground color 
    uint8 penSize;                                      // pen size for drawing functions
    const font_t * font;                                // text font
    uint8 bmMode;                                       // bitmap mode
    uint8 textMode;                                     // text mode (i.e. transparent, fill)
    uint8 GRAM[OLED_WIDTH*(OLED_HEIGHT/8)];             // "internal copy" of oled graphic RAM (GRAM)  
} oled_t;

//======================================
// oled_Init()
// this function initializes the oled_t struct
// and sends the necessary initialization
// commands to the display
//
// Parameters:
//     oled - pointer to a oled_t structure
//     slaveAddr - 7-bit I2C address
//     SendStart - function pointer to I2C_MasterSendStart()
//     SendStop - function pointer to I2C_MasterSendStop()
//     WriteByte - function pointer to I2C_MasterWriteByte()
//======================================
void oled_Init(oled_t * oled, uint8 slaveAddr, uint8 (*SendStart)(uint8, uint8), uint8 (*SendStop)(void), uint8 (*WriteByte)(uint8));

//========================================
// oled_SetColor()
// this function sets the foreground color
//
// Parameters:
//     oled - pointer to a oled_t structure
//     color: 0 (black) or 1 (white)
//========================================
void oled_SetColor(oled_t * oled, uint8 color);

//========================================
// oled_GetColor()
// this function returns the current foreground
// color
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetColor(oled_t * oled);

//========================================
// oled_SetBkColor()
// this function sets the background color
//
// Parameters:
//     oled - pointer to a oled_t structure
//     bkColor: 0 (black) or 1 (white)
//========================================
void oled_SetBkColor(oled_t * oled, uint8 bkColor);

//========================================
// oled_GetBkColor()
// this function returns the current background
// color
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetBkColor(oled_t * oled);

//========================================
// oled_SetPenSize()
// this function sets the pen size
// for drawing operations. A pen size of 0
// draws the thinnest lines (1 pixel
// thick)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     penSize - pen size
//========================================
void oled_SetPenSize(oled_t * oled, uint8 penSize);

//========================================
// oled_GetPenSize()
// this function returns the current pen size
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetPenSize(oled_t * oled);

//========================================
// oled_SetFont()
// this function sets the font for 
// text displaying operations (i.e. oled_DispChar()
// and oled_DispString())
//
// Parameters:
//     oled - pointer to a oled_t structure
//     font - pointer to a font_t structure
//========================================
void oled_SetFont(oled_t * oled, font_t * font);

//========================================
// oled_SetBmMode()
// this function sets the bitmap display
// mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//     bmMode - bitmap display mode
//========================================
void oled_SetBmMode(oled_t * oled, uint8 bmMode);

//========================================
// oled_GetBmMode()
// this function returns the current bitmap 
// display mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetBmMode(oled_t * oled);

//========================================
// oled_SetTextMode()
// this function sets the text display
// mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//     textMode - text display mode
//========================================
void oled_SetTextMode(oled_t * oled, uint8 textMode);

//========================================
// oled_GetTextMode()
// this function returns the text display
// mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetTextMode(oled_t * oled);

//========================================
// oled_Clear()
// this functions clears the screan (fills it
// with the background color)
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
void oled_Clear(oled_t * oled);

//========================================
// oled_ClearRect()
// this functions clears a rectangle defined by
// its upper left corner (x0, y0) and lower right
// corner (x1, y1)
// 
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - upper left x-coordinate
//     y0 - upper left y-coordinate
//     x1 - lower right x-coordinate
//     y1 - lower right y-coordinate
//========================================

void oled_ClearRect(oled_t * oled, int x0, int y0, int x1, int y1);
//========================================
// oled_DrawPixel()
// this function fills the pixel located at (x, y)
// with the forground color.
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x - x-coordinate of pixel
//     y - y-coordinate of pixel
//========================================
void oled_DrawPixel(oled_t * oled, int x, int y);

//========================================
// oled_DrawLine()
// this functions draws a line between the 
// points (x0, y0) and (x1, y1)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate of the first point
//     y0 - y-coordinate of the first point
//     x1 - x-coordinate of the second point
//     y1 - y-coordinate of the second point
//========================================
void oled_DrawLine(oled_t * oled, int x0, int y0, int x1, int y1);

//========================================
// oled_FillRect()
// this functions fills a rectangle defined by
// its upper left corner (x0, y0) and lower right
// corner (x1, y1)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - upper left x-coordinate
//     y0 - upper left y-coordinate
//     x1 - lower right x-coordinate
//     y1 - lower right y-coordinate
//========================================
void oled_FillRect(oled_t * oled, int x0, int y0, int x1, int y1);

//========================================
// oled_FillRoundedRect()
// this functions fills a rectangle defined by
// its upper left corner (x0, y0) and lower right
// corner (x1, y1). The corners of the rectangle 
// are rounded with radius r
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - upper left x-position
//     y0 - upper left y-position
//     x1 - lower right x-position
//     y1 - lower right y-position
//     r - radius to be used for rounded corners
//         (must be less than or equal to 1/2 
//         the smallest side length)
//========================================
void oled_FillRoundedRect(oled_t * oled, int x0, int y0, int x1, int y2, int r);

//========================================
// oled_DrawRect()
// this functions draws a rectangle defined by
// its upper left corner (x0, y0) and lower right
// corner (x1, y1)
// 
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - upper left x-position
//     y0 - upper left y-position
//     x1 - lower right x-position
//     y1 - lower right y-position
//========================================
void oled_DrawRect(oled_t * oled, int x0, int y0, int x1, int y1);

//========================================
// oled_DrawRoundedRect()
// this functions draws a rectangle defined by
// its upper left corner (x0, y0) and lower right
// corner (x1, y1). The corners of the rectangle 
// are rounded with radius r
// 
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - upper left x-position
//     y0 - upper left y-position
//     x1 - lower right x-position
//     y1 - lower right y-position
//     r - radius to be used for rounded corners
//         (must be less than or equal to 1/2 
//         the smallest side length)
//========================================
void oled_DrawRoundedRect(oled_t * oled, int x0, int y0, int x1, int y2, int r);

//========================================
// oled_DrawCircle()
// this functions draws a circle or radius r
// centered at (xc, yc)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     xc - x-coordinate of the center of the circle
//     yc - y-coordinate of the center of the circle
//     r - radius of circle
//========================================
void oled_DrawCircle(oled_t * oled, int xc, int yc, int r);

//========================================
// oled_DrawArc()
// this function draws an arc of radius r
// centered at (xc, yc). a0 and a1 specify
// the start and end angles for the arc
// 
// Parameters:
//     oled - pointer to a oled_t structure
//     xc - x-coordinate of the center of the arc (center of circle)
//     yc - y-coordinate of the center of the arc (center of circle)
//     r - radius of arc
//     a0 - start angle (angle = a0*pi/4)
//     a1 - end angle (angle = a1*pi/4)
//========================================
void oled_DrawArc(oled_t * oled, int xc, int yc, int r, int a0, int a1);

//========================================
// oled_FillCircle()
// this function fills in a circle with radius r 
// centered at (xc, yc)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     xc - x-coordinate of the center of the circle
//     yc - y-coordinate of the center of the circle
//     r - radius of circle
//========================================
void oled_FillCircle(oled_t * oled, int xc, int yc, int r);

//========================================
// oled_DrawPoint()
// this function fills in a circle centered 
// at (xc, yc).  The radius of the circle
// is the current pen size
// 
//
// Parameters:
//     oled - pointer to a oled_t structure
//     xc - x-coordinate of point
//     yc - y-coordinate of point
//========================================
void oled_DrawPoint(oled_t * oled, int xc, int yc);

//========================================
// oled_DrawPie()
// this function draws an circular sector 
// of radius r  centered at (xc, yc). a0 and a1
// specify the start and end angles for the sector
// 
// Parameters:
//     oled - pointer to a oled_t structure
//     xc - x-coordinate of the center of the arc (center of circle)
//     yc - y-coordinate of the center of the arc (center of circle)
//     r - radius of circular sector
//     a0 - start angle (angle = a0*pi/4)
//     a1 - end angle (angle = a1*pi/4)
//========================================
void oled_DrawPie(oled_t * oled, int xc, int yc, int r, int a0, int a1);

//======================================
// oled_DispBitmap()
// this function displays the given bitmap
// with its top left corner at (x0, y0). 
// Option for how the bitmap is displayed 
// can be found in the user manual and can
// be set using oled_SetBmMode() 
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate for upper left corner of bitmap
//     y0 - y-coordinate for upper left corner of bitmap
//     bitmap - pointer to an array containing pixel values 
//              (.xbm format)
//     width - width of the bitmap (in pixels)
//     height - height of the bitmap (in pixels)
//======================================
void oled_DispBitmap(oled_t * oled, int x0, int y0, const uint8 * bitmap, uint16 width, uint16 height);

//========================================
// oled_DispChar()
// this function displays the character c
// with its top left corner at (x0, y0). The
// character is displayed in the font specified
// by oled.font. Option for how the character is 
// displayed can be found in the user manual 
// and can be set using oled_SetTextMode()
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate for upper left corner of 
//          the character
//     y0 - y-coordinate for upper left corner of 
//          the character
//     c - character to be displayed
//========================================
void oled_DispChar(oled_t * oled, int x0, int y0, const char c);

//========================================
// oled_DispString
// this function displays the string s
// with the top left corner of the first character
// at (x0, y0). The characters are displayed in the 
// font specified by oled.font. Option for how the 
// characters are displayed can be found in the user
// manual and can be set using oled_SetTextMode()
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate for upper left corner of 
//          the first character in the string
//     y0 - y-coordinate for upper left corner of 
//          the first character in the string
//     s - pointer to the string
//========================================
void oled_DispString(oled_t * oled, int x0, int y0, const char * s);

#endif

//========================================
// END OF FILE
//========================================