%INSTRUCTIONS TO READ DATA:
%Drop .txt over Workspace
%Clic on Semicolumn, clic on tab, clic on Next, clic on Finish.

t=1:1:length(data(:,1));
HR_SI7021=data(:,1);
Temp_SI7021=data(:,2);
Temp_BMP180=data(:,3);
PressBase_BMP180=data(:,4);
PressCurr_BMP180=data(:,5);
Alt_BMP180=data(:,6);
Acel_MPU6050_XAxis=data(:,7);
Acel_MPU6050_YAxis=data(:,8);
Acel_MPU6050_ZAxis=data(:,9);
Giro_MPU6050_XAxis=data(:,7);
Giro_MPU6050_YAxis=data(:,8);
Giro_MPU6050_ZAxis=data(:,9);

subplot(2,3,1);
plot(t,HR_SI7021);
xlim([0 length(data(:,1))]);
ylim([min(data(:,1))-0.1 0.1+max(data(:,1))]);
xlabel('Tiempo [s]');
ylabel('Humedad Relativa [%]');
legend('show');
legend('HR SI7021[%]')

subplot(2,3,2);
plot(t,Temp_SI7021,t,Temp_BMP180);
xlim([0 length(data(:,1))]);
ylim([min(min(data(:,2)),min(data(:,3)))-0.1 0.1+max(max(data(:,2)),max(data(:,3)))]);
xlabel('Tiempo [s]');
ylabel('Temp SI7021&BMP180 [ºC]');
legend('show');
legend('Temp SI7021 [ºC]','Temp BMP180 [ºC]')

subplot(2,3,3);
plot(t,PressBase_BMP180,t,PressCurr_BMP180);
xlim([0 length(data(:,1))]);
ylim([min(min(data(:,4)),min(data(:,5)))-0.1 0.1+max(max(data(:,4)),max(data(:,5)))]);
xlabel('Tiempo [s]');
ylabel('Press Base&Curr [Pa]');
legend('show');
legend('Base [Pa]','PressCurr BMP180[Pa]')

subplot(2,3,4);
plot(t,Alt_BMP180);
xlim([0 length(data(:,1))]);
ylim([min(data(:,6))-0.1 0.1+max(data(:,6))]);
xlabel('Tiempo [s]');
ylabel('Altitud [m]');
legend('show');
legend('Altitud [m]');

subplot(2,3,5);
plot(t,Acel_MPU6050_XAxis,t,Acel_MPU6050_YAxis,t,Acel_MPU6050_ZAxis,t,sqrt(Acel_MPU6050_XAxis.*Acel_MPU6050_XAxis+Acel_MPU6050_YAxis.*Acel_MPU6050_YAxis+Acel_MPU6050_ZAxis.*Acel_MPU6050_ZAxis));
xlim([0 length(data(:,1))]);
ylim([min(min(min(data(:,7)),min(data(:,8))),min(data(:,9)))-0.1 0.1+max(max(max(data(:,7)),max(data(:,8))),max(data(:,9)))]);
xlabel('Tiempo [s]');
ylabel('Acel [m/s2]');
legend('show');
legend('Acel_X [m/s2]','Acel_Y [m/s2]','Acel_Z [m/s2]','Acel_Mod [m/s2]');


subplot(2,3,6);
plot(t,Giro_MPU6050_XAxis,t,Giro_MPU6050_YAxis,t,Giro_MPU6050_ZAxis);
xlim([0 length(data(:,1))]);
ylim([min(min(min(data(:,10)),min(data(:,11))),min(data(:,12)))-0.1 0.1+max(max(max(data(:,10)),max(data(:,11))),max(data(:,12)))]);
xlabel('Tiempo [s]');
ylabel('Vel [rad/s]');
legend('show');
legend('Giro_X [rad/s]','Giro_Y [rad/s]','Giro_Z [rad/s]');

set(gcf,'units','normalized','outerposition',[0 0 1 1]);
whitebg(1,'k')
set(gcf,'Color','black')
