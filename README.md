# STM32F446 Driver Development

Bare-metal peripheral driver development for the **STM32F446RE** microcontroller, written in C at the register level with no HAL/LL abstraction layer. This project is focused on understanding how STM32 peripherals actually work under the hood — configuring registers directly using the reference manual (RM0390) rather than relying on ST's HAL library.

## Overview

This repository contains custom peripheral drivers built from scratch for the STM32F446RE (Nucleo-F446RE), along with the startup code and linker scripts needed to build and flash a bare-metal project. The goal is to develop a solid, low-level understanding of ARM Cortex-M4 microcontroller internals — GPIO, communication peripherals, interrupts, and clock configuration — by implementing drivers directly against the memory-mapped registers described in the datasheet and reference manual.

## Repository Structure

```
.
├── drivers/                     # Custom peripheral driver source and header files
├── Src/                         # Application/test source files that exercise the drivers
├── Startup/                     # Startup assembly code (reset handler, vector table, etc.)
├── STM32F446RETX_FLASH.ld       # Linker script for running code from Flash
├── STM32F446RETX_RAM.ld         # Linker script for running code from RAM
├── .project / .cproject         # Eclipse/STM32CubeIDE project files
└── .gitignore
```

## Target Hardware

- **MCU:** STM32F446RE (ARM Cortex-M4, up to 180 MHz)
- **Board:** ST Nucleo-F446RE (or any board built around the STM32F446RE)

## Toolchain

- **IDE:** STM32CubeIDE (Eclipse-based `.project` / `.cproject` files included)
- **Compiler:** `arm-none-eabi-gcc`
- **Debugger/Programmer:** ST-Link (onboard on the Nucleo board)
- **Language:** C, with ARM Cortex-M4 startup assembly

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/malhar-11/stm32f446-driver-development.git
   ```
2. Open the project in **STM32CubeIDE** (`File → Open Projects from File System...` and select the cloned folder), or build it from the command line using `arm-none-eabi-gcc` with the provided linker scripts.
3. Connect a Nucleo-F446RE board via USB (ST-Link) and flash the build.
4. Use the appropriate linker script depending on your target:
   - `STM32F446RETX_FLASH.ld` — run from Flash (typical for standalone operation)
   - `STM32F446RETX_RAM.ld` — run from RAM (useful for quick debugging/testing)

## Drivers

Each peripheral driver is implemented as a register-level `.c`/`.h` pair inside `drivers/`, exposing an API for configuration, data transfer, and interrupt handling — without relying on the STM32 HAL or LL libraries. Example/test applications demonstrating each driver live in `Src/`.

> As drivers are added, list them here, e.g.:
> - GPIO
> - SPI
> - I2C
> - USART
> - EXTI / NVIC (interrupts)

## Motivation

This project is part of an ongoing effort to build embedded systems fundamentals from the ground up — reading and applying microcontroller reference manuals directly, rather than depending on vendor abstraction layers, in order to understand exactly what happens at the hardware level when a peripheral is configured or used.

## License

No license has been specified yet. Until one is added, all rights are reserved by the author.

## Author

**Malhar** ([@malhar-11](https://github.com/malhar-11))
