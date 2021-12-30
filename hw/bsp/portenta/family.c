/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019
 *    William D. Jones (thor0505@comcast.net),
 *    Ha Thach (tinyusb.org)
 *    Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de
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
 * This file is part of the TinyUSB stack.
 */

#include "stm32h7xx_hal.h"
#include "bsp/board.h"
#include "board.h"
#include "../../../hw/mcu/st/stm32h7xx_hal_driver/Inc/stm32h7xx_hal_i2c.h"

//--------------------------------------------------------------------+
// Forward USB interrupt events to TinyUSB IRQ Handler
//--------------------------------------------------------------------+

// Despite being call USB2_OTG
// OTG_FS is marked as RHPort0 by TinyUSB to be consistent across stm32 port
void OTG_FS_IRQHandler(void)
{
  tud_int_handler(0);
}

// Despite being call USB2_OTG
// OTG_HS is marked as RHPort1 by TinyUSB to be consistent across stm32 port
void OTG_HS_IRQHandler(void)
{
  tud_int_handler(1);
}


//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM
//--------------------------------------------------------------------+

UART_HandleTypeDef UartHandle;
I2C_HandleTypeDef hI2C;

void board_init(void)
{
  board_stm32h7_clock_init();

  // Enable All GPIOs clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE(); // USB ULPI NXT
  __HAL_RCC_GPIOC_CLK_ENABLE(); // USB ULPI NXT
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE(); // USB ULPI NXT
  __HAL_RCC_GPIOI_CLK_ENABLE(); // USB ULPI NXT
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  // Enable UART Clock
  UART_CLK_EN();


  // 1ms tick timer
  SysTick_Config(SystemCoreClock / 1000);


  
  GPIO_InitTypeDef  GPIO_InitStruct;

  // LED
  GPIO_InitStruct.Pin   = LED_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = GPIO_PIN_5;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);
  // i2c
  // GPIO_InitTypeDef GPIO_InitStruct;
 
  GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  // __GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  
  hI2C.Instance = I2C1;
  hI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hI2C.Init.ClockSpeed = 10240;
  hI2C.Init.OwnAddress1 = 0x5A;
//  hI2C.Init.DutyCycle = I2C_DUTYCYCLE_2;
  HAL_I2C_Init(&hI2C);
  __I2C1_CLK_ENABLE();
  
  
//  uint8_t data = 0x42;
//  uint8_t data1 = 1;
  uint8_t outbuffer[2] = {0x42, 1};
  HAL_I2C_Master_Transmit(&hI2C, 0x08, outbuffer, 2, 1); 
//  HAL_I2C_Master_Transmit(&hI2C, 0x08, &data1, 1, 1); 
     // LDO3 to 1.2V
//  data[0] = 0x52;
//  data[1] = 0x9;
  uint8_t outbuffer2[2] = {0x52, 0x9};
  HAL_I2C_Master_Transmit(&hI2C, 0x08, outbuffer2, 2, 1); 
 // data[0] = 0x53;
 // data[1] = 0xF;
  uint8_t outbuffer3[2] = {0x53, 0xF};
  HAL_I2C_Master_Transmit(&hI2C, 0x08, outbuffer3, 2, 1); 

    // SW2 to 3.3V (SW2_VOLT)
//  data[0] = 0x3B;
//  data[1] = 0xF;
  uint8_t outbuffer4[2] = {0x3B, 0xF};
  HAL_I2C_Master_Transmit(&hI2C, 0x08, outbuffer4, 2, 1); 
    // SW1 to 3.0V (SW1_VOLT)
//  data[0] = 0x35;
//  data[1] = 0xF;
  uint8_t outbuffer5[2] = {0x35, 0xF};
  HAL_I2C_Master_Transmit(&hI2C, 0x08, outbuffer5, 2, 1); 
  
   // Button
  GPIO_InitStruct.Pin   = BUTTON_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

  // Uart
  GPIO_InitStruct.Pin       = UART_TX_PIN | UART_RX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = UART_GPIO_AF;
  HAL_GPIO_Init(UART_GPIO_PORT, &GPIO_InitStruct);

  UartHandle.Instance        = UART_DEV;
  UartHandle.Init.BaudRate   = CFG_BOARD_UART_BAUDRATE;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&UartHandle);


  // Disable VBUS sense (B device) via pin PA9
  USB_OTG_FS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

  // B-peripheral session valid override enable
  USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;
  USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;

  struct {
    GPIO_TypeDef* port;
    uint32_t pin;
  } const ulpi_pins[] =
  {
    ULPI_PINS
  };

  for (uint8_t i=0; i < sizeof(ulpi_pins)/sizeof(ulpi_pins[0]); i++)
  {
    GPIO_InitStruct.Pin       = ulpi_pins[i].pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
    HAL_GPIO_Init(ulpi_pins[i].port, &GPIO_InitStruct);
  }

  // Enable USB HS & ULPI Clocks
  __HAL_RCC_USB1_OTG_HS_ULPI_CLK_ENABLE();
  __HAL_RCC_USB1_OTG_HS_CLK_ENABLE();


  // No VBUS sense
  USB_OTG_HS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

  // B-peripheral session valid override enable
  USB_OTG_HS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;
  USB_OTG_HS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;


  // Force device mode
  USB_OTG_HS->GUSBCFG &= ~USB_OTG_GUSBCFG_FHMOD;
  USB_OTG_HS->GUSBCFG |= USB_OTG_GUSBCFG_FDMOD;

//  HAL_PWREx_EnableUSBVoltageDetector();

  // For waveshare openh743 ULPI PHY reset walkaround
  board_stm32h7_post_init();


}

//--------------------------------------------------------------------+
// Board porting API
//--------------------------------------------------------------------+

void board_led_write(bool state)
{
//  (void)state;
	HAL_GPIO_WritePin(LED_PORT, LED_PIN, state ? LED_STATE_ON : (1-LED_STATE_ON));
}

uint32_t board_button_read(void)
{
  return (BUTTON_STATE_ACTIVE == HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN)) ? 1 : 0;
}

int board_uart_read(uint8_t* buf, int len)
{
  (void) buf; (void) len;
  return 0;
}

int board_uart_write(void const * buf, int len)
{
  HAL_UART_Transmit(&UartHandle, (uint8_t*)(uintptr_t) buf, len, 0xffff);
  return len;
}


#if CFG_TUSB_OS == OPT_OS_NONE
volatile uint32_t system_ticks = 0;
void SysTick_Handler(void)
{
  system_ticks++;
}

uint32_t board_millis(void)
{
  return system_ticks;
}
#endif

void tud_cdc_line_coding_cb(__unused uint8_t itf, cdc_line_coding_t const* p_line_coding) {
    if (p_line_coding->bit_rate == 1200) {
  		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, 1);
  		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_6, 0);
  		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5, 1);
        reboot1200bps();
    }
}

void HardFault_Handler(void)
{
  asm("bkpt");
}

// Required by __libc_init_array in startup code if we are compiling using
// -nostdlib/-nostartfiles.
void _init(void)
{

}
