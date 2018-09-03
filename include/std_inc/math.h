/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/





#ifndef __MATH_H__
#define __MATH_H__

/*
 * On AIX we need _ALL_SOURCE defined to compile/configure native-lib, but can't
 *  have it defined to compile fdlibm.  UGH.
 */
#ifdef _AIX
#undef _ALL_SOURCE
#endif

//#include <config.h>
#include <stdlib.h>
#include <sdk_init.h>

/*
 * AIX includes a header that defines hz,
 * which conflicts with an fdlibm variable in some functions.
 */
#ifdef _AIX
#undef hz
#endif

/* GCJ LOCAL: Include files.  */
// #include "ieeefp.h"
#define __IEEE_LITTLE_ENDIAN
/* CLASSPATH LOCAL: */
//#include "namespace.h"

//#include "mprec.h"

/* CYGNUS LOCAL: Default to XOPEN_MODE.  */
#define _XOPEN_MODE

#ifdef __P
#undef __P
#endif

/* Sometimes it's necessary to define __LITTLE_ENDIAN explicitly
   but these catch some common cases. */

#if 0
#if defined(i386) || defined(i486) || \
    defined(intel) || defined(x86) || defined(i86pc) || \
    defined(__alpha) || defined(__osf__)
#define __LITTLE_ENDIAN
#endif
#endif
#define __LITTLE_ENDIAN
#ifdef __LITTLE_ENDIAN
#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x
#define __HIp(x) *(1+(int*)x)
#define __LOp(x) *(int*)x
#else
#define __HI(x) *(int*)&x
#define __LO(x) *(1+(int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1+(int*)x)
#endif
// #endif

#ifdef __STDC__
#define __P(p)  p
#else
#define __P(p)  ()
#endif

/*
 * ANSI/POSIX
 */

extern int signgam;

#define MAXFLOAT    ((float)3.40282346638528860e+38)

enum fdversion {fdlibm_ieee = -1, fdlibm_svid, fdlibm_xopen, fdlibm_posix};

#define _LIB_VERSION_TYPE enum fdversion
#define _LIB_VERSION _fdlib_version

/* if global variable _LIB_VERSION is not desirable, one may
 * change the following to be a constant by:
 *  #define _LIB_VERSION_TYPE const enum version
 * In that case, after one initializes the value _LIB_VERSION (see
 * s_lib_version.c) during compile time, it cannot be modified
 * in the middle of a program
 */
extern  _LIB_VERSION_TYPE  _LIB_VERSION;

#define _IEEE_  fdlibm_ieee
#define _SVID_  fdlibm_svid
#define _XOPEN_ fdlibm_xopen
#define _POSIX_ fdlibm_posix

struct exception
{
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
};

enum
  {
    FP_NAN =
# define FP_NAN 0
      FP_NAN,
    FP_INFINITE =
# define FP_INFINITE 1
      FP_INFINITE,
    FP_ZERO =
# define FP_ZERO 2
      FP_ZERO,
    FP_SUBNORMAL =
# define FP_SUBNORMAL 3
      FP_SUBNORMAL,
    FP_NORMAL =
# define FP_NORMAL 4
      FP_NORMAL
  };

int __fpclassify(double);
int __fpclassifyf(float);

static __inline unsigned __FLOAT_BITS(float __f)
{
	union {float __f; unsigned __i;} __u;
	__u.__f = __f;
	return __u.__i;
}
static __inline unsigned long long __DOUBLE_BITS(double __f)
{
	union {double __f; unsigned long long __i;} __u;
	__u.__f = __f;
	return __u.__i;
}

#define fpclassify(x) ( \
	sizeof(x) == sizeof(float) ? __fpclassifyf(x) : \
	                             __fpclassify(x) )

#define isinf(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) == 0x7f800000 : \
	                             (__DOUBLE_BITS(x) & -1ULL>>1) == 0x7ffULL<<52 )
