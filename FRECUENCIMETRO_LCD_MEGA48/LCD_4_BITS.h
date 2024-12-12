/*

		LCD_4_BITS.h

*/

///////////////////////////////////////////////////////////////////
//////////////////  ETIQUETAS PARA LOS PINES  /////////////////////

#define RS_PIN 0x01
#define EN_PIN 0x04
#define LED 0x08

///////////////////////////////////////////////////////////////////
//////////////////  ETIQUETAS PARA COMANDOS  //////////////////////

#define ON_DISPLAY	0x0C
#define clear	0x01
#define home	0x02
#define ADDR_LCD_ 0x4E // direccion del modulo i2c 01001110 ==>0x4E

///////////////////////////////////////////////////////////////////
////////////////////  FUNCIONES PROTOTIPO  ////////////////////////

void LCD_Init(void);
void LCD_Cursor(uint8_t X, uint8_t Y);
void LCD_Print_Str(const char *dat);
void LCD_Print_Char(uint8_t dat);
void LCD_Clear();
void Enviar_CMD_4_Bits(uint8_t CMD);
void Enviar_DAT_4_Bits(uint8_t DAT);
void Envio_Dat_I2C(uint8_t DATA);




