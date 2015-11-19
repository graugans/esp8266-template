# ESP8266 Template

A template for the Espressif Systems ESP8266 chipset.

# Requirements

- [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk) installed on your system
- A ruby installation 2.0+

# Flashing the device

```
$ esptool.py elf2image build/xtensa-lx106-elf/dbg/apps/blinky
$ esptool.py --port /dev/ttyUSB0 write_flash 0x00000 build/xtensa-lx106-elf/dbg/apps/blinky-0x00000.bin 0x40000 build/xtensa-lx106-elf/dbg/apps/blinky-0x40000.bin
```

