#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rm67162.h>
#include "Arduino.h"
#include "driver/spi_master.h"
#include "doomdef.h"

#include "lprintf.h"  // jff 08/03/98 - declaration of lprintf

#define MEM_PER_TRANS SCREENHEIGHT * 8

extern spi_device_handle_t spi;
extern int16_t lcdpal[256];
u_int16_t *screen;

void spi_lcd_init() {
  rm67162_init();
  lcd_setRotation(0);
  screen = malloc(MEM_PER_TRANS * 2);
}

void spi_lcd_wait_finish() {
}


int frame = 0;

void spi_lcd_send(uint16_t *scr) {
  uint8_t *currFbPtr = (uint8_t*)scr;
  spi_transaction_t *rtrans;

  size_t len = SCREENWIDTH * SCREENHEIGHT;
  lcd_address_set(0, 0,  SCREENHEIGHT - 1, SCREENWIDTH - 1);

  TFT_CS_L;

  bool first_send = 1;
  int pos = 0;
  int x=SCREENWIDTH-1;
  int y=0;
  int y_index=0;

  do {
    size_t chunk_size = len;

    spi_transaction_ext_t t = {0};
    memset(&t, 0, sizeof(t));
    if (first_send)
    {
      t.base.flags = SPI_TRANS_MODE_QIO /* | SPI_TRANS_MODE_DIOQIO_ADDR */;
      t.base.cmd = 0x32 /* 0x12 */;
      t.base.addr = 0x002C00;
      first_send = 0;
    }
    else
    {
      t.base.flags = SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD |
                     SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY;
      t.command_bits = 0;
      t.address_bits = 0;
      t.dummy_bits = 0;
    }

    if (chunk_size > MEM_PER_TRANS)
    {
      chunk_size = MEM_PER_TRANS;
    }    
    
    for (int i=0; i<chunk_size; i++) {
        int index = x+y_index;
        uint32_t d=currFbPtr[index];
				screen[i+0]=lcdpal[(d>>0)&0xff];
        y++;
        y_index+=SCREENWIDTH;
        if(y>=SCREENHEIGHT) {
          y=0;
          y_index=0;
          x--;
        }
    }
    t.base.tx_buffer = screen;
    t.base.length = chunk_size * 16;    

    spi_device_polling_transmit(spi, (spi_transaction_t *)&t);
    len -= chunk_size;  
    pos += chunk_size;  
  } while (len > 0);
  
  TFT_CS_H;
  //lcd_PushColors_rect(0, 0, 16, 16, screen);
  frame++;
}