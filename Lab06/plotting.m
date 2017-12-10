% plotting.m
% Make sure to change file names and location
clear all
clc
a = load('im2.txt');
b = load('im2_sharp.txt');

figure(1)
imshow(a)
title('Image before filtering');

figure(2)
imshow(b)
title('Image after sharpening');