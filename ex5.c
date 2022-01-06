#include <stdbool.h> 
#include <math.h>
#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b) 
#define calcIndex(i,j,n) ((i)*(n)+(j))

//sizeof(pixel) is 3
typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernel[3][3], int kernelScale, bool filter) {

	int ii, jj;
	int currRow, currCol;
	pixel_sum sum;
	pixel current_pixel;
	int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
	int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
	int min_row, min_col, max_row, max_col;
	pixel loop_pixel;

	//Initializes all fields of sum to 0
	sum.red = sum.green = sum.blue = 0;

	for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
			int kRow, kCol;
			int t1= ii*dim;

			// compute row index in kernel
			if (ii < i) {
				kRow = 0;
			} else if (ii > i) {
				kRow = 2;
			} else {
				kRow = 1;
			}

		for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {

			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			int resultKernel=kernel[kRow][kCol];
			int resultSrc= t1 + jj;

			// apply kernel on pixel at [ii,jj]
			sum.red += ((int) src[resultSrc].red) * resultKernel;
			sum.green += ((int) src[resultSrc].green) * resultKernel;
			sum.blue += ((int) src[resultSrc].blue) * resultKernel;
		}
	}

	if (filter) {
		// find min and max coordinates
		for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
			int t2=ii*dim;
			for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
				// check if smaller than min or higher than max and update
				loop_pixel = src[t2+jj];
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
					min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					min_row = ii;
					min_col = jj;
				}
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
					max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					max_row = ii;
					max_col = jj;
				}
			}
		}
		// filter out min and max
		//Sums pixel values, scaled by given weight
		int result3=(min_row)*(dim)+(min_col);
		int result4=(max_row)*(dim)+(max_col);

		sum.red -= ((int) src[result3].red);
		sum.green -= ((int) src[result3].green);
		sum.blue -= ((int) src[result3].blue);
		sum.red -= ((int) src[result4].red);
		sum.green -= ((int) src[result4].green);
		sum.blue -= ((int) src[result4].blue);
	}

	// assign kernel's result to pixel at [i,j]
	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
	current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
	current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
	current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));
	
	return current_pixel;
}

void doConvolution(Image *image, int kernel[3][3], int kernelScale, bool filter) {

	double t3= pow((double)m,2);
	double z=t3+t3+t3;
	pixel* pixelsImg = malloc(z);
	pixel* backupOrg = malloc(z);

	//instead of charsToPixels
	int row, col;
	for (row = 0 ; row < m ; row++) {
		int t4=row*m;
		for (col = 0 ; col < m ; col++) {
			int t5=t4+col;
			int t6=t5+t5+t5;
			pixelsImg[t5].red = image->data[t6];
			pixelsImg[t5].green = image->data[t6 + 1];
			pixelsImg[t5].blue = image->data[t6 + 2];
		}
	}

	//instead of copyPixels 
	for (row = 0 ; row < m ; row++) {
		int t7=row*m;
		for (col = 0 ; col < m ; col++) {
			int t8=t7+col;
			backupOrg[t8].red = pixelsImg[t8].red;
			backupOrg[t8].green = pixelsImg[t8].green;
			backupOrg[t8].blue = pixelsImg[t8].blue;
		}
	}

/* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
* column index smaller than 1 */
	int i, j;
	for (i = 1 ; i < m - 1; i++) {
		int t3=i*m;
		for (j =  1 ; j < m - 1 ; j++) {
			pixelsImg[t3+j] = applyKernel(m, i, j, backupOrg, kernel, kernelScale, filter);
		}
	}

	//instead of pixelsToChars
	for (row = 0 ; row < m ; row++) {
		int t9=row*m;
		for (col = 0 ; col < m ; col++) {
			int t10=row*m+col;
			int t11=t10+t10+t10;
			image->data[t11] = pixelsImg[t10].red;
			image->data[t11 + 1] = pixelsImg[t10].green;
			image->data[t11 + 2] = pixelsImg[t10].blue;
		}
	}

	free(pixelsImg);
	free(backupOrg);
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	if (flag == '1') {	
		// blur image
		doConvolution(image, blurKernel, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		doConvolution(image, sharpKernel, 1, false);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution(image, blurKernel, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}
