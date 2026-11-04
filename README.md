# Bare-Bootloader 🔐

A bare-metal bootloader for the STM32F411RE (Nucleo-F411RE) that updates firmware over UART and only boots firmware it can authenticate. Written in C on top of [libopencm3](https://github.com/libopencm3/libopencm3).

**Status:** complete

## Acknowledgments

Architecture based on the [Low Byte Productions](https://www.youtube.com/@LowByteProductions) "[Blinky to Bootloader](https://www.youtube.com/watch?v=uQQsDWLRDuI&list=PLP29wDx6QmW7HaCrRydOnxcy8QmW0SNdQ)" series. The watchdog, update-path bounds checks, flash read-back verification, and host fixes were added in my second pass.

## 🗺️ Flash Layout

```
0x0800_0000 ┌──────────────────────────┐
            │ Bootloader (32 KB)       │  sectors 0-1
0x0800_8000 ├──────────────────────────┤
            │ Application              │  sectors 2-7 (480 KB)
            │ + firmware info          │
            │ + authentication tag     │
            └──────────────────────────┘
```

## 🔄 How an Update Works

1. **Sync:** the host and device lock onto each other.
2. **Handshake:** update request, device ID check, firmware length. Impossible lengths are rejected here, before anything is erased.
3. **Erase:** the application region is cleared.
4. **Receive:** firmware streams in as 18-byte packets with CRC-8, ACK/RETX retransmission, and `READY_FOR_DATA` flow control. Every flash write is read back and checked.
5. **Verify:** the bootloader checks the AES-128 CBC-MAC tag and only reports success if the image is valid.
6. **Boot:** the tag is checked again at every boot before jumping to the app.

An independent watchdog runs the whole time, so a hang in the bootloader or the app resets the device instead of freezing it.

## 🛠️ Build and Run

Needs `arm-none-eabi-gcc`, `make`, Python 3, OpenSSL, Node.js, and an ST-Link.

```bash
git clone --recursive https://github.com/Delmainn/Bare-Bootloader
cd Bare-Bootloader/libopencm3 && make TARGETS=stm32/f4
cd ../bootloader && make        # flash bootloader.bin to 0x08000000 with your ST-Link tool
cd ../app && make
cd ../fw-signer && python3 main.py ../app/firmware.bin 1
cd ../fw-updater && npm install
npx tsx index.ts ../fw-signer/signed.bin   # press RESET on the board at "Attempting to sync"
```

## 🧪 Tested on Hardware

| Test | Result |
|---|---|
| Tampered code, metadata, or authentication tag | Rejected |
| Corrupted length field | Rejected |
| Transfer interrupted halfway | Rejected, recovered by re-sending the image |
| Update with an impossible length | Rejected before erase, working app kept |
| App that hangs on purpose | Watchdog reset, device back in the bootloader |

## 🧰 Repo Tour

| Folder | What's inside |
|---|---|
| `bootloader/` | Update state machine, packet protocol, flash driver, AES |
| `app/` | Demo application (PWM LED fade) |
| `shared/` | UART driver, ring buffer, timer, CRC |
| `fw-signer/` | Python + OpenSSL signing tool, script for making tampered test images |
| `fw-updater/` | TypeScript/Node.js serial updater |

## 🐛 Bugs I Hunted Down

- **Wrong CRCs:** `&packet` passed where `packet` was expected, so the CRC read stack garbage.
- **Lost UART bytes:** a single-byte "data available" flag raced with the ISR; replaced it with a ring buffer, then fixed a mask off-by-one in it.
- **Host stuck out of sync:** stale bytes in the serial backlog misaligned packet boundaries; fixed the parser to drop one byte on CRC failure and retry.
- **Device hung on a corrupted length:** the boot check trusted the length field before authenticating it; now it's bounded first.
- **Bad update wiped a working app:** an impossible length was accepted and the app was erased; now it's rejected before erase.
- **"Update complete" for tampered images:** success was reported before the image was checked; now the tag is verified first.
- **Updater quit on old messages:** NACKs left over from before the session made it exit; it now ignores everything until sync.
- **Stale bootloader in app builds:** `make` didn't notice when `bootloader.bin` changed; added the missing dependency.

## ⚠️ Known Limitations

- **Symmetric authentication:** images are authenticated with AES-128 CBC-MAC using a demo key stored in the bootloader. A production design would use asymmetric signatures (e.g. ECDSA) so the device only holds a public key.
- **No rollback protection:** an older, validly signed image will be accepted.
- **Single application slot:** an interrupted update leaves no runnable app until a new image is sent (the bootloader stays recoverable over UART).
- **Manual update entry:** the device only listens for updates during a 5-second window after reset.

## 🔩 Hardware

- **MCU board:** STM32F411RE Nucleo (Arm Cortex-M4)
- **Programmer/UART bridge:** onboard ST-LINK/V2-1
