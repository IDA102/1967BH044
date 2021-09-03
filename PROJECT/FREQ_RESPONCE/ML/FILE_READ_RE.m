%{
������ ��� ���������� ��������� ��� �������� �������.
������ ��� �� 5 �� 200 ���, � ����� 5 ���.
%}
clear all;
clc;
%% PARAMETRS
LENGTH     = 200;%���������� samples
ROWS       = 40;%���������� �������
START_FREQ = 5e6;
STOP_FREQ  = 200e6;
AMP_BUF    = zeros(ROWS,1);
OFFSET     = 2.15e9;%�������� 0. �.�. ������� ��� ������.(diff ����)
IND_START  = 1;
IND_STOP   = 200;
AMPLITUDE_SIGNAL_IN = 0.7;%������
%% FILE_OPEN
ID = fopen('W:\ML\MSO\FREQ_RESPONCE\ADC_SWEEP.bin');
MASS_IN = fread(ID,'uint32');
fclose(ID);
%% MAIN
TMP(1:8) = OFFSET;
MASS_IN = [MASS_IN(9:end);TMP'];
%% �������� � "0"
for(i = 1:length(MASS_IN))
    if(MASS_IN(i,1) > OFFSET)
        MASS(i,1) = ((MASS_IN(i,1) - OFFSET))/OFFSET;
    end
    if(MASS_IN(i,1) < OFFSET)
        MASS(i,1) = (MASS_IN(i,1) - OFFSET)/OFFSET;
    end
    if(MASS_IN(i,1) == (OFFSET))
        MASS(i,1) = 0;
    end
end
%% ���������� ���
MASS = abs(MASS);

for(i = 1:ROWS)
    AMP_BUF(i,1) = sum(MASS(IND_START:IND_STOP)); 
    IND_START = IND_START + LENGTH;
    IND_STOP = IND_STOP + LENGTH; 
end
AMP_BUF = AMP_BUF./115;%�������!!!!!!!!-----------������ ��������������!!!! 
AMP_BUF = AMP_BUF./0.7;%����������
%% �������
subplot(3,1,1);
plot(MASS_IN);
title('������� ������');
grid on;
subplot(3,1,2);
% plot(MASS);
semilogy(MASS);
title('������ ������� � ��������������� �������� �� ��� �������');
grid on;
subplot(3,1,3);
f = START_FREQ:5e6:STOP_FREQ;f = f./1e6;
hold on;

plot(f,AMP_BUF,'o')
plot(f,AMP_BUF)
grid on;
title('��������������� �������� ���');
xlabel('f (MHz)')
ylabel('|�(f)|')