@ECHO �����۲���  Compilando cabeceras 	����������
@C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend start.asm
@IF ERRORLEVEL 1 GOTO ERRORES
@C:\agb\Amateur\ARM250\Bin\zarmasm -CPU ARM7TDMI -Littleend data.asm
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO �����۲���      Compilando binario 	����������
@C:\agb\Amateur\ARM250\Bin\zarmcc -c -Wall -Otime -ansic -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst demo.c -o demo.o
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO �����۲���           Linkeando 		����������
@C:\agb\Amateur\ARM250\Bin\zarmlink -bin -first start.o start.o data.o demo.o -map -ro-base 0x02000000 -rw-base 0x03000000 -o demo.mb
@IF ERRORLEVEL 1 GOTO ERRORES
@ECHO �����۲���             ENJOY! 		����������
@ECHO -- ----------- -- ---- -------
@ECHO La compilaci�n ha sido exitosa
@ECHO -- ----------- -- ---- -------
@ECHO �����۲���         Eleminando *.o 	����������
@del *.o
@pause
@exit
:ERRORES
@ECHO -- --- --------- -------
@ECHO Se han producido errores
@ECHO -- --- --------- -------
@ECHO �����۲���         Eleminando *.o 	����������
@del *.o
@pause
@exit
