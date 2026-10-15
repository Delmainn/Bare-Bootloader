# Bare-Bootloader 🔐

A bare-metal bootloader for the STM32F411RE (Nucleo-F411RE) that updates firmware over UART and only boots firmware it can authenticate. Written in C on top of [libopencm3](https://github.com/libopencm3/libopencm3).

**Status: complete (September 2026)**

## Acknowledgments

Architecture based on the [Low Byte Productions](https://www.youtube.com/@LowByteProductions) "[Blinky to Bootloader](https://www.youtube.com/watch?v=uQQsDWLRDuI&list=PLP29wDx6QmW7HaCrRydOnxcy8QmW0SNdQ)" series.

## 🗺️ Flash Layout

```
0x0800_0000 ┌──────────────────────────┐
            │ Bootloader (32 KB)       │
0x0800_8000 ├──────────────────────────┤
            │ Application              │
            │ + firmware info          │
            │ + authentication tag     │
            └──────────────────────────┘
```

## 🔄 How an Update Works

1. **Sync:** the host and device lock onto each other.
2. **Handshake:** update request, device ID check, firmware length.
3. **Erase:** the application region is cleared.
4. **Receive:** firmware streams in as 18-byte packets with CRC-8, ACK/RETX retransmission, and `READY_FOR_DATA` flow control.
5. **Boot:** the bootloader verifies the AES-128 CBC-MAC tag before jumping to the app.

## 🧰 Repo Tour

| Folder | What's inside |
|---|---|
| `bootloader/` | Update state machine, packet protocol, flash driver, AES |
| `app/` | Demo application (PWM LED fade) |
| `shared/` | UART driver, ring buffer, timer, CRC |
| `fw-signer/` | Python + OpenSSL signing tool |
| `fw-updater/` | TypeScript/Node.js serial updater |

## 🐛 Bugs I Hunted Down

- **Wrong CRCs:** `&packet` passed where `packet` was expected, so the CRC read stack garbage.
- **Lost UART bytes:** a single-byte "data available" flag raced with the ISR; replaced it with a ring buffer, then fixed a mask off-by-one in it.
- **Host stuck out of sync:** stale bytes in the serial backlog misaligned packet boundaries; fixed the parser to drop one byte on CRC failure and retry.
- **"Optimized out" in GDB:** traced to `-Os`; switched the bootloader build to `-Og` for debugging.

## 🔩 Hardware

- **MCU board:** STM32F411RE Nucleo (Arm Cortex-M4)
- **Programmer/UART bridge:** onboard ST-LINK/V2-1
