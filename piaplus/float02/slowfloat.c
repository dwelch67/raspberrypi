

/*
===============================================================================

This C source file is part of TestFloat, Release 2a, a package of programs
for testing the correctness of floating-point arithmetic complying to the
IEC/IEEE Standard for Floating-Point.

Written by John R. Hauser.  More information is available through the Web
page `http://HTTP.CS.Berkeley.EDU/~jhauser/arithmetic/TestFloat.html'.

THIS SOFTWARE IS DISTRIBUTED AS IS, FOR FREE.  Although reasonable effort
has been made to avoid it, THIS SOFTWARE MAY CONTAIN FAULTS THAT WILL AT
TIMES RESULT IN INCORRECT BEHAVIOR.  USE OF THIS SOFTWARE IS RESTRICTED TO
PERSONS AND ORGANIZATIONS WHO CAN AND WILL TAKE FULL RESPONSIBILITY FOR ANY
AND ALL LOSSES, COSTS, OR OTHER PROBLEMS ARISING FROM ITS USE.

Derivative works are acceptable, even for commercial purposes, so long as
(1) they include prominent notice that the work is derivative, and (2) they
include prominent notice akin to these four paragraphs for those parts of
this code that are retained.

===============================================================================
*/


/*

This is heavily modified in that just enough stuff from testfloat
and a few lines from softfloat in order to get a 32 bit add function.

*/


//#include "milieu.h"
typedef char flag;
typedef unsigned char uint8;
typedef signed char int8;
typedef int uint16;
typedef int int16;
typedef unsigned int uint32;
typedef signed int int32;

typedef unsigned char bits8;
typedef signed char sbits8;
typedef unsigned short int bits16;
typedef signed short int sbits16;
typedef unsigned int bits32;
typedef signed int sbits32;

enum {
    FALSE = 0,
    TRUE  = 1
};

//#include "softfloat.h"

typedef unsigned int float32;
extern signed char float_detect_tininess;
enum {
    float_tininess_after_rounding  = 0,
    float_tininess_before_rounding = 1
};
extern signed char float_rounding_mode;
enum {
    float_round_nearest_even = 0,
    float_round_down         = 1,
    float_round_up           = 2,
    float_round_to_zero      = 3
};
extern signed char float_exception_flags;
enum {
    float_flag_invalid   =  1,
    float_flag_divbyzero =  4,
    float_flag_overflow  =  8,
    float_flag_underflow = 16,
    float_flag_inexact   = 32
};

//#include "slowfloat.h"


typedef struct {
    bits32 a0, a1;
} bits64X;

typedef struct {
    flag isNaN;
    flag isInf;
    flag isZero;
    flag sign;
    int16 exp;
    bits64X sig;
} floatX;


int8 slow_float_rounding_mode;
int8 slow_float_exception_flags;
int8 slow_float_detect_tininess;


static const floatX floatXNaN = { TRUE, FALSE, FALSE, FALSE, 0, { 0, 0 } };
static const floatX floatXPositiveZero =
    { FALSE, FALSE, TRUE, FALSE, 0, { 0, 0 } };
static const floatX floatXNegativeZero =
    { FALSE, FALSE, TRUE, TRUE, 0, { 0, 0 } };


float32 slow_float32_add( float32, float32 );


static bits64X shortShift64Left( bits64X a, int8 shiftCount )
{
    int8 negShiftCount;

    negShiftCount = ( - shiftCount & 31 );
    a.a0 = ( a.a0<<shiftCount ) | ( a.a1>>negShiftCount );
    a.a1 <<= shiftCount;
    return a;

}

static bits64X shortShift64RightJamming( bits64X a, int8 shiftCount )
{
    int8 negShiftCount;
    bits32 extra;

    negShiftCount = ( - shiftCount & 31 );
    extra = a.a1<<negShiftCount;
    a.a1 = ( a.a0<<negShiftCount ) | ( a.a1>>shiftCount ) | ( extra != 0 );
    a.a0 >>= shiftCount;
    return a;

}

static bits64X neg64( bits64X a )
{

    if ( a.a1 == 0 ) {
        a.a0 = - a.a0;
    }
    else {
        a.a1 = - a.a1;
        a.a0 = ~ a.a0;
    }
    return a;

}

static bits64X add64( bits64X a, bits64X b )
{

    a.a1 += b.a1;
    a.a0 += b.a0 + ( a.a1 < b.a1 );
    return a;

}

static flag eq64( bits64X a, bits64X b )
{

    return ( a.a0 == b.a0 ) && ( a.a1 == b.a1 );

}

//static flag le64( bits64X a, bits64X b )
//{

    //return ( a.a0 < b.a0 ) || ( ( a.a0 == b.a0 ) && ( a.a1 <= b.a1 ) );