/*
#define isnan(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) > 0x7f800000 : \
	sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & -1ULL>>1) > 0x7ffULL<<52 : \
	__fpclassifyl(x) == FP_NAN)
*/

#if !defined(isnan)
#define isnan(x) ((x) != (x))
#endif

#define isnormal(x) ( \
	sizeof(x) == sizeof(float) ? ((__FLOAT_BITS(x)+0x00800000) & 0x7fffffff) >= 0x01000000 : \
	                             ((__DOUBLE_BITS(x)+(1ULL<<52)) & -1ULL>>1) >= 1ULL<<53 )

#define isfinite(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) < 0x7f800000 : \
	                             (__DOUBLE_BITS(x) & -1ULL>>1) < 0x7ffULL<<52 )

  
int __signbit(double);
int __signbitf(float);

#define signbit(x) ( \
	sizeof(x) == sizeof(float) ? (int)(__FLOAT_BITS(x)>>31) : \
	                             (int)(__DOUBLE_BITS(x)>>63) )

double      nan(const char *);
float       nanf(const char *);

double      nearbyint(double);
float       nearbyintf(float);

double      log2(double);
float       log2f(float);

double      trunc(double);
float       truncf(float);


#define HUGE        MAXFLOAT

/*
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */

#define X_TLOSS     1.41484755040568800000e+16

#define DOMAIN      1
#define SING        2
#define OVERFLOW    3
#define UNDERFLOW   4
#define TLOSS       5
#define PLOSS       6



#ifndef _HUGE_ENUF
    #define _HUGE_ENUF  1e+300  // _HUGE_ENUF*_HUGE_ENUF must overflow
#endif
#ifdef INFINITY
#error "INFINITY defined"
#endif 
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define HUGE_VAL   ((double)INFINITY)
#define HUGE_VALF  ((float)INFINITY)
#define HUGE_VALL  ((long double)INFINITY)
#define NAN        ((float)(INFINITY * 0.0F))   // (0.0f/0.0f)

/* */
/* Define some useful macros */
/* */

#undef  ABS
#define ABS(a)          (((a) < 0) ? (-(a)) : (a))

#undef  MAX
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

#undef  MIN
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))

#undef  SIGN
#define SIGN(a)         ((a) < 0 ? (-1) : (1))

#ifndef SHEEN_VC_DEBUG
#define abs ABS
#endif


/* These typedefs are true for the targets running Java. */

#define _IEEE_LIBM

