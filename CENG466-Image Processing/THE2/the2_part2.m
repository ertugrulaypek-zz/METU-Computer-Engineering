% Ertugrul Aypek,	2171270
% Kemal Baþ,		2237048


%%%%%%%%%%%%%%%%% B1 %%%%%%%%%%%%%%%%%
B1 = (imread('B1.jpg'));
B1 = rgb2gray(B1);

fstB1 = size(B1,1);
sndB1 = size(B1,2);

B1_centerX = round(fstB1/2);
B1_centerY = round(sndB1/2);

B1_highPassMask = ones(fstB1,sndB1);
B1_maskRadius = 40;


% Create smooth high pass filter
for i=B1_centerX-B1_maskRadius:B1_centerX+B1_maskRadius
	for j=B1_centerY-B1_maskRadius:B1_centerY+B1_maskRadius
        dist = sqrt( (B1_centerX-i).^2 + (B1_centerY-j).^2 );
		if(dist < B1_maskRadius )
			B1_highPassMask(i,j) = B1_highPassMask(i,j)*(dist/B1_maskRadius);
		end
	end
end


fftB1 = fftshift(fft2(B1));
fftB1 = log(fftB1);

filteredB1 = fftB1.*B1_highPassMask;

e=(exp(1));
for a=1:sndB1
    for b=1:fstB1
        filteredB1(b,a)= e^filteredB1(b,a);
    end
end

resultB1 = ifft2(filteredB1);

resultB1 = (resultB1(:,:,1));
resultB1 = abs(resultB1/256);

imwrite(resultB1,'B1_output.jpg');


%%%%%%%%%%%%%%%%% B2 %%%%%%%%%%%%%%%%%
B2 = (imread('B2.jpg'));
B2 = rgb2gray(B2);

fstB2 = size(B2,1);
sndB2 = size(B2,2);

B2_centerX = round(fstB2/2);
B2_centerY = round(sndB2/2);

B2_highPassMask = ones(fstB2,sndB2);
B2_maskRadius = 55;


% Create smooth high pass filter
for i=B2_centerX-B2_maskRadius:B2_centerX+B2_maskRadius
	for j=B2_centerY-B2_maskRadius:B2_centerY+B2_maskRadius
        dist = sqrt( (B2_centerX-i).^2 + (B2_centerY-j).^2 );
		if(dist < B2_maskRadius )
			B2_highPassMask(i,j) = B2_highPassMask(i,j)*(dist/B2_maskRadius);
		end
	end
end


fftB2 = fftshift(fft2(B2));
fftB2 = log(fftB2);

filteredB2 = fftB2.*B2_highPassMask;



e=(exp(1));
for a=1:sndB2
    for b=1:fstB2
        filteredB2(b,a)= e^filteredB2(b,a);
    end
end


resultB2 = ifft2(filteredB2);

resultB2 = (resultB2(:,:,1));
resultB2 = abs(resultB2/256);

imwrite(resultB2,'B2_output.jpg');