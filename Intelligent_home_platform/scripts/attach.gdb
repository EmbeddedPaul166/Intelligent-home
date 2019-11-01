file bin/home_management_system.elf
target remote | openocd -f interface/stlink-v2-1.cfg -f target/stm32f1x.cfg -f ./scripts/gdb_pipe.cfg
monitor halt
monitor gdb_sync
stepi
