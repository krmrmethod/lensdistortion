# Lens Distortion

## OpenCV

Using the OpenCV implementation we can undistort images using either a single or multiple calibration images (i.e. for stereo applications). The process is as follows:

1. Collect images and apply some pre-processing operations

   1. Blur using bilateral filter (or Gaussian)
   2. Create binary image using a threshold

2. Using pre-processed images extract calibration parameters

   * Distortion coefficients

   ![image-20211018011526714](C:\Users\Nelson Suarez\AppData\Roaming\Typora\typora-user-images\image-20211018011526714.png)

   * Camera Matrix

   ![image-20211018011547018](C:\Users\Nelson Suarez\AppData\Roaming\Typora\typora-user-images\image-20211018011547018.png)

   ​		where $f_x$, $f_y$ refer to the focal length and $c_x$, $c_y$ are the optical centers.

3. Finally using this parameters to apply an undistort function and interpolate the pixels in the image.

## Experimental

### First Step

* Extract candidate areas from Sobel images for the X and Y direction. 

### Second Step

* Extract pixels of lines from previously calculated candidate areas depending on the direction of their Sobel derivative.
* Group lines by candidate areas for later inspection.

### Third Step

* In each group of lines, compare angle between each of the pixels in the line and find the closest angle to 0 (x or y axis)
* Create an image with synthetized lines for later comparison with the Sobel image

### Forth Step

* Compare image of synthetized lines with Sobel image and calculate the distance between synthetized line and the pixel corresponding to that position in the sobel image. 
* Save distance for all the comparisons since this will be used for the factors for interpolating the image in the end

