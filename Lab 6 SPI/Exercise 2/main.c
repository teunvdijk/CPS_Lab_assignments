#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// ---- SPI0 Master Init ----
void SPI0_MasterInit(void) {
	// MOSI0 (PB3), SCK0 (PB5), SS0 (PB2) as outputs
	DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);
	// MISO0 (PB4) as input
	DDRB &= ~(1 << PB4);

	// SPI Control Register 0:
	// SPE0 = enable SPI0
	// MSTR0 = master mode
	// CPOL0=1, CPHA0=1 ? Mode 3
	// SPR00=1 ? Fosc/16
	SPCR0 = (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA) | (1 << SPR0);
}

// ---- SPI0 Master Transmit ----
uint8_t SPI0_MasterTransmit(uint8_t data) {
	SPDR0 = data; // write data to SPI Data Register 0
	while (!(SPSR0 & (1 << SPIF))); // wait until transfer complete
	return SPDR0; // return received byte (from MISO0)
}

// ---- Main ----
int main(void) {
	SPI0_MasterInit();

	while (1) {
		PORTB &= ~(1 << PB2);     // SS LOW ? select slave
		SPI0_MasterTransmit(0xAA); // send 0xAA (10101010)
		PORTB |= (1 << PB2);      // SS HIGH ? deselect slave
		_delay_ms(1000);
	}
}
