# Lens Distortion

## First Phase

* Extract candidate areas from Sobel images for the X and Y direction. 

## Second Phase

* Extract pixels of lines from previously calculated candidate areas depending on the direction of their Sobel derivative.
* Group lines by candidate areas for later inspection.

## Third Phase

* In each group of lines, compare angle between each of the pixels in the line and find the closest angle to 0 (x or y axis)
* Create an image with synthetized lines for later comparison with the Sobel image

## Forth Phase

* Compare image of synthetized lines with Sobel image and calculate the distance between synthetized line and the pixel corresponding to that position in the sobel image. 
* Save distance for all the comparisons since this will be used for the factors for interpolating the image in the end

