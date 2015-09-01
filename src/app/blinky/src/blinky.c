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

#include "user_config.h" /* This is needed by the SDK */

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

    /* enable output mode on PIN2 */
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    /* set PIN2 to high */
    GPIO_OUTPUT_SET(GPIO_ID_PIN(2),1);

    /* clear any running timer */
    os_timer_disarm(&blinkTimer /* timer instance */);

    /* timer setup */
    os_timer_setfn(                &blinkTimer, /* timer instance */
                   (os_timer_func_t *) doBlink, /* function to be called on expiration */
                                          NULL  /* context pointer for the timer function */ );
    os_timer_arm( &blinkTimer, /* timer instance */
                          500, /* period in ms */
                            1  /* 0 == singleshot, 1 == continuous*/  );

}

/** @brief do the blink stuff
 *
 * @param arg here we can provide some context to the timer
 */
void doBlink(void* __attribute__((unused)) arg /* context */  )
{
    uint32_t nextState = GPIO_INPUT_GET(GPIO_ID_PIN(2));
    GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 0x00000001 ^ nextState); /* toggle the bit */
}
