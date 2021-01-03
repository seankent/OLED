//======================================
// include                                         
//======================================
#include "oled.h"

//======================================
// private function declarations                                        
//======================================
void SetColumnAddress(oled_t * oled, uint8 A, uint8 B);
void SetPageAddress(oled_t * oled, uint8 A, uint8 B);
void WriteGRAM(oled_t * oled, int x0, int y0, int x1, int y1, uint8 color);
void DrawHLine(oled_t * oled, int y, int x0, int x1);
void DrawVLine(oled_t * oled, int x, int y0, int y1);
void BresenhamLineAlgorithm(oled_t * oled, int x0, int y0, int dx, int dy, uint8 rot);
void PlotPerpLineL(oled_t * oled, int x0, int y0, int dx, int dy, int x, int y, int step, uint8 rot);
void PlotPerpLineR(oled_t * oled, int x0, int y0, int dx, int dy, int x, int y, int step, uint8 rot);
void PlotPoint(oled_t * oled, int x0, int y0, int x, int y, uint8 rot);
void BresenhamCircleAlgorithm(oled_t *oled, int xc, int yc, int rIn, int rOut, uint8 octants);
void PlotCircle(oled_t * oled, int xc, int yc, int x, int yIn, int yOut, uint8 octants);
void Swap(int* var0, int* var1);

//======================================
// initialization commands for ssd1306 driver                             
//======================================
const uint8 initCmds[] = {
    0x00,
    SSD1306_CHARGEPUMP,
    0x14,
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDIRECTION,
    SSD1306_DISPLAYON,
    SSD1306_MEMORYADDRESSINGMODE,   // set memory addressing mode to Horizonatal Addressing Mode
    0x00
};

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
void oled_Init(oled_t * oled, uint8 slaveAddr, uint8 (*SendStart)(uint8, uint8), uint8 (*SendStop)(void), uint8 (*WriteByte)(uint8))
{
    oled->slaveAddr = slaveAddr;                    // set oled's I2C address
    oled->SendStart = SendStart;                    // pass oled the I2C_MasterSendStart() function
    oled->SendStop = SendStop;                      // pass oled the I2C_MasterSendStop() function
    oled->WriteByte = WriteByte;                    // pass oled the I2C_MasterWriteByte() function
    oled->bkColor = 0;                              // set background color
    oled->color = 1;                                // set foreground color
    oled->penSize = 0;                              // set pen size
    oled->font = &font;                             // set font
    oled->bmMode = OLED_BM_NORMAL;                  // set bitmap mode
    oled->textMode = OLED_TEXT_TRAN;                // set text mode

    oled->SendStart(slaveAddr, 0x00);               // generate Start condition and send slave address with R/W# bit = 0 (write mode)
    for (int i=0; i<(int)sizeof(initCmds); i++){  
        oled->WriteByte(initCmds[i]);               // send initialization commands/parameters
    }   
    oled->SendStop();                               // generate Stop condition
}

//========================================
// oled_SetColor()
// this function sets the foreground color
//
// Parameters:
//     oled - pointer to a oled_t structure
//     color: 0 (black) or 1 (white)
//========================================
void oled_SetColor(oled_t * oled, uint8 color)
{
    if ((color == 0) || (color == 1)) oled->color = color;
}

//========================================
// oled_GetColor()
// this function returns the current foreground
// color
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetColor(oled_t * oled)
{
    return oled->color;
}

//========================================
// oled_SetBkColor()
// this function sets the background color
//
// Parameters:
//     oled - pointer to a oled_t structure
//     bkColor: 0 (black) or 1 (white)
//========================================
void oled_SetBkColor(oled_t * oled, uint8 bkColor)
{
    if ((bkColor == 0) || (bkColor == 1)) oled->bkColor = bkColor;
}

//========================================
// oled_GetBkColor()
// this function returns the current background
// color
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetBkColor(oled_t * oled)
{
    return oled->bkColor;
}

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
void oled_SetPenSize(oled_t * oled, uint8 penSize)
{
    oled->penSize = penSize;
}

