/*

		LCD_4_BITS.c

*/

#include "LCD_4_BITS.h"

///////////////////////////////////////////////////////////////////
//////////////////   INICIALIZACION DEL LCD  //////////////////////

void LCD_Init(void){
	_delay_ms(20);
	
	Envio_Dat_I2C(EN_PIN|0x30) ;
	Envio_Dat_I2C(0x30);
	//_delay_ms(5);
	 
	Envio_Dat_I2C(EN_PIN|0x30);
	Envio_Dat_I2C(0x30);
	//_delay_ms(100);
	
	Envio_Dat_I2C(EN_PIN|0x30);
	Envio_Dat_I2C(0x30);
	//_delay_ms(100);
	
	Envio_Dat_I2C(EN_PIN|0x20);
	Envio_Dat_I2C(0x20);
	//_delay_ms(100);
	
	Enviar_CMD_4_Bits(0x28); // entrada de 4 bits y 2 lineas
	Enviar_CMD_4_Bits(0x08); // cursor apagado
	Enviar_CMD_4_Bits(clear); // limpia el lcd
	Enviar_CMD_4_Bits(0x06); // modo incremento sin desplazamiento
	Enviar_CMD_4_Bits(ON_DISPLAY); // display encendido
}

///////////////////////////////////////////////////////////////////
////////////   ENVIO DE 4 BITS PARA COMANDOS Y DATOS  /////////////

void Enviar_CMD_4_Bits(uint8_t CMD){ // divide y envia el comando en 2 nibles
	Envio_Dat_I2C(EN_PIN|LED|(CMD & 0xF0)); // se envia la parte alta del byte
	Envio_Dat_I2C(LED|(CMD & 0xF0));
	Envio_Dat_I2C(EN_PIN|LED|(CMD << 4)); // se envia la parte baja del byte
	Envio_Dat_I2C(LED|(CMD << 4));
	
	if (CMD == clear || CMD == home){ // si el comando es clear o home
		_delay_ms(2); // espera 2 ms
	} 
	else{ // de lo contrario espera 60 ms
		_delay_ms(60);
	}
}

void Enviar_DAT_4_Bits(uint8_t DAT){ // divide y envia el dato en 2 nibles
	Envio_Dat_I2C(EN_PIN|RS_PIN|LED|(DAT & 0xF0)); // se envia la parte alta del byte
	Envio_Dat_I2C(RS_PIN|LED|(DAT & 0xF0));
	Envio_Dat_I2C(EN_PIN|RS_PIN|LED|(DAT << 4)); // se envia la parte baja del byte
	Envio_Dat_I2C(RS_PIN|LED|(DAT << 4));
	//_delay_ms(60);
}

///////////////////////////////////////////////////////////////////
//////////////   FUNCIONES PARA CONTROLAR EL LCD  /////////////////

void LCD_Cursor(uint8_t X, uint8_t Y){ // posicion del cursor
	uint8_t DIR_MEM;
	
	switch(Y){
		case 0:
		DIR_MEM = 0x00; break; // direccion de memoria linea 0
		case 1:
		DIR_MEM = 0x40; break; // direccion de memoria linea 1
		case 2:
		DIR_MEM = 0x14; break; // direccion de memoria linea 2
		case 3:
		DIR_MEM = 0x54; break; // direccion de memoria linea 3
		default:
		DIR_MEM = 0x00; break;
	}
	
	DIR_MEM+=X; // incrementa columna
	Enviar_CMD_4_Bits(DIR_MEM | 0x80); // envia la posicion X,Y
	
}

void LCD_Print_Str(const char *dat){ // escribe la cadena de texto
	while(*dat!=0){ // si no es diferente de cero
		Enviar_DAT_4_Bits(*dat); // envia el caracter
		dat++; // incrementa al siguiente caracter
	}
}

void LCD_Print_Char(uint8_t dat){ // escribe un caracter
	Enviar_DAT_4_Bits(dat); // envia elcaracter
}

void Envio_Dat_I2C(uint8_t DATA){ // envia el dato por I2C
	I2C_START(); // inicio de trama
	I2C_WRITE(ADDR_LCD_); // se envia la direccion de esclavo
	I2C_WRITE(DATA); // envia el dato
	I2C_STOP(); // fin de trama
}

void LCD_Clear(){ // limpia caracteres del LCD
	Enviar_CMD_4_Bits(clear); // envia comando clear
}
