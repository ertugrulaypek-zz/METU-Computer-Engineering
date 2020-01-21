% Ertugrul Aypek, 2171270


C1_original = imread('C1.jpg');
C2_original = imread('C2.jpg');


smoothingKernel = [1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25];

C1_result = the1_convolution(C1_original,smoothingKernel);
imwrite(round(C1_result), 'C1_result.jpg');

C2_result = the1_convolution(C2_original,smoothingKernel);
imwrite(round(C2_result), 'C2_result.jpg');


