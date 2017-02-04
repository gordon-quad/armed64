armed64
=======

Project intended to clone key64 for STM32F103 controllers.

Firmware loosely based on 5x5 (https://github.com/dijkstrw/5x5).

Building
========

    git submodule init
    git submodule update
    make -C libopencm3

for master part:

    make TARGET=master

or for slave part:

    make TARGET=slave
