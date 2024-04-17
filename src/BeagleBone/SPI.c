/* SPI
*
* Author: Sam Sandelin (4/8/2024)
* Purpose: General purpose SPI communication implementation
*/

#include "SPI.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define pabort(s) perror(s); return 0;

static const char* device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;

uint8_t TransferSPI(SPI_t* spiControl, uint8_t* tx, uint8_t* rx, uint64_t len) {
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(spiControl->fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
	return 1;
}
uint8_t ReadSPI(SPI_t* spiControl, uint8_t* rx, uint64_t len) {
	struct spi_ioc_transfer spi_message = {
		.tx_buf = (unsigned long)0,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	int ret = ioctl(spiControl->fd, SPI_IOC_MESSAGE(1), &spi_message);
	if (ret < 1)
		pabort("can't read from spi");
	return 1;
}
uint8_t WriteSPI(SPI_t* spiControl, uint8_t* tx, uint64_t len) {
	struct spi_ioc_transfer spi_message = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)0,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	int ret = ioctl(spiControl->fd, SPI_IOC_MESSAGE(1), &spi_message);
	if (ret < 1)
		pabort("can't write to spi");
	return 1;
}

static void print_usage(const char* prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev0.1)\n"
		"  -s --speed    max speed (Hz)\n"
		"  -d --delay    delay (usec)\n"
		"  -b --bpw      bits per word \n"
		"  -l --loop     loopback\n"
		"  -H --cpha     clock phase\n"
		"  -O --cpol     clock polarity\n"
		"  -L --lsb      least significant bit first\n"
		"  -C --cs-high  chip select active high\n"
		"  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

uint8_t StartSPI(SPI_t* spiControl)
{
	uint8_t ret = 0;
	
	spiControl->fd = open(device, O_RDWR);
	if (spiControl->fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(spiControl->fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(spiControl->fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(spiControl->fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(spiControl->fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(spiControl->fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(spiControl->fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);

	return 1;
}

uint8_t StopSPI(SPI_t* spiControl) {
	if (spiControl->fd < 0)
		return 0;
	close(spiControl->fd);
	return 1;
}
