#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// ---------------- UART ----------------
void UART_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_print(const char *str) {
	while (*str) {
		UART_transmit(*str++);
	}
}

// ---------------- I2C (TWI0) ----------------
void I2C_init(void) {
	TWSR0 = 0x00;       // prescaler = 1
	TWBR0 = 72;         // 100kHz @ 16MHz
}

void I2C_start(uint8_t addr) {
	TWCR0 = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR0 & (1 << TWINT)));
	TWDR0 = addr;
	TWCR0 = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR0 & (1 << TWINT)));
}

void I2C_write(uint8_t data) {
	TWDR0 = data;
	TWCR0 = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR0 & (1 << TWINT)));
}

uint8_t I2C_readAck(void) {
	TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR0 & (1 << TWINT)));
	return TWDR0;
}

uint8_t I2C_readNack(void) {
	TWCR0 = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR0 & (1 << TWINT)));
	return TWDR0;
}

void I2C_stop(void) {
	TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

// ---------------- MPU9250 ----------------
#define MPU_ADDR 0x68

void MPU9250_init(void) {
	I2C_start(MPU_ADDR << 1);   // write
	I2C_write(0x6B);            // PWR_MGMT_1
	I2C_write(0x00);            // wake up
	I2C_stop();
}

int16_t MPU9250_readGyroZ(void) {
	I2C_start(MPU_ADDR << 1);   // write
	I2C_write(0x47);            // GYRO_ZOUT_H
	I2C_stop();

	I2C_start((MPU_ADDR << 1) | 1); // read
	uint8_t high = I2C_readAck();
	uint8_t low  = I2C_readNack();
	I2C_stop();

	return (high << 8) | low;
}

// ---------------- PWM (PD6 OC0A) ----------------
void PWM_init(void) {
	DDRD |= (1 << PD6); // PD6 output
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Fast PWM, non-inverting
	TCCR0B = (1 << CS01) | (1 << CS00); // prescaler = 64
}

void PWM_setDuty(uint8_t duty) {
	OCR0A = duty; // 0-255
}

// ---------------- MAIN ----------------
int main(void) {
	UART_init(F_CPU/16/9600 - 1);
	I2C_init();
	MPU9250_init();
	PWM_init();

	char buf[32];

	while (1) {
		int16_t rawZ = MPU9250_readGyroZ();
		float gyroZ = rawZ / 131.0; // °/s (±250 dps scale)

		// Map gyroZ to duty cycle
		uint8_t duty;
		if (gyroZ > 100) duty = 200;
		else if (gyroZ > 50) duty = 120;
		else duty = 50;

		PWM_setDuty(duty);

		// Debug print
		sprintf(buf, "GyroZ: %.2f dps | Duty: %d\r\n", gyroZ, duty);
		UART_print(buf);

		_delay_ms(500);
	}
}
