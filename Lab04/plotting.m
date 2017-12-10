clear;
clc;
file = load('sinus1.txt');

t = file(:,1);
a = file(:,2);

for i=2:2499
    y0(i-1) = abs(a(i+1)-a(i-1));
end
t = t(2:2499);

plot(t,y0)