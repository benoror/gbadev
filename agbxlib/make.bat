@C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend start.asm
@C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend data.asm
@C:\agb\Amateur\ARM250\Bin\zarmcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst dezgex.c -o dezgex.o
pause
@C:\agb\Amateur\ARM250\Bin\zarmlink -bin -first start.o start.o data.o dezgex.o -map -ro-base 0x08000000 -o dezgex.bin
@del *.o