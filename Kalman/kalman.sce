//https://github.com/TKJElectronics/KalmanFilter/blob/master/Kalman.h
funcprot(0);
clear all;

MDEG = 8.75; //milistopni na sekundę

Q_angle = 0.001;
Q_bias = 0.003;
R_measure = 0.03;

angle = 0;
bias = 0;
rate = 0;

P=[0,0,0,0];
K=[0,0];
dt = 0.02;

function angle2=getAngle(X) //X(1) -> newAngle , X(2) -> newRate
    rate = X(2) - bias;
    angle = angle + dt * rate;
    
    P(1) = P(2) + dt*(dt*P(4) - P(2) - P(3) + Q_angle);
    P(2) = P(2) - dt*P(4);
    P(3) = P(3) - dt*P(4);
    P(4) = P(4) + Q_bias * dt;
    
    S = P(1) + R_measure;
    
    K(1) = P(1) / S;
    K(2) = P(3) / S;
    
    y = X(1) - angle;
    
    angle = angle + K(1) * y;
    bias = K(2) * y;
    
    P(1) = P(1) - K(1) * P(1);
    P(2) = P(2) - K(1) * P(2);
    P(3) = P(3) - K(2) * P(1);
    P(4) = P(4) - K(2) * P(2);
    
    angle2 = angle;
endfunction

A = fscanfMat("/home/marcin/Quadro-STM32F100C8T6B/Kalman/akcelerometr.txt", "%lf");

i = 0;
wektor = [0,0];

while (i < 100)
    wektor(1) = A(i+1,2);
    wektor(2) = round(rand(1)*800)-400 * MDEG * 0.001; //zeby bylo w stopniach
    kat = getAngle(wektor);
    kat
    i = i+1;
end;