//========================================
// oled_GetPenSize()
// this function returns the current pen size
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetPenSize(oled_t * oled)
{
    return oled->penSize;
}

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
void oled_SetFont(oled_t * oled, font_t * font)
{
    oled->font = font;
}

//========================================
// oled_SetBmMode()
// this function sets the bitmap display
// mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//     bmMode - bitmap display mode
//========================================
void oled_SetBmMode(oled_t * oled, uint8 bmMode)
{
    if ((bmMode == OLED_BM_NORMAL) || 
        (bmMode == OLED_BM_INV) ||
        (bmMode == OLED_BM_WHITE_TRAN) || 
        (bmMode == OLED_BM_BLACK_TRAN) ||
        (bmMode == OLED_BM_WHITE_FILL) || 
        (bmMode == OLED_BM_BLACK_FILL)) oled->bmMode = bmMode;
}

//========================================
// oled_GetBmMode()
// this function returns the current bitmap 
// display mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetBmMode(oled_t * oled)
{
    return oled->bmMode;
}

//========================================
// oled_SetTextMode()
// this function sets the text display
// mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//     textMode - text display mode
//========================================
void oled_SetTextMode(oled_t * oled, uint8 textMode)
{
    if ((textMode == OLED_TEXT_TRAN) ||
        (textMode == OLED_TEXT_FILL)) oled->textMode = textMode;
}

//========================================
// oled_GetTextMode()
// this function returns the text display
// mode
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
uint8 oled_GetTextMode(oled_t * oled)
{
    return oled->textMode;
}

