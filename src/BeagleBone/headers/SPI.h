#ifndef SPI_H
#define SPI_H

/* SPI
*
* Author: Sam Sandelin (4/8/2024)
* Purpose: General purpose SPI communication implementation
*
* Pulled and modified from the torvalds spidev_text.c (https://raw.githubusercontent.com/torvalds/linux/master/tools/spi/spidev_test.c)
*/

typedef struct {
    int fd;
} SPI_t;

uint8_t TransferSPI(SPI_t* spiControl, uint8_t* tx, uint8_t* rx, uint64_t len);
uint8_t ReadSPI(SPI_t* spiControl, uint8_t* rx, uint64_t len);
uint8_t WriteSPI(SPI_t* spiControl, uint8_t* tx, uint64_t len);
uint8_t StartSPI(SPI_t* spiControl);
uint8_t StopSPI(SPI_t* spiControl);

#endif