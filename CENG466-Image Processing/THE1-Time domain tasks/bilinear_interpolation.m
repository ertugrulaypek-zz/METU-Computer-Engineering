% Ertugrul Aypek, 2171270

function resultImage = bilinear_interpolation(inputImage, final_size)


dimension1 = final_size(1);
dimension2 = final_size(2);
resultImage = imresize(inputImage, [dimension1,dimension2], 'bilinear');

end