//========================================
// oled_Clear()
// this functions clears the screan (fills it
// with the background color)
//
// Parameters:
//     oled - pointer to a oled_t structure
//========================================
void oled_Clear(oled_t * oled)
{
    WriteGRAM(oled, 0, 0, OLED_WIDTH-1, OLED_HEIGHT-1, oled->bkColor);      // fill screen with background color         
}

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
void oled_ClearRect(oled_t * oled, int x0, int y0, int x1, int y1)
{
    WriteGRAM(oled, x0, y0, x1, y1, oled->bkColor);     // clear rectangle   
}

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
void oled_DrawPixel(oled_t * oled, int x, int y)
{
    WriteGRAM(oled, x, y, x, y, oled->color);       // draw pixel
} 

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
void oled_DrawLine(oled_t * oled, int x0, int y0, int x1, int y1)
{
    if (x0 == x1){                              // check if line is vertical
        DrawVLine(oled, x0, y1, y0);            // draw vertical line
        return;
    }
    if (y0 == y1){                              // check if line is horizontal
        DrawHLine(oled, y0, x1, x0);            // draw horizontal line
        return;
    }
    if (x1 < x0){                               // check if x0 <= x1 (if not, swap coordinates)
        Swap(&x0, &x1);                         // swap x coordinates
        Swap(&y0, &y1);                         // swap y coordinates
    }
                                                // Bresenham's line algorith assumes the slope of the line is between 0 and 1. If the line does not satisfy this condition,
                                                // swap coordinates to put the line in this form. rot keeps track of which rotation was made
    uint8 rot = 1;                              // 0 <= slope <= 1
    if (x1-x0 < y1-y0){                         // 1 < slope
       rot = 0;
       Swap(&x0, &y0);
       Swap(&x1, &y1);
    }                            
    else if ((x1-x0 >= y0-y1) && (y0 > y1)){    // -1 <= slope < 0
        rot = 2;  
        y0 = -y0;                                   
        y1 = -y1;
    }
    else if (x1-x0 < y0-y1){                    // slope < -1
        rot = 3; 
        y0 = -y0;                                   
        y1 = -y1;
        Swap(&x0, &y0);                             
        Swap(&x1, &y1);
    }
    
    BresenhamLineAlgorithm(oled, x0, y0, x1-x0, y1-y0, rot);        // draw line using the Bresenham Line Algorithm   
}

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
void oled_FillRect(oled_t * oled, int x0, int y0, int x1, int y1)
{
    WriteGRAM(oled, x0, y0, x1, y1, oled->color);       // fill rectangle
}

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
void oled_FillRoundedRect(oled_t * oled, int x0, int y0, int x1, int y1, int r)
{
    if ((x1 - x0 < 2*r) || (y1 - y0 < 2*r)) return;     // raidus must be less than or equal to 1/2 the smallest side length
    oled_FillRect(oled, x0+r, y0, x1-r, y1);            // fill in rectangle
    oled_FillRect(oled, x0, y0+r, x0+r, y1-r);
    oled_FillRect(oled, x1-r, y0+r, x1, y1-r);
    oled_DrawPie(oled, x1-r, y0+r, r, 6, 8);            // fill in corners
    oled_DrawPie(oled, x0+r, y0+r, r, 4, 6);
    oled_DrawPie(oled, x0+r, y1-r, r, 2, 4);
    oled_DrawPie(oled, x1-r, y1-r, r, 0, 2);
}

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
void oled_DrawRect(oled_t * oled, int x0, int y0, int x1, int y1)
{
    if ((x1 < x0) || (y1 < y0)) return;     // check for bad parameters
    DrawHLine(oled, y0, x0, x1);            // draw boundries of rectange
    DrawHLine(oled, y1, x0, x1);
    DrawVLine(oled, x0, y0, y1);
    DrawVLine(oled, x1, y0, y1);
}

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
void oled_DrawRoundedRect(oled_t * oled, int x0, int y0, int x1, int y1, int r)
{
    if ((x1 - x0 < 2*r) || (y1 - y0 < 2*r)) return;     // raidus must be less than or equal to 1/2 the smallest side length
    DrawHLine(oled, y0, x0+r, x1-r);                    // draw boundries of rectange
    DrawHLine(oled, y1, x0+r, x1-r);
    DrawVLine(oled, x0, y0+r, y1-r);
    DrawVLine(oled, x1, y0+r, y1-r);
    oled_DrawArc(oled, x1-r, y0+r, r, 6, 8);            // draw rounded edges
    oled_DrawArc(oled, x0+r, y0+r, r, 4, 6);
    oled_DrawArc(oled, x0+r, y1-r, r, 2, 4);
    oled_DrawArc(oled, x1-r, y1-r, r, 0, 2);
}

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
void oled_DrawCircle(oled_t * oled, int xc, int yc, int r)
{
    BresenhamCircleAlgorithm(oled, xc, yc, r-oled->penSize, r+oled->penSize, 0xFF);     // draw circle using Bresenham's Circle Algorithm
}

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
void oled_DrawArc(oled_t * oled, int xc, int yc, int r, int a0, int a1)
{   
    if (a1 < a0) Swap(&a0, &a1);                        // make sure a0 is less than a1
    if ((a0 < 0) || (a1 > 8)) return;                   // make sure a0 and a1 are between 0 and 8 (inclusive)

    uint8 octants = (0xFF >> (8-a1)) & (0xFF << a0);    
    BresenhamCircleAlgorithm(oled, xc, yc, r-oled->penSize, r+oled->penSize, octants);  // draw arc using Bresenham's Circle Algorithm  
}

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
void oled_FillCircle(oled_t * oled, int xc, int yc, int r)
{
    BresenhamCircleAlgorithm(oled, xc, yc, 0, r, 0xFF);     // fill circle using Bresenham's Circle Algorithm (with inner radius set to 0)
}

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
void oled_DrawPoint(oled_t * oled, int xc, int yc)
{
    BresenhamCircleAlgorithm(oled, xc, yc, 0, oled->penSize, 0xFF);     // draw point using Bresenham's Circle Algorithm (same as oled_FillCircle() but with r = oled->penSize)
}

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
void oled_DrawPie(oled_t * oled, int xc, int yc, int r, int a0, int a1)
{
    if (a1 < a0) Swap(&a0, &a1);                            // make sure a0 is less than a1
    if ((a0 < 0) || (a1 > 8)) return;                       // make sure a0 and a1 are between 0 and 8 (inclusive)

    uint8 octants = (0xFF >> (8-a1)) & (0xFF << a0);    
    BresenhamCircleAlgorithm(oled, xc, yc, 0, r, octants);  // draw circular sector ("pie") using Bresenham's Circle Algorithm
}

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
void oled_DispBitmap(oled_t * oled, int x0, int y0, const uint8 * bitmap, uint16 width, uint16 height){
    uint8 color;
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            color = (bitmap[i*((width-1)/8 + 1) + j/8] >> (j%8)) & 0x01;                // get color of pixel
            
            if (oled->bmMode == OLED_BM_NORMAL){                                        // OLED_BM_NORMAL: bitmap displayed "as is"
                WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, color);
            }
            else if (oled->bmMode == OLED_BM_INV){                                      // OLED_BM_INV: bitmap displayed with pixels inverted
                WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, ~(color | 0xfe));
            }
            if (oled->bmMode == OLED_BM_WHITE_TRAN){                                    // OLED_BM_WHITE_TRAN: white pixels are displayed in the forground color, black pixels are transparent
                if (color == 1) WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, oled->color);   
            }
            else if (oled->bmMode == OLED_BM_BLACK_TRAN){                               // OLED_BM_BLACK_TRAN: black pixels are displayed in the forground color, white pixels are transparent
                if (color == 0) WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, oled->color);   
            }
            else if (oled->bmMode == OLED_BM_WHITE_FILL){                               // OLED_BM_WHITE_FILL: white pixels are displayed in the forground color, black pixels are displayed in the background color
                if (color == 1) WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, oled->color);
                else WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, oled->bkColor);
            }
            else if (oled->bmMode ==  OLED_BM_BLACK_FILL){                              // OLED_BM_BLACK_FILL: black pixels are displayed in the forground color, white pixels are displayed in the forground color
                if (color == 0) WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, oled->color); 
                else WriteGRAM(oled, x0+j, y0+i, x0+j, y0+i, oled->bkColor);
            }
        }
    }
}

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
void oled_DispChar(oled_t * oled, int x0, int y0, const char c){
    uint8 bmMode = oled_GetBmMode(oled);        // store current bitmap mode
    oled_SetBmMode(oled, oled->textMode);       // set bmMode to text mode
    int i = c - 32;
    if ((i < 0) || (i > 94)) return;            // character out of range
    oled_DispBitmap(oled, x0, y0, oled->font->characters[i], oled->font->width, oled->font->height);    // display char
    oled_SetBmMode(oled, bmMode);               // restore bitmap mode
}

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
void oled_DispString(oled_t * oled, int x0, int y0, const char * s){
    int i = 0, j = 0;
    while(s[i] != '\0'){                                                // iterate through chars in string until NUL '\0' is reached
        if (s[i] == '\n'){                                              // ENTER char, start new line
            y0 += oled->font->height;                                   
            j = 0;
        }
        else {
            oled_DispChar(oled, x0 + j*oled->font->width, y0, s[i]);    // display char
            j++;
        }
        i++;
    }
}

//======================================
// SetPageAddress()
// this function sets the oled's page start 
// address and page stop address.
//
// Parameters:
//     oled - pointer to a oled_t structure
//     A - page start address (integer in range 0-7)
//     B - page end address (integer in range 0-7)
//======================================
void SetPageAddress(oled_t * oled, uint8 A, uint8 B)
{
    if ((A > 7) || (B > 7)) return;                 // check for valid page addresses
    oled->SendStart(oled->slaveAddr, 0x00);         // generate Start condition and send slave address with R/W# bit = 0 (write mode)
    oled->WriteByte(0x00);                          // send control byte
    oled->WriteByte(SSD1306_SETPAGEADDRESS);        // send Set Page Address command
    oled->WriteByte(A);                             // send page start address
    oled->WriteByte(B);                             // send page stop address
    oled->SendStop();                               // generate Stop condition
}

//======================================
// SetColumnAddress()
// this function sets the oled's column start 
// address and column stop address
//
// Parameters:
//     oled - pointer to a oled_t structure
//     A: column start address (integer in range 0-127)
//     B: column end address (integer in range 0-127)
//======================================
void SetColumnAddress(oled_t * oled, uint8 A, uint8 B)
{
    if ((A > 127) || (B > 127)) return;             // check for valid column addresses
    oled->SendStart(oled->slaveAddr, 0x00);         // generate Start condition and send slave address with R/W# bit = 0 (write mode)
    oled->WriteByte(0x00);                          // send control byte
    oled->WriteByte(SSD1306_SETCOLUMNADDRESS);      // send Set Column Address command
    oled->WriteByte(A);                             // send column start address
    oled->WriteByte(B);                             // send column stop address
    oled->SendStop();                               // generate Stop condition
}

