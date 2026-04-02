@ECHO €€€€€€≤≤≤≤  Compilando cabeceras 	≤≤≤≤€€€€€€
@c:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend start.asm
@IF ERRORLEVEL 1 GOTO ERRORES
@c:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend data.asm
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO €€€€€€≤≤≤≤      Compilando binario 	≤≤≤≤€€€€€€
@c:\agb\Amateur\ARM250\Bin\zarmcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst demo.c -o demo.o
@pause
@cls
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO €€€€€€≤≤≤≤           Linkeando 		≤≤≤≤€€€€€€
@c:\agb\Amateur\ARM250\Bin\zarmlink -bin -first start.o start.o data.o demo.o -map -ro-base 0x08000000 -rw-base 0x03000000 -o demo.bin
@pause
@cls
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO €€€€€€≤≤≤≤             ENJOY! 		≤≤≤≤€€€€€€
@ECHO -- ----------- -- ---- -------
@ECHO La compilaciÛn ha sido exitosa
@ECHO -- ----------- -- ---- -------
@ECHO €€€€€€≤≤≤≤         Eleminando *.o 	≤≤≤≤€€€€€€
@del *.o
@pause
@exit
:ERRORES
@ECHO -- --- --------- -------
@ECHO Se han producido errores
@ECHO -- --- --------- -------
@ECHO €€€€€€≤≤≤≤         Eleminando *.o 	≤≤≤≤€€€€€€
@del *.o
@pause
@exit
