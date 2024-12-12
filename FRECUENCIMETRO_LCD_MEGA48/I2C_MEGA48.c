

/*

		I2C_MEGA48.c

*/

#include "I2C_MEGA48.h"

void I2C_CONFIG(bool master_slave){
/*
							  F_CPU					  
			FSCL = ------------------------------ 
					  16 + 2(TWBR)*(PRESCALER)
						
							  F_CPU							 16
			TWBR = ------------------------------  -  ----------------
					   2(PRESCALER)*(FSCL)				2(PRESCALER)
	
*/
	if (master_slave == MASTER){
		TWBR =( (F_CPU/CLK_I2C)-16)/2;  // ajuste de frecuencia en SCL a 100KHz a 8MHz
		TWCR =(1<<TWEN);
	} 
	else{
		TWAR = (uint8_t)(ADDRS_SLAVE<<1);
		TWCR =(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	}
	sei();
}

///////////////////////////////////////////////////////////////////
/////////////  CONDICIONES DE INICIO Y FIN DE TRAMA  //////////////

void I2C_START(){
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
	while(!( TWCR & (1<<TWINT) ));
}

void I2C_STOP(){
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

///////////////////////////////////////////////////////////////////
////////////////   ENVIO Y RECEPCION DE DATOS  ////////////////////

void I2C_WRITE(uint8_t data){
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Esperar transmisión
}
