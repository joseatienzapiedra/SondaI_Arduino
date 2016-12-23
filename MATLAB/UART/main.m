function [tiempo,hora,HR_SI7021,Temp_SI7021,Temp_BMP180,PressBase_BMP180,PressCurr_BMP180,Alt_BMP180,T_sensors,T_last_loop,x_flag] = main( )
close all;
COMconfig;

span=5;                    %RANGO DE TIEMPO VISIBLE DURANTE LA LECTURA
duracion=[1 0 0];          %DURACION DE LA MEDIDA [HORAS MINUTOS SEGUNDOS]

iteraciones=duracion(1)*3600+duracion(2)*60+duracion(3);
t=span+1;
datos=zeros(iteraciones+span,9);
HR_SI7021=zeros(iteraciones+span);
Temp_SI7021=zeros(iteraciones+span);
Temp_BMP180=zeros(iteraciones+span);
PressBase_BMP180=zeros(iteraciones+span); 
PressCurr_BMP180=zeros(iteraciones+span);
Alt_BMP180=zeros(iteraciones+span);
T_sensors=zeros(iteraciones+span); 
T_last_loop=zeros(iteraciones+span);
x_flag=zeros(iteraciones+span);

tiempo=zeros(iteraciones+span,1);
hora=zeros(iteraciones+span,6);
figure(1);
set(gcf,'units','normalized','outerposition',[0 0 1 1]);
subplot(3,2,1);


while t<=iteraciones+span
tiempo(t)=t;
hora(t,:)=clock;
datos(t,:)=str2num(read(s)); %#ok<ST2NM>

HR_SI7021(t)=datos(t,1);
subplot(3,2,1)
plot(tiempo(1:t),HR_SI7021(1:t));
xlim([t-span t]);
ylim([min(HR_SI7021(t-span:t))-0.2 max(HR_SI7021(t-span:t))+0.2]);
xlabel('Tiempo [s]');
ylabel('Humedad Relativa [%]');
legend('show');
legend('HR SI7021[%]')

Temp_SI7021(t)=datos(t,2);
Temp_BMP180(t)=datos(t,3);
subplot(3,2,2)
plot(tiempo(1:t),Temp_SI7021(1:t),tiempo(1:t),Temp_BMP180(1:t));
xlim([t-span t]);
ylim([min(min(Temp_SI7021(t-span:t)),min(Temp_BMP180(t-span:t)))-0.2 max(max(Temp_SI7021(t-span:t)),max(Temp_BMP180(t-span:t)))+0.2]);
xlabel('Tiempo [s]');
ylabel('Temp SI7021&BMP180 [ºC]');
legend('show');
legend('Temp SI7021 [ºC]','Temp BMP180 [ºC]')

PressBase_BMP180(t)=datos(t,4);
PressCurr_BMP180(t)=datos(t,5);
subplot(3,2,3)
plot(tiempo(1:t),PressBase_BMP180(1:t),tiempo(1:t),PressCurr_BMP180(1:t));
xlim([t-span t]);
ylim([min(min(PressBase_BMP180(t-span:t)),min(PressCurr_BMP180(t-span:t)))-0.2 max(max(PressBase_BMP180(t-span:t)),max(PressCurr_BMP180(t-span:t)))+0.2]);
xlabel('Tiempo [s]');
ylabel('Press Base&Curr [Pa]');
legend('show');
legend('Base [Pa]','PressCurr BMP180[Pa]')

Alt_BMP180(t)=datos(t,6);
subplot(3,2,4)
plot(tiempo(1:t),Alt_BMP180(1:t));
xlim([t-span t]);
ylim([min(Alt_BMP180(t-span:t))-0.2 max(Alt_BMP180(t-span:t))+0.2]);
xlabel('Tiempo [s]');
ylabel('Altitud [m]');
legend('show');
legend('Altitud [m]')

T_sensors(t)=datos(t,7);
T_last_loop(t)=datos(t,8);
subplot(3,2,5)
plot(tiempo(1:t),T_sensors(1:t),tiempo(1:t),T_last_loop(1:t));
xlim([t-span t]);
ylim([min(min(T_sensors(t-span:t)),min(T_last_loop(t-span:t)))-0.2 max(max(T_sensors(t-span:t)),max(T_last_loop(t-span:t)))+0.2]);
xlabel('Tiempo [s]');
ylabel('Tiempo Sensores&Loop [ms]');
legend('show');
legend('Tiempo sensores','Tiempo Loop [ms]')

x_flag(t)=datos(t,9);
subplot(3,2,6)
stairs(tiempo(1:t),x_flag(1:t));
xlim([t-span t]);
ylim([min(x_flag(t-span:t))-0.2 max(x_flag(t-span:t))+0.2]);
xlabel('Tiempo [s]');
ylabel('RX Status [0/1]');
legend('show');
legend('RX Status [0/1]')

whitebg(1,'k')
set(gcf,'Color','black')
drawnow;

clc;
disp('Tiempo restante en segundos:')
disp(iteraciones+span-t);
t=t+1;
end

close all;
tiempo=tiempo-span;
tiempo=tiempo(span+1:iteraciones+span);

HR_SI7021=HR_SI7021(span+1:iteraciones+span);
subplot(3,2,1)
plot(tiempo,HR_SI7021);
xlim([1 iteraciones]);
ylim([min(HR_SI7021)-0.2 max(HR_SI7021)+0.2]);
xlabel('Tiempo [s]');
ylabel('Humedad Relativa [%]');
legend('show');
legend('HR SI7021[%]')

Temp_SI7021=Temp_SI7021(span+1:iteraciones+span);
Temp_BMP180=Temp_BMP180(span+1:iteraciones+span);
subplot(3,2,2)
plot(tiempo,Temp_SI7021,tiempo,Temp_BMP180);
xlim([1 iteraciones]);
ylim([min(min(Temp_SI7021),min(Temp_BMP180))-0.2 max(max(Temp_SI7021),max(Temp_BMP180))+0.2]);
xlabel('Tiempo [s]');
ylabel('Temp SI7021&BMP180 [ºC]');
legend('show');
legend('Temp SI7021 [ºC]','Temp BMP180 [ºC]')

PressBase_BMP180=PressBase_BMP180(span+1:iteraciones+span);
PressCurr_BMP180=PressCurr_BMP180(span+1:iteraciones+span);
subplot(3,2,3)
plot(tiempo,PressBase_BMP180,tiempo,PressCurr_BMP180);
xlim([1 iteraciones]);
ylim([min(min(PressBase_BMP180),min(PressCurr_BMP180))-0.2 max(max(PressBase_BMP180),max(PressCurr_BMP180))+0.2]);
xlabel('Tiempo [s]');
ylabel('Press Base&Curr [Pa]');
legend('show');
legend('Base [Pa]','PressCurr BMP180[Pa]')

Alt_BMP180=Alt_BMP180(span+1:iteraciones+span);
subplot(3,2,4)
plot(tiempo,Alt_BMP180);
xlim([1 iteraciones]);
xlim([1 iteraciones]);
ylim([min(Alt_BMP180)-0.2 max(Alt_BMP180)+0.2]);
xlabel('Tiempo [s]');
ylabel('Altitud [m]');
legend('show');
legend('Altitud [m]')

T_sensors=T_sensors(span+1:iteraciones+span);
T_last_loop=T_last_loop(span+1:iteraciones+span);
T_last_loop(1)=mean(T_last_loop(2:iteraciones)); % De esta forma quitamos el íncomodo valor inicial 0
subplot(3,2,5)
plot(tiempo,T_sensors,tiempo,T_last_loop);
xlim([1 iteraciones]);
ylim([min(min(T_sensors),min(T_last_loop))-0.2 max(max(T_sensors),max(T_last_loop))+0.2]);
xlabel('Tiempo [s]');
ylabel('Tiempo Sensores&Loop [ms]');
legend('show');
legend('Tiempo sensores','Tiempo Loop [ms]')

x_flag=x_flag(span+1:iteraciones+span);
subplot(3,2,6)
stairs(tiempo,x_flag);
xlim([1 iteraciones]);
ylim([min(x_flag)-0.2 max(x_flag)+0.2]);
xlabel('Tiempo [s]');
ylabel('RX Status [0/1]');
legend('show');
legend('RX Status [0/1]')

set(gcf,'units','normalized','outerposition',[0 0 1 1]);
whitebg(1,'k')
set(gcf,'Color','black')

delete(instrfindall);
clc;
end

