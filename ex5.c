#include <stdbool.h> 
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
		//instead of doConvolution(image, blurKernel, 9, false)
			int t1= m*m;
			int z=t1+t1+t1;
			pixel* pixelsImg = malloc(z);
			pixel* backupOrg = malloc(z);

			//instead of charsToPixels
			for (int row = 0 ; row < m ; row++) {
				int t2=row*m;
				for (int col = 0 ; col < m ; col++) {
					int t3=t2+col;
					int t4=t3+t3+t3;
					pixelsImg[t3].red = image->data[t4];
					pixelsImg[t3].green = image->data[t4 + 1];
					pixelsImg[t3].blue = image->data[t4 + 2];
					backupOrg[t3].red = pixelsImg[t3].red;
					backupOrg[t3].green = pixelsImg[t3].green;
					backupOrg[t3].blue = pixelsImg[t3].blue;
				}
			}

		/* Apply the kernel over each pixel.
		* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
		* column index smaller than 1 */
			for (int i = 1 ; i < m - 1; i++) {
				int t2=i*m;
				int iiLimit = min(i+1, m-1);
				for (int j =  1 ; j < m - 1 ; j++) {

					// Applies kernel for pixel at (i,j)
					//	instead of kernelApply function
						int jjLimit=min(j+1, m-1);
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

						for(ii = max(i-1, 0); ii <= iiLimit; ii++) {
							int kRow, kCol;
							int t3= ii*m;

							// compute row index in kernel
							if (ii < i) {
								kRow = 0;
								} else if (ii > i) {
										kRow = 2;
								} else {
										kRow = 1;
								}

							for(jj = max(j-1, 0); jj <= jjLimit; jj++) {

								// compute column index in kernel
								if (jj < j) {
									kCol = 0;
								} else if (jj > j) {
										kCol = 2;
								} else {
										kCol = 1;
								}

								int resultSrc= t3 + jj;

								// apply kernel on pixel at [ii,jj]
								sum.red += ((int) backupOrg[resultSrc].red);
								sum.green += ((int) backupOrg[resultSrc].green);
								sum.blue += ((int) backupOrg[resultSrc].blue);
							}
						}

					sum.red = sum.red / 9;
					sum.green = sum.green / 9;
					sum.blue = sum.blue / 9;

					// truncate each pixel's color values to match the range [0,255]
					current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
					current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
					current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));

					pixelsImg[t2+j] = current_pixel;
				}
			}

			//instead of pixelsToChars
			for (int row = 0 ; row < m ; row++) {
				int t3=row*m;
				for (int col = 0 ; col < m ; col++) {
					int t2=t3+col;
					int t4=t2+t2+t2;
					image->data[t4] = pixelsImg[t2].red;
					image->data[t4 + 1] = pixelsImg[t2].green;
					image->data[t4 + 2] = pixelsImg[t2].blue;
				}
			}

			free(pixelsImg);
			free(backupOrg);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		//instead of doConvolution(image, sharpKernel, 1, false);
		pixel* pixelsImg2 = malloc(z);
		pixel* backupOrg2 = malloc(z);

		//instead of charsToPixels
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				pixelsImg2[t3].red = image->data[t4];
				pixelsImg2[t3].green = image->data[t4 + 1];
				pixelsImg2[t3].blue = image->data[t4 + 2];
				backupOrg2[t3].red = pixelsImg2[t3].red;
				backupOrg2[t3].green = pixelsImg2[t3].green;
				backupOrg2[t3].blue = pixelsImg2[t3].blue;
			}
		}

	/* Apply the kernel over each pixel.
	* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
	* column index smaller than 1 */
		for (int i = 1 ; i < m - 1; i++) {
			int t3=i*m;
			int iiLimit=min(i+1, m-1);
			for (int j =  1 ; j < m - 1 ; j++) {

				// Applies kernel for pixel at (i,j)
				//	instead of kernelApply function

					int ii, jj;
					int jjLimit=min(j+1, m-1);
					int currRow, currCol;
					pixel_sum sum;
					pixel current_pixel;
					int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
					int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
					int min_row, min_col, max_row, max_col;
					pixel loop_pixel;

					//Initializes all fields of sum to 0
					sum.red = sum.green = sum.blue = 0;

					for(ii = max(i-1, 0); ii <= iiLimit ; ii++) {
						int kRow, kCol;
						int t2= ii*m;

						// compute row index in kernel
						if (ii < i) {
							kRow = 0;
							} else if (ii > i) {
									kRow = 2;
							} else {
									kRow = 1;
							}

						for(jj = max(j-1, 0); jj <= jjLimit ; jj++) {

							// compute column index in kernel
							if (jj < j) {
								kCol = 0;
							} else if (jj > j) {
									kCol = 2;
							} else {
									kCol = 1;
							}

							int resultKernel=sharpKernel[kRow][kCol];
							int resultSrc= t2 + jj;

							// apply kernel on pixel at [ii,jj]
							sum.red += ((int) backupOrg2[resultSrc].red) * resultKernel;
							sum.green += ((int) backupOrg2[resultSrc].green) * resultKernel;
							sum.blue += ((int) backupOrg2[resultSrc].blue) * resultKernel;
						}
					}

				// truncate each pixel's color values to match the range [0,255]
				current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
				current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
				current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));

				pixelsImg2[t3+j] = current_pixel;
			}
		}

		//instead of pixelsToChars
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				image->data[t4] = pixelsImg2[t3].red;
				image->data[t4 + 1] = pixelsImg2[t3].green;
				image->data[t4 + 2] = pixelsImg2[t3].blue;
			}
		}

		free(pixelsImg2);
		free(backupOrg2);

			
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		//instead of doConvolution(image, blurKernel, 7, true);
		int t1= m*m;
		int z=t1+t1+t1;
		pixel* pixelsImg = malloc(z);
		pixel* backupOrg = malloc(z);

		//instead of charsToPixels
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				pixelsImg[t3].red = image->data[t4];
				pixelsImg[t3].green = image->data[t4 + 1];
				pixelsImg[t3].blue = image->data[t4 + 2];
			}
		}

		//instead of copyPixels 
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				backupOrg[t3].red = pixelsImg[t3].red;
				backupOrg[t3].green = pixelsImg[t3].green;
				backupOrg[t3].blue = pixelsImg[t3].blue;
			}
		}

	/* Apply the kernel over each pixel.
	* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
	* column index smaller than 1 */
		for (int i = 1 ; i < m - 1; i++) {
			int t3=i*m;
			for (int j =  1 ; j < m - 1 ; j++) {

				// Applies kernel for pixel at (i,j)
				//	instead of kernelApply function

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

					for(ii = max(i-1, 0); ii <= min(i+1, m-1); ii++) {
						int kRow, kCol;
						int t2= ii*m;

						// compute row index in kernel
						if (ii < i) {
							kRow = 0;
							} else if (ii > i) {
									kRow = 2;
							} else {
									kRow = 1;
							}

						for(jj = max(j-1, 0); jj <= min(j+1, m-1); jj++) {

							// compute column index in kernel
							if (jj < j) {
								kCol = 0;
							} else if (jj > j) {
									kCol = 2;
							} else {
									kCol = 1;
							}

							int resultSrc= t2 + jj;

							// apply kernel on pixel at [ii,jj]
							sum.red += ((int) backupOrg[resultSrc].red);
							sum.green += ((int) backupOrg[resultSrc].green);
							sum.blue += ((int) backupOrg[resultSrc].blue);
						}
					}

					// find min and max coordinates
					for(ii = max(i-1, 0); ii <= min(i+1, m-1); ii++) {
						int t2=ii*m;
						for(jj = max(j-1, 0); jj <= min(j+1, m-1); jj++) {
							// check if smaller than min or higher than max and update
							loop_pixel = backupOrg[t2+jj];
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
					int result3=min_row*m+min_col;
					int result4=max_row*m+max_col;

					sum.red -= ((int) backupOrg[result3].red);
					sum.green -= ((int) backupOrg[result3].green);
					sum.blue -= ((int) backupOrg[result3].blue);
					sum.red -= ((int) backupOrg[result4].red);
					sum.green -= ((int) backupOrg[result4].green);
					sum.blue -= ((int) backupOrg[result4].blue);

				// assign kernel's result to pixel at [i,j]
				// divide by kernel's weight
				sum.red = sum.red / 7;
				sum.green = sum.green / 7;
				sum.blue = sum.blue / 7;

				// truncate each pixel's color values to match the range [0,255]
				current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
				current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
				current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));

				pixelsImg[t3+j] = current_pixel;
			}
		}

		//instead of pixelsToChars
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				image->data[t4] = pixelsImg[t3].red;
				image->data[t4 + 1] = pixelsImg[t3].green;
				image->data[t4 + 2] = pixelsImg[t3].blue;
			}
		}

		free(pixelsImg);
		free(backupOrg);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		//instead of doConvolution(image, sharpKernel, 1, false);
		pixel* pixelsImg2 = malloc(z);
		pixel* backupOrg2 = malloc(z);

		//instead of charsToPixels
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				pixelsImg2[t3].red = image->data[t4];
				pixelsImg2[t3].green = image->data[t4 + 1];
				pixelsImg2[t3].blue = image->data[t4 + 2];
			}
		}

		//instead of copyPixels 
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				backupOrg2[t3].red = pixelsImg2[t3].red;
				backupOrg2[t3].green = pixelsImg2[t3].green;
				backupOrg2[t3].blue = pixelsImg2[t3].blue;
			}
		}

	/* Apply the kernel over each pixel.
	* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
	* column index smaller than 1 */
		for (int i = 1 ; i < m - 1; i++) {
			int t3=i*m;
			for (int j =  1 ; j < m - 1 ; j++) {

				// Applies kernel for pixel at (i,j)
				//	instead of kernelApply function

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

					for(ii = max(i-1, 0); ii <= min(i+1, m-1); ii++) {
						int kRow, kCol;
						int t2= ii*m;

						// compute row index in kernel
						if (ii < i) {
							kRow = 0;
							} else if (ii > i) {
									kRow = 2;
							} else {
									kRow = 1;
							}

						for(jj = max(j-1, 0); jj <= min(j+1, m-1); jj++) {

							// compute column index in kernel
							if (jj < j) {
								kCol = 0;
							} else if (jj > j) {
									kCol = 2;
							} else {
									kCol = 1;
							}

							int resultKernel=sharpKernel[kRow][kCol];
							int resultSrc= t2 + jj;

							// apply kernel on pixel at [ii,jj]
							sum.red += ((int) backupOrg2[resultSrc].red) * resultKernel;
							sum.green += ((int) backupOrg2[resultSrc].green) * resultKernel;
							sum.blue += ((int) backupOrg2[resultSrc].blue) * resultKernel;
						}
					}

				// truncate each pixel's color values to match the range [0,255]
				current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
				current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
				current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));

				pixelsImg2[t3+j] = current_pixel;
			}
		}

		//instead of pixelsToChars
		for (int row = 0 ; row < m ; row++) {
			int t2=row*m;
			for (int col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				image->data[t4] = pixelsImg2[t3].red;
				image->data[t4 + 1] = pixelsImg2[t3].green;
				image->data[t4 + 2] = pixelsImg2[t3].blue;
			}
		}

		free(pixelsImg2);
		free(backupOrg2);
			
		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}