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
#include <user_interface.h>
#include <espconn.h>

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
void check_station_info(void);

void wifiEventHandler(System_Event_t *event);

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
    ets_printf("\n\r");
    ets_printf("\n\r");
    ets_printf("---------------------------------------------------\n\r");
    ets_printf("Starting Wifi SOFTAP demo application \n\r");
    ets_printf("Espressif SDK version  : %s \n\r",system_get_sdk_version());
    ets_printf("Espressif BOOT version : 0x03%x \n\r", (int) system_get_boot_version());
    ets_printf("CPU Speed              : %d MHz\n\r", system_get_cpu_freq());
    ets_printf("Mem information        :  \n\r");
    system_print_meminfo(); /* dump some memory information */
    ets_printf("---------------------------------------------------\n\r");
    system_set_os_print(0); /* disabling those chatty system messages */

    wifi_softap_dhcps_stop();



    //wifi_set_opmode(SOFTAP_MODE);
    ets_printf("-= Wifi SoftAP setup =-\r\n");
    char ssid[32] = "ESP8266";
    char password[64] = "1234";
    struct softap_config softap_conf;
    memset(&softap_conf, 0, sizeof(softap_conf));

    os_strncpy((char*)softap_conf.ssid, ssid, 32);
    os_strncpy((char*)softap_conf.password, password, 64);
    softap_conf.channel = 1;
    softap_conf.authmode = AUTH_OPEN;
    softap_conf.max_connection = 4;
    softap_conf.ssid_hidden = 0;
    softap_conf.ssid_len = os_strlen((char*)softap_conf.ssid);


    ets_printf("Wifi SSID     : %s\r\n",softap_conf.ssid);
    ets_printf("Wifi Password : %s\r\n",softap_conf.password);

    uint8 macaddr[6];
    if(wifi_get_macaddr(SOFTAP_IF, macaddr))
    {
    ets_printf("ESP MAC       : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
               macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
    }

    struct ip_info ip_info;
    if(wifi_get_ip_info(SOFTAP_IF, &ip_info))
    {
    ets_printf("ESP IP        : %d.%d.%d.%d \r\n",IP2STR(&ip_info.ip));
    ets_printf("ESP gateway   : %d.%d.%d.%d \r\n",IP2STR(&ip_info.gw));
    ets_printf("ESP netmask   : %d.%d.%d.%d \r\n",IP2STR(&ip_info.netmask));
    }

    wifi_softap_set_config(&softap_conf);
    wifi_set_opmode(STATIONAP_MODE);
    wifi_set_event_handler_cb(wifiEventHandler);

    wifi_softap_dhcps_start();

    switch(wifi_softap_dhcps_status())
    {
      case DHCP_STOPPED:
          ets_printf("DHCP Server stopped!\r\n");
      break;
      case DHCP_STARTED:
          ets_printf("DHCP Server started!\r\n");
      break;
      default:
          ets_printf("DHCP Server unknown state!\r\n");
      break;
    }
}

/** @brief do the blink stuff
 *
 * @param arg here we can provide some context to the timer
 */
void doBlink(void* arg __attribute__((unused))  /* context */  )
{
    static unsigned int count = 0;
    uint32_t nextState = GPIO_INPUT_GET(GPIO_ID_PIN(5));
    GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 0x00000001 ^ nextState); /* toggle the bit */
    if(0 == (++count%10)) /* list connected devices every 10 seconds */
    {
        check_station_info();
    }
}

void connectCB(void *arg)
{
        ets_printf("\r\nInside server_callback\r\n");
        struct espconn *pesp_conn = (struct espconn *)arg;
}

void check_station_info()
{

    int count = 0;

      struct station_info * station = wifi_softap_get_station_info();
      while(station)
      {
          ets_printf("bssid : "MACSTR", ip : "IPSTR"\n", MAC2STR(station->bssid), IP2STR(&station->ip));
          count++;
          station = STAILQ_NEXT(station, next);
      }
}


void wifiEventHandler(System_Event_t *event)
{
    char data = 0;
        char ret;
    switch(event->event) {
        case EVENT_STAMODE_CONNECTED:
            ets_printf("\r\nEVENT_STAMODE_CONNECTED:\r\n");
            break;
        case EVENT_STAMODE_DISCONNECTED:
            ets_printf("\r\nEVENT_STAMODE_DISCONNECTED:\r\n");
            break;
        case EVENT_STAMODE_AUTHMODE_CHANGE:
            ets_printf("\r\nEVENT_STAMODE_AUTHMODE_CHANGE\r\n");
            break;
        case EVENT_SOFTAPMODE_STACONNECTED:
            ets_printf("\r\nEVENT_SOFTAPMODE_STACONNECTED:\r\n");
            uint8 stationCount = wifi_softap_get_station_num();
            ets_printf("stationCount = %d\r\n",stationCount);
            ets_printf("station: " MACSTR " join, AID = %d\n",
                                   MAC2STR(event->event_info.sta_connected.mac),
                                   event->event_info.sta_connected.aid);
            check_station_info();
            break;
        case EVENT_SOFTAPMODE_STADISCONNECTED:
            ets_printf("\r\nEVENT_SOFTAPMODE_STADISCONNECTED:\r\n");
            break;
        default:
            ets_printf("\r\n DEFAUlT!!!!t \r\n");
            break;
    }
}
