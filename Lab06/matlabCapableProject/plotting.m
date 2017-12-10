fp = load('highpassFAS.txt');

freq = fp(:,1);
ampl = fp(:,2);

figure
plot(freq,ampl)
title('highpass frequency amplitude spectrum');
ylabel('Amplitude');
xlabel('');