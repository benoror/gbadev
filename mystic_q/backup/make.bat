@ echo off
C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend start.asm
C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend data.asm
C:\agb\Amateur\ARM250\Bin\zarmcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst demo.c -o demo.o
@ pause
C:\agb\Amateur\ARM250\Bin\zarmlink -bin -first start.o start.o data.o demo.o -map -ro-base 0x08000000 -rw-base 0x03000000 -o demo.bin -errors ha.txt
del *.o
exit
@ pause