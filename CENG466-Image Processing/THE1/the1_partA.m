% Ertugrul Aypek, 2171270


%%%%%%%%%%%%%%%%%% Part for A1.jpg %%%%%%%%%%%%%%%%%%
A1_shrinkedImage = imread('A1_shrinked.jpg');

A1_originalImage = imread('A1.jpg');

A1_result_bilinear = bilinear_interpolation(A1_shrinkedImage, size(A1_originalImage));

A1_result_bicubic = bicubic_interpolation(A1_shrinkedImage, size(A1_originalImage))

imwrite(A1_result_bilinear, 'A1_result_bilinear.jpg');

imwrite(A1_result_bicubic, 'A1_result_bicubic.jpg');


%%%%%%%%%%%%%%%%%% Part for A2.jpg %%%%%%%%%%%%%%%%%%
A2_shrinkedImage = imread('A2_shrinked.jpg');

A2_originalImage = imread('A2.jpg');

A2_result_bilinear = bilinear_interpolation(A2_shrinkedImage, size(A2_originalImage));

A2_result_bicubic = bicubic_interpolation(A2_shrinkedImage, size(A2_originalImage))

imwrite(A2_result_bilinear, 'A2_result_bilinear.jpg');

imwrite(A2_result_bicubic, 'A2_result_bicubic.jpg');


%%%%%%%%%%%%%%%%%% Part for A3.jpg %%%%%%%%%%%%%%%%%%
A3_shrinkedImage = imread('A3_shrinked.jpg');

A3_originalImage = imread('A3.jpg');

A3_result_bilinear = bilinear_interpolation(A3_shrinkedImage, size(A3_originalImage));

A3_result_bicubic = bicubic_interpolation(A3_shrinkedImage, size(A3_originalImage))

imwrite(A3_result_bilinear, 'A3_result_bilinear.jpg');

imwrite(A3_result_bicubic, 'A3_result_bicubic.jpg');



%%%%%%%%%%%%%%%%%% Euclidean distance between images %%%%%%%%%%%%%%%%%%
A1_bilinear_squared_difference = ( double(A1_result_bilinear) - double(A1_originalImage) ).^2
A1_bilinear_euclidean_distance = sum(A1_bilinear_squared_difference(:)).^0.5

A1_bicubic_squared_difference = ( double(A1_result_bicubic) - double(A1_originalImage) ).^2
A1_bicubic_euclidean_distance = sum(A1_bicubic_squared_difference(:)).^0.5


A2_bilinear_squared_difference = ( double(A2_result_bilinear) - double(A2_originalImage) ).^2
A2_bilinear_euclidean_distance = sum(A2_bilinear_squared_difference(:)).^0.5

A2_bicubic_squared_difference = ( double(A2_result_bicubic) - double(A2_originalImage) ).^2
A2_bicubic_euclidean_distance = sum(A2_bicubic_squared_difference(:)).^0.5


A3_bilinear_squared_difference = ( double(A3_result_bilinear) - double(A3_originalImage) ).^2
A3_bilinear_euclidean_distance = sum(A3_bilinear_squared_difference(:)).^0.5

A3_bicubic_squared_difference = ( double(A3_result_bicubic) - double(A3_originalImage) ).^2
A3_bicubic_euclidean_distance = sum(A3_bicubic_squared_difference(:)).^0.5


disp('A1_bilinear_euclidean_distance')
disp(A1_bilinear_euclidean_distance)

disp('A1_bicubic_euclidean_distance')
disp(A1_bicubic_euclidean_distance)

disp('A2_bilinear_euclidean_distance')
disp(A2_bilinear_euclidean_distance)

disp('A2_bicubic_euclidean_distance')
disp(A2_bicubic_euclidean_distance)

disp('A3_bilinear_euclidean_distance')
disp(A3_bilinear_euclidean_distance)

disp('A3_bicubic_euclidean_distance')
disp(A3_bicubic_euclidean_distance)