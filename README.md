#README
Contributors: Tijs Hol, Yann Rosema

##Insructions
To get access to the arm compiler on et4351 type:

source /opt/eds/Xilinx/Vivado/2016.4/settings64.csh

Now you should be able to 'make' the software in the sw directory


Thereafter go to the tb directory and type 'make'

Then execute the virtual machine:

./sctop ../sw/app.elf

Use gtkwave to view the signaltraces:

gtkwave tr.vcd

