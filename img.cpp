/************************************
*	Ver 0.1 - 10-4-2017				*
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


/************************************
*	Includes/dependencies			*
************************************/
#include <stdlib.h>
#include <iostream>
#include "img.h"


/************************************
*	IMG image Class					*
************************************/
IMG::IMG()
{
	// set all values to a default
	m_img_pt = nullptr;
	m_size.X = 0;
	m_size.Y = 0;
	m_pixel_count = 0;
	m_is_binary_image = false;
}

IMG::IMG(const IMG &img)
{
	/*
	*	Make a deep copy of the data
	*/
		
	
	// copy the data
	m_size = img.m_size;
	m_pixel_count = img.m_pixel_count;
	m_is_binary_image = img.m_is_binary_image;

	// allocate memory
	m_img_pt = new PIXEL[m_pixel_count];
	//m_img_pt = new short[m_pixel_count];

	// copy the pixel data
	for (int i = 0; i < m_pixel_count; i++)
	{
		m_img_pt[i] = img.m_img_pt[i];
	}
	
}

IMG::IMG(int width, int heigth)
{
		
	// image is default non-binary
	m_is_binary_image = false;

	// set the width and heigth
	m_size.X = width;
	m_size.Y = heigth;

	// set the total amount of pixels
	m_pixel_count = width * heigth;

	// initialize pointer
	m_img_pt = new PIXEL[m_pixel_count];
	//m_img_pt = new short[m_pixel_count];
}

IMG::IMG(const char* filename)
{
	
	// set pointer and read BMP data
	BMP* img = new BMP;
	img->ReadFromFile(filename);

	// copy and convert image data from BMP
	gray_copy(img);

	// delete pointer to BMP
	delete img;
}

IMG& IMG::operator=(const IMG &img_source)
{
	/*
	*	Make a deep copy of the data
	*/

	// check for self-assignment
	if (this == &img_source) return *this;
	
	// check if the source contains pixel data, if not than do not copy
	if (img_source.m_img_pt == nullptr)
	{
		std::cout << "\nPixel data is NULL pointer, no data has been copied\n";
		return *this;
	}
	
	// make sure there is no data
	this->dispose();

	// copy the data
	m_size				= img_source.m_size;
	m_pixel_count		= img_source.m_pixel_count;
	m_is_binary_image	= img_source.m_is_binary_image;
	
	// allocate memory
	m_img_pt = new PIXEL[m_pixel_count];
	//m_img_pt = new short[m_pixel_count];

	// copy the pixel data
	for (int i = 0; i < m_pixel_count; i++)
	{
			m_img_pt[i] = img_source.m_img_pt[i];	
	}
	
	return *this;
}

const IMG operator+(IMG& img1, IMG& img2)
{

	// check for data

	// check if image sizes are equal

	// check if bin or 256

	IMG out;

	// initialize output image
	out.init(img1.m_size);

	for (int i = 0;i< out.m_pixel_count;i++)

	return out;
}

const IMG operator-(IMG& img1, IMG& img2)
{


	return img1;
}

void IMG::dispose(void)
{
	if (m_img_pt) delete[] m_img_pt;
	m_img_pt = nullptr;
}

void IMG::gray_copy(BMP *img)
{

	int		width,
			heigth;

	/*
	*	Clear any image data
	*/
	dispose();

	// get width and heigth BMP image
	width = img->TellWidth();
	heigth = img->TellHeight();

	// initialize binIMG
	init(width, heigth);

	// loop over all pixels
	for (int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// determine gray value using RGB->YUV
			m_img_pt[i * m_size.X + j] = (PIXEL)floor(0.299* img->GetPixel(j, i).Red +
				0.587*img->GetPixel(j, i).Green +
				0.114*img->GetPixel(j, i).Blue);

		}
	}
}

void IMG::binary_copy(BMP *img)
{

	int				width,
					heigth;

	/*
	*	Clear any image data
	*/
	dispose();

	// get width and heigth BMP image
	width = img->TellWidth();
	heigth = img->TellHeight();

	// initialize binIMG
	init(width, heigth);

	// flag image as binary
	m_is_binary_image = true;

	// loop over all pixels
	for (int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < width; j++)
		{
			
			// look at info in red channel, maybe improve by looking at other channels
			if (img->GetPixel(j, i).Red == 0)  // j and i is reversed
			{
				m_img_pt[i * width + j] = 0;
			}
			else
			{
				m_img_pt[i * width + j] = 1;
			}
		}

	}
}

void IMG::init(XY size)
{
	// make sure there is no data when the image struct is set
	if (m_img_pt != nullptr)
	{
		dispose();
	}

	// set the total amount of pixels
	m_pixel_count = size.X * size.Y;
	m_size = size;

	// allocate memory for image
	m_img_pt = new PIXEL[m_pixel_count];
	//m_img_pt = new short[m_pixel_count];
		
	// set all values default to zero
	clear_data();
}

void IMG::init(int width, int heigth)
{
	// make sure there is no data when the image struct is set
	if (m_img_pt != nullptr)
	{
		dispose();
	}

	// set the width and heigth
	m_size.X = width;
	m_size.Y = heigth;

	// set the total amount of pixels
	m_pixel_count = width * heigth;

	// allocate memory for image
	m_img_pt = new PIXEL[m_pixel_count];
	//m_img_pt = new short[m_pixel_count];

	// set all values default to zero
	clear_data();

}

bool IMG::set_pixel(int width, int heigth, PIXEL pix)
{
	
	// if there is no data, return false
	if (m_img_pt == nullptr) return false;

	// return pixel value
	m_img_pt[heigth * m_size.X + width] = pix;

	// if the pixel is set return true
	return true;
}

void IMG::clear_data(PIXEL val)
{

	// if there is no data, return without doing something
	if (m_img_pt == nullptr) return;

	// set all points to zeros
	for (int i = 0; i < m_size.Y; i++)
	{
		for (int j = 0; j < m_size.X; j++)
		{		
			m_img_pt[i * m_size.X + j] = val;
		}
	}
}

void IMG::set_values_to_false(void)
{
	/* *** NEEDS TO BE REVISED *** */

	/*
	*	Set all points to -1
	*/
	for (int i = 0; i < m_size.Y; i++)
	{
		for (int j = 0; j < m_size.X; j++)
		{
			m_img_pt[i * m_size.X + j] = NULL;
		}
	}
}

void IMG::set_is_binary(bool val)
{
	m_is_binary_image = val;
}

PIXEL IMG::get_pixel(int ind)	const
{

	// if there is no data, return without doing something
	if (m_img_pt == nullptr) return -1;

	// return pixel value
	return m_img_pt[ind];
}

PIXEL IMG::get_pixel(int width, int heigth)	const
{

	// if there is no data, return without doing something
	if (m_img_pt == nullptr) return -1;

	// return pixel value
	return m_img_pt[heigth * m_size.X + width];
}

short IMG::get_width(void) const
{

	return m_size.X;
}

short IMG::get_heigth(void) const
{

	return m_size.Y;
}

int IMG::get_pixelcount(void) const
{

	return m_pixel_count;
}

bool IMG::is_binary(void) const
{
	// returns if the data in the image is binary
	return m_is_binary_image;
}

void IMG::import_test_data(const char* filename)
{

	std::cout << "\n" << "Read test data from file : " << filename << "\n";

	FILE		*pFile;

	/*
	*	Open bin file
	*/
	pFile = fopen(filename, "rb");
	if (pFile == NULL) { fputs("File error", stderr); return; }

	/*
	*	Read meta data, heigth (y) and width (x)
	*/
	fread(&m_size.Y, sizeof(int), 1, pFile);
	fread(&m_size.X, sizeof(int), 1, pFile);

	/*
	*	Initialize data pointer
	*/
	init(m_size);

	/*
	*	Copy data to memory
	*/
	fread(m_img_pt, sizeof(PIXEL), m_pixel_count, pFile);
	//fread(m_img_pt, sizeof(short), m_pixel_count, pFile);

	/*
	*	Close bin file
	*/
	fclose(pFile);

	/*
	*	Show result
	*/
	std::cout << "Width  : " << m_size.X << "\n";
	std::cout << "Heigth : " << m_size.Y << "\n";

	show_img_onscreen();

	std::cout << "\n\nImported image OK\n";

}

void IMG::import_from_BMP(const char* filename)
{
	
	std::cout << "\n" << "Import image data from bitmap (bmp) file : " << filename << "\n";

	// set pointer and read BMP data
	BMP* img = new BMP;
	img->ReadFromFile(filename);

	// copy and convert image data from BMP
	gray_copy(img);

	// delete pointer to BMP
	delete img;

}

void IMG::export_img(const char* filename) const
{

	std::cout << "\n" << "write data to file : " << filename << "\n";

	FILE	*pFile;
	pFile = fopen(filename, "w");

	for (int i = 0; i < m_size.Y; i++) // heigth
	{
		for (int j = 0; j < m_size.X; j++) // width
		{
			if (j + 1 == m_size.X)
			{
				fprintf(pFile, "%d", (short)m_img_pt[i * m_size.X + j]);
			}
			else
			{
				fprintf(pFile, "%d, ", (short)m_img_pt[i * m_size.X + j]);
			}
		}

		// each new row a new line
		fprintf(pFile, "\n");
	}

	fclose(pFile);

}

void IMG::export_to_BMP(const char* filename) const
{

	BMP			*outputIMG;
	RGBApixel	setpixel;

	outputIMG = new BMP;

	/*
	*	Setup the BMP image
	*/
	outputIMG->SetSize(m_size.X, m_size.Y);

	/*
	*	filling loop over all pixels
	*/
	for (int i = 0; i < m_size.Y; i++)
	{
		for (int j = 0; j < m_size.X; j++)
		{
			// set the pixel in gray value
			if (!m_is_binary_image)
			{
				setpixel.Blue	= m_img_pt[i * m_size.X + j];
				setpixel.Red	= m_img_pt[i * m_size.X + j];
				setpixel.Green	= m_img_pt[i * m_size.X + j];
				// save the pixel
				outputIMG->SetPixel(j, i, setpixel);
			}
			// set the pixel in binary (1 -> 255, 0 -> 0)
			else
			{
				if (m_img_pt[i * m_size.X + j] > 0)
				{
					setpixel.Blue	= 255;
					setpixel.Red	= 255;
					setpixel.Green	= 255;
				}
				else
				{
					setpixel.Blue	= 0;
					setpixel.Red	= 0;
					setpixel.Green	= 0;
				}
				// save the pixel
				outputIMG->SetPixel(j, i, setpixel);
			}
		}
	}

	/*
	*	Write result to file
	*/
	outputIMG->WriteToFile(filename);

	// remove pointer to BMP export
	delete outputIMG;
}

void IMG::padded_image(IMG* img, short pad_siz, PIXEL pad_val, bool use_border) const
{

	if (pad_siz < 0)
	{
		std::cout << "\nInvalid pad size, padding cancelled\n";
		return;
	}
		
	/*
	*	Initialize padded bin image data struct,
	*	on each side an extra row with zeros the 
	*	size of pad_siz
	*/
	img->init(m_size.X + (pad_siz * 2), m_size.Y + (pad_siz * 2));

	// padding is a fixed value, default 0
	if (!use_border)
	{
	
		// set border value to the padding value
		img->clear_data(pad_val);

		// deep copy of the original image data to the new allocated memory
		for (int i = pad_siz; i <= m_size.Y + pad_siz - 1; i++)
		{
			for (int j = pad_siz; j <= m_size.X + pad_siz - 1; j++)
			{		
			
				img->m_img_pt[i * img->m_size.X + j] = m_img_pt[(i - pad_siz) * m_size.X + (j - pad_siz)];			
			}
		}
	}
	// padding is equal to the border of the image
	else
	{
		// loop over all pixels, padding included
		for (int i = 0; i < img->get_heigth(); i++)
		{
			for (int j = 0; j < img->get_width(); j++)
			{				
				// fill left upper corner
				if ( i <= pad_siz  && j <= pad_siz )
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[0];
				}
				// first row
				else if (i < pad_siz && j > pad_siz && j < m_size.X + pad_siz - 1)
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[j - pad_siz];
				}
				// fill right upper corner
				else if ( i <= pad_siz && (j >= m_size.X + pad_siz - 1) )
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[m_size.X - 1];
				}
				// first column
				else if (j < pad_siz && i > pad_siz && i < m_size.Y + pad_siz - 1)
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[(i - pad_siz) * m_size.X];
				}
				// second column
				else if (j > pad_siz + m_size.X - 1 && i > pad_siz && i < m_size.Y + pad_siz - 1)
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[(i - pad_siz) * m_size.X + m_size.X - 1];
				}
				// fill left bottom corner 
				else if ( ( i >= img->m_size.Y - pad_siz - 1 ) && j <= pad_siz)
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[m_pixel_count - m_size.X];
				}
				// fill rigth bottom corner
				else if ( ( i >= img->m_size.Y - pad_siz - 1 ) && (j >= m_size.X + pad_siz - 1))
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[m_pixel_count - 1];
				}
				// last row
				else if (i > pad_siz + m_size.Y - 1 && j > pad_siz && j < m_size.X + pad_siz - 1)
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[m_pixel_count - m_size.X  + j - pad_siz];
				}
				// copy image data to the padded image
				else
				{
					img->m_img_pt[i * img->m_size.X + j] = m_img_pt[(i - pad_siz) * m_size.X + (j - pad_siz)];
				}
			}
		}
	}

	// inherance the binary tag
	if (m_is_binary_image) img->m_is_binary_image = true;

}

void IMG::show_img_onscreen() const
{
	/*
	Show boolIMG data on screen
	*/
	#ifdef _MSC_VER
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	#endif
	
	std::cout << "\n";
	std::cout << "\nIMG data\n";
	for (int i = 0; i < m_size.Y; i++)
	{
		for (int j = 0; j < m_size.X; j++)
		{
			// set text color to red
			if (m_img_pt[i * m_size.X + j] > 0) 
			{

				#ifdef _MSC_VER
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				#endif

				if (m_img_pt[i * m_size.X + j] < 0)
				{
					std::cout << (short)m_img_pt[i * m_size.X + j] << "  ";
				}
				else if (m_img_pt[i * m_size.X + j] < 9)
				{
					std::cout << (short)m_img_pt[i * m_size.X + j] << "   ";
				}
				else if (m_img_pt[i * m_size.X + j] < 99)
				{
					std::cout << (short)m_img_pt[i * m_size.X + j] << "  ";
				}
				else
				{
					std::cout << (short)m_img_pt[i * m_size.X + j] << " ";
				}
			}
			// set text color to white
			else{
				
				#ifdef _MSC_VER
				SetConsoleTextAttribute(hConsole, 15);
				#endif

				std::cout << (short)m_img_pt[i * m_size.X + j] << "   ";
			}
		}
		std::cout << "\n";
	}

	// set text color back to white
	#ifdef _MSC_VER
	SetConsoleTextAttribute(hConsole, 15);
	#endif
}

bool IMG::gaussian_blur(short ker_siz, float sigma, DERIV order, IMG* img_out)
{
	/********************************************************************************/
	/*																				*/
	/*	Gaussian Blur separate filter 1D convolution Implementation					*/
	/*																				*/
	/*	Pixel values at the bounderies are repeated to deal with the edge effect	*/
	/*																				*/
	/********************************************************************************/


	float			sum(0)		;
	unsigned int	k_ind		;
	PIXEL			pixel_b		,
					pixel_e		;

	// initialize pointer and allocate memory of temp image 
	IMG*	temp = new IMG;
			
	// initialize pointer to kernel
	kernel_f* ker = nullptr;

	// get Gaussian kernel
	if (!gaussian_kernel1d(ker, ker_siz, sigma, order))
	{
		if (ker) delete[] ker;
		return false;
	}
		
	// initialize destination Image
	img_out->init(m_size);

	// initialize temp image
	temp->init(m_size);

	// Gaussian, horizontal loop
	for (int i = 0; i < m_size.Y; i++)	// column loop
	{
		for (int j = 0; j < m_size.X; j++)	// row loop
		{

			// set pixel for begin and end loop (deal with edge effect)
			pixel_b = m_img_pt[i *  m_size.X];
			pixel_e = m_img_pt[(i + 1) * m_size.X - 1];

			// reset sum of kernel
			sum = 0;

			// kernel loop
			for (int k = 0; k < ker_siz; k++)
			{
				// compute pixel index, including kernel
				k_ind = (i * m_size.X + j) - (ker_siz / 2) + k;

				// kernel exceeds min row index
				if (k_ind < i * m_size.X) sum += ker[k] * pixel_b;
				// kernel is between max and min row index
				else if (k_ind < m_size.X*(i + 1)) sum += ker[k] * m_img_pt[k_ind];
				// kernel exceeds max row index
				else sum += ker[k] * pixel_e;
			}
			// fill image data with the sum of the kernel pixels
			temp->m_img_pt[i * m_size.X + j] = (PIXEL)sum;
		}
	}

	// Gaussian vertical loop
	for (int i = 0; i < m_size.X; i++)	// row loop
	{
		for (int j = 0; j < m_size.Y; j++)	// column loop
		{
			img_out->m_img_pt[j * m_size.X + i] = m_img_pt[j * m_size.X + i];

			// set pixel for begin and end loop
			pixel_b = temp->m_img_pt[i];
			pixel_e = temp->m_img_pt[m_pixel_count - m_size.X + i];

			// reset sum of kernel
			sum = 0;

			// kernel loop
			for (int k = 0; k < ker_siz; k++)
			{
				// compute pixel index, including kernel
				k_ind = (j * m_size.X + i) - ((ker_siz / 2) - k) * m_size.X;

				// kernel exceeds min row index
				if (k_ind  < 0) sum += ker[k] * pixel_b;
				// kernel is between max and min row index
				else if ((m_pixel_count - m_size.X + i) >= k_ind) sum += ker[k] * temp->m_img_pt[k_ind];
				// kernel exceeds max row index
				else sum += ker[k] * pixel_e;

			}
			// fill image data with the sum of the kernel pixels
			img_out->m_img_pt[j * m_size.X + i] = (PIXEL)sum;
		}
	}

	// clear temp image memory
	temp->dispose();
	delete temp;

	// clear kernel
	delete[] ker;

	// return true if ok
	return true;

}

bool IMG::median_filter(short ker_siz, IMG* img_out)
{
	/*
	*	2D Median filter
	*/

	IMG*			padded		;
	kernel_s*		ker			;
	unsigned int	k_im_ind	;
	short			pad_siz		,
					ker_tot		, 
					k_ind		,
					x			,
					y			;

	// if there is no image data return false
	if (m_img_pt == nullptr || m_pixel_count <= 0)
	{
		return false;
	}

	// padding
	pad_siz = (ker_siz - 1) / 2;

	// initialize output image
	img_out->init(m_size);

	// kernel
	ker_tot = ker_siz*ker_siz;
	ker = new kernel_s[ker_tot];
	
	// padded img the size of the original
	padded = new IMG;
	padded_image(padded, pad_siz, 0, true);

	// loop with kernel over padded image
	for (int i = pad_siz; i < m_size.Y + pad_siz; i++)
	{
		for (int j = pad_siz; j < m_size.X + pad_siz; j++)
		{		
			// fill kernel
			for (short k_y = 0; k_y < ker_siz; k_y++)
			{
				for (short k_x = 0; k_x < ker_siz; k_x++)
				{				
					x = j - pad_siz + k_x;
					y = i - pad_siz + k_y;

					k_im_ind = y * padded->m_size.X + x;
					k_ind = k_y * ker_siz + k_x;
					
					ker[k_ind] = padded->m_img_pt[k_im_ind];
				}
			}

			// compute median and set filtered image data
			img_out->m_img_pt[(i - pad_siz)*m_size.X + (j - pad_siz)] = (PIXEL)median(ker, ker_tot);			
		}
	}
		
	// free allocated memory padded image
	padded->dispose();
	delete padded;

	// free allocated kernal memory
	delete[] ker;

	// if all went ok return true
	return true;
}

bool IMG::mean_filter(short ker_siz, IMG* img_out)
{
	/*
	*	2D Mean filter
	*/

	IMG*			padded		;
	kernel_s*		ker			;
	unsigned int	k_im_ind	;
	short			pad_siz		,
					ker_tot		,
					k_ind		,
					x			,
					y			;

	// if there is no image data return false
	if (m_img_pt == nullptr || m_pixel_count <= 0)
	{
		return false;
	}

	// padding
	pad_siz = (ker_siz - 1) / 2;

	// initialize output image
	img_out->init(m_size);

	// kernel
	ker_tot = ker_siz*ker_siz;
	ker = new kernel_s[ker_tot];

	// padded img the size of the original
	padded = new IMG;
	padded_image(padded, pad_siz, 0, true);

	// loop with kernel over padded image
	for (int i = pad_siz; i < m_size.Y + pad_siz; i++)
	{
		for (int j = pad_siz; j < m_size.X + pad_siz; j++)
		{

			// fill kernel
			for (short k_y = 0; k_y < ker_siz; k_y++)
			{
				for (short k_x = 0; k_x < ker_siz; k_x++)
				{
					x = j - pad_siz + k_x;
					y = i - pad_siz + k_y;
					
					k_im_ind = y * padded->m_size.X + x;
					k_ind = k_y * ker_siz + k_x;

					ker[k_ind] = padded->m_img_pt[k_im_ind];
				}
			}

			// compute mean and set image data
			img_out->m_img_pt[(i - pad_siz)*m_size.X + (j - pad_siz)] = mean(ker, ker_tot);
		}

	}
	
	// free allocated memory padded image
	padded->dispose();
	delete padded;

	// free allocated kernal memory
	delete[] ker;

	// if all went ok return true
	return true;
}

bool IMG::gaussian_kernel2d(kernel_f* ker, short size_ker, float sigma)
{
	/*
	*	2D Gaussian kernel
	*	
	*	kernel size: size_ker is a MxM
	*/
	
	// make sure the kernel is empty to start with
	if (ker)
	{
		delete[] ker;
	}
	
	// check if the size of the kernel is an odd number and return if it is not
	if (!(size_ker % 2)) return false;

	short half_ker_siz = (size_ker - 1) / 2;
	float sig_sq = sigma * sigma;
	float xy, x_ker, y_ker, sum(0);
	short total_ker = size_ker*size_ker;
	

	// allocate memory for kernel
	ker = new kernel_f[total_ker];

	// compute values
	for (int x = 0; x < size_ker; x++)
	{
		for (int y = 0; y < size_ker; y++)
		{
			x_ker = x - half_ker_siz;
			y_ker = y - half_ker_siz;

			xy = (x_ker * x_ker + y_ker * y_ker);

			ker[y*size_ker + x] = exp( -xy / (2 * sig_sq) ) / (2 * PI * sig_sq);

			// get sum of all values for normalization
			sum += ker[y*size_ker + x];
		}
	}

	// normalize values
	for (int x = 0; x < size_ker; x++)
	{
		for (int y = 0; y < size_ker; y++)
		{
			ker[y*size_ker + x] /= sum;
		}
	}

	// if no error, return ok.
	return true;
}

bool IMG::gaussian_kernel1d(kernel_f*& ker, short size_ker, float sigma, DERIV order)
{
	/*
	*	1D Gaussian kernel, zeroth, first or second derivative
	*
	*	kernel size: size_ker is the length of the array
	*	
	*/
	
	// make sure the kernel is empty to start with
	if (ker)
	{
		delete[] ker;
	}

	// initialize pointer to kernel and allocate memory 
	ker = new kernel_f[size_ker];

	// check if the size of the kernel is an odd number and return if it is not
	if (!(size_ker % 2)) return false;

	short half_ker_siz = (size_ker - 1) / 2;
	float sig_sq = sigma * sigma;
	float x_sq, x_ker, sum(0);

	// compute values
	for (int x = 0; x < size_ker; x++)
	{
		x_ker = x - half_ker_siz;
	
		x_sq = x_ker * x_ker;

		ker[x] = exp(-x_sq / (2 * sig_sq)) / (sqrt(2 * PI) / sigma);

		// get sum of all values for normalization
		sum += ker[x];
	}

	// normalize values
	for (int x = 0; x < size_ker; x++)
	{
		ker[x] /= sum;
	}
	
	kernel_f* dst_ker;

	// compute derivative
	switch (order)
	{
	case NONE:
		// just return the calculated kernel
		return true;
	
	case FIRST:
		// compute first derivative of current kernel
		kernel_deriv1d(ker, dst_ker, size_ker, FIRST);
		// free initial kernel memory
		delete[] ker;
		// appoint kernel pointer to new kernel data
		ker = dst_ker;
		return true;

	case SECOND:
		// compute second derivative of current kernel
		kernel_deriv1d(ker, dst_ker, size_ker, SECOND);
		// free initial kernel memory
		delete[] ker;
		// appoint kernel pointer to new kernel data
		ker = dst_ker;
		return true;
	}

	return false;
}

bool IMG::kernel_deriv1d(const kernel_f* src_ker, kernel_f*& dst_ker, short siz_ker, DERIV order)
{
	
	/* 
	*	Calculate first or second order derivative of (gaussian) kernel,
	*	derivative calculated using central finite difference method
	*/
	
	// return false if the source kernel is empty
	if (src_ker == nullptr) return false;

	// allocate memory for new kernel
	kernel_f* temp = new kernel_f[siz_ker];

	// 
	switch (order)
	{
	case NONE:
		// shouldn't be called at this level
		return false;
	case FIRST:
				
		for (short k = 0; k < siz_ker; k++)
		{
			if (k == 0)					temp[k] = src_ker[k + 1] - src_ker[k];
			else if (k < siz_ker - 1)	temp[k] = (src_ker[k + 1] - src_ker[k - 1]) / 2;
			else						temp[k] = src_ker[k] - src_ker[k - 1];
		}
		
		break;
	case SECOND:

		for (short k = 0; k < siz_ker; k++)
		{
			if (k == 0)					temp[k] = src_ker[k + 1] - (2 * src_ker[k]);
			else if (k < siz_ker - 1)	temp[k] = src_ker[k - 1] - (2 * src_ker[k]) + src_ker[k + 1];
			else						temp[k] = (2 * src_ker[k]) - src_ker[k - 1];
		}
		
		break;
	}
		
	// point destination pointer to newly allocated kernel data
	dst_ker = temp;
	
	// return true if all went ok
	return true;
}

void IMG::threshold(short threshold, THRES mode)
{

}

void IMG::test_things()
{
	
	PIXEL test[5];
	
	std::cout << "\n\n Unsigned Char test\n\n";

	for (int i = 0; i < 5; i++)
	{
		test[i] = 2 * i;

		std::cout << (short)test[i] << " ";
	}
	
	std::cout << "\n\n";
	
}

/* 
*	Class independent functions
*/

static short mean(short* arr, short arr_siz)
{
	/*
	*	Computes the mean of an array
	*/

	float sum(0);

	for (short i = 0; i < arr_siz; i++)
	{
		sum += arr[i];
	}

	return (sum / arr_siz) + 0.49;
}

static short median(short* arr, short arr_siz)
{
	/*
	*	Calculates the median of an array using the Quicksort algorithm.
	*	In case of an even array the mean of the two medians is calculated.
	*	The input array is order from min to max when the median is calulated
	*/

	short	half_arr_siz	;
	float	median(0)		;

	// sort values in the array
	quicksort(arr, 0, arr_siz - 1);

	// check if array is odd
	if (arr_siz % 2)
	{
		half_arr_siz = 1 + (arr_siz - 1) / 2;
		median = arr[half_arr_siz - 1];
	}
	// else the array is even
	else
	{
		half_arr_siz = arr_siz / 2;
		median = (arr[half_arr_siz - 1] + arr[half_arr_siz]) / 2;
	}

	// return median
	return median + 0.49;
}

static void quicksort(short* arr, short p, short r)
{
	/*
	*	Quicksort algorithm, breaks the array around a pivot element and sorts
	*	the input array.
	*	
	*	arr		- pointer to the input array
	*	p		- start index of the array
	*	r		- last index of the array
	*/

	if (p < r)
	{
		// partition the array
		short q = partition(arr, p, r);
		// recurrence
		quicksort(arr, p, q - 1);
		quicksort(arr, q + 1, r);
	}
}

static short partition(short* arr, short p, short r)
{
	short x = arr[r];
	short i(p - 1);

	for (short j = p; j <= r - 1; j++)
	{
		if (arr[j] <= x)
		{
			i += 1;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[r]);

	return i + 1;
}

static void swap(short& a, short& b)
{
	// simply swap values a and b
	short temp = b;
	b = a;
	a = temp;
}


// END OF FILE
