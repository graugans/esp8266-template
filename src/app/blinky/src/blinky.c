/* @file
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Christian Ege <ch@ege.io>
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

/* user configuration from driver_lib */
#include "user_config.h"

/* driver_lib includes */
#include "driver/uart.h"

/* SDK includes */
#include "osapi.h"
#include "ets_sys.h"
#include "gpio.h"
#include "os_type.h"

/* ssystem includes */
#include <stdint.h>

 /******************************************
  * external variables
  ******************************************/

extern UartDevice UartDev; /* defined in the SDK code */
/******************************************
 * static variables
 ******************************************/

static volatile os_timer_t blinkTimer;

/************************************
 * forward declaration of functions
 ************************************/
void doBlink(void *arg);
static void dummyTask(os_event_t *events);


/* @brief main entry point called by the SDK */
void ICACHE_FLASH_ATTR user_init() {

    /* initialize the GPIO subsystem */
    gpio_init();

    /* enable output mode on PIN5 */
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);

    /* set PIN2 to high */
    GPIO_OUTPUT_SET(GPIO_ID_PIN(5),1);

    /* uart init */
    /* UartDev is defined in SDK code */
    UartDev.data_bits = EIGHT_BITS;
    UartDev.parity = NONE_BITS;
    UartDev.stop_bits = ONE_STOP_BIT;
    UartDev.baut_rate = BIT_RATE_115200;
    uart_init(BIT_RATE_115200, BIT_RATE_115200);

    /* clear any running timer */
    os_timer_disarm(&blinkTimer /* timer instance */);

    /* timer setup */
    os_timer_setfn(                &blinkTimer, /* timer instance */
                   (os_timer_func_t *) doBlink, /* function to be called on expiration */
                                          NULL  /* context pointer for the timer function */ );
    os_timer_arm( &blinkTimer, /* timer instance */
                          1000, /* period in ms */
                            1  /* 0 == singleshot, 1 == continuous*/  );
    ets_printf("Starting blinky demo application \n\r");
    system_set_os_print(0);

}

/** @brief do the blink stuff
 *
 * @param arg here we can provide some context to the timer
 */
void doBlink(void* arg __attribute__((unused))  /* context */  )
{
    uint32_t nextState = GPIO_INPUT_GET(GPIO_ID_PIN(5));
    GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 0x00000001 ^ nextState); /* toggle the bit */
    ets_printf("Blinky demo application \n\r");
}
