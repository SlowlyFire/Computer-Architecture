#include <stdbool.h> 
#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b) 

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

	if (flag == '1') {	
		// blur image
		//instead of doConvolution(image, blurKernel, 9, false)
			int t1= m*m;
			int z=t1+t1+t1;
			pixel* pixelsImg = malloc(z);
			pixel* backupOrg = malloc(z);
			register int t2=0;
            register int t3;
            register int t4;
            int row, col;
			//instead of charsToPixels and copyPixels
			for (row = 0 ; row < m ; row++) {
				for (col = 0 ; col < m ; col++) {
					t3=t2+col;
					t4=t3+t3+t3;
					pixelsImg[t3].red = image->data[t4];
					pixelsImg[t3].green = image->data[t4 + 1];
					pixelsImg[t3].blue = image->data[t4 + 2];
					backupOrg[t3].red = pixelsImg[t3].red;
					backupOrg[t3].green = pixelsImg[t3].green;
					backupOrg[t3].blue = pixelsImg[t3].blue;
				}
                //int t2=row*m;
				t2+=m;
			}

		/* Apply the kernel over each pixel.
		* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
		* column index smaller than 1 */
            t2=0;
			pixel_sum sum;
			pixel current_pixel;
            int i,j;
			for (i = 1 ; i < m - 1; i++) {
				//int t2=i*m;
				t2+=m;

				for (j =  1 ; j < m - 1 ; j++) {
					// Applies kernel for pixel at (i,j)
					//Initializes all fields of sum to 0
					sum.red = sum.green = sum.blue = 0;
					// apply kernel on pixel at [ii,jj]
					sum.red += ((int) backupOrg[t2+j-m-1].red)+((int) backupOrg[t2+j-m].red)+((int) backupOrg[t2+j+1-m].red)+((int) backupOrg[t2+j-1].red)+((int) backupOrg[t2+j].red)+((int) backupOrg[t2+j+1].red)+((int) backupOrg[t2+m+j-1].red)+((int) backupOrg[t2+m+j].red)+((int) backupOrg[t2+m+j+1].red);
					sum.green += ((int) backupOrg[t2+j-m-1].green)+((int) backupOrg[t2+j-m].green)+((int) backupOrg[t2+j+1-m].green)+((int) backupOrg[t2+j-1].green)+((int) backupOrg[t2+j].green)+((int) backupOrg[t2+j+1].green)+((int) backupOrg[t2+m+j-1].green)+((int) backupOrg[t2+m+j].green)+((int) backupOrg[t2+m+j+1].green);
					sum.blue += ((int) backupOrg[t2+j-m-1].blue)+((int) backupOrg[t2+j-m].blue)+((int) backupOrg[t2+j+1-m].blue)+((int) backupOrg[t2+j-1].blue)+((int) backupOrg[t2+j].blue)+((int) backupOrg[t2+j+1].blue)+((int) backupOrg[t2+m+j-1].blue)+((int) backupOrg[t2+m+j].blue)+((int) backupOrg[t2+m+j+1].blue);
					sum.red = sum.red / 9;
					sum.green = sum.green / 9;
					sum.blue = sum.blue / 9;

					// truncate each pixel's color values to match the range [0,255]
					current_pixel.red = (unsigned char) min(sum.red, 255);
					current_pixel.green = (unsigned char) min(sum.green, 255);
					current_pixel.blue = (unsigned char) min(sum.blue, 255);
					pixelsImg[t2+j] = current_pixel;
				}
			}

			//instead of pixelsToChars
            t3=0;
			for (row = 0 ; row < m ; row++) {
				for (col = 0 ; col < m ; col++) {
					t2=t3+col;
					t4=t2+t2+t2;
					image->data[t4] = pixelsImg[t2].red;
					image->data[t4 + 1] = pixelsImg[t2].green;
					image->data[t4 + 2] = pixelsImg[t2].blue;
				}
                //t3=row*m;
                t3+=m;
			}

			free(pixelsImg);
			free(backupOrg);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		//instead of doConvolution(image, sharpKernel, 1, false);
		pixel* pixelsImg2 = malloc(z);
		pixel* backupOrg2 = malloc(z);

		//instead of charsToPixels and copyPixels
        t2=0;
		for (row = 0 ; row < m ; row++) {
			for (col = 0 ; col < m ; col++) {
				t3=t2+col;
				t4=t3+t3+t3;
				pixelsImg2[t3].red = image->data[t4];
				pixelsImg2[t3].green = image->data[t4 + 1];
				pixelsImg2[t3].blue = image->data[t4 + 2];
				backupOrg2[t3].red = pixelsImg2[t3].red;
				backupOrg2[t3].green = pixelsImg2[t3].green;
				backupOrg2[t3].blue = pixelsImg2[t3].blue;
			}
            //t2=row*m;
            t2+=m;
		}

		/* Apply the kernel over each pixel.
		* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
		* column index smaller than 1 */
        t2=0;
		for (i = 1 ; i < m - 1; i++) {
			//int t2=i*m;
            t2+=m;
			for (j =  1 ; j < m - 1 ; j++) {
				// Applies kernel for pixel at (i,j)
				//	instead of kernelApply function
				//Initializes all fields of sum to 0
				sum.red = sum.green = sum.blue = 0;

				// apply kernel on pixel at [ii,jj]
				sum.red -= ((int) backupOrg2[t2+j-m-1].red)+((int) backupOrg2[t2+j-m].red)+((int) backupOrg2[t2+j+1-m].red)+((int) backupOrg2[t2+j-1].red)-((int) backupOrg2[t2+j].red)*9+((int) backupOrg2[t2+j+1].red)+((int) backupOrg2[t2+m+j-1].red)+((int) backupOrg2[t2+m+j].red)+((int) backupOrg2[t2+m+j+1].red);
				sum.green -= ((int) backupOrg2[t2+j-m-1].green)+((int) backupOrg2[t2+j-m].green)+((int) backupOrg2[t2+j+1-m].green)+((int) backupOrg2[t2+j-1].green)-((int) backupOrg2[t2+j].green)*9+((int) backupOrg2[t2+j+1].green)+((int) backupOrg2[t2+m+j-1].green)+((int) backupOrg2[t2+m+j].green)+((int) backupOrg2[t2+m+j+1].green);
				sum.blue -= ((int) backupOrg2[t2+j-m-1].blue)+((int) backupOrg2[t2+j-m].blue)+((int) backupOrg2[t2+j+1-m].blue)+((int) backupOrg2[t2+j-1].blue)-((int) backupOrg2[t2+j].blue)*9+((int) backupOrg2[t2+j+1].blue)+((int) backupOrg2[t2+m+j-1].blue)+((int) backupOrg2[t2+m+j].blue)+((int) backupOrg2[t2+m+j+1].blue);
						
				// truncate each pixel's color values to match the range [0,255]
				current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
				current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
				current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));
				pixelsImg2[t2+j] = current_pixel;
			}
		}

		//instead of pixelsToChars
        t2=0;
		for (row = 0 ; row < m ; row++) {
			for (col = 0 ; col < m ; col++) {
				t3=t2+col;
				t4=t3+t3+t3;
				image->data[t4] = pixelsImg2[t3].red;
				image->data[t4 + 1] = pixelsImg2[t3].green;
				image->data[t4 + 2] = pixelsImg2[t3].blue;
			}
            //t2=row*m;
            t2+=m;
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
		register int row;
		register int col;
		register int t2=0;
        register int t3;
        register int t4;

		//instead of charsToPixels and copyPixels
		for (row = 0 ; row < m ; row++) {
			for (col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				pixelsImg[t3].red = image->data[t4];
				pixelsImg[t3].green = image->data[t4 + 1];
				pixelsImg[t3].blue = image->data[t4 + 2];
				backupOrg[t3].red = pixelsImg[t3].red;
				backupOrg[t3].green = pixelsImg[t3].green;
				backupOrg[t3].blue = pixelsImg[t3].blue;
			}
            //t2=row*m;
            t2+=m;
		}

	/* Apply the kernel over each pixel.
	* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
	* column index smaller than 1 */
		int i, j;
        pixel_sum sum;
		pixel current_pixel;
		t3=0;
		for (i = 1 ; i < m - 1; i++) {
			//t3=i*m;
			t3+=m;

			for (j =  1 ; j < m - 1 ; j++) {
					// Applies kernel for pixel at (i,j)
					//	instead of kernelApply function	
					int currRow, currCol;
					int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
					int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
					int min_row, min_col, max_row, max_col;
					pixel loop_pixel;
					//Initializes all fields of sum to 0
					sum.red = sum.green = sum.blue = 0;
                    // apply kernel on pixel at [ii,jj]
					t2=t3-m+j-1;
                	sum.red += ((int) backupOrg[t2].red)+((int) backupOrg[t2+1].red)+((int) backupOrg[t2+2].red)+((int) backupOrg[t2+m].red)+((int) backupOrg[t2+m+1].red)+((int) backupOrg[t2+m+2].red)+((int) backupOrg[t2+m+m].red)+((int) backupOrg[t2+m+m+1].red)+((int) backupOrg[t2+m+m+2].red);
					sum.green += ((int) backupOrg[t2].green)+((int) backupOrg[t2+1].green)+((int) backupOrg[t2+2].green)+((int) backupOrg[t2+m].green)+((int) backupOrg[t2+m+1].green)+((int) backupOrg[t2+m+2].green)+((int) backupOrg[t2+m+m].green)+((int) backupOrg[t2+m+m+1].green)+((int) backupOrg[t2+m+m+2].green);
					sum.blue += ((int) backupOrg[t2].blue)+((int) backupOrg[t2+1].blue)+((int) backupOrg[t2+2].blue)+((int) backupOrg[t2+m].blue)+((int) backupOrg[t2+m+1].blue)+((int) backupOrg[t2+m+2].blue)+((int) backupOrg[t2+m+m].blue)+((int) backupOrg[t2+m+m+1].blue)+((int) backupOrg[t2+m+m+2].blue);

					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i-1;
								min_col = j-1;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i-1;
								max_col = j-1;
						}
					
					t2++;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i-1;
								min_col = j;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i-1;
								max_col = j;
						}
					
					t2++;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i-1;
								min_col = j+1;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i-1;
								max_col = j+1;
						}
			
					t2=t2+m-2;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i;
								min_col = j-1;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i;
								max_col = j-1;
						}

					t2++;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i;
								min_col = j;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i;
								max_col = j;
						}
					

					t2++;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i;
								min_col = j+1;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i;
								max_col = j+1;
						}
					
					t2=t2+m-2;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i+1;
								min_col = j-1;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i+1;
								max_col = j-1;
						}

					t2++;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i+1;
								min_col = j;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i+1;
								max_col = j;
						}
					
					t2++;
					loop_pixel = backupOrg[t2];
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
								min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								min_row = i+1;
								min_col = j+1;
						}
						if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
								max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
								max_row = i+1;
								max_col = j+1;
						}

					// filter out min and max
					//Sums pixel values, scaled by given weight
					int result3=min_row*m+min_col;
					int result4=max_row*m+max_col;

					sum.red -= ((int) backupOrg[result3].red+(int) backupOrg[result4].red);
					sum.green -= ((int) backupOrg[result3].green+(int) backupOrg[result4].green);
					sum.blue -= ((int) backupOrg[result3].blue+(int) backupOrg[result4].blue);

				// assign kernel's result to pixel at [i,j]
				// divide by kernel's weight
				sum.red = sum.red / 7;
				sum.green = sum.green / 7;
				sum.blue = sum.blue / 7;

				// truncate each pixel's color values to match the range [0,255]
				current_pixel.red = (unsigned char) min(sum.red, 255);
				current_pixel.green = (unsigned char) min(sum.green, 255);
				current_pixel.blue = (unsigned char) min(sum.blue, 255);
				pixelsImg[t3+j] = current_pixel;
			}
		}

		//instead of pixelsToChars
        t2=0;
		for (row = 0 ; row < m ; row++) {
			for (col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				image->data[t4] = pixelsImg[t3].red;
				image->data[t4 + 1] = pixelsImg[t3].green;
				image->data[t4 + 2] = pixelsImg[t3].blue;
			}
            //t2=row*m;
            t2+=m;
		}

		free(pixelsImg);
		free(backupOrg);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		//instead of doConvolution(image, sharpKernel, 1, false);
		pixel* pixelsImg2 = malloc(z);
		pixel* backupOrg2 = malloc(z);

		//instead of charsToPixels and copyPixels
        t2=0;
		for (row = 0 ; row < m ; row++) {
			for (col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				pixelsImg2[t3].red = image->data[t4];
				pixelsImg2[t3].green = image->data[t4 + 1];
				pixelsImg2[t3].blue = image->data[t4 + 2];
				backupOrg2[t3].red = pixelsImg2[t3].red;
				backupOrg2[t3].green = pixelsImg2[t3].green;
				backupOrg2[t3].blue = pixelsImg2[t3].blue;
			}
            //t2=row*m;
            t2+=m;
		}

	/* Apply the kernel over each pixel.
	* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 1 and/or
	* column index smaller than 1 */
        t3=0;
		for (i = 1 ; i < m - 1; i++) {
			//int t3=i*m;
            t3+=m;
			for (j =  1 ; j < m - 1 ; j++) {
				// Applies kernel for pixel at (i,j)
				//	instead of kernelApply function
				//Initializes all fields of sum to 0
				sum.red = sum.green = sum.blue = 0;

				// apply kernel on pixel at [ii,jj]
				t2=t3-m+j-1;
                sum.red -= ((int) backupOrg2[t2].red)+((int) backupOrg2[t2+1].red)+((int) backupOrg2[t2+2].red)+((int) backupOrg2[t2+m].red)-((int) backupOrg2[t2+m+1].red)*9+((int) backupOrg2[t2+m+2].red)+((int) backupOrg2[t2+m+m].red)+((int) backupOrg2[t2+m+m+1].red)+((int) backupOrg2[t2+m+m+2].red);
				sum.green -= ((int) backupOrg2[t2].green)+((int) backupOrg2[t2+1].green)+((int) backupOrg2[t2+2].green)+((int) backupOrg2[t2+m].green)-((int) backupOrg2[t2+m+1].green)*9+((int) backupOrg2[t2+m+2].green)+((int) backupOrg2[t2+m+m].green)+((int) backupOrg2[t2+m+m+1].green)+((int) backupOrg2[t2+m+m+2].green);
				sum.blue -= ((int) backupOrg2[t2].blue)+((int) backupOrg2[t2+1].blue)+((int) backupOrg2[t2+2].blue)+((int) backupOrg2[t2+m].blue)-((int) backupOrg2[t2+m+1].blue)*9+((int) backupOrg2[t2+m+2].blue)+((int) backupOrg2[t2+m+m].blue)+((int) backupOrg2[t2+m+m+1].blue)+((int) backupOrg2[t2+m+m+2].blue);
				// truncate each pixel's color values to match the range [0,255]
				current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
				current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
				current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));
				pixelsImg2[t3+j] = current_pixel;
			}
		}

		//instead of pixelsToChars
        t2=0;
		for (row = 0 ; row < m ; row++) {
			for (col = 0 ; col < m ; col++) {
				int t3=t2+col;
				int t4=t3+t3+t3;
				image->data[t4] = pixelsImg2[t3].red;
				image->data[t4 + 1] = pixelsImg2[t3].green;
				image->data[t4 + 2] = pixelsImg2[t3].blue;
			}
            //t2=row*m;
            t2+=m;
		}

		free(pixelsImg2);
		free(backupOrg2);
			
		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}