# CMODA7 example design

## Pinout
The pinout is defined in [system_top.xdc](synth/system_top.xdc). There are 2 possibilities. Please verify the file and un-comment the correct one:

  * The ui_board is connected via an IDC ribbon cable to the CMOD A7 (default).
  * The ui_board is directly, without cable, plugged into the CMOD A7 PMOD header.

In both cases, make sure that the power pins (the 4 lower-most pins on the PMOD) are aligned correctly.

## Dependencies
This example has a dependency on [Bedrock](https://github.com/BerkeleyLab/Bedrock).
Clone it and make sure that `common/common.mk` points to your local `Bedrock/dir_list.mk`.

Downloading the .bit file to the FPGA needs `xc3sprog` installed. The bootloader needs `pyserial`.

## About the demo
It was mostly intended as a quick test if all the features on the ui_board_1u work as expected.

  * The absolute encoder position is printed
  * Every encoder tick causes the color of one LED to change
  * A short push on the encoder selects which LED is targeted
  * A long push on the encoder selects between ui_board and ui_board_1u mode and re-initializes the ui_board. The difference between the 2 modes is the LED polarity and the number of steps per encoder notch.
  * A short push on the back-button disables the LEDs
  * A long push on the back-button inverts the OLED screen

## Building it
```bash
cd cmod/synth

# Build the firmware
make system32.hex

# Build the .bit file
make system_top.bit

# Configure the FPGA on the CMODA7
make system_top_config

# Boot-load the firmware
make system_load
```

## UART
To see the printf debug statements, use a serial terminal at 921600 Baud/s.
You can Soft-reset the CPU by pushing `CTRL+T` twice.
```bash
pyserial-miniterm /dev/ttyUSB1 921600
  # CTRL+T, CTRL+T
  ook
  Hi, this is UI_BOARD demo on CMODA7
```