//======================================
// WriteGRAM()
// this functions writes the specified color to
// the rectangular block of GRAM defined by
// its upper left corner (x0, y0) and lower right
// corner (x1, y1)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - upper left x-coordinate
//     y0 - upper left y-coordinate
//     x1 - lower right x-coordinate
//     y1 - lower right y-coordinate
//======================================
void WriteGRAM(oled_t * oled, int x0, int y0, int x1, int y1, uint8 color)
{
    if (x0 < 0) x0 = 0;                                 // ensure x0, y0, x1, y1 are within display bounds
    if (x1 > OLED_WIDTH - 1) x1 = OLED_WIDTH - 1;
    if (x1 < x0) return;                                                
    if (y0 < 0) y0 = 0;
    if (y1 > OLED_HEIGHT - 1) y1 = OLED_HEIGHT - 1;
    if (y1 < y0) return;
    
    SetPageAddress(oled, y0/8, y1/8);                   // set the start page address and stop page address
    SetColumnAddress(oled, x0, x1);                     // set the start column address and stop column address
    
    oled->SendStart(oled->slaveAddr, 0x00);             // generate Start condition and send slave address with R/W# bit = 0 (write mode)
    oled->WriteByte(0x40);                              // send "GRAM write" control byte
    
    uint8 mask;
    for (int i=y0/8; i<=y1/8; i++){                     // iterate over pages that will be modified
        mask = 0x00;                                    // create a bit mask to preserve pixels that are outside the defined rectangle
        if (i == y0/8){                                               
            mask = 0xFF >> (8 - y0%8);
        }
        if (i == y1/8){
            mask = mask | (0xFF << (1 + y1%8));
        }
        for (int j=x0; j<=x1; j++){                                                             // iterate over columns
            if (color == 0){
                oled->GRAM[i*OLED_WIDTH + j] = oled->GRAM[i*OLED_WIDTH + j] & mask;             // update the byte in GRAM
            }
            else {
                oled->GRAM[i*OLED_WIDTH + j] = ~mask | (oled->GRAM[i*OLED_WIDTH + j] & mask);   // update the byte in GRAM
            }
        oled->WriteByte(oled->GRAM[i*OLED_WIDTH + j]);                                          // write byte to the display
        }
    }
    
    oled->SendStop();                                   // generate Stop condition
}

//========================================
// DrawHLine()
// this functions draws a horizontal line
// between the points (x0, y) and (x1, y)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     y - y-coordinate of line
//     x0 - x-coordinate of the first point
//     x1 - x-coordinate of the second point
//========================================
void DrawHLine(oled_t * oled, int y, int x0, int x1)
{
    if (x0 <= x1) WriteGRAM(oled, x0-oled->penSize, y-oled->penSize, x1+oled->penSize, y+oled->penSize, oled->color);   // draw horizontal line (x0 <= x1)
    else WriteGRAM(oled, x1-oled->penSize, y-oled->penSize, x0+oled->penSize, y+oled->penSize, oled->color);            // draw horizontal line (x0 > x1)
}

