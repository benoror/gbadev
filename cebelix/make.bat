@ECHO €€€€€€≤≤≤≤  Compilando cabeceras 	≤≤≤≤€€€€€€
@C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend start.asm
@IF ERRORLEVEL 1 GOTO ERRORES
@C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend data.asm
@ echo _
@pause
@cls
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO €€€€€€≤≤≤≤  Compilando binario 		≤≤≤≤€€€€€€
@IF "%1" == "-t" goto THUMB
@C:\agb\Amateur\ARM250\Bin\zarmcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst cebelix.c -o cebelix.o
:THUMB
@IF "%1" == "" goto NOTHUMB
@IF "%1" == "-mb" goto NOTHUMB
@C:\agb\Amateur\ARM250\Bin\ztcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst cebelix.c -o cebelix.o
:NOTHUMB
@ echo _
@pause
@cls
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO €€€€€€≤≤≤≤           Linkeando 		≤≤≤≤€€€€€€
@IF "%1" == "-mb" goto MULTIBOOT
@C:\agb\Amateur\ARM250\Bin\zarmlink -bin -first start.o start.o data.o cebelix.o -map -ro-base 0x08000000 -rw-base 0x03000000 -o cebelix.bin
:MULTIBOOT
@IF "%1" == "" goto NOMULTIBOOT
@IF "%1" == "-t" goto NOMULTIBOOT
@C:\agb\Amateur\ARM250\Bin\zarmlink -bin -first start.o start.o data.o cebelix.o -map -ro-base 0x02000000 -rw-base 0x03000000 -o cebelix.mb
:NOMULTIBOOT
@ echo _
@pause
@cls
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO €€€€€€≤≤≤≤          ENJOY! %1 %2		≤≤≤≤€€€€€€
@ECHO €€€€€€≤≤≤≤         Eleminando *.o 	≤≤≤≤€€€€€€
@del *.o
@ echo _
@pause
@exit
:ERRORES
@ECHO €€€€€€≤≤≤≤	    ERRORES!		≤≤≤≤€€€€€€
@ECHO €€€€€€≤≤≤≤         Eleminando *.o 	≤≤≤≤€€€€€€
@del *.o
@ echo _
@pause
@exit