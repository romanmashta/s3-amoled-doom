#!/bin/bash
python ~/.platformio/packages/tool-esptoolpy/esptool.py --chip esp32-s3 --port "/dev/cu.usbmodem83201" --baud $((921600/2)) --before default_reset --after hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size detect 0x120000 data/prboom.wad
python ~/.platformio/packages/tool-esptoolpy/esptool.py --chip esp32-s3 --port "/dev/cu.usbmodem83201" --baud $((921600/2)) --before default_reset --after hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size detect 0x1A0000 data/DOOM2.WAD
