# Bare-Bootloader

A bare-metal bootloader for the STM32F411RE (Nucleo-F411RE), written in C on top of
[libopencm3](https://github.com/libopencm3/libopencm3) as a thin register abstraction.

## Goal

Write a bootloader for the STM32F411RE that allows firmware updates over USB serial,
with the possibility of adding a signed-firmware verification mechanism on top.

Built following the Lowbyte Productions *"Blinky to Bootloader"* series.

## Hardware

- **MCU board:** STM32F411RE Nucleo (ARM Cortex-M4)
- **Programmer/UART bridge:** onboard ST-LINK/V2-1 
- **Debug/verification:** HiLetgo 8-channel USB logic analyzer 

## Repository layout
```
Bare-Bootloader/
├── app/
│ ├── inc/ # headers (common-defines.h)    
│ ├── src/ # firmware sources (firmware.c)
│ ├── linkerscript.ld # flash/RAM memory layout + vector table placement
│ └── Makefile # build + flash targets
├── libopencm3/ # peripheral library (git submodule, pinned commit)
└── .vscode/ # build/debug task configuration
```
## Progress

Milestones are added here as they're completed.

- **GPIO LED blink** — 84 MHz system clock via PLL, bare-metal GPIO toggle.
