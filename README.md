# Bare-Bootloader

A bare-metal bootloader for the STM32F411RE (Nucleo-F411RE), written in C on top of
[libopencm3](https://github.com/libopencm3/libopencm3) as a thin register abstraction.

## Goal

Write a bootloader for the STM32F411RE that allows firmware updates over USB serial,
with signed-firmware authentication.

Built following the [LowByte Productions](https://www.youtube.com/@LowByteProductions) *"[Blinky to Bootloader](https://www.youtube.com/watch?v=uQQsDWLRDuI&list=PLP29wDx6QmW7HaCrRydOnxcy8QmW0SNdQ)"* series.

## Hardware

- **MCU board:** STM32F411RE Nucleo (ARM Cortex-M4)
- **Programmer/UART bridge:** onboard ST-LINK/V2-1 



