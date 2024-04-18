#ifndef UARTRVC_H
#define UARTRVC_H

#include <stdint.h>
#include <time.h>
#include "Quaternion.h"

// BNO08X UART-RVC protocol constants
#define UARTRVC_HEADER 0xAA
#define UARTRVC_BAUD_RATE 115200

typedef struct {
    int fd;
    uint8_t buffer[19];
    uint8_t index;
    int buffer_ix;
    int timeout_ms;
    struct timespec packet_start;
} UARTRVC_t;

// External Functions
UARTRVC_t* UARTRVC_new(int uart_no, int timeout_ms, uint8_t blocking);
int UARTRVC_read(UARTRVC_t* uartrvc, Vector_t* orientation, Vector_t* acceleration);
void UARTRVC_close(UARTRVC_t* uartrvc);
// Internal Functions
int open_uartrvc(int uart_no, uint8_t blocking);
int decode_uart_packet(uint8_t* packet, Vector_t* orientation, Vector_t* acceleration);

#endif
