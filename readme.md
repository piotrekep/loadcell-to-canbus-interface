
# Digital Scale with CAN Bus

This repository contains firmware for a digital scale system based on the ATmega328 microcontroller. Developed over 10 years ago, this project reads analog data via an ADS1234 ADC and transmits measurements over a CAN bus using an MCP2515 controller through an SPI interface.

## Overview

The firmware implements a digital weighing system that:
 
- **Acquires Data**: Reads four channels of analog data from an ADS1234 ADC.
- **CAN Bus Communication**: Transmits the acquired data in two separate CAN messages (IDs 0x250 and 0x251).
- **SPI Interface**: Uses custom SPI routines to communicate with both the CAN controller and the ADS1234.
- **Robust Operation**: Utilizes a watchdog timer and interrupt-driven routines to ensure reliable operation.

## Hardware Setup

### ATmega328
- **Microcontroller**: ATmega328
- **Clock Speed**: 20 MHz (FOSC = 20,000,000)

### ADS1234 ADC
- **Data Acquisition**: 
  - **Input Selection**: Controlled via two pins on PORTC (A0 on PC1 and A1 on PC0).
  - **Gain Control**: Connected to PORTD (GAIN0 on PD1 and GAIN1 on PD2).
  - **Speed Control**: Connected to PC5.
  - **Data**: Read from PC2.
  - **Clock**: Driven from PC3.
  - **Power**: Controlled on PC4.
- **Channel Selection**: Four channels are read sequentially, with channels AIN1–AIN4 selected via software.

### CAN Bus (MCP2515)
- **Communication**: Uses the SPI interface.
  - **MOSI**: PB3
  - **SCK**: PB5
  - **Chip Select (CS)**: PB2
- **CAN Messaging**: Two messages are prepared and sent:
  - **Message 1 (ID 0x250)**: Contains data from channels AIN1 and AIN2.
  - **Message 2 (ID 0x251)**: Contains data from channels AIN3 and AIN4.

### UART (Debugging)
- **Serial Communication**: Basic serial routines are provided for debugging using UART0 at 115200 baud.

## Project Structure

```
.
├── ads1234/
│   ├── ads1234.c
│   └── ads1234.h
├── CAN_drv/
│   ├── can_drv.c
│   └── can_drv.h
├── SPI_drv/
│   └── SPIm328.c
├── twi/             (if applicable)
│   └── twi.c
├── uartsimple/
│   ├── uartsimple.c
│   └── usartsimple.h
├── types/
│   └── tdef.h
└── main.c
```

## Build Instructions

This project is configured for the AVR-GCC toolchain and was originally built using Atmel AVR Studio. Two build configurations are provided:

- **Debug**: Compiled with debugging symbols and optimization level O1.
- **Release**: Compiled with optimizations for size (-Os).

To compile the project from the command line, you might use:

```bash
avr-gcc -mmcu=atmega328p -Os -DF_CPU=20000000UL -o digital_scale.elf \
  main.c ads1234/ads1234.c CAN_drv/can_drv.c SPI_drv/SPIm328.c uartsimple/uartsimple.c
avr-objcopy -O ihex -R .eeprom digital_scale.elf digital_scale.hex
```

Be sure to adjust include paths and source files as needed.

## Flashing the Firmware

You can use your preferred AVR programmer (such as avrdude) to flash the firmware. For example:

```bash
avrdude -c usbtiny -p m328p -U flash:w:digital_scale.hex:i
```

## Configuration Details

- **Clock Speed**: Set to 20 MHz.
- **UART Baud Rate**: 115200 baud.
- **CAN Bus Baud Rate**: Configured using the `CAN_setBaud20` function. Baud rate options include 125, 250, and 500 kbps.
- **Watchdog Timer**: Enabled with a 2-second timeout to help ensure robust operation.

## Notes

- This project is a legacy codebase and may require updates for modern toolchains or hardware revisions.
- Detailed configuration settings (such as ADC channel selection and CAN bus setup) can be found within the source code files.