#ifdef __cplusplus
extern "C" {
#endif

// /*
//  * ANSI/POSIX
//  */
extern double acos __P((double));
// inline double acos(double x){return CSDK_FUNC(acos)(x);}
extern double asin __P((double));
// // #define asin     CSDK_FUNC(asin)
// inline double asin(double x){return CSDK_FUNC(asin)(x);}
extern double atan __P((double));
// // #define atan     CSDK_FUNC(atan)
// inline double atan(double x){return CSDK_FUNC(atan)(x);}
extern double atan2 __P((double, double));
// // #define atan2     CSDK_FUNC(atan2)
// inline double atan2(double x, double y){return CSDK_FUNC(atan2)(x,y);}
extern double cos __P((double));
// // #define cos     CSDK_FUNC(cos)
// inline double cos(double x){return CSDK_FUNC(cos)(x);}
extern double sin __P((double));
// // #define sin     CSDK_FUNC(sin)
// inline double sin(double x){return CSDK_FUNC(sin)(x);}
extern double tan __P((double));
// // #define tan     CSDK_FUNC(tan)
// inline double tan(double x){return CSDK_FUNC(tan)(x);}

extern double cosh __P((double));
// // #define cosh     CSDK_FUNC(cosh)
// inline double cosh(double x){return CSDK_FUNC(cosh)(x);}
extern double sinh __P((double));
// // #define sinh     CSDK_FUNC(sinh)
// inline double sinh(double x){return CSDK_FUNC(sinh)(x);}
extern double tanh __P((double));
// // #define tanh     CSDK_FUNC(tanh)
// inline double tanh(double x){return CSDK_FUNC(tanh)(x);}

extern double exp __P((double));
// // #define exp     CSDK_FUNC(exp)
// inline double exp(double x){return CSDK_FUNC(exp)(x);}
extern double frexp __P((double, int *));
// // #define frexp     CSDK_FUNC(frexp)
// inline double frexp(double x, int* y){return CSDK_FUNC(frexp)(x,y);}
extern double ldexp __P((double, int));
// // #define ldexp     CSDK_FUNC(ldexp)
// inline double ldexp(double x, int y){return CSDK_FUNC(ldexp)(x,y);}
extern double log __P((double));
// // #define log     CSDK_FUNC(log)
// inline double log(double x){return CSDK_FUNC(log)(x);}
extern double log10 __P((double));
// // #define log10     CSDK_FUNC(log10)
// inline double log10(double x){return CSDK_FUNC(log10)(x);}
extern double modf __P((double, double *));
// // #define modf     CSDK_FUNC(modf)
// inline double modf(double x, double* y){return CSDK_FUNC(modf)(x,y);}

extern double pow __P((double, double));
// // #define pow     CSDK_FUNC(pow)
// static inline double pow(double x, double y){return CSDK_FUNC(pow)(x,y);}
extern double sqrt __P((double));
// // #define sqrt     CSDK_FUNC(sqrt)
// inline double sqrt(double x){return CSDK_FUNC(sqrt)(x);}

extern double ceil __P((double));
// // #define ceil     CSDK_FUNC(ceil)
// inline double ceil(double x){return CSDK_FUNC(ceil)(x);}
extern double fabs __P((double));
// // #define fabs     CSDK_FUNC(fabs)
// inline double fabs(double x){return CSDK_FUNC(fabs)(x);}
extern double floor __P((double));
// // #define floor     CSDK_FUNC(floor)
// inline double floor(double x){return CSDK_FUNC(floor)(x);}
extern double fmod __P((double, double));
// // #define fmod     CSDK_FUNC(fmod)
// inline double fmod(double x, double y){return CSDK_FUNC(fmod)(x,y);}

extern double erf __P((double));
// // #define erf     CSDK_FUNC(erf)
// inline double erf (double x){return CSDK_FUNC(erf)(x);}
extern double erfc __P((double));
// // #define erfc     CSDK_FUNC(erfc)
// inline double erfc(double x){return CSDK_FUNC(erfc)(x);}
extern double gamma __P((double));
// // #define gamma     CSDK_FUNC(gamma)
// inline double gamma(double x){return CSDK_FUNC(gamma)(x);}
// inline double tgamma(double x){return CSDK_FUNC(gamma)(x);}
extern double tgamma(double x);
extern double hypot __P((double, double));
// // #define hypot     CSDK_FUNC(hypot)
// inline double hypot(double x, double y){return CSDK_FUNC(hypot)(x,y);}


extern int finite __P((double));
// // #define finite     CSDK_FUNC(finite)
// inline int finite(double x){return CSDK_FUNC(finite)(x);}
extern double j0 __P((double));
extern double j1 __P((double));
extern double jn __P((int, double));
extern double lgamma __P((double));
// // #define lgamma     CSDK_FUNC(lgamma)
// inline double lgamma(double x){return CSDK_FUNC(lgamma)(x);}
// // extern double y0 __P((double));
// // extern double y1 __P((double));
// // extern double yn __P((int, double));

extern double acosh __P((double));
// // #define acosh     CSDK_FUNC(acosh)
// inline double acosh(double x){return CSDK_FUNC(acosh)(x);}
extern double asinh __P((double));
// // #define asinh     CSDK_FUNC(asinh)
// inline double asinh(double x){return CSDK_FUNC(asinh)(x);}
extern double atanh __P((double));
// // #define atanh     CSDK_FUNC(atanh)
// inline double atanh(double x){return CSDK_FUNC(atanh)(x);}
extern double cbrt __P((double));
// // #define cbrt     CSDK_FUNC(cbrt)
// inline double cbrt(double x){return CSDK_FUNC(cbrt)(x);}
extern double logb __P((double));
// // #define logb     CSDK_FUNC(logb)
// inline double logb(double x){return CSDK_FUNC(logb)(x);}
extern double nextafter __P((double, double));
// // #define nextafter     CSDK_FUNC(nextafter)
// inline double nextafter(double x,double y){return CSDK_FUNC(nextafter)(x,y);}
extern double remainder __P((double, double));
// // #define remainder     CSDK_FUNC(remainder)
// inline double remainder(double x, double y){return CSDK_FUNC(remainder)(x,y);}
// // #ifdef _SCALB_INT
// extern double scalb __P((double, int));
// // #else
extern double scalb __P((double, double));
// // #define scalb     CSDK_FUNC(scalb)
// // #endif

extern int matherr __P((struct exception *));

// /*
//  * IEEE Test Vector
//  */
extern double significand __P((double));
// // #define significand     CSDK_FUNC(significand)
// inline double significand(double x){return CSDK_FUNC(significand)(x);}

// /*
//  * Functions callable from C, intended to support IEEE arithmetic.
//  */
extern double copysign __P((double, double));
// // #define copysign     CSDK_FUNC(copysign)
// inline double copysign(double x, double y){return CSDK_FUNC(copysign)(x,y);}
extern int ilogb __P((double));
// // #define ilogb     CSDK_FUNC(ilogb)
// inline int ilogb(double x){return CSDK_FUNC(ilogb)(x);}
extern double rint __P((double));
// // #define rint     CSDK_FUNC(rint)
// inline double rint(double x){return CSDK_FUNC(rint)(x);}
extern double scalbn __P((double, int));
// // #define scalbn     CSDK_FUNC(scalbn)
// inline double scalbn(double x, int y){return CSDK_FUNC(scalbn)(x,y);}

// /*
//  * BSD math library entry points
//  */
extern double expm1 __P((double));
// // #define expm1     CSDK_FUNC(expm1)
// inline double expm1(double x){return CSDK_FUNC(expm1)(x);}
extern double log1p __P((double));
// // #define log1p     CSDK_FUNC(log1p)
// inline double log1p(double x){return CSDK_FUNC(log1p)(x);}

#if 0
/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
#ifdef _REENTRANT
extern double gamma_r __P((double, int *));
extern double lgamma_r __P((double, int *));
#endif  /* _REENTRANT */

/* ieee style elementary functions */
extern double __ieee754_sqrt __P((double));
extern double __ieee754_acos __P((double));
extern double __ieee754_acosh __P((double));
extern double __ieee754_log __P((double));
extern double __ieee754_atanh __P((double));
extern double __ieee754_asin __P((double));
extern double __ieee754_atan2 __P((double,double));
extern double __ieee754_exp __P((double));
extern double __ieee754_cosh __P((double));
extern double __ieee754_fmod __P((double,double));
extern double __ieee754_pow __P((double,double));
extern double __ieee754_lgamma_r __P((double,int *));
extern double __ieee754_gamma_r __P((double,int *));
extern double __ieee754_lgamma __P((double));
extern double __ieee754_gamma __P((double));
extern double __ieee754_log10 __P((double));
extern double __ieee754_sinh __P((double));
extern double __ieee754_hypot __P((double,double));
extern double __ieee754_j0 __P((double));
extern double __ieee754_j1 __P((double));
extern double __ieee754_y0 __P((double));
extern double __ieee754_y1 __P((double));
extern double __ieee754_jn __P((int,double));
extern double __ieee754_yn __P((int,double));
extern double __ieee754_remainder __P((double,double));
extern int32_t    __ieee754_rem_pio2 __P((double,double*));
#ifdef _SCALB_INT
extern double __ieee754_scalb __P((double,int));
#else
extern double __ieee754_scalb __P((double,double));
#endif

/* fdlibm kernel function */
extern double __kernel_standard __P((double,double,int));
extern double __kernel_sin __P((double,double,int));
extern double __kernel_cos __P((double,double));
extern double __kernel_tan __P((double,double,int));
extern int    __kernel_rem_pio2 __P((double*,double*,int,int,int,const int*));

/* Classpath extensions */

/* The original code used statements like
   n0 = ((*(int*)&one)>>29)^1;             * index of high word *
   ix0 = *(n0+(int*)&x);                   * high word of x *
   ix1 = *((1-n0)+(int*)&x);               * low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

#endif

#ifndef __IEEE_BIG_ENDIAN
#ifndef __IEEE_LITTLE_ENDIAN
#error Must define endianness
#endif
#endif

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

#ifdef __IEEE_BIG_ENDIAN

typedef union
{
    double value;
    struct
    {
        uint32_t msw;
        uint32_t lsw;
    } parts;
} ieee_double_shape_type;

#endif

#ifdef __IEEE_LITTLE_ENDIAN

typedef union
{
    double value;
    struct
    {
        uint32_t lsw;
        uint32_t msw;
    } parts;
} ieee_double_shape_type;

#endif

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)                                  \
  do {                                                            \
    ieee_double_shape_type ew_u;                                  \
    ew_u.value = (d);                                             \
    (ix0) = ew_u.parts.msw;                                       \
    (ix1) = ew_u.parts.lsw;                                       \
  } while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)                                      \
  do {                                                            \
    ieee_double_shape_type gh_u;                                  \
    gh_u.value = (d);                                             \
    (i) = gh_u.parts.msw;                                         \
  } while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)                                       \
  do {                                                            \
    ieee_double_shape_type gl_u;                                  \
    gl_u.value = (d);                                             \
    (i) = gl_u.parts.lsw;                                         \
  } while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)                                 \
  do {                                                            \
    ieee_double_shape_type iw_u;                                  \
    iw_u.parts.msw = (ix0);                                       \
    iw_u.parts.lsw = (ix1);                                       \
    (d) = iw_u.value;                                             \
  } while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)                                      \
  do {                                                            \
    ieee_double_shape_type sh_u;                                  \
    sh_u.value = (d);                                             \
    sh_u.parts.msw = (v);                                         \
    (d) = sh_u.value;                                             \
  } while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)                                       \
  do {                                                            \
    ieee_double_shape_type sl_u;                                  \
    sl_u.value = (d);                                             \
    sl_u.parts.lsw = (v);                                         \
    (d) = sl_u.value;                                             \
  } while (0)

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
    float value;
    uint32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)                 \
do {                                \
  ieee_float_shape_type gf_u;                   \
  gf_u.value = (d);                     \
  (i) = gf_u.word;                      \
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)                 \
do {                                \
  ieee_float_shape_type sf_u;                   \
  sf_u.word = (i);                      \
  (d) = sf_u.value;                     \
} while (0)

#define FLT_EVAL_METHOD 0

#define FORCE_EVAL(x) do {                        \
	if (sizeof(x) == sizeof(float)) {         \
		volatile float __x;               \
		__x = (x);                        \
                (void)__x;                        \
	} else if (sizeof(x) == sizeof(double)) { \
		volatile double __x;              \
		__x = (x);                        \
                (void)__x;                        \
	} else {                                  \
		volatile long double __x;         \
		__x = (x);                        \
                (void)__x;                        \
	}                                         \
} while(0)

#ifdef __cplusplus
}
#endif

#endif /* __CLASSPATH_FDLIBM_H__ */

