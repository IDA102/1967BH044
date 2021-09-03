%{
Скрипт для построения огибающей АЧХ точечным методом.
Полоса АЧХ от 5 до 200 МГц, с шагом 5 МГц.
%}
clear all;
clc;
%% PARAMETRS
LENGTH     = 200;%Количество samples
ROWS       = 40;%Количество выборок
START_FREQ = 5e6;
STOP_FREQ  = 200e6;
AMP_BUF    = zeros(ROWS,1);
OFFSET     = 2.15e9;%Условный 0. Т.к. уровень АЦП смещён.(diff вход)
IND_START  = 1;
IND_STOP   = 200;
AMPLITUDE_SIGNAL_IN = 0.7;%Вольты
%% FILE_OPEN
ID = fopen('W:\ML\MSO\FREQ_RESPONCE\ADC_SWEEP.bin');
MASS_IN = fread(ID,'uint32');
fclose(ID);
%% MAIN
TMP(1:8) = OFFSET;
MASS_IN = [MASS_IN(9:end);TMP'];
%% Смещение к "0"
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
%% Построение АЧХ
MASS = abs(MASS);

for(i = 1:ROWS)
    AMP_BUF(i,1) = sum(MASS(IND_START:IND_STOP)); 
    IND_START = IND_START + LENGTH;
    IND_STOP = IND_STOP + LENGTH; 
end
AMP_BUF = AMP_BUF./115;%Костыль!!!!!!!!-----------Оценка приблизительна!!!! 
AMP_BUF = AMP_BUF./0.7;%Нормировка
%% Графики
subplot(3,1,1);
plot(MASS_IN);
title('Входной массив');
grid on;
subplot(3,1,2);
% plot(MASS);
semilogy(MASS);
title('Модуль массива в логарифмическом масштабе по оси ординат');
grid on;
subplot(3,1,3);
f = START_FREQ:5e6:STOP_FREQ;f = f./1e6;
hold on;

plot(f,AMP_BUF,'o')
plot(f,AMP_BUF)
grid on;
title('Отнормированная точечная АЧХ');
xlabel('f (MHz)')
ylabel('|А(f)|')