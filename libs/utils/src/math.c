#include <math.h>
#include <stdint.h>
#include <float.h>
typedef double double_t;
typedef float float_t;

/*****************************************************************************/
/*****************************************************************************/
// fpclassify from musl
/*****************************************************************************/
/*****************************************************************************/

int __fpclassify(double x)
{
	union {double f; uint64_t i;} u = {x};
	int e = u.i>>52 & 0x7ff;
	if (!e) return u.i<<1 ? FP_SUBNORMAL : FP_ZERO;
	if (e==0x7ff) return u.i<<12 ? FP_NAN : FP_INFINITE;
	return FP_NORMAL;
}

int __fpclassifyf(float x)
{
	union {float f; uint32_t i;} u = {x};
	int e = u.i>>23 & 0xff;
	if (!e) return u.i<<1 ? FP_SUBNORMAL : FP_ZERO;
	if (e==0xff) return u.i<<9 ? FP_NAN : FP_INFINITE;
	return FP_NORMAL;
}


int __signbit(double x)
{
	union {
		double d;
		uint64_t i;
	} y = { x };
	return y.i>>63;
}

int __signbitf(float x)
{
	union {
		float f;
		uint32_t i;
	} y = { x };
	return y.i>>31;
}


double nan(const char *s)
{
	return NAN;
}

float nanf(const char *s)
{
	return NAN;
}


// #if FLT_EVAL_METHOD==0
#define EPS FLT_EPSILON
// #elif FLT_EVAL_METHOD==1
// #define EPS DBL_EPSILON
// #elif FLT_EVAL_METHOD==2
// #define EPS LDBL_EPSILON
// #endif

static const float_t toint = 1/EPS;

float rintf(float x)
{
	union {float f; uint32_t i;} u = {x};
	int e = u.i>>23 & 0xff;
	int s = u.i>>31;
	float_t y;

	if (e >= 0x7f+23)
		return x;
	if (s)
		y = x - toint + toint;
	else
		y = x + toint - toint;
	if (y == 0)
		return s ? -0.0f : 0.0f;
	return y;
}


double nearbyint(double x)
{
#ifdef FE_INEXACT
	#pragma STDC FENV_ACCESS ON
	int e;

	e = fetestexcept(FE_INEXACT);
#endif
	x = rint(x);
#ifdef FE_INEXACT
	if (!e)
		feclearexcept(FE_INEXACT);
#endif
	return x;
}


float nearbyintf(float x)
{
#ifdef FE_INEXACT
	#pragma STDC FENV_ACCESS ON
	int e;

	e = fetestexcept(FE_INEXACT);
#endif
	x = rintf(x);
#ifdef FE_INEXACT
	if (!e)
		feclearexcept(FE_INEXACT);
#endif
	return x;
}

double trunc(double x)
{
	union {double f; uint64_t i;} u = {x};
	int e = (int)(u.i >> 52 & 0x7ff) - 0x3ff + 12;
	uint64_t m;

	if (e >= 52 + 12)
		return x;
	if (e < 12)
		e = 1;
	m = -1ULL >> e;
	if ((u.i & m) == 0)
		return x;
	FORCE_EVAL(x + 0x1p120f);
	u.i &= ~m;
	return u.f;
}


float truncf(float x)
{
	union {float f; uint32_t i;} u = {x};
	int e = (int)(u.i >> 23 & 0xff) - 0x7f + 9;
	uint32_t m;

	if (e >= 23 + 9)
		return x;
	if (e < 9)
		e = 1;
	m = -1U >> e;
	if ((u.i & m) == 0)
		return x;
	FORCE_EVAL(x + 0x1p120f);
	u.i &= ~m;
	return u.f;
}

#include "sdk_init.h"
inline double acos(double x){return CSDK_FUNC(acos)(x);}
inline double asin(double x){return CSDK_FUNC(asin)(x);}
inline double atan(double x){return CSDK_FUNC(atan)(x);}
inline double atan2(double x, double y){return CSDK_FUNC(atan2)(x,y);}
inline double cos(double x){return CSDK_FUNC(cos)(x);}
inline double sin(double x){return CSDK_FUNC(sin)(x);}
inline double tan(double x){return CSDK_FUNC(tan)(x);}
inline double cosh(double x){return CSDK_FUNC(cosh)(x);}
inline double sinh(double x){return CSDK_FUNC(sinh)(x);}
inline double tanh(double x){return CSDK_FUNC(tanh)(x);}
inline double exp(double x){return CSDK_FUNC(exp)(x);}
inline double frexp(double x, int* y){return CSDK_FUNC(frexp)(x,y);}
inline double ldexp(double x, int y){return CSDK_FUNC(ldexp)(x,y);}
inline double log(double x){return CSDK_FUNC(log)(x);}
inline double log10(double x){return CSDK_FUNC(log10)(x);}
inline double modf(double x, double* y){return CSDK_FUNC(modf)(x,y);}
inline double pow(double x, double y){return CSDK_FUNC(pow)(x,y);}
inline double sqrt(double x){return CSDK_FUNC(sqrt)(x);}
inline double ceil(double x){return CSDK_FUNC(ceil)(x);}
inline double fabs(double x){return CSDK_FUNC(fabs)(x);}
double floor(double x){return CSDK_FUNC(floor)(x);}
inline double fmod(double x, double y){return CSDK_FUNC(fmod)(x,y);}
inline double erf (double x){return CSDK_FUNC(erf)(x);}
inline double erfc(double x){return CSDK_FUNC(erfc)(x);}
inline double gamma(double x){return CSDK_FUNC(gamma)(x);}
inline double tgamma(double x){return CSDK_FUNC(gamma)(x);}
inline double hypot(double x, double y){return CSDK_FUNC(hypot)(x,y);}
inline int finite(double x){return CSDK_FUNC(finite)(x);}
inline double lgamma(double x){return CSDK_FUNC(lgamma)(x);}
inline double acosh(double x){return CSDK_FUNC(acosh)(x);}
inline double asinh(double x){return CSDK_FUNC(asinh)(x);}
inline double atanh(double x){return CSDK_FUNC(atanh)(x);}
inline double cbrt(double x){return CSDK_FUNC(cbrt)(x);}
inline double logb(double x){return CSDK_FUNC(logb)(x);}
inline double nextafter(double x,double y){return CSDK_FUNC(nextafter)(x,y);}
inline double remainder(double x, double y){return CSDK_FUNC(remainder)(x,y);}
inline double significand(double x){return CSDK_FUNC(significand)(x);}
inline double copysign(double x, double y){return CSDK_FUNC(copysign)(x,y);}
inline int ilogb(double x){return CSDK_FUNC(ilogb)(x);}
inline double rint(double x){return CSDK_FUNC(rint)(x);}
inline double scalbn(double x, int y){return CSDK_FUNC(scalbn)(x,y);}
inline double expm1(double x){return CSDK_FUNC(expm1)(x);}
inline double log1p(double x){return CSDK_FUNC(log1p)(x);}



