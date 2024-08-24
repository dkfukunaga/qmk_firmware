CUSTOM_MATRIX = lite

UART_DRIVER_REQUIRED = yes

# Bootloader selection
#BOOTLOADER = rp2040
#CONVERT_TO=kb2040

# Project specific files
SRC += matrix.c \
       common/bitc_led.c \
       big_led.c \
       common/remote_kb.c
