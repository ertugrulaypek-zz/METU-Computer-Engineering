% Ertugrul Aypek,	2171270
% Kemal Baþ,		2237048

function the2_part1()

Img = imread('A1.png');
%figure,imshow(Img(:,:,1), []);
%title('original 2');
Img = fftshift(fft2(Img));
Img = log(Img);
for x=1:485
    if x<194 || x>292
        Img(x,364) = 0;
    end
end
for y=1:727
    if y<315 || y>413
        Img(243,y) = 0;
    end
end
%figure,imshow(Img(:,:,1), []);
%title('deleted freq');

for a=1:727
    for b=1:485
        Img(b,a)= (exp(1))^Img(b,a);
    end
end
Img = ifft2(ifftshift( Img));
%figure,imshow(Img(:,:,1), []);
%title('result1');
name1 = 'A1_denoised.png';
o = (Img(:,:,1));
o = abs(o/256);
%imshow(o);
imwrite(o,name1);



Img2 = imread('A2.png');
Img2 = fftshift(fft2(Img2));
Img2 = log(Img2);

%figure,imshow(Img2(:,:,1), []);
%title('original 2');

for x=1:1049
    for y=1:1679
        if abs(Img2(x,y))>9.163 && abs(Img2(x,y))<9.35 && ((y-840)^2 + (x-525)^2) > 3600
            Img2(x,y) = 0;
        end
    end
    
end
%figure,imshow(Img2(:,:,1), []);
%title('deleted freq');

for a=1:1049
    for b=1:1679
        Img2(a,b)= (exp(1))^Img2(a,b);
    end
end
Img2 = ifft2(ifftshift( Img2));
%figure,imshow(Img2(:,:,1), []);
%title('result2');
name2 = 'A2_denoised.png';
Img2 = Img2 - min(Img2(:)) ;
Img2 = Img2 / max(Img2(:)) ;
%figure,imshow(Img2);
%title('asd');
imwrite(Img2,name2);



Img3 = imread('A3.png');
r = Img3(:,:,1);
g = Img3(:,:,2);
b = Img3(:,:,3);


r = fftshift(fft2(r));
r = log(r);
%figure,imshow(r(:,:,1), []);
%title('original 1');

g = fftshift(fft2(g));
g = log(g);
%figure,imshow(r(:,:,1), []);
%title('original 2');

b = fftshift(fft2(b));
b = log(b);
%figure,imshow(r(:,:,1), []);
%title('original 3');


for x=1:1141
    for y=1:1857
        if y == 883 || y == 882 || y == 975 || y == 976 || x == 542 || x == 543 || x == 599 || x == 600 || ((x == 571) && (y<883 || y>975)) || ((y == 929) && (x<542 || x>600)) || x == 485|| x == 514 || x == 628 || x == 657 
            r(x,y) = 0;
            g(x,y) = 0;
            b(x,y) = 0;
        end
    end
end

%figure,imshow(r(:,:,1), []);
%title('deleted freq1');

%figure,imshow(g(:,:,1), []);
%title('deleted freq2');

%figure,imshow(b(:,:,1), []);
%title('deleted freq3');


e = (exp(1));
for a=1:1141
    for c=1:1857
        r(a,c)= e^r(a,c);
        g(a,c)= e^g(a,c);
        b(a,c)= e^b(a,c);
    end
end

r = ifft2(ifftshift( r));
%figure,imshow(r, []);
%title('result1');

g = ifft2(ifftshift( g));
%figure,imshow(g, []);
%title('result2');

b = ifft2(ifftshift( b));
%figure,imshow(b, []);
%title('result2');

rgb = cat(3,r,g,b);
rgb = rgb/256;
%figure,imshow(rgb/256);
%title('result3');
name3 = 'A3_denoised.png';
imwrite(rgb,name3);

end