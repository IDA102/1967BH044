%{
Скрипт формирования SIN сигнала в опорном генераторе.
FREQ - входной параметр частоты которую нужно установить.
%}
function OUT = VK_GENERATION_ASG_N5173B_FUNCTION_FREQ_RESPONCE(FREQ)
% clear all;
% clc;
%% PARAMETRS
DEVICE          = 'USB0::0x0957::0x1F01::MY53271202::0::INSTR';
T               = 0.8; % Timeout
% AMPLITUDE       = 1; % Вольт
%% CONNECT_STEPS
vN5173B = visa('AGILENT', sprintf('%s',DEVICE));
fopen(vN5173B);
%% COMMANDS_CHANEL_1  
fprintf(vN5173B, sprintf(':SOUR:FREQ %f',FREQ));%FREQ
pause(T);
 %% CLEANUP
fclose(vN5173B);
delete(vN5173B);
clear vN5173B;
fprintf('END SCRIPT\n');
OUT = 1;
end