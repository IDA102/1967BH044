%{
Скрипт обработчика прерывания COM порта.
Принимает сообщение от DSP о готовности работать.
Устанавливает требуемую частоту в генераторе. 
Отправляет сообщение DSP, что бы DSP  записало новую порцию данных.
%}
function INTERRUPT(obj,event)
persistent FREQ;
persistent  CNT;
OUT = 0;
N = 1;
if(isempty(FREQ))
    FREQ = 10000000;
    CNT = 1;
end
 DATA = fscanf(obj);
if(CNT < N) 
    if('2' == DATA(1))
    CNT = CNT + 1;
    FREQ = FREQ + 5e6;
    OUT = VK_GENERATION_ASG_N5173B_FUNCTION_FREQ_RESPONCE(FREQ);
    while(~OUT)%Ждёт окончание установки частоты в генераторе
    end
    pause(5);
    fprintf(obj, '1');
    end
end
end