//}

//static flag lt64( bits64X a, bits64X b )
//{

    //return ( a.a0 < b.a0 ) || ( ( a.a0 == b.a0 ) && ( a.a1 < b.a1 ) );

//}

static floatX roundFloatXTo24( flag isTiny, floatX zx )
{

    if ( zx.sig.a1 ) {
        slow_float_exception_flags |= float_flag_inexact;
        if ( isTiny ) slow_float_exception_flags |= float_flag_underflow;
        switch ( slow_float_rounding_mode ) {
         case float_round_nearest_even:
            if ( zx.sig.a1 < 0x80000000 ) goto noIncrement;
            if ( ( zx.sig.a1 == 0x80000000 ) && ! ( zx.sig.a0 & 1 ) ) {
                goto noIncrement;
            }
            break;
         case float_round_to_zero:
            goto noIncrement;
         case float_round_down:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case float_round_up:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        ++zx.sig.a0;
        if ( zx.sig.a0 == 0x01000000 ) {
            zx.sig.a0 = 0x00800000;
            ++zx.exp;
        }
    }
 noIncrement:
    zx.sig.a1 = 0;
    return zx;

}

static floatX roundFloatXTo53( flag isTiny, floatX zx )
{
    int8 roundBits;

    roundBits = zx.sig.a1 & 7;
    zx.sig.a1 -= roundBits;
    if ( roundBits ) {
        slow_float_exception_flags |= float_flag_inexact;
        if ( isTiny ) slow_float_exception_flags |= float_flag_underflow;
        switch ( slow_float_rounding_mode ) {
         case float_round_nearest_even:
            if ( roundBits < 4 ) goto noIncrement;
            if ( ( roundBits == 4 ) && ! ( zx.sig.a1 & 8 ) ) goto noIncrement;
            break;
         case float_round_to_zero:
            goto noIncrement;
         case float_round_down:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case float_round_up:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        zx.sig.a1 += 8;
        zx.sig.a0 += ( zx.sig.a1 == 0 );
        if ( zx.sig.a0 == 0x01000000 ) {
            zx.sig.a0 = 0x00800000;
            ++zx.exp;
        }
    }
 noIncrement:
    return zx;

}

//static floatX int32ToFloatX( int32 a )
//{
    //floatX ax;

    //ax.isNaN = FALSE;
    //ax.isInf = FALSE;
    //ax.sign = ( a < 0 );
    //ax.sig.a1 = ax.sign ? - a : a;
    //ax.sig.a0 = 0;
    //if ( a == 0 ) {
        //ax.isZero = TRUE;
        //return ax;
    //}
    //ax.isZero = FALSE;
    //ax.sig = shortShift64Left( ax.sig, 23 );
    //ax.exp = 32;
    //while ( ax.sig.a0 < 0x00800000 ) {
        //ax.sig = shortShift64Left( ax.sig, 1 );
        //--ax.exp;
    //}
    //return ax;

//}

static int32 floatXToInt32( floatX ax )
{
    int8 savedExceptionFlags;
    int16 shiftCount;
    int32 z;

    if ( ax.isInf || ax.isNaN ) {
        slow_float_exception_flags |= float_flag_invalid;
        return ( ax.isInf & ax.sign ) ? 0x80000000 : 0x7FFFFFFF;
    }
    if ( ax.isZero ) return 0;
    savedExceptionFlags = slow_float_exception_flags;
    shiftCount = 52 - ax.exp;
    if ( 56 < shiftCount ) {
        ax.sig.a1 = 1;
        ax.sig.a0 = 0;
    }
    else {
        while ( 0 < shiftCount ) {
            ax.sig = shortShift64RightJamming( ax.sig, 1 );
            --shiftCount;
        }
    }
    ax = roundFloatXTo53( FALSE, ax );
    ax.sig = shortShift64RightJamming( ax.sig, 3 );
    z = ax.sig.a1;
    if ( ax.sign ) z = - z;
    if (    ( shiftCount < 0 )
         || ax.sig.a0
         || ( ( z != 0 ) && ( ( ax.sign ^ ( z < 0 ) ) != 0 ) )
       ) {
        slow_float_exception_flags = savedExceptionFlags | float_flag_invalid;
        return ax.sign ? 0x80000000 : 0x7FFFFFFF;
    }
    return z;

}

static floatX float32ToFloatX( float32 a )
{
    int16 expField;
    floatX ax;

    ax.isNaN = FALSE;
    ax.isInf = FALSE;
    ax.isZero = FALSE;
    ax.sign = ( ( a & 0x80000000 ) != 0 );
    expField = ( a>>23 ) & 0xFF;
    ax.sig.a1 = 0;
    ax.sig.a0 = a & 0x007FFFFF;
    if ( expField == 0 ) {
        if ( ax.sig.a0 == 0 ) {
            ax.isZero = TRUE;
        }
        else {
            expField = 1 - 0x7F;
            do {
                ax.sig.a0 <<= 1;
                --expField;
            } while ( ax.sig.a0 < 0x00800000 );
            ax.exp = expField;
        }
    }
    else if ( expField == 0xFF ) {
        if ( ax.sig.a0 == 0 ) {
            ax.isInf = TRUE;
        }
        else {
            ax.isNaN = TRUE;
        }
    }
    else {
        ax.sig.a0 |= 0x00800000;
        ax.exp = expField - 0x7F;
    }
    return ax;

}

static float32 floatXToFloat32( floatX zx )
{
    floatX savedZ;
    flag isTiny;
    int16 expField;
    float32 z;

    z=0;

    if ( zx.isZero ) return zx.sign ? 0x80000000 : 0;
    if ( zx.isInf ) return zx.sign ? 0xFF800000 : 0x7F800000;
    if ( zx.isNaN ) return 0xFFFFFFFF;
    while ( 0x01000000 <= zx.sig.a0 ) {
        zx.sig = shortShift64RightJamming( zx.sig, 1 );
        ++zx.exp;
    }
    while ( zx.sig.a0 < 0x00800000 ) {
        zx.sig = shortShift64Left( zx.sig, 1 );
        --zx.exp;
    }
    savedZ = zx;
    isTiny =
           ( slow_float_detect_tininess == float_tininess_before_rounding )
        && ( zx.exp + 0x7F <= 0 );
    zx = roundFloatXTo24( isTiny, zx );
    expField = zx.exp + 0x7F;
    if ( 0xFF <= expField ) {
        slow_float_exception_flags |=
            float_flag_overflow | float_flag_inexact;
        if ( zx.sign ) {
            switch ( slow_float_rounding_mode ) {
             case float_round_nearest_even:
             case float_round_down:
                z = 0xFF800000;
                break;
             case float_round_to_zero:
             case float_round_up:
                z = 0xFF7FFFFF;
                break;
            }
        }
        else {
            switch ( slow_float_rounding_mode ) {
             case float_round_nearest_even:
             case float_round_up:
                z = 0x7F800000;
                break;
             case float_round_to_zero:
             case float_round_down:
                z = 0x7F7FFFFF;
                break;
            }
        }
        return z;
    }
    if ( expField <= 0 ) {
        isTiny = TRUE;
        zx = savedZ;
        expField = zx.exp + 0x7F;
        if ( expField < -27 ) {
            zx.sig.a1 = ( zx.sig.a0 != 0 ) || ( zx.sig.a1 != 0 );
            zx.sig.a0 = 0;
        }
        else {
            while ( expField <= 0 ) {
                zx.sig = shortShift64RightJamming( zx.sig, 1 );
                ++expField;
            }
        }
        zx = roundFloatXTo24( isTiny, zx );
        expField = ( 0x00800000 <= zx.sig.a0 ) ? 1 : 0;
    }
    z = expField;
    z <<= 23;
    if ( zx.sign ) z |= 0x80000000;
    z |= zx.sig.a0 & 0x007FFFFF;
    return z;

}




static floatX floatXInvalid( void )
{

    slow_float_exception_flags |= float_flag_invalid;
    return floatXNaN;

}

static floatX floatXAdd( floatX ax, floatX bx )
{
    int16 expDiff;
    floatX zx;

    if ( ax.isNaN ) return ax;
    if ( bx.isNaN ) return bx;
    if ( ax.isInf && bx.isInf ) {
        if ( ax.sign == bx.sign ) return ax;
        return floatXInvalid();
    }
    if ( ax.isInf ) return ax;
    if ( bx.isInf ) return bx;
    if ( ax.isZero && bx.isZero ) {
        if ( ax.sign == bx.sign ) return ax;
        goto completeCancellation;
    }
    if (    ( ax.sign != bx.sign )
         && ( ax.exp == bx.exp )
         && eq64( ax.sig, bx.sig )
       ) {
 completeCancellation:
        return
              ( slow_float_rounding_mode == float_round_down ) ?
                  floatXNegativeZero
            : floatXPositiveZero;
    }
    if ( ax.isZero ) return bx;
    if ( bx.isZero ) return ax;
    expDiff = ax.exp - bx.exp;
    if ( expDiff < 0 ) {
        zx = ax;
        zx.exp = bx.exp;
        if ( expDiff < -56 ) {
            zx.sig.a1 = 1;
            zx.sig.a0 = 0;
        }
        else {
            while ( expDiff < 0 ) {
                zx.sig = shortShift64RightJamming( zx.sig, 1 );
                ++expDiff;
            }
        }
        if ( ax.sign != bx.sign ) zx.sig = neg64( zx.sig );
        zx.sign = bx.sign;
        zx.sig = add64( zx.sig, bx.sig );
    }
    else {
        zx = bx;
        zx.exp = ax.exp;
        if ( 56 < expDiff ) {
            zx.sig.a1 = 1;
            zx.sig.a0 = 0;
        }
        else {
            while ( 0 < expDiff ) {
                zx.sig = shortShift64RightJamming( zx.sig, 1 );
                --expDiff;
            }
        }
        if ( ax.sign != bx.sign ) zx.sig = neg64( zx.sig );
        zx.sign = ax.sign;
        zx.sig = add64( zx.sig, ax.sig );
    }
    if ( zx.sig.a0 & 0x80000000 ) {
        zx.sig = neg64( zx.sig );
        zx.sign = ! zx.sign;
    }
    return zx;

}


float32 slow_float32_add( float32 a, float32 b )
{

    return
        floatXToFloat32(
            floatXAdd( float32ToFloatX( a ), float32ToFloatX( b ) ) );

}

int32 slow_float32_to_int32 ( float32 a )
{
    return(floatXToInt32(float32ToFloatX(a)));
}


//#define int32NumP1 124
#define int32NumP1 79

static const uint32 cases32[ int32NumP1 ] =
{
    0x00000000,
    //0x00000001,
    //0x00000002,
    //0x00000004,
    //0x00000008,
    //0x00000010,
    //0x00000020,
    //0x00000040,
    //0x00000080,
    //0x00000100,
    //0x00000200,
    //0x00000400,
    //0x00000800,
    //0x00001000,
    //0x00002000,
    //0x00004000,
    //0x00008000,
    //0x00010000,
    //0x00020000,
    //0x00040000,
    //0x00080000,
    //0x00100000,
    //0x00200000,
    //0x00400000,
    0x00800000,
    0x01000000,
    0x02000000,
    0x04000000,
    0x08000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000,
    0xC0000000,
    0xE0000000,
    0xF0000000,
    0xF8000000,
    0xFC000000,
    0xFE000000,
    0xFF000000,
    0xFF800000,
    0xFFC00000,
    0xFFE00000,
    0xFFF00000,
    0xFFF80000,
    0xFFFC0000,
    0xFFFE0000,
    0xFFFF0000,
    0xFFFF8000,
    0xFFFFC000,
    0xFFFFE000,
    0xFFFFF000,
    0xFFFFF800,
    0xFFFFFC00,
    0xFFFFFE00,
    0xFFFFFF00,
    0xFFFFFF80,
    0xFFFFFFC0,
    0xFFFFFFE0,
    0xFFFFFFF0,
    0xFFFFFFF8,
    0xFFFFFFFC,
    0xFFFFFFFE,
    0xFFFFFFFF,
    0xFFFFFFFD,
    0xFFFFFFFB,
    0xFFFFFFF7,
    0xFFFFFFEF,
    0xFFFFFFDF,
    0xFFFFFFBF,
    0xFFFFFF7F,
    0xFFFFFEFF,
    0xFFFFFDFF,
    0xFFFFFBFF,
    0xFFFFF7FF,
    0xFFFFEFFF,
    0xFFFFDFFF,
    0xFFFFBFFF,
    0xFFFF7FFF,
    0xFFFEFFFF,
    0xFFFDFFFF,
    0xFFFBFFFF,
    0xFFF7FFFF,
    0xFFEFFFFF,
    0xFFDFFFFF,
    0xFFBFFFFF,
    0xFF7FFFFF,
    0xFEFFFFFF,
    0xFDFFFFFF,
    0xFBFFFFFF,
    0xF7FFFFFF,
    0xEFFFFFFF,
    0xDFFFFFFF,
    0xBFFFFFFF,
    0x7FFFFFFF,
    0x3FFFFFFF,
    0x1FFFFFFF,
    0x0FFFFFFF,
    0x07FFFFFF,
    0x03FFFFFF,
    0x01FFFFFF,
    0x00FFFFFF,
    //0x007FFFFF,
    //0x003FFFFF,
    //0x001FFFFF,
    //0x000FFFFF,
    //0x0007FFFF,
    //0x0003FFFF,
    //0x0001FFFF,
    //0x0000FFFF,
    //0x00007FFF,
    //0x00003FFF,
    //0x00001FFF,
    //0x00000FFF,
    //0x000007FF,
    //0x000003FF,
    //0x000001FF,
    //0x000000FF,
    //0x0000007F,
    //0x0000003F,
    //0x0000001F,
    //0x0000000F,
    //0x00000007,
    //0x00000003
};



