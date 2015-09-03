/*
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
 * ==============================================================================
 * DO NOT RENAME THIS FILE IT IS NEEDED BY THE SDK !!!!
 * ==============================================================================
 */

#ifndef C_USER_CONFIG_H__
#define C_USER_CONFIG_H__

#define MQTT_HOST			"192.168.11.122" //or "mqtt.yourdomain.com"
#define MQTT_PORT			1880
#define MQTT_BUF_SIZE		1024
#define MQTT_KEEPALIVE		120	 /*second*/

#define MQTT_CLIENT_ID		"DVES_%08X"
#define MQTT_USER			"DVES_USER"
#define MQTT_PASS			"DVES_PASS"

#define MQTT_RECONNECT_TIMEOUT 	5	/*second*/
#define PROTOCOL_NAMEv31	/*MQTT version 3.1 compatible with Mosquitto v0.15*/

#define CFG_LOCATION	0x3C	/* Please don't change or if you know what you doing */
#define CFG_HOLDER	0x00FF55A4	/* Change this value to load default configurations */

#define STA_SSID "DVES_HOME"
#define STA_PASS "yourpassword"
#define STA_TYPE AUTH_WPA2_PSK

#define DEFAULT_SECURITY	0

#endif /* C_USER_CONFIG_H__  end */