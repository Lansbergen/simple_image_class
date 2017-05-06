#pragma once
/************************************************************************
*																		*
*								IMG.h									* 
*		- latest version: 0.1											*
*																		*
*																		*
*																		*
*	a 2D image class with *.BMP support.								*
*	for BMP support the class is depended								*		
*	the EasyBMP class, info at:											*
*	(http://easybmp.sourceforge.net/)									*
*																		*
*	This class includes both image 										*
*	processing operations as well as basic								*
*	image editing functionality 										*
*																		*
*																		*
*	ver. 0.1 - SL - 10-4-2017 - added the first functions				*
*																		*
*																		*
*																		*
*	to do (for 1.0 release):											*
*	- size padded_image changable										*
*	- (done) add gaussian blur filter									*
*	- (done) add mean filter											*
*	- (done) add median filter											*
*	- (done) converted pixel type from short to							*
*			 PIXEL (unsigned char)										*
*	- (done) cross platform compatible	(linux)							*
*	- thresholding														*
*	- +operator															*
*	- -operator															*
*	- sharpening filter													*
*		(http://homepages.inf.ed.ac.uk/rbf/HIPR2/unsharp.htm)			*
*	*****************************************************************	*
*	- add export/import from *.ppm (for 1.1 release)					*
*	- AVI writer (add own class with interface for IMG class)			*
*																		*
*																		*
*	bug fixing															*
*	- set_values_to_false needs to be revised.							*
*																		*
************************************************************************/

/************************************
*	Includes/dependencies			*
************************************/
#include <math.h>
#include "EasyBMP.h"
#include "img_filters.h"


/************************************
*	Enums							*
************************************/
enum DERIV
{
	NONE,
	FIRST,
	SECOND
};

enum THRES
{
	DIVIDE,
	TRUNC_DOWN,
	TRUNC_UP
};

/************************************
*	Typedefs						*
************************************/
typedef unsigned char PIXEL;
typedef float kernel_f;
typedef short kernel_s;
typedef PIXEL kernel_p;

/************************************
*	Data structs					*
************************************/
typedef struct XY {
	int X;
	int Y;
}XY;

typedef struct XYF {
	float X;
	float Y;
}XYF;

/************************************
*	Globals							*
************************************/
#define PI 3.14159265358979323846

static short mean(short* arr, short arr_siz);
static short median(short* arr, short arr_siz);
static void swap(short& a, short& b);
static void quicksort(short* arr, short p, short r);
static short partition(short* arr, short p, short r);


/************************************
*	Classes							*
************************************/
class IMG
{
	// declare friend class(es)
	friend class IMG_FILTERS;

public:

	/*
	*	Constructor(s) and destructor
	*/
	IMG();	// default constructor
	IMG(const IMG &img);	// copy constructor	
	IMG(int width, int heigth);	
	IMG(const char* filename);
	~IMG() { dispose(); }

	/*
	*	Operators
	*/
	IMG& operator=(const IMG &img_source);		// default copy function
	friend const IMG operator+(IMG& img1, IMG& img2);
	friend const IMG operator-(IMG& img1, IMG& img2);

	/*
	*	Functions to initialize and set the data
	*/
	void	init(XY size);
	void	init(int width, int heigth)						;
	

	// dispose data
	void	dispose(void)									;

	// set data functions
	void	gray_copy(BMP *img)								;
	void	binary_copy(BMP *img)							;
	

	// set functions
	void	set_values_to_false(void)						;
	void	clear_data(PIXEL val = 0)						;
	bool	set_pixel(int width, int heigth, PIXEL pix)		;
	void	set_is_binary(bool val)							;

	/*
	*	Get Functions
	*/
	PIXEL	get_pixel(int ind)						const	;
	PIXEL	get_pixel(int width, int heigth)		const	;
	short	get_width(void)							const	;
	short	get_heigth(void)						const	;
	int		get_pixelcount(void)					const	;
	bool	is_binary(void)							const	;

	/*
	*	Import/export data functions
	*/
	void	import_test_data(const char* filename)			;
	void	import_from_BMP(const char* filename)			;
	void	export_img(const char* filename)		const	;
	void	export_to_BMP(const char* filename)		const	;

	/*
	*	Various image data functions
	*/
	void	show_img_onscreen()						const	;
	void	padded_image(IMG *img, short pad_siz, PIXEL pad_val = 0, bool use_border = false)	const;
	void	threshold(short threshold,THRES mode)			;

	/*
	*	Filter functions
	*/
	bool	gaussian_blur(short ker_siz, float sigma, DERIV order, IMG* img_out);
	
	bool	median_filter(short ker_siz, IMG* img_out);
	bool	mean_filter(short ker_siz, IMG* img_out);

	void	test_things();
	
private:
	bool	gaussian_kernel2d(kernel_f* ker, short size_ker, float sigma);
	bool	gaussian_kernel1d(kernel_f*& ker, short size_ker, float sigma, DERIV order);
	bool	kernel_deriv1d(const kernel_f* src_ker, kernel_f*& dst_ker, short siz_ker, DERIV order);

	

private:

	/*
	*	Image variables
	*/
	XY		m_size;
	int		m_pixel_count;
	bool	m_is_binary_image;
	//short	*m_img_pt;		// pointer to data
	PIXEL	*m_img_pt;		// pointer to data
		

};	/* End binIMG class	*/


// END OF FILE
