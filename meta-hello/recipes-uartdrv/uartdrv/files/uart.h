#ifndef UART_H
#define UART_H

typedef struct
{
 char buf[1024];
 int num_of_bytes;
 int rd_idx,wr_idx;
}cbuf_t;


#endif
