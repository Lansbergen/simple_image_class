#pragma once
/************************************************************************
*																		*
*							IMG_filters.h								*
*		- latest version: 0.1											*
*																		*
*																		*
*																		*
*																		*
*																		*
*																		*
*																		*
*																		*
*	ver. 0.1 - SL - 2-5-2017 - added the first functions				*
*																		*
*																		*
*																		*
*	to do (for 1.0 release):											*
*																		*
*																		*
*	bug fixing															*
*																		*
*																		*
************************************************************************/

/************************************
*	Includes/dependencies			*
************************************/
#include "img.h"

/************************************
*	Enums							*
************************************/


/************************************
*	Data structs					*
************************************/


/************************************
*	Globals							*
************************************/
#define PI 3.14159265358979323846

// forward declaration
class IMG;
/************************************
*	Classes							*
************************************/
class IMG_FILTERS
{
public:

	IMG_FILTERS() { m_img = nullptr; };
	~IMG_FILTERS();
	IMG_FILTERS(IMG* img);


	// ALTERNATIVE GAUSSIAN BLUR - A
	bool	gaussian_box_blur_A(IMG* img_in, IMG* img_out, float sigma);
	// ALTERNATIVE GAUSSIAN BLUR - B
	bool	gaussian_box_blur_B(IMG* img_in, IMG* img_out, float sigma);

private:
	
	short*	box_size(float sigma, short n);

	// ALTERNATIVE GAUSSIAN BLUR - A
	void	Agaussian_box_H(IMG* src, IMG* dst, short box_size);
	void	Agaussian_box_T(IMG* src, IMG* dst, short box_size);
	void	Agaussian_box(IMG* src, IMG* dst, short box_size);
	// ALTERNATIVE GAUSSIAN BLUR - B
	void	Bgaussian_box_H(IMG* src, IMG* dst, short box_size);
	void	Bgaussian_box_T(IMG* src, IMG* dst, short box_size);
	void	Bgaussian_box(IMG* src, IMG* dst, short box_size);


private:
	// member variables
	IMG* m_img;
};