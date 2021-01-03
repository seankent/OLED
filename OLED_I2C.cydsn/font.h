#ifndef FONT_H
#define FONT_H

//======================================
// include                   
//======================================
#include "project.h"

//======================================
// font struct                  
//======================================
typedef struct {
    const uint8 width;
    const uint8 height;
    const uint8 characters[95][32];
} font_t;

const extern font_t font;

#endif

//========================================
// END OF FILE
//========================================
