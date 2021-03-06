/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bsp/board.h"
#include "tusb.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include <math.h>
//------------- prototypes -------------//
static void cdc_task(void);

/*------------- MAIN -------------*/
int main(void)
{
  SCB->VTOR = 0x08040000;

  board_init();

  tusb_init();
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, 1);
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5, 0);
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_6, 1);

  while (1)
  {
    tud_task(); // tinyusb device task
    cdc_task();
  }

  return 0;
}

#define LED_ON 1
#define LED_OFF 0

bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
  // nothing to with DATA & ACK stage
  if (stage != CONTROL_STAGE_SETUP) return true;

  switch (request->bmRequestType_bit.type)
  {
    case TUSB_REQ_TYPE_VENDOR:
      if (request->bRequest == LED_ON)
      {
		if ( request->wIndex == 0 )
			{
				HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, 0);
//		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5, 1);
			}
	    else if ( request->wIndex == 1 )
			{
				HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5, 0);
			}
      }
	  else if (request->bRequest == LED_OFF)
      {
		if ( request->wIndex == 0 )
			{
				HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, 1);
//		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5, 1);
			}
	    else if ( request->wIndex == 1 )
			{
				HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5, 1);
			}
      }
        // response with status OK
        return tud_control_status(rhport, request);
      }

  // stall unknown request
  return 1;
}


void board_led_write2(bool state)
{
  (void)state;
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, 1);
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5, 1);
}
// echo to either Serial0 or Serial1
// with Serial0 as all lower case, Serial1 as all upper case
static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count)
{
  for(uint32_t i=0; i<count; i++)
  {
    if (itf == 0)
    {
      // echo back 1st port as lower case
      if (isupper(buf[i])) buf[i] += 'a' - 'A';
    }
    else
    {
      // echo back 2nd port as upper case
      if (islower(buf[i])) buf[i] -= 'a' - 'A';
    }

    tud_cdc_n_write_char(itf, buf[i]);
  }
  tud_cdc_n_write_flush(itf);
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
static void cdc_task(void)
{
  uint8_t itf;

  for (itf = 0; itf < CFG_TUD_CDC; itf++)
  {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_n_connected(itf) )
    {
      if ( tud_cdc_n_available(itf) )
      {

//		board_led_write2(1); 

        uint8_t buf[64];

        uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

        // echo back to both serial ports
        echo_serial_port(0, buf, count);
        echo_serial_port(1, buf, count);
      }
    }
  }
}
