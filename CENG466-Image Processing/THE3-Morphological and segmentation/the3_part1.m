% Ertugrul Aypek,	2171270
% Kemal Baþ,		2237048

A1 = rgb2gray(imread('A1.png'));
A2 = rgb2gray(imread('A2.png'));
A3 = rgb2gray(imread('A3.png'));
A4 = rgb2gray(imread('A4.png'));
A5 = (imread('A5.png'));
A5 = A5(:,:,3);
A6 = (imread('A6.png'));
A6 = A6(:,:,3);

A1_height = size(A1,1);
A1_width = size(A1,2);
A2_height = size(A2,1);
A2_width = size(A2,2);
A3_height = size(A3,1);
A3_width = size(A3,2);
A4_height = size(A4,1);
A4_width = size(A4,2);
A5_height = size(A5,1);
A5_width = size(A5,2);;
A6_height = size(A6,1);
A6_width = size(A6,2);


%%%%%%%%%%%%%%%%%%%% A1 %%%%%%%%%%%%%%%%%%%%

A1_binary = zeros(A1_height, A1_width, 'logical'); 
for i=1:A1_height
    for j=1:A1_width
        if (A1(i,j) < 100)
            A1_binary(i,j) = 1;
        else
            A1_binary(i,j) = 0;
        end
    end
end

structuringElement = ones(5,5,'logical');
A1_binary = imclose(A1_binary, structuringElement);

[result, n] = bwlabel(A1_binary);

imwrite(result, 'part1_A1.png');
fprintf('The number of flying jets in image A1 is %d\n', n);


%%%%%%%%%%%%%%%%%%%% A2 %%%%%%%%%%%%%%%%%%%%


A2_binary = zeros(A2_height, A2_width, 'logical'); 
for i=1:A2_height
    for j=1:A2_width
        if (A2(i,j) < 100 && A2(i,j)>75)
            A2_binary(i,j) = 1;
        else
            A2_binary(i,j) = 0;
        end
    end
end

topHalfGetterMask = zeros(A2_height, A2_width, 'logical');
topHalfGetterMask([1:A2_height/2], [1:A2_width]) = 1;
topHalf = and(A2_binary, topHalfGetterMask);

structuringElement = ones(3,3,'logical');
A2_binary = imopen(A2_binary, structuringElement);

A2_binary = or(A2_binary, topHalf);

structuringElement = ones(15,15,'logical');
A2_binary = imdilate(A2_binary, structuringElement);

A2_binary = imfill(A2_binary, 'holes');

A2_binary = bwareaopen(A2_binary,1000);

[result, n] = bwlabel(A2_binary);

imwrite(result, 'part1_A2.png');
fprintf('The number of flying jets in image A2 is %d\n', n);

%%%%%%%%%%%%%%%%%%%% A3 %%%%%%%%%%%%%%%%%%%%

A3_binary = zeros(A3_height, A3_width, 'logical'); 
for i=1:A3_height
    for j=1:A3_width
        if (A3(i,j) < 55)
            A3_binary(i,j) = 1;
        else
            A3_binary(i,j) = 0;
        end
    end
end

structuringElement = ones(5,5,'logical');
A3_binary = imclose(A3_binary, structuringElement);
A3_binary = bwareaopen(A3_binary, 10);

[result, n] = bwlabel(A3_binary);

imwrite(result, 'part1_A3.png');

fprintf('The number of flying jets in image A3 is %d\n', n);



%%%%%%%%%%%%%%%%%%%% A4 %%%%%%%%%%%%%%%%%%%%

A4_binary = zeros(A4_height, A4_width, 'logical');
for i=1:A4_height
    for j=1:A4_width
        if (A4(i,j) < 70)
            A4_binary(i,j) = 1;
        else
            A4_binary(i,j) = 0;
        end
    end
end


A4_binary = bwareaopen(A4_binary, 200);

A4_binary = imfill(A4_binary,'holes');


[result, n] = bwlabel(A4_binary);

imwrite(result, 'part1_A4.png');

fprintf('The number of flying jets in image A4 is %d\n', n);


%%%%%%%%%%%%%%%%%%%% A5 %%%%%%%%%%%%%%%%%%%%

A5_binary = zeros(A5_height, A5_width, 'logical');
for i=1:A5_height
    for j=1:A5_width
        if (A5(i,j) < 150)
            A5_binary(i,j) = 1;
        else
            A5_binary(i,j) = 0;
        end
    end
end

structuringElement = ones(11,11,'logical');

A5_binary = imclose(A5_binary, structuringElement);

A5_binary = bwareaopen(A5_binary, 5000);

A5_binary = imfill(A5_binary,'holes');

[result, n] = bwlabel(A5_binary);

imwrite(result, 'part1_A5.png');

fprintf('The number of flying jets in image A5 is %d\n', n);


%%%%%%%%%%%%%%%%%%%% A6 %%%%%%%%%%%%%%%%%%%%

A6_binary = zeros(A6_height, A6_width, 'logical'); 
for i=1:A6_height
    for j=1:A6_width
        if (A6(i,j) < 55 )
            A6_binary(i,j) = 1;
        else
            A6_binary(i,j) = 0;
        end
    end
end

A6_binary = bwareaopen(A6_binary, 10);

structuringElement = ones(5,5,'logical');
A6_binary = imdilate(A6_binary, structuringElement);

A6_binary = imfill(A6_binary,'holes');

A6_binary = bwareaopen(A6_binary, 1650);

A6_mask = ones(A6_height, A6_width,'logical');
A6_mask([A6_height/2-25:A6_height/2+200], [1:A6_width/2]) = 0;

A6_binary = and(A6_binary, A6_mask);

[result, n] = bwlabel(A6_binary);

imwrite(result, 'part1_A6.png');
fprintf('The number of flying jets in image A6 is %d\n', n);
