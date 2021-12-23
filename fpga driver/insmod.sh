#!/bin/bash

sudo insmod fpga_interface_driver.ko
sudo insmod fpga_push_switch_driver.ko
sudo mknod /dev/fpga_test_push_switch c 265 0

sudo insmod fpga_text_lcd_driver.ko
sudo mknod /dev/fpga_text_lcd c 263 0

sudo insmod fpga_dot_driver.ko
sudo mknod /dev/fpga_dot c 262 0

