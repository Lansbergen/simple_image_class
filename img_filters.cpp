/************************************
*	Ver 0.1 - 2-5-2017				*
************************************/

#ifdef __GNUC__
/*code for GNU C compiler */
#elif _MSC_VER
// MS Visual Studio gives warnings when using 
// fopen. But fopen_s is not going to work well 
// with most compilers, and fopen_s uses different 
// syntax than fopen. (i.e., a macro won't work) 
// So, we'll use this:
#define _CRT_SECURE_NO_DEPRECATE

// windows specific include headers
#include <Windows.h>

#elif __MINGW32__
/*code specific to mingw compilers*/
#endif

/************************************/
/*		Includes/dependencies		*/
/************************************/
#include "img_filters.h"


/************************************/
/*		IMG_FILTER Class			*/
/************************************/

//IMG_FILTERS::IMG_FILTERS(){}

IMG_FILTERS::IMG_FILTERS(IMG* img)
{
	m_img = nullptr;

	m_img = new IMG(*img);
}

IMG_FILTERS::~IMG_FILTERS() 
{ 
	if( m_img )  
	{ 
		// free allocated memory (pixel data)
		m_img->dispose();
		delete m_img;
	}
}


/************************************/
/*		GAUSSIAN BLUR FILTERS		*/
/************************************/

/*
*	Gaussian Box Blur
*/

// standard deviation, number of boxes
short* IMG_FILTERS::box_size(float sigma, short n)
{
	// http://www.peterkovesi.com/matlabfns/Spatial/solveinteg.m
	// http://blog.ivank.net/fastest-gaussian-blur.html

	if (sigma < 0.8)
	{
		std::cout << "Sigma values below about 0.8 cannot be represented";
		return false;
	}

	float wIdeal = sqrt((12 * sigma*sigma / n) + 1);  // Ideal averaging filter width 
	int wl = floor(wIdeal);  if (wl % 2 == 0) wl--;
	int wu = wl + 2;

	float mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
	int m = round(mIdeal);
	//int sigmaActual = sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

	short* sizes = new short[n];

	for (int i = 0; i < n; i++)
	{
		// Ternary operator ? :
		sizes[i] = (i < m ? wl : wu);
	}

	return sizes;

}

// ALTERNATIVE GAUSSIAN BOX BLUR - A
bool IMG_FILTERS::gaussian_box_blur_A(IMG* img_in, IMG* img_out, float sigma)
{

	// initialize destination Image
	img_out->init(img_in->m_size);

	// set initial varaibles  *** remove when done ***

	int itterations = 3;

	// get sizes of box for the blur
	short* sizes = box_size(sigma, itterations);

	// DEBUG *** breakpoint stop
	int stop(0);

	IMG cpy(*img_in);



	Agaussian_box(&cpy, img_out, sizes[0]);
	Agaussian_box(img_out, &cpy, sizes[1]);
	Agaussian_box(&cpy, img_out, sizes[2]);


	// free allocated memory
	if (sizes) delete[] sizes;


	return true;

}

void IMG_FILTERS::Agaussian_box_H(IMG* src, IMG* dst, short box_size)
{

	short val;

	short width = src->get_width();
	short heigth = src->get_heigth();
	short r = (box_size - 1) / 2;

	// Horizontal loop
	for (short i = 0; i < heigth; i++) //Y
	{
		for (short j = 0; j < width; j++) // X
		{

			// clear kernel total for each pixel
			val = 0;

			// kernel loop
			for (short ix = j - r; ix < j + r + 1; ix++)
			{
				// deals with the edge effect
				short x = min((width - 1), max(0, ix));

				// fill kernel ( cumulative )
				val += src->m_img_pt[i*heigth + x];
			}

			// take average kernel value and fill image
			dst->m_img_pt[i*heigth + j] = val / box_size;
		}
	}
		
}

void IMG_FILTERS::Agaussian_box_T(IMG* src, IMG* dst, short box_size)
{

	short val;

	short width = src->get_width();
	short heigth = src->get_heigth();
	short r = (box_size - 1) / 2;

	// Total (vertical) loop
	for (short i = 0; i < heigth; i++) //Y
	{
		for (short j = 0; j < width; j++) // X
		{
			// clear kernel total for each pixel
			val = 0;

			// kernel loop
			for (short iy = i - r; iy<i + r + 1; iy++)
			{
				// deals with the edge effect
				short y = min(heigth - 1, max(0, iy));

				// fill kernel ( cumulative )
				val += src->m_img_pt[y*width + j];
			}
			// take average kernel value and fill image
			dst->m_img_pt[i*width + j] = val / box_size;
		}
	}

}

void IMG_FILTERS::Agaussian_box(IMG* src, IMG* dst, short box_size)
{
	for (int n = 0; n < src->m_pixel_count; n++)
	{
		dst->m_img_pt[n] = src->m_img_pt[n];
	}
		
	Agaussian_box_H(dst, src, box_size);
	Agaussian_box_T(src, dst, box_size);

}

// ALTERNATIVE GAUSSIAN BOX BLUR - B
bool IMG_FILTERS::gaussian_box_blur_B(IMG* img_in, IMG* img_out, float sigma)
{

	// initialize destination Image
	img_out->init(img_in->m_size);

	// set initial varaibles  *** remove when done ***

	int itterations = 3;

	// get sizes of box for the blur
	short* sizes = box_size(sigma, itterations);

	// DEBUG *** breakpoint stop
	int stop(0);

	IMG cpy(*img_in);



	Bgaussian_box(&cpy, img_out, sizes[0]);
	Bgaussian_box(img_out, &cpy, sizes[1]);
	Bgaussian_box(&cpy, img_out, sizes[2]);


	// free allocated memory
	if (sizes) delete[] sizes;


	return true;

}

void IMG_FILTERS::Bgaussian_box(IMG* src, IMG* dst, short box_size)
{
	for (int n = 0; n < src->m_pixel_count; n++)
	{
		dst->m_img_pt[n] = src->m_img_pt[n];
	}

	Bgaussian_box_H(dst, src, box_size);
	Bgaussian_box_T(src, dst, box_size);

}

void IMG_FILTERS::Bgaussian_box_H(IMG* src, IMG* dst, short box_size)
{

	short val;

	short width = src->m_size.X;
	short heigth = src->m_size.Y;
	short r = (box_size - 1) / 2;

	// weighting factor to compute box average
	float iarr = 1.0 / (r + r + 1.0);

	// loop over all horizontal lines
	for (short i = 0; i < heigth; i++) // Y
	{
		// compute indici 
		int ti = i*width, li = ti, ri = ti + r;

		int fv = src->m_img_pt[ti], lv = src->m_img_pt[ti + width - 1], val = (r + 1)*fv;

		// loop over (boxsize - 1) / 2 -> one leg of the array from 0th index
		for (short j = 0; j < r; j++)
		{
			val += src->m_img_pt[ti + j];
		}

		// loop over (boxsize - 1) / 2 -> one leg of the array and the middel pixel	from 0th index
		for (short j = 0; j <= r; j++)
		{
			val += src->m_img_pt[ri++] - fv;
			dst->m_img_pt[ti++] = round(val*iarr);
		}

		// loop over (boxsize - 1) / 2 -> one leg of the array and the middel pixel from middel value index
		for (short j = r + 1; j < width - r; j++)
		{
			val += src->m_img_pt[ri++] - src->m_img_pt[li++];
			dst->m_img_pt[ti++] = round(val*iarr);
		}

		// loop over (boxsize - 1) / 2 -> one leg of the array and the middel pixel from middel value index
		for (short j = width - r; j<width; j++)
		{
			val += lv - src->m_img_pt[li++];
			dst->m_img_pt[ti++] = round(val*iarr);
		}
	}



}

void IMG_FILTERS::Bgaussian_box_T(IMG* src, IMG* dst, short box_size)
{

	short val;

	short width = src->m_size.X;
	short heigth = src->m_size.Y;
	short r = (box_size - 1) / 2;

	// weighting factor to compute box average
	float iarr = 1.0 / (r + r + 1.0);

	// loop over all horizontal lines
	for (short i = 0; i < width; i++) // Y
	{
		// compute indici 
		int ti = i, li = ti, ri = ti + r*width;

		int fv = src->m_img_pt[ti], lv = src->m_img_pt[ti + width*(heigth - 1)], val = (r + 1)*fv;

		// 1
		for (short j = 0; j < r; j++)
		{
			val += src->m_img_pt[ti + j*width];
		}

		// 2				
		for (short j = 0; j <= r; j++)
		{
			val += src->m_img_pt[ri] - fv;
			dst->m_img_pt[ti] = round(val*iarr);
			ri += width;
			ti += width;
		}

		// 3
		for (short j = r + 1; j < heigth - r; j++)
		{
			val += src->m_img_pt[ri] - src->m_img_pt[li];
			dst->m_img_pt[ti] = round(val*iarr);
			li += width;
			ri += width;
			ti += width;
		}

		// 4
		for (short j = heigth - r; j < heigth; j++)
		{
			val += lv - src->m_img_pt[li];
			dst->m_img_pt[ti] = round(val*iarr);
			li += width;
			ti += width;
		}
	}


}
