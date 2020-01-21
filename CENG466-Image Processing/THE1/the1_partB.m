% Ertugrul Aypek, 2171270

%%%%%%%%%%%%%%%%%%%%%%%%%%%% Read images %%%%%%%%%%%%%%%%%%%%%%%%%%%%
imB1 = imread('B1.jpg')
imB2 = imread('B2.jpg')
imB3 = imread('B3.jpg')
imB4 = imread('B4.jpg')

%%%%%%%%%%%%%%%%%%%%%%%%%%%% Get histograms of each channel %%%%%%%%%%%%%%%%%%%%%%%%%%%%
B1_R_histogram = getHistogramOfChannel(imB1(:,:,1))
B1_G_histogram = getHistogramOfChannel(imB1(:,:,2))
B1_B_histogram = getHistogramOfChannel(imB1(:,:,3))

B2_R_histogram = getHistogramOfChannel(imB2(:,:,1))
B2_G_histogram = getHistogramOfChannel(imB2(:,:,2))
B2_B_histogram = getHistogramOfChannel(imB2(:,:,3))

B3_R_histogram = getHistogramOfChannel(imB3(:,:,1))
B3_G_histogram = getHistogramOfChannel(imB3(:,:,2))
B3_B_histogram = getHistogramOfChannel(imB3(:,:,3))

B4_R_histogram = getHistogramOfChannel(imB4(:,:,1))
B4_G_histogram = getHistogramOfChannel(imB4(:,:,2))
B4_B_histogram = getHistogramOfChannel(imB4(:,:,3))


%%%%%%%%%%%%%%%%%%%%%%%%%%%% Get cdf of each channel %%%%%%%%%%%%%%%%%%%%%%%%%%%%
B1_R_cdf = getCDF(B1_R_histogram, size(imB1,1) * size(imB1,2))
B1_G_cdf = getCDF(B1_G_histogram, size(imB1,1) * size(imB1,2))
B1_B_cdf = getCDF(B1_B_histogram, size(imB1,1) * size(imB1,2))

B2_R_cdf = getCDF(B2_R_histogram, size(imB2,1) * size(imB2,2))
B2_G_cdf = getCDF(B2_G_histogram, size(imB2,1) * size(imB2,2))
B2_B_cdf = getCDF(B2_B_histogram, size(imB2,1) * size(imB2,2))

B3_R_cdf = getCDF(B3_R_histogram, size(imB3,1) * size(imB3,2))
B3_G_cdf = getCDF(B3_G_histogram, size(imB3,1) * size(imB3,2))
B3_B_cdf = getCDF(B3_B_histogram, size(imB3,1) * size(imB3,2))

B4_R_cdf = getCDF(B4_R_histogram, size(imB4,1) * size(imB4,2))
B4_G_cdf = getCDF(B4_G_histogram, size(imB4,1) * size(imB4,2))
B4_B_cdf = getCDF(B4_B_histogram, size(imB4,1) * size(imB4,2))


x = zeros(256,1,'uint8')
for i=1:256
	x(i)=i
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%% Match B1 when reference is B2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%
map_B1_R_ref_B2 = getMappingArray(B1_R_cdf,B2_R_cdf)
map_B1_B_ref_B2 = getMappingArray(B1_B_cdf,B2_B_cdf)
map_B1_G_ref_B2 = getMappingArray(B1_G_cdf,B2_G_cdf)

B1_matched_R = map_B1_R_ref_B2( double(imB1(:,:,1)) + 1)
B1_matched_G = map_B1_G_ref_B2( double(imB1(:,:,2)) + 1)
B1_matched_B = map_B1_B_ref_B2( double(imB1(:,:,3)) + 1)

B1_matched_ref_B2 = uint8(cat(3, B1_matched_R, B1_matched_G, B1_matched_B))

plot(x,getHistogramOfChannel(B1_matched_R),'R',x,getHistogramOfChannel(B1_matched_G),'G',x,getHistogramOfChannel(B1_matched_B),'B')
saveas(gcf, 'B1_histmatch.jpg')

imwrite(B1_matched_ref_B2, 'B1_histmatch_output.jpg')


%%%%%%%%%%%%%%%%%%%%%%%%%%%% Match B2 when reference is B1 %%%%%%%%%%%%%%%%%%%%%%%%%%%%
map_B2_R_ref_B1 = getMappingArray(B2_R_cdf,B1_R_cdf)
map_B2_B_ref_B1 = getMappingArray(B2_B_cdf,B1_B_cdf)
map_B2_G_ref_B1 = getMappingArray(B2_G_cdf,B1_G_cdf)

B2_matched_R = map_B2_R_ref_B1( double(imB2(:,:,1)) + 1)
B2_matched_G = map_B2_G_ref_B1( double(imB2(:,:,2)) + 1)
B2_matched_B = map_B2_B_ref_B1( double(imB2(:,:,3)) + 1)

B2_matched_ref_B1 = uint8(cat(3, B2_matched_R, B2_matched_G, B2_matched_B))

plot(x,getHistogramOfChannel(B2_matched_R),'R',x,getHistogramOfChannel(B2_matched_G),'G',x,getHistogramOfChannel(B2_matched_B),'B')
saveas(gcf, 'B2_histmatch.jpg')

imwrite(B2_matched_ref_B1, 'B2_histmatch_output.jpg')



%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Match B3 when reference is B4 %%%%%%%%%%%%%%%%%%%%%%%%%%%%
map_B3_R_ref_B4 = getMappingArray(B3_R_cdf,B4_R_cdf)
map_B3_B_ref_B4 = getMappingArray(B3_B_cdf,B4_B_cdf)
map_B3_G_ref_B4 = getMappingArray(B3_G_cdf,B4_G_cdf)

B3_matched_R = map_B3_R_ref_B4( double(imB3(:,:,1)) + 1)
B3_matched_G = map_B3_G_ref_B4( double(imB3(:,:,2)) + 1)
B3_matched_B = map_B3_B_ref_B4( double(imB3(:,:,3)) + 1)

B3_matched_ref_B4 = uint8(cat(3, B3_matched_R, B3_matched_G, B3_matched_B))

plot(x,getHistogramOfChannel(B3_matched_R),'R',x,getHistogramOfChannel(B3_matched_G),'G',x,getHistogramOfChannel(B3_matched_B),'B')
saveas(gcf, 'B3_histmatch.jpg')

imwrite(B3_matched_ref_B4, 'B3_histmatch_output.jpg')


%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Match B4 when reference is B3 %%%%%%%%%%%%%%%%%%%%%%%%%%%%
map_B4_R_ref_B3 = getMappingArray(B4_R_cdf,B3_R_cdf)
map_B4_B_ref_B3 = getMappingArray(B4_B_cdf,B3_B_cdf)
map_B4_G_ref_B3 = getMappingArray(B4_G_cdf,B3_G_cdf)

B4_matched_R = map_B4_R_ref_B3( double(imB4(:,:,1)) + 1)
B4_matched_G = map_B4_G_ref_B3( double(imB4(:,:,2)) + 1)
B4_matched_B = map_B4_B_ref_B3( double(imB4(:,:,3)) + 1)

B4_matched_ref_B3 = uint8(cat(3, B4_matched_R, B4_matched_G, B4_matched_B))

plot(x,getHistogramOfChannel(B4_matched_R),'R',x,getHistogramOfChannel(B4_matched_G),'G',x,getHistogramOfChannel(B4_matched_B),'B')
saveas(gcf, 'B4_histmatch.jpg')

imwrite(B4_matched_ref_B3, 'B4_histmatch_output.jpg')



function histogram = getHistogramOfChannel(inputImage)

	dimension1 = size(inputImage, 1);
	dimension2 = size(inputImage, 2);
	histogram = zeros(1,256,'uint32')

	for i=1:dimension1
	
		for j=1:dimension2
			histogram(double(inputImage(i,j))+1) = histogram(double(inputImage(i,j))+1) + 1
		end
	end

end


function cumulative = getCDF(histogram, matrixSize)

	cumulative = zeros(1,256,'uint32')
	cumulative(1) = histogram(1)

	for i=2:256
		cumulative(i) = cumulative(i-1) + histogram(i)
	end

	cumulative = cumulative /  matrixSize

end

function mappingArray = getMappingArray(cdfToMatch, cdfBase)

	mappingArray = zeros(1,256,'uint32')

	for i=1:256
		differenceForPixelValue = abs(cdfToMatch(i) - cdfBase)
		[~, minDistanceIndex] = min(differenceForPixelValue)
		mappingArray(i) = minDistanceIndex-1
	end

end