//========================================
// DrawVLine()
// this functions draws a vertical line
// between the points (x, y0) and (x, y1)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x - x-coordinate of line
//     y0 - y-coordinate of the first point
//     y1 - y-coordinate of the second point
//========================================
void DrawVLine(oled_t * oled, int x, int y0, int y1)
{
    if (y0 <= y1) WriteGRAM(oled, x-oled->penSize, y0-oled->penSize, x+oled->penSize, y1+oled->penSize, oled->color);   // draw vertical line (y0 <= y1)
    else WriteGRAM(oled, x-oled->penSize, y1-oled->penSize, x+oled->penSize, y0+oled->penSize, oled->color);            // draw vertical line (y0 > y1)
}

//========================================
// BresenhamLineAlgorithm()
// this function uses Bresenham's line algorithm to
// draw a line between the points (x0, y0) and 
// (x0+dx, y0+dy). Bresenham's algorithm requires
// the slope of the line to be between 0 and 1, however
// the parameter rot allows the caller to specify 
// whether the line should be rotated when plotted 
// on the display
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate of the start of the line
//     y0 - y-coordinate of the start of the line
//     dx - length of the line in the x-direction
//     dy - length of the line in the y-direction
//     rot - desired rotation (integer in range 0-3)
//========================================
void BresenhamLineAlgorithm(oled_t * oled, int x0, int y0, int dx, int dy, uint8 rot)
{
    if ((dx < 0) || (dy < 0) || (dy > dx)) return;              // make sure the slope is between 0 and 1
    
    int error = 0;                                              // initialize error to 0 
    int x = 0, y = 0;                                           // set x and y to 0 (start algorithm at the origin)
    int thresh = dx - 2*dy;                                     // decision threshold (increase y if error becomes greater than thresh)
    uint8 step = 0;                                             // this variable keeps track of whether y was incrementd on the previous itteration (1 if y was incremented, 0 otherwise)
    
    for(; x<=dx; x++){                                          // iterate over  x coordinates
        if ((x == 0) && (-2*dy + 2*dx > thresh)) step = 1;      // set step = 1 if a step would have been made right before the begining of the line
        PlotPerpLineL(oled, x0, y0, dx, dy, x, y, step, rot);
        PlotPerpLineR(oled, x0, y0, dx, dy, x, y, step, rot);
        if (error > thresh){                                    // check decision threshold
            y++;                                                // increment the y coordinate
            error += 2*dy - 2*dx;                               // update the error
            step = 1;                                           // step was made, so set step to 1
        }
        else {
            error += 2*dy;                                      // update the error
            step = 0;                                           // step was not made, so set step to 0
        }
    }
}

//========================================
// PlotPerpLineL()
// this function is a helper function for 
// BresenhamLineAlgorithm(). It uses a modified 
// version of Bresenham's line algorithm to add
// thickness to the line being drawn in BresenhamLineAlgorithm().
// It does this by plotting a perpendicular line
// to the left side of the line being drawn. The 
// starting point for this perpendicular line is
// the point (x0+x, y0+y)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate of the start of the line
//     y0 - y-coordinate of the start of the line
//     dx - length of the line in the x-direction
//     dy - length of the line in the y-direction
//     x - x-coordinate offset for the starting point
//         of the perpendicular line
//     y - y-coordinate offset for the starting point
//         of the perpendicular line
//     step - binary value which indicates if the y 
//            offset was increment in the previous
//            iteration of Bresenham's algorithm     
//     rot - desired rotation (integer in range 0-3)
//
// Note: the "line" refered to in the parameter descriptions
// is used in reference to the line being drawn in
// BresenhamLineAlgorithm() unless stated otherwise
//========================================
void PlotPerpLineL(oled_t * oled, int x0, int y0, int dx, int dy, int x, int y, int step, uint8 rot)
{
    int error = 0;
    int thresh = dx - 2*dy;             // decision threshold (decrease x if error becomes greater than thresh)
    
    for (int i=0; i<oled->penSize+1; i++){
        PlotPoint(oled, x0, y0, x, y, rot); // plot pixel
        if (error > thresh){            // check decision threshold
            x--;                        // decrement the x coordinate
            error += 2*dy - 2*dx;       // update the error
            if ((step == 1) && (i != oled->penSize)){
                PlotPoint(oled, x0, y0, x, y, rot);      // fill in "gap" pixel      
            }
        }
        else {
            error += 2*dy;              // update the error   
        }
        y++;                            // increment y on every itteratioon
    }
}

//========================================
// PlotPerpLineR()
// this function is a helper function for 
// BresenhamLineAlgorithm(). It uses a modified 
// version of Bresenham's line algorithm to add
// thickness to the line being drawn in BresenhamLineAlgorithm().
// It does this by plotting a perpendicular line
// to the right side of the line being drawn. The 
// starting point for this perpendicular line is
// the point (x0+x, y0+y)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate of the start of the line
//     y0 - y-coordinate of the start of the line
//     dx - length of the line in the x-direction
//     dy - length of the line in the y-direction
//     x - x-coordinate offset for the starting point
//         of the perpendicular line
//     y - y-coordinate offset for the starting point
//         of the perpendicular line
//     step - binary value which indicates if the y 
//            offset was increment in the previous
//            iteration of Bresenham's algorithm     
//     rot - desired rotation (integer in range 0-3)
//
// Note: the "line" refered to in the parameter descriptions
// is used in reference to the line being drawn in
// BresenhamLineAlgorithm() unless stated otherwise
//========================================
void PlotPerpLineR(oled_t * oled, int x0, int y0, int dx, int dy, int x, int y, int step, uint8 rot)
{
    int error = 0;
    int thresh = dx - 2*dy;             // decision threshold (increase x if error becomes greater than thresh)
    
    for (int i=0; i<oled->penSize+1; i++){
        if (i != 0) PlotPoint(oled, x0, y0, x, y, rot);     // plot pixel (don't plot the first pixel. This will have already been plotted by PlotPerpLineL()
        if (error > thresh){            // check decision threshold
            x++;                        // increment the x coordinate
            error += 2*dy - 2*dx;       // update the error
            if ((step == 1) && (i != oled->penSize)){
                PlotPoint(oled, x0, y0, x-1, y-1, rot);             // fill in "gap" pixel    
            }
        }
        else {
            error += 2*dy;              // update the error   
        }
        y--;                            // decrement y on every itteratioon
    }
}

//========================================
// PlotPoint()
// this function is a helper function for 
// PlotPerpLineL()/PlotPerpLineR(). It plots
// the points calculated by the line algorithms
// with the specified rotation (i.e the 
// transformation necesssary to draw lines
// with slopes less than 0 or greater than 1)
//
// Parameters:
//     oled - pointer to a oled_t structure
//     x0 - x-coordinate of the start of the line
//     y0 - y-coordinate of the start of the line
//     x - x-coordinate offset for the starting point
//         of the perpendicular line
//     y - y-coordinate offset for the starting point
//         of the perpendicular line    
//     rot - desired rotation (integer in range 0-3)
//
// Note: the "line" refered to in the parameter descriptions
// is used in reference to the line being drawn in
// BresenhamLineAlgorithm() unless stated otherwise
//========================================
void PlotPoint(oled_t * oled, int x0, int y0, int x, int y, uint8 rot)
{
    if (rot == 0) oled_DrawPixel(oled, y0+y, x0+x);            // slope > 1
    else if (rot == 1) oled_DrawPixel(oled, x0+x, y0+y);       // 0 <= slope <= 1
    else if (rot == 2) oled_DrawPixel(oled, x0+x, -y0-y);      // -1 <= slope < 0
    else if (rot == 3) oled_DrawPixel(oled, y0+y, -x0-x);      // slope < -1
}

//========================================
// BresenhamCircleAlgorithm()
// this function uses Bresenham's circle algorithm to
// draw a circle of radius r centered at (xc, yc). 
//
// Parameters:
//     oled - pointer to a oled_t structure
//     xc - x-coordinate of the center of the circle
//     yc - y-coordinate of the center of the circle
//     rIn - radius of circle
//     rOut - outer radius of circle
//     octants - 8-bit number: xxxx xxxx. Each bit
//               corresponds to an octant of the circle
//               and should be set to 1 if that octant
//               should be plotted (the LSB corresponds
//               to the octant 0)
//========================================
void BresenhamCircleAlgorithm(oled_t *oled, int xc, int yc, int rIn, int rOut, uint8 octants)
{
    if ((rIn > rOut) || (rOut == 0)) return;    // inner radius must be less than or equal to the outer radius
    int dIn = 3 - 2*rIn;                        // initialize decision parameter for inner circle
    int dOut = 3 - 2*rOut;                      // initialize decision parameter for inner circle
    int x = 0, yIn = rIn, yOut = rOut;          // initialize first point on the inner cicle to (0, rIn) and first point on the outer cicle to (0, rOut)

    while (yIn <= yOut){                        // itterate over x until x becomes larger than yIn
        PlotCircle(oled, xc, yc, x, yIn, yOut, octants);
        if (dOut < 0){                          // check outer circle desision parameter
            dOut += 4*x + 6;                    // update outer circle desision parameter
        }
        else {
            dOut += 4*(x - yOut) + 10;          // update outer circle desision parameter
            yOut--;                             // decrement yOut coordinate 
        }
        if (x < yIn){                          
            if (dIn < 0){                       // check inner circle desision parameter
                dIn += 4*x + 6;                 // update desision parameter
            }
            else {
                dIn += 4*(x - yIn) + 10;        // update inner circle desision parameter
                yIn--;                          // decrement yIn coordinate
            }
        }
        else yIn++;                             // once yIn = x, keep incrementing yIn so that yIn = x
        x++;                                    // increment x on each iteration
    }
}

//========================================
// PlotCircle()
// this function is a helper function for
// BresenhamCircleAlgorithm(). It plots
// the points calculated by the circle algorithms
//
// Parameters:
//     oled - pointer to a oled_t structure
//     xc - x-coordinate of the center of the circle
//     yc - y-coordinate of the center of the circle
//     x - x-offset for the point (octant 1)
//     yIn - y-offset for the inner point (octant 1)
//     yOut - y-offset for the outer point (octant 1)
//     octants - 8-bit number: xxxx xxxx. Each bit
//               corresponds to an octant of the circle
//               and should be set to 1 if that octant
//               should be plotted (the LSB corresponds
//               to the fist octant)
//========================================
void PlotCircle(oled_t * oled, int xc, int yc, int x, int yIn, int yOut, uint8 octants){
    if (octants & 0x01) WriteGRAM(oled, xc+yIn, yc+x, xc+yOut, yc+x, oled->color);    // plot octant 0
    if (octants & 0x02) WriteGRAM(oled, xc+x, yc+yIn, xc+x, yc+yOut, oled->color);    // plot octant 1
    if (octants & 0x04) WriteGRAM(oled, xc-x, yc+yIn, xc-x, yc+yOut, oled->color);    // plot octant 2
    if (octants & 0x08) WriteGRAM(oled, xc-yOut, yc+x, xc-yIn, yc+x, oled->color);    // plot octant 3
    if (octants & 0x10) WriteGRAM(oled, xc-yOut, yc-x, xc-yIn, yc-x, oled->color);    // plot octant 4
    if (octants & 0x20) WriteGRAM(oled, xc-x, yc-yOut, xc-x, yc-yIn, oled->color);    // plot octant 5
    if (octants & 0x40) WriteGRAM(oled, xc+x, yc-yOut, xc+x, yc-yIn, oled->color);    // plot octant 6
    if (octants & 0x80) WriteGRAM(oled, xc+yIn, yc-x, xc+yOut, yc-x, oled->color);    // plot octant 7
}

//========================================
// Swap()
// this function swaps the values of var0
// and var1
//
// Parameters:
//     var0 - pointer to var0
//     var1 - pointer to var1
//========================================
void Swap(int* var0, int* var1)
{
    int temp = *var0;
    *var0 = *var1;
    *var1 = temp;
}

//========================================
// END OF FILE
//========================================

