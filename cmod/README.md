# CMODA7 example design

## Dependencies
This example has a dependency on [Bedrock](https://github.com/BerkeleyLab/Bedrock).
Clone it and make sure that `common/common.mk` points to your local `Bedrock/dir_list.mk`.

Downloading the .bit file to the FPGA needs `xc3sprog` installed. The bootloader needs `pyserial`.

## Building it
```bash
cd cmod/synth

# Build the firmware
make system32.hex

# Build the .bit file
make system_top.bit

# Configure the FPGA on the CMODA7
make system_config

# Boot-load the firmware
make system_load
```
