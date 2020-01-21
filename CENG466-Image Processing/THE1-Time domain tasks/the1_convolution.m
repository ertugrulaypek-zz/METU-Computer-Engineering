% Ertugrul Aypek, 2171270


function convolvedResult = the1_convolution(inputImage, kernel)
	

	inputImage = double(inputImage);
	kernel = double(kernel);

	inputSize = size(inputImage);	% 3D
	kernelSize = size(kernel);		% 2D

	convolvedResult = zeros(inputSize(1)-kernelSize(1)+1, inputSize(2)-kernelSize(2)+1, 3, 'double');

	for i=1:inputSize(1) - kernelSize(1) + 1
		for j=1:inputSize(2) - kernelSize(2) + 1
			sumR = double(0);
			sumG = double(0);
			sumB = double(0);
			
			for iter1=1:kernelSize(1)
				for iter2=1:kernelSize(2)

					% apply different filters for each channel
					if size(kernelSize, 2)==3		
						sumR = sumR + inputImage(i+iter1-1, j+iter2-1,1) * kernel(iter1,iter2,1);
						sumG = sumG + inputImage(i+iter1-1, j+iter2-1,2) * kernel(iter1,iter2,2);
						sumB = sumB + inputImage(i+iter1-1, j+iter2-1,3) * kernel(iter1,iter2,3);	

					% apply the same filter for each channel
					else						
						sumR = sumR + inputImage(i+iter1-1, j+iter2-1,1) * kernel(iter1,iter2);
						sumG = sumG + inputImage(i+iter1-1, j+iter2-1,2) * kernel(iter1,iter2);
						sumB = sumB + inputImage(i+iter1-1, j+iter2-1,3) * kernel(iter1,iter2);
					end

				end
			end

			convolvedResult(i,j,1) = sumR;
			convolvedResult(i,j,2) = sumG;
			convolvedResult(i,j,3) = sumB;
		end
	end

end
