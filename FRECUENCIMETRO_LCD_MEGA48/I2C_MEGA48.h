/*

		I2C_MEGA48.h

*/

#define MASTER 1
#define ADDRS_SLAVE 0xF8
#define CLK_I2C 100000

///////////////////////////////////////////////////////////////////
////////////////////  FUNCIONES PROTOTIPO  ////////////////////////

void I2C_CONFIG(bool master_slave);
void I2C_START();
void I2C_STOP();
void I2C_WRITE(uint8_t data);