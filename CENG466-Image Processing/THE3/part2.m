% Ertugrul Aypek,	2171270
% Kemal Baþ,		2237048
function part2()

    mkdir('Segmentation_results_algo1');
    mkdir('Segmentation_results_algo2');

    thresh = 0.05;

	A = imread('CENG466_THE3_Part2/106025.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(A);
    imwrite(BW,'Segmentation_results_algo1/106025.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    A = rgb2gray(A);
    mask1 = false(size(A));
    mask1(240,160) = true;
    W = graydiffweight(A, mask1, 'GrayDifferenceCutoff', 45);
    BW = imsegfmm(W, mask1, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/106025.png')
    
    B1 = imread('CENG466_THE3_Part2/8049.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B1);
    imwrite(BW,'Segmentation_results_algo1/8049.png')
    figure
    imshow(BW)
    title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B1 = rgb2gray(B1);
    mask2 = false(size(B1)); 
    mask2(160,240) = true;
    W = graydiffweight(B1, mask2, 'GrayDifferenceCutoff', 45);
    BW = imsegfmm(W, mask2, thresh);
    figure
    imshow(BW)
    title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/8049.png')
    
    B2 = imread('CENG466_THE3_Part2/12003.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B2);
    imwrite(BW,'Segmentation_results_algo1/12003.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B2 = rgb2gray(B2);    
    W = graydiffweight(B2, mask2, 'GrayDifferenceCutoff', 45);
    thresh = 0.12;
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/12003.png')
    
    thresh = 0.05;    
    B3 = imread('CENG466_THE3_Part2/35058.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B3);
    imwrite(BW,'Segmentation_results_algo1/35058.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B3 = rgb2gray(B3);    
    W = graydiffweight(B3, mask2, 'GrayDifferenceCutoff', 35);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/35058.png')
    
    B4 = imread('CENG466_THE3_Part2/35070.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B4);
    imwrite(BW,'Segmentation_results_algo1/35070.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B4 = rgb2gray(B4);    
    W = graydiffweight(B4, mask2, 'GrayDifferenceCutoff', 45);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/35070.png')
    
    B5 = imread('CENG466_THE3_Part2/41004.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B5);
    imwrite(BW,'Segmentation_results_algo1/41004.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B5 = rgb2gray(B5);    
    W = graydiffweight(B5, mask2, 'GrayDifferenceCutoff', 45);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/41004.png')
    
    thresh = 0.21;
    B6 = imread('CENG466_THE3_Part2/42044.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B6);
    imwrite(BW,'Segmentation_results_algo1/42044.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B6 = rgb2gray(B6);    
    W = graydiffweight(B6, mask2, 'GrayDifferenceCutoff', 45);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/42044.png')
    
    thresh = 0.05;
    B7 = imread('CENG466_THE3_Part2/42078.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B7);
    imwrite(BW,'Segmentation_results_algo1/42078.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B7 = rgb2gray(B7);    
    W = graydiffweight(B7, mask2, 'GrayDifferenceCutoff', 45);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/42078.png')
    
    B8 = imread('CENG466_THE3_Part2/94079.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B8);
    imwrite(BW,'Segmentation_results_algo1/94079.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B8 = rgb2gray(B8);  
    W = graydiffweight(B8, mask2, 'GrayDifferenceCutoff', 45);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/94079.png')
    
    thresh = 0.035;
    B9 = imread('CENG466_THE3_Part2/100075.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B9);
    imwrite(BW,'Segmentation_results_algo1/100075.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B9 = rgb2gray(B9);  
    W = graydiffweight(B9, mask2, 'GrayDifferenceCutoff', 65);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/100075.png')
    
    thresh = 0.05;
    B10 = imread('CENG466_THE3_Part2/100080.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B10);
    imwrite(BW,'Segmentation_results_algo1/100080.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B10 = rgb2gray(B10);  
    W = graydiffweight(B10, mask1, 'GrayDifferenceCutoff', 65);
    BW = imsegfmm(W, mask1, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/100080.png')
    
    thresh = 0.015;
    B11 = imread('CENG466_THE3_Part2/105019.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B11);
    imwrite(BW,'Segmentation_results_algo1/105019.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B11 = rgb2gray(B11);  
    W = graydiffweight(B11, mask2, 'GrayDifferenceCutoff',25);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/105019.png')
    
    thresh = 0.09;
    B12 = imread('CENG466_THE3_Part2/105053.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B12);
    imwrite(BW,'Segmentation_results_algo1/105053.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B12 = rgb2gray(B12);  
    W = graydiffweight(B12, mask2, 'GrayDifferenceCutoff',45);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/105053.png')
    
    thresh = 0.15;
    B13 = imread('CENG466_THE3_Part2/108041.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B13);
    imwrite(BW,'Segmentation_results_algo1/108041.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B13 = rgb2gray(B13);  
    W = graydiffweight(B13, mask1, 'GrayDifferenceCutoff',65);
    BW = imsegfmm(W, mask1, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/108041.png')
    
    B14 = imread('CENG466_THE3_Part2/108073.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B14);
    imwrite(BW,'Segmentation_results_algo1/108073.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B14 = rgb2gray(B14);  
    W = graydiffweight(B14, mask2, 'GrayDifferenceCutoff',65);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/108073.png')
    
    thresh = 0.015;
    B15 = imread('CENG466_THE3_Part2/112082.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B15);
    imwrite(BW,'Segmentation_results_algo1/112082.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B15 = rgb2gray(B15);  
    W = graydiffweight(B15, mask2, 'GrayDifferenceCutoff',35);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/112082.png')
    
    B16 = imread('CENG466_THE3_Part2/113009.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B16);
    imwrite(BW,'Segmentation_results_algo1/113009.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B16 = rgb2gray(B16);  
    W = graydiffweight(B16, mask1, 'GrayDifferenceCutoff',35);
    BW = imsegfmm(W, mask1, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/113009.png')
    
    thresh = 0.15;
    B17 = imread('CENG466_THE3_Part2/113044.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B17);
    imwrite(BW,'Segmentation_results_algo1/113044.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B17 = rgb2gray(B17);  
    W = graydiffweight(B17, mask2, 'GrayDifferenceCutoff',70);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/113044.png')
    
    B18 = imread('CENG466_THE3_Part2/134052.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B18);
    imwrite(BW,'Segmentation_results_algo1/134052.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B18 = rgb2gray(B18);  
    W = graydiffweight(B18, mask2, 'GrayDifferenceCutoff',70);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/134052.png')
    
    thresh = 0.07;
    B19 = imread('CENG466_THE3_Part2/135069.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B19);
    imwrite(BW,'Segmentation_results_algo1/135069.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B19 = rgb2gray(B19);  
    W = graydiffweight(B19, mask2, 'GrayDifferenceCutoff',70);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/135069.png')
    
    B20 = imread('CENG466_THE3_Part2/163014.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B20);
    imwrite(BW,'Segmentation_results_algo1/163014.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B20 = rgb2gray(B20);  
    W = graydiffweight(B20, mask1, 'GrayDifferenceCutoff',50);
    BW = imsegfmm(W, mask1, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/163014.png')
    
    thresh = 0.05;
    B21 = imread('CENG466_THE3_Part2/268002.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B21);
    imwrite(BW,'Segmentation_results_algo1/268002.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B21 = rgb2gray(B21);  
    W = graydiffweight(B21, mask1, 'GrayDifferenceCutoff',35);
    BW = imsegfmm(W, mask1, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/268002.png')
    
    %because of the fishes and the water are same color in grayscale, only
    %the water bubbles grouped
    B22 = imread('CENG466_THE3_Part2/314016.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B22);
    imwrite(BW,'Segmentation_results_algo1/314016.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B22 = rgb2gray(B22);  
    W = graydiffweight(B22, mask2, 'GrayDifferenceCutoff',35);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/314016.png')
    
    thresh = 0.08;
    B23 = imread('CENG466_THE3_Part2/317080.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B23);
    imwrite(BW,'Segmentation_results_algo1/317080.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B23 = rgb2gray(B23);  
    W = graydiffweight(B23, mask2, 'GrayDifferenceCutoff',95);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/317080.png')
    
    thresh = 0.19;
    B24 = imread('CENG466_THE3_Part2/326038.jpg');
    %watershed segmentation to run it please take out of comment
    BW = watershed(B24);
    imwrite(BW,'Segmentation_results_algo1/326038.png')
    %figure
    %imshow(BW)
    %title('Segmented Image')
    %binary image segmentation to show result take out of comment last 3
    B24 = rgb2gray(B24);  
    W = graydiffweight(B24, mask2, 'GrayDifferenceCutoff',70);
    BW = imsegfmm(W, mask2, thresh);
    %figure
    %imshow(BW)
    %title('Segmented Image')
    imwrite(BW,'Segmentation_results_algo2/326038.png')
end
