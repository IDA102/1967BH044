%{
Скрипт запуска снятия АЧХ АЦП_5101НВ01Н4.
Устанавливает начальную частоту опорного генератора. 
Инициализирует COM порт и 1 транзакцию записи порции данных.
%}
%% PARAMETRS
global COM9;
global FREQ;
FREQ = 5e6;
%% GENERATOR_DEFAULT_SETTING
VK_GENERATION_ASG_N5173B_FUNCTION_FREQ_RESPONCE(FREQ);
%% CONNECT_STEPS
instrreset;%Удаление всех обЪектов.
COM9 = serial('COM9');
set(COM9, 'BaudRate', 9500);
COM9.InputBufferSize = 1;  
COM9.BytesAvailableFcnCount = 1;
COM9.BytesAvailableFcnMode = 'byte';
COM9.BytesAvailableFcn = @(COM9,event) INTERRUPT(COM9,event);
fopen(COM9);
fprintf(COM9, '1'); 