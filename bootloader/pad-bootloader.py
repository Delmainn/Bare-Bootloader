
BOOTLOADER_SIZE = 0x8000
BOOTLOADER_FILE = "bootloader.bin"

with open(BOOTLOADER_FILE, "rb") as f:
    raw_file = f.read()


    bytes_to_pad = BOOTLOADER_SIZE - len(raw_file)          # how many bytes short of 32KB we are
    padding = bytes([0xff for _ in range(bytes_to_pad)])    # 0xFF = the "erased flash" bit pattern

    with open(BOOTLOADER_FILE, "wb") as f:
        f.write(raw_file + padding)