perc = 50:5:85;
numCross = [15 15 15 15 15 13 13 11];
figure; plot(perc,numCross)
xlabel('Percentage (%)');
ylabel('Number of Threshold Crossings');
title('Optimizing the Event Detection Algorithm 1');

blank = [0.1 0.2 0.3];
numCross = [15 15 15];
figure; plot(blank,numCross)
xlabel('Blanking Period (seconds)');
ylabel('Number of Threshold Crossings');
title('Optimizing the Event Detection Algorithm 2');