clc;
clear all;
%%PARAMETRS
FLAG = 1;
N = 32768;
%%
if(FLAG == 1)
    % system('load.bat')
    ID = fopen('W:\ML\MSO\DMA_UP_DOWN_CONVERTER\ODDUC.bin');
    MASS = fread(ID,'int16','ieee-le');
    fclose(ID);
    % plot(MASS)
    I = MASS(1:2:end);
    Q = MASS(2:2:end);
    clf reset;
    hold on;
    grid on;
    plot(I);
    plot(Q);
elseif(FLAG == 2)
    ID = fopen('W:\ML\MSO\DMA_UP_DOWN_CONVERTER\IDDUC.bin','w+b');
    X = 100 * ones(1,2^20); 
%     X = 255 .* ones(1,N*4); 
%     X = 255 * cos(2*pi*64*[0:N-1]/N).*sign(cos(2*pi*[0:N-1]/N));
    fwrite(ID,X,'uint16','ieee-le');
    fclose(ID);
end