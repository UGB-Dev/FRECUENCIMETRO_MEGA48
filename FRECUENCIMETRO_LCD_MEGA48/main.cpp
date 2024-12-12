/*
 * FRECUENCIMETRO_LCD_MEGA48.cpp
 *
 * Created: 08/12/2024 04:24:32 a. m.
 * Author : UGB
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C_MEGA48.c"
#include "LCD_4_BITS.c"



/////////////////////////////////////////////////////////////////////////////
/////////////////////////// FUNCIONES PROTOTIPO /////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void Medir_Evento();
void Imprimir_Frecuencia();

uint32_t FREQ;
uint32_t CAPTURA_A=0;
volatile uint8_t OVERFLOW=0;
uint32_t CAPTURA_B=0;

int main(void){
	sei();
	uint32_t aux=0;
	I2C_CONFIG(MASTER);
	LCD_Init();
	_delay_ms(500);
	TIMSK1 |= (1<< TOIE1); // overflow 
    while (1){
		for (uint8_t j=0; j<10; j++){ // se realizan 10 capturas
			Medir_Evento();
			aux += FREQ;
		}
		
		FREQ = aux/10; // promedio de las capturas
		Imprimir_Frecuencia(); 
		_delay_ms(1000);
		aux=0;
    }
}

void Medir_Evento(){
	uint32_t conteo_while=0; // variable para contar el error acumulado
	TCCR1A =TCNT1=0; // inicializacion del timer
	TIFR1 = (1<<ICF1); // limpia la bandera input capture 

	TCCR1B = 0x01; // se habilita el timer
	while ((TIFR1&(1<<ICF1)) == 0); // espera una interrupcion por flanco de bajada
	CAPTURA_A = ICR1 + (65536*OVERFLOW) ; // se captura el valor de ICR1 y sobreflujo
	TIFR1 = (1<<ICF1); // limpia la bandera input capture 
	
	while ((TIFR1 & (1<<ICF1)) == 0){// espera una interrupcion por flanco de bajada
		conteo_while++; // error acumulado
	}
	CAPTURA_B = ICR1 + (65536*OVERFLOW); // se captura el valor de ICR1 y sobreflujo
	TIFR1 = (1<<ICF1); // limpia la bandera input capture 

	TCCR1B = OVERFLOW = 0; // deshabilita timer
	
	FREQ = F_CPU /  (CAPTURA_B - CAPTURA_A - (conteo_while/5)) ; // se calcula la frecuencia
}

ISR(TIMER1_OVF_vect){ // interrupcion por sobreflujo en TCNT1
	OVERFLOW++;
}

void Imprimir_Frecuencia(){
	uint8_t CM,DM,UM,CENTE,DECE,UNID;
	// se divide en sus unidades respertivas
	CM=FREQ/100000;
	FREQ%=100000;
	DM=FREQ/10000;
	FREQ%=10000;
	UM= FREQ/1000;
	FREQ%=1000;
	CENTE = FREQ/100;
	FREQ%=100;
	DECE = FREQ/10;
	UNID = FREQ%10;
	
	LCD_Clear(); // limpia pantalla
	LCD_Print_Str("Frecuencia: "); // se envia un texto
	
	LCD_Cursor(5,1); // posicion x=5 ; y=1
	// se envian los caracteres numericos
	LCD_Print_Char(CM + '0'); 
	LCD_Print_Char(DM + '0');
	LCD_Print_Char(UM + '0');
	LCD_Print_Char(CENTE + '0');
	LCD_Print_Char(DECE + '0');
	LCD_Print_Char(UNID + '0');
	LCD_Print_Str(" Hz");
}

/*
ISR(TIMER1_CAPT_vect){ // rutina de interrupcion ICP1
	if(CONTADOR%2 == 0){
		CONTADOR++;
		bandera++;
		TCNT1 = 0;
	}
	else if(CONTADOR==13){
		//cli();
		Config_Timer_ICP1_reset();
		CONTADOR=0;
	}
	else{
		Data_ICP1[CONTADOR-bandera]=ICR1;
		CONTADOR++;
	}
}

void Config_Timer_ICP1(void){ // captura de TCNT1 por flanco de bajada ICP1(PB0)
	TCCR1B |= (1<<CS10); // timer sin prescalador clck/1
	TIMSK1 |= (1<<ICIE1); // se habilita la interrupcion por ICP1
}

void Config_Timer_ICP1_reset(void){
	TCCR1B = TIMSK1 = 0; // se habilita la interrupcion por ICP1
}*/
