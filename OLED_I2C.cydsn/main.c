//======================================
// include                                         
//======================================
#include "oled.h"

//======================================
// function declarations                                       
//======================================
void DemoGraphics(void);
void DemoText(void);
void DemoBitmap(void);

//======================================
// main
//======================================
int main(void)
{
    CyGlobalIntEnable;      // enable global interrupts
    I2C_OLED_Start();       // initialize I2C_OLED
    CyDelay(100);           // delay while OLED powers on

    //DemoGraphics();
    DemoText();
    //DemoBitmap();
    
    for(;;){}
}

//========================================
// DemoGraphics
//========================================
void DemoGraphics()
{
    oled_t oled;
    uint8 slaveAddr = 0x3c;         // OLED slave address
    oled_Init(&oled, slaveAddr, I2C_OLED_MasterSendStart, I2C_OLED_MasterSendStop, I2C_OLED_MasterWriteByte);   // initialize OLED
    oled_SetPenSize(&oled, 1);      // set pen size to 1
    int delay = 1000;
    for (;;){
        oled_Clear(&oled);                                  // oled_Clear()
        CyDelay(delay);
        oled_DrawLine(&oled, 32, 32, 96, 32);               // oled_DrawLine()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_DrawLine(&oled, 32, 16, 96, 48);  
        CyDelay(delay);
        oled_Clear(&oled);
        oled_DrawLine(&oled, 64, 16, 64, 48);  
        CyDelay(delay);
        oled_Clear(&oled);
        oled_FillRect(&oled, 32, 16, 96, 48);               // oled_FillRect()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_FillRoundedRect(&oled, 32, 16, 96, 48, 10);    // oled_FillRoundedRect()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_DrawRect(&oled, 32, 16, 96, 48);               // oled_DrawRect()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_DrawRoundedRect(&oled, 32, 16, 96, 48, 10);    // oled_DrawRoundedRect()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_DrawCircle(&oled,64, 32, 20);                  // oled_DrawCircle()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_DrawArc(&oled,64, 32, 20, 5, 8);               // oled_DrawCircle()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_FillCircle(&oled,64, 32, 20);                  // oled_FillCircle()
        CyDelay(delay);
        oled_Clear(&oled);
        oled_DrawPie(&oled,64, 32, 20, 5, 8);               // oled_DrawPie()
        CyDelay(delay);
        oled_Clear(&oled);   
    }
}

//========================================
// oled_DemoText
//========================================
void DemoText(void)
{
    oled_t oled;
    uint8 slaveAddr = 0x3c;         // OLED slave address
    oled_Init(&oled, slaveAddr, I2C_OLED_MasterSendStart, I2C_OLED_MasterSendStop, I2C_OLED_MasterWriteByte);   // initialize OLED
    oled_Clear(&oled);
    oled_DispString(&oled, 0, 0, "WELCOME\nTO\nELECTRONIX\nFIRST");
}

//========================================
// DemoBitmap
//========================================
void DemoBitmap(void)
{
    oled_t oled;
    uint8 slaveAddr = 0x3c;                 // OLED slave address
    oled_Init(&oled, slaveAddr, I2C_OLED_MasterSendStart, I2C_OLED_MasterSendStop, I2C_OLED_MasterWriteByte);   // initialize OLED
    oled_Clear(&oled);
    int MIT_width = 64, MIT_height = 38;    // height and width of bitmap
    static unsigned char MIT_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0xff, 0x3f, 0xf8, 0xc1, 0x0f, 0x7e,
        0xf0, 0x83, 0xff, 0x3f, 0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0xff, 0x3f,
        0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0xff, 0x3f, 0xf8, 0xc1, 0x0f, 0x7e,
        0xf0, 0x83, 0xff, 0x3f, 0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0xff, 0x3f,
        0xf8, 0xc1, 0x0f, 0x7e, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xc1, 0x0f, 0x7e,
        0x00, 0x00, 0x00, 0x00, 0xf8, 0xc1, 0x0f, 0x7e, 0x00, 0x00, 0x00, 0x00,
        0xf8, 0xc1, 0x0f, 0x7e, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xc1, 0x0f, 0x7e,
        0x00, 0x00, 0x00, 0x00, 0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0x1f, 0x00,
        0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0x1f, 0x00, 0xf8, 0xc1, 0x0f, 0x7e,
        0xf0, 0x83, 0x1f, 0x00, 0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0x1f, 0x00,
        0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0x1f, 0x00, 0xf8, 0xc1, 0x0f, 0x7e,
        0xf0, 0x83, 0x1f, 0x00, 0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0x1f, 0x00,
        0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0x1f, 0x00, 0xf8, 0xc1, 0x0f, 0x7e,
        0xf0, 0x83, 0x1f, 0x00, 0xf8, 0xc1, 0x0f, 0x7e, 0xf0, 0x83, 0x1f, 0x00,
        0xf8, 0x01, 0x00, 0x7e, 0xf0, 0x83, 0x1f, 0x00, 0xf8, 0x01, 0x00, 0x7e,
        0xf0, 0x83, 0x1f, 0x00, 0xf8, 0x01, 0x00, 0x7e, 0xf0, 0x83, 0x1f, 0x00,
        0xf8, 0x01, 0x00, 0x7e, 0xf0, 0x83, 0x1f, 0x00, 0xf8, 0x01, 0x00, 0x7e,
        0xf0, 0x83, 0x1f, 0x00, 0xf8, 0x01, 0x00, 0x7e, 0xf0, 0x83, 0x1f, 0x00,
        0xf8, 0x01, 0x00, 0x7e, 0xf0, 0x83, 0x1f, 0x00, 0xf8, 0x01, 0x00, 0x7e,
        0xf0, 0x83, 0x1f, 0x00, 0xf8, 0x01, 0x00, 0x7e, 0xf0, 0x83, 0x1f, 0x00,
        0xf8, 0x01, 0x00, 0x7e, 0xf0, 0x83, 0x1f, 0x00, 0xf8, 0x01, 0x00, 0x7e,
        0xf0, 0x83, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00 
    };
    oled_DispBitmap(&oled, 32, 13, MIT_bits, MIT_width, MIT_height);    // display bitmap
}

//======================================
// END OF FILE
//======================================