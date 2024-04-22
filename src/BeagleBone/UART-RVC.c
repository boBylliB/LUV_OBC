#include "UART-RVC.h"
#include "settings.h"

#include <fcntl.h>
#include <linux/serial.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

static uint8_t prevIndex = 0;
UARTRVC_t* UARTRVC_new(int uart_no, int timeout_ms, uint8_t blocking) {
    int fd = open_uartrvc(uart_no, blocking);
    if (fd < 0) {
        fprintf(stderr, "open_uart fail\n");
        return NULL;
    }

    UARTRVC_t* uartrvc = (UARTRVC_t*)malloc(sizeof(UARTRVC_t));
    if (!uartrvc) {
        fprintf(stderr, "malloc fail\n");
        return NULL;
    }

    uartrvc->fd = fd;
    uartrvc->buffer_ix = 0;
    uartrvc->timeout_ms = timeout_ms;
    memset(uartrvc->buffer, 0, 19);
    clock_gettime(CLOCK_MONOTONIC_RAW, &uartrvc->packet_start);

    prevIndex = 0;

    return uartrvc;
}

int UARTRVC_read(UARTRVC_t* uartrvc, Vector_t* orientation, Vector_t* acceleration) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    uint64_t delta_ms = (now.tv_sec - uartrvc->packet_start.tv_sec) * 1000 + (now.tv_nsec - uartrvc->packet_start.tv_nsec) / 1000000;

    // If packet has timed out, just reset the buffer.
    if (delta_ms > uartrvc->timeout_ms) {
        uartrvc->buffer_ix = 0;
    }
    int to_read = 19 - uartrvc->buffer_ix;
    //    fprintf(stderr, "attempting to read from input serial buffer\n");
    ssize_t count = read(uartrvc->fd, uartrvc->buffer + uartrvc->buffer_ix, to_read);
    //    fprintf(stderr, "read %d from input serial buffer\n", count);
    if (count <= 0) {
        errno = count == 0 ? EAGAIN : errno;
        return -1;
    }
    if (uartrvc->buffer_ix == 0) {
        // search for the HEADER bytes.
        for (int i = 1; i < count; i++) {
            //fprintf(stderr, "buffer: %d\n", uartrvc->buffer[i]);
            if (uartrvc->buffer[i-1] == UARTRVC_HEADER && uartrvc->buffer[i] == UARTRVC_HEADER) {
                // found the header; throw away everything in the buffer up to this point

                // number of bytes we've read from a valid packet
                int packet_count = count - i;

                // shift bytes from header onward to front of the buffer
                memmove(uartrvc->buffer, uartrvc->buffer + i, packet_count);
                uartrvc->buffer_ix = packet_count;

                // update packet start timestamp
                clock_gettime(CLOCK_MONOTONIC_RAW, &uartrvc->packet_start);

                break;
            }
        }
    }
    else {
        uartrvc->buffer_ix += count;
    }

    if (uartrvc->buffer_ix == 19) {
        // we have a full packet, now we must decode it.
        uartrvc->buffer_ix = 0;

        return decode_uart_packet(uartrvc->buffer, orientation, acceleration);
    }
    else if (count == to_read) {
        // we don't have a full packet but there may still be more to read
        return UARTRVC_read(uartrvc, orientation, acceleration);
    }

    // didn't get a full, valid packet
    errno = EAGAIN;
    return -1;
}

uint8_t flipBits(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}
void flipPacket(uint8_t* packet) {
    uint8_t temp;
    for (int idx = 0; idx < 8; ++idx) {
        temp = flipBits(packet[idx]);
        packet[idx] = flipBits(packet[16 - idx]);
        packet[16 - idx] = temp;
    }
}
int decode_uart_packet(uint8_t* packet, Vector_t* orientation, Vector_t* acceleration) {
    // flipPacket(packet);
    //uint8_t index = packet[0];
    //if (index != prevIndex + 1)
        //fprintf(stderr, "Decoded UARTRVC index indicates missed packets!\n");
    //prevIndex = index;
    // Orientation is given in increments of 0.01 degrees, want increments of 1 degree
    orientation->z = (DEGTORAD)*(double)((int)packet[2] << 8 + (int)packet[1]) / 100;
    orientation->y = (DEGTORAD)*(double)((int)packet[4] << 8 + (int)packet[3]) / 100;
    orientation->x = (DEGTORAD)*(double)((int)packet[6] << 8 + (int)packet[5]) / 100;
    fprintf(stderr, "Unfiltered X: 0x%X%X\n", packet[6], packet[5]);
    // Acceleration is given in mg, want m/s^2
    acceleration->z = 9.81 * (double)((int)packet[8] << 8 + (int)packet[7]) / 1000000;
    acceleration->y = 9.81 * (double)((int)packet[10] << 8 + (int)packet[9]) / 1000000;
    acceleration->x = 9.81 * (double)((int)packet[12] << 8 + (int)packet[11]) / 1000000;
    // Calculate what the checksum should be from the received data
    uint8_t checksum = 0;
    for (int idx = 0; idx < 16; ++idx) checksum += packet[idx];

    if (checksum == packet[16])
        return 0;
    else {
        //fprintf(stderr, "Read invalid checksum!\n");
        return -1;
    }
}

void UARTRVC_close(UARTRVC_t* uartrvc) {
    close(uartrvc->fd);
}

int open_uartrvc(int uart_no, uint8_t blocking) {
    char tty_dev[256];
    snprintf(tty_dev, 256, "/dev/ttyS%d", uart_no);
    int fd = open(tty_dev, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "Error when trying to open\n");
        return -1;
    }

    struct termios options;
    struct serial_struct serinfo;
    int rate = UARTRVC_BAUD_RATE;

    serinfo.reserved_char[0] = 0;
    if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0) {
        fprintf(stderr, "ioctl error\n");
        return -1;
    }

    serinfo.flags &= ~ASYNC_SPD_MASK;
    serinfo.flags |= ASYNC_SPD_CUST;
    serinfo.custom_divisor = (serinfo.baud_base + (rate / 2)) / rate;
    if (serinfo.custom_divisor < 1) {
        serinfo.custom_divisor = 1;
    }
    if (ioctl(fd, TIOCSSERIAL, &serinfo) < 0) {
        fprintf(stderr, "Error with TIOCSSERIAL\n");
        return -1;
    }
    if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0) {
        fprintf(stderr, "Error with TIOCGSERIAL\n");
        return -1;
    }
    if (serinfo.custom_divisor * rate != serinfo.baud_base) {
        fprintf(stderr, "actual baudrate is %d / %d = %f",
            serinfo.baud_base, serinfo.custom_divisor,
            (float)serinfo.baud_base / serinfo.custom_divisor);
    }

    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &options);
    cfsetispeed(&options, B38400);
    cfsetospeed(&options, B38400);
    cfmakeraw(&options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CRTSCTS;
    options.c_cc[VMIN] = blocking ? 1 : 0;
    options.c_cc[VTIME] = 1;
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        fprintf(stderr, "tcsetattr error\n");
        return -1;
    }

    return fd;
}
