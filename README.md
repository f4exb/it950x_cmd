it950x_cmd
==========

Linux command line utilities for IT950x based Rx and Tx DVB-T USB keys

This works with the HiDes UT-100x DVB-T Transmit/Receive USB keys
See: http://www.hides.com.tw/product_cg74469_eng.html
It is based on the original utilities it950x_testkit_rx and it950x_testkit_tx 
from HiDes but works in command line mode.

You have to install driver as per instructions found on HiDes website or CD.  

Installation:

After cloning this repository execute the bootstrap.sh script in the local copy
Launch the configure script from a build directory
Use --prefix to specify the installation directory
Do make and make install.

Usage:

Use it950x_cmd_rx to control receiver and it950x_cmd_tx to control transmitter
Use -h option to get a list of options.

Typical commands:

> it950x_cmd_tx -f 437M -B 2M -R 1200k -i file.ts
> it950x_cmd_rx -f 437M -B 2M -o - | fifo -b 2000000 | ffplay -

The reception command uses an intermediate fifo, see https://github.com/cskiraly/fifo
