%Limpieza de puerto
COMScrubber20;
%Configurar Baudrate, y asignaci�n.
s=serial('COM4', 'BaudRate', 9600);
%Abrir el puerto
fopen(s);
