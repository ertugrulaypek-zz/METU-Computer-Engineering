% Ertugrul Aypek, 2171270

smoothingKernel = [1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25; 1/25,1/25,1/25,1/25,1/25];

sobelKernelX = [-1, 0, 1; -2, 0, 2; -1, 0, 1];
sobelKernelY = [-1, -2, -1; 0, 0, 0; 1, 2, 1];



%%%%%%%%%%%%%%%% Operations on C1 %%%%%%%%%%%%%%%%
C1_original = imread('C1.jpg');
C1_smooth = the1_convolution(C1_original,smoothingKernel);

C1_sobelX = the1_convolution(C1_smooth, sobelKernelX);
C1_sobelY = the1_convolution(C1_smooth, sobelKernelY);

C1_edges = max(C1_sobelX, C1_sobelY);
C1_edges = the1_convolution(C1_edges, smoothingKernel);

imwrite(rgb2gray(C1_edges), 'C1_edges.jpg');



%%%%%%%%%%%%%%%% Operations on C2 %%%%%%%%%%%%%%%%
C2_original = imread('C2.jpg');
C2_smooth = the1_convolution(C2_original,smoothingKernel);

C2_sobelX = the1_convolution(C2_smooth, sobelKernelX);
C2_sobelY = the1_convolution(C2_smooth, sobelKernelY);

C2_edges = max(C2_sobelX, C2_sobelY);
C2_edges = the1_convolution(C2_edges, smoothingKernel);

imwrite(rgb2gray(C2_edges), 'C2_edges.jpg');

