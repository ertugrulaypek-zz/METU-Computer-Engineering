/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "e2171270",              /* Student ID */

    "Ertugrul Aypek",     /* full name */
    "e2171270@ceng.metu.edu.tr",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * CONVOLUTION KERNEL
 ***************/

/******************************************************
 * Your different versions of the convolution functions  go here
 ******************************************************/

/* 
 * naive_conv - The naive baseline version of convolution 
 */
char naive_conv_descr[] = "naive_conv: Naive baseline implementation";
void naive_conv(int dim,int *src, int *ker,int *dst) {
    int i,j,k,l;

    for(i = 0; i < dim-8+1; i++)
        for(j = 0; j < dim-8+1; j++) {
            dst[j*dim+i] = 0;
            for(k = 0; k < 8; k++)
                for(l = 0; l < 8; l++) {
                    dst[j*dim+i] = dst[j*dim+i] +src[(j+l)*dim+(i+k)]*ker[l*dim+k];
                }
        }
            
        
}

/* 
 * convolution - Your current working version of convolution
 * IMPORTANT: This is the version you will be graded on
 */

char convolution_descr[] = "Dot product: Current working version";
void convolution(int dim,int *src, int *ker,int *dst) 
{
	int i, j, currentRow, srcIndex, temp;
	int cond=dim-7;
	
	int ker1=ker[0], ker2=ker[1], ker3=ker[2], ker4=ker[3], ker5=ker[4], ker6=ker[5], ker7=ker[6], ker8=ker[7];
	int ind = dim;
	int ker9=ker[ind++], ker10=ker[ind++], ker11=ker[ind++], ker12=ker[ind++], ker13=ker[ind++], ker14=ker[ind++], ker15=ker[ind++], ker16=ker[ind];
	ind = 2*dim;
	int ker17=ker[ind++], ker18=ker[ind++], ker19=ker[ind++], ker20=ker[ind++], ker21=ker[ind++], ker22=ker[ind++], ker23=ker[ind++], ker24=ker[ind];
	ind = 3*dim;
	int ker25=ker[ind++], ker26=ker[ind++], ker27=ker[ind++], ker28=ker[ind++], ker29=ker[ind++], ker30=ker[ind++], ker31=ker[ind++], ker32=ker[ind];
	ind = 4*dim;
	int ker33=ker[ind++], ker34=ker[ind++], ker35=ker[ind++], ker36=ker[ind++], ker37=ker[ind++], ker38=ker[ind++], ker39=ker[ind++], ker40=ker[ind];
	ind = 5*dim;
	int ker41=ker[ind++], ker42=ker[ind++], ker43=ker[ind++], ker44=ker[ind++], ker45=ker[ind++], ker46=ker[ind++], ker47=ker[ind++], ker48=ker[ind];
	ind = 6*dim;
	int ker49=ker[ind++], ker50=ker[ind++], ker51=ker[ind++], ker52=ker[ind++], ker53=ker[ind++], ker54=ker[ind++], ker55=ker[ind++], ker56=ker[ind];
	ind = 7*dim;
	int ker57=ker[ind++], ker58=ker[ind++], ker59=ker[ind++], ker60=ker[ind++], ker61=ker[ind++], ker62=ker[ind++], ker63=ker[ind++], ker64=ker[ind];
		
    for(i = 0; i < cond; i++){
		currentRow = dim * i; 
        for(j = 0; j < cond; j++) {
        	
			srcIndex = currentRow + j;
			
			temp = src[srcIndex++] * ker1;
			temp+= src[srcIndex++] * ker2;
			temp+= src[srcIndex++] * ker3;
			temp+= src[srcIndex++] * ker4;
			temp+= src[srcIndex++] * ker5;
			temp+= src[srcIndex++] * ker6;
			temp+= src[srcIndex++] * ker7;
			temp+= src[srcIndex]   * ker8;
			
			srcIndex += cond; // equal to srcIndex= currentRow + dim + j
			
			temp+= src[srcIndex++] * ker9;
			temp+= src[srcIndex++] * ker10;
			temp+= src[srcIndex++] * ker11;
			temp+= src[srcIndex++] * ker12;
			temp+= src[srcIndex++] * ker13;
			temp+= src[srcIndex++] * ker14;
			temp+= src[srcIndex++] * ker15;
			temp+= src[srcIndex]   * ker16;
			
			srcIndex += cond; // equal to srcIndex= currentRow + 2*dim + j
			
			temp+= src[srcIndex++] * ker17;
			temp+= src[srcIndex++] * ker18;
			temp+= src[srcIndex++] * ker19;
			temp+= src[srcIndex++] * ker20;
			temp+= src[srcIndex++] * ker21;
			temp+= src[srcIndex++] * ker22;
			temp+= src[srcIndex++] * ker23;
			temp+= src[srcIndex]   * ker24;
			
			srcIndex += cond;
			
			temp+= src[srcIndex++] * ker25 ;
			temp+= src[srcIndex++] * ker26 ;
			temp+= src[srcIndex++] * ker27 ;
			temp+= src[srcIndex++] * ker28 ;
			temp+= src[srcIndex++] * ker29 ;
			temp+= src[srcIndex++] * ker30 ;
			temp+= src[srcIndex++] * ker31 ;
			temp+= src[srcIndex]   * ker32 ;
			
			srcIndex += cond;
			
			temp+= src[srcIndex++] * ker33;
			temp+= src[srcIndex++] * ker34;
			temp+= src[srcIndex++] * ker35;
			temp+= src[srcIndex++] * ker36;
			temp+= src[srcIndex++] * ker37;
			temp+= src[srcIndex++] * ker38;
			temp+= src[srcIndex++] * ker39;
			temp+= src[srcIndex]   * ker40;
			
			srcIndex += cond;
			kerIndex += cond;
			temp+= src[srcIndex++] * ker41;
			temp+= src[srcIndex++] * ker42;
			temp+= src[srcIndex++] * ker43;
			temp+= src[srcIndex++] * ker44;
			temp+= src[srcIndex++] * ker45;
			temp+= src[srcIndex++] * ker46;
			temp+= src[srcIndex++] * ker47;
			temp+= src[srcIndex]   * ker48;
			
			srcIndex += cond;
			kerIndex += cond;
			temp+= src[srcIndex++] * ker49;
			temp+= src[srcIndex++] * ker50;
			temp+= src[srcIndex++] * ker51;
			temp+= src[srcIndex++] * ker52;
			temp+= src[srcIndex++] * ker53;
			temp+= src[srcIndex++] * ker54;
			temp+= src[srcIndex++] * ker55;
			temp+= src[srcIndex]   * ker56;
			
			srcIndex += cond;
			kerIndex += cond;
			temp+= src[srcIndex++] * ker57;
			temp+= src[srcIndex++] * ker58;
			temp+= src[srcIndex++] * ker59;
			temp+= src[srcIndex++] * ker60;
			temp+= src[srcIndex++] * ker61;
			temp+= src[srcIndex++] * ker62;
			temp+= src[srcIndex++] * ker63;
			temp+= src[srcIndex]   * ker64;
			
			dst[currentRow + j] = temp;
			
        }
	}
    

}

/*********************************************************************
 * register_conv_functions - Register all of your different versions
 *     of the convolution functions  with the driver by calling the
 *     add_conv_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_conv_functions() {
    add_conv_function(&naive_conv, naive_conv_descr);   
    add_conv_function(&convolution, convolution_descr); 
	  
    /* ... Register additional test functions here */
}




/***************
 * MATRIX MULTIP KERNEL
 ***************/

/******************************************************
 * Your different versions of the matrix multiplications  go here
 ******************************************************/

/* 
 * naive_matrix_multiplication - The naive baseline version of matrix multiplication 
 */
char naive_matrix_multiplication_descr[] = "Naive_matrix_multiplication: Naive baseline implementation";
void naive_matrix_multiplication(int dim,int *src, int *src2,int *dst) {
    int i,j,k;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {
            dst[j*dim+i]=0;
            for(k = 0; k < dim; k++) 
                dst[j*dim+i] = dst[j*dim+i] + src[j*dim+k]*src2[i+k*dim];
        }    
}


/* 
 * matrix_multiplication - Your current working version of matrix_multiplication
 * IMPORTANT: This is the version you will be graded on
 */
char matrix_multiplication_descr[] = "Matrix multiplications: Current working version";
void matrix_multiplication(int dim,int *src, int *src2,int *dst) 
{
	int temp, keep, keep2, tempResult, currentRow, iters, srcIndex;
    
	//in place transpose of second matrix to make element accesses faster while multiplication:
	for(int i = 0; i < dim; i++){
    	keep = i*dim;
		for(int j = i+1; j < dim; j++){
			keep2=j*dim;
        	temp = src2[keep+j];
        	src2[keep+j] = src2[keep2+i];
        	src2[keep2+i] = temp;
      	}
  	}
  	
  	for(int i = 0; i < dim; i++){
  		currentRow = i*dim;
		for(int j = 0; j < dim; j++){
  			tempResult = 0;
  			iters = dim/32;
  			for(int k = 0; k<iters; k++){
  				srcIndex = currentRow + k*32;
  				tempResult += src[srcIndex] * src2[srcIndex]
  				tempResult += src[srcIndex+1]  * src2[srcIndex+1];
  				tempResult += src[srcIndex+2]  * src2[srcIndex+2];
  				tempResult += src[srcIndex+3]  * src2[srcIndex+3];
  				tempResult += src[srcIndex+4]  * src2[srcIndex+4];
  				tempResult += src[srcIndex+5]  * src2[srcIndex+5];
  				tempResult += src[srcIndex+6]  * src2[srcIndex+6]; 
  				tempResult += src[srcIndex+7]  * src2[srcIndex+7]; 
  				tempResult += src[srcIndex+8]  * src2[srcIndex+8]; 
  				tempResult += src[srcIndex+9]  * src2[srcIndex+9]; 
  				tempResult += src[srcIndex+10] * src2[srcIndex+10];
  				tempResult += src[srcIndex+11] * src2[srcIndex+11];
  				tempResult += src[srcIndex+12] * src2[srcIndex+12];
  				tempResult += src[srcIndex+13] * src2[srcIndex+13];
  				tempResult += src[srcIndex+14] * src2[srcIndex+14];
  				tempResult += src[srcIndex+15] * src2[srcIndex+15];
  				tempResult += src[srcIndex+16] * src2[srcIndex+16];
  				tempResult += src[srcIndex+17] * src2[srcIndex+17];
  				tempResult += src[srcIndex+18] * src2[srcIndex+18];
  				tempResult += src[srcIndex+19] * src2[srcIndex+19];
  				tempResult += src[srcIndex+20] * src2[srcIndex+20];
  				tempResult += src[srcIndex+21] * src2[srcIndex+21];
  				tempResult += src[srcIndex+22] * src2[srcIndex+22];
  				tempResult += src[srcIndex+23] * src2[srcIndex+23];
  				tempResult += src[srcIndex+24] * src2[srcIndex+24];
  				tempResult += src[srcIndex+25] * src2[srcIndex+25];
  				tempResult += src[srcIndex+26] * src2[srcIndex+26];
  				tempResult += src[srcIndex+27] * src2[srcIndex+27];
  				tempResult += src[srcIndex+28] * src2[srcIndex+28];
  				tempResult += src[srcIndex+29] * src2[srcIndex+29];
  				tempResult += src[srcIndex+30] * src2[srcIndex+30];
  				tempResult += src[srcIndex+31] * src2[srcIndex+31];  				
			}
			dst[currentRow + j] = tempResult;
		}
	}
  	
  

}

/*********************************************************************
 * register_matrix_multiplication_functions - Register all of your different versions
 *     of the matrix multiplication  with the driver by calling the
 *     add_matrix_multiplication_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_matrix_multiplication_functions() {
    add_matrix_multiplication_function(&naive_matrix_multiplication, naive_matrix_multiplication_descr);   
    add_matrix_multiplication_function(&matrix_multiplication, matrix_multiplication_descr);   
    /* ... Register additional test functions here */
}

