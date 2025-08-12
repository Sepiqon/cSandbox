
//  gcc  -o prog  -LC:\Users\RyZen\Desktop\ch431 CH341WriteI2C.c -lCH341DLL
#include <windows.h>
#include <stdio.h>
#include <time.h>
#define e 2.718281828459045
#define pi 3.141592653
#define precyzyjnosc_ln 100
#define precyzyjnosc_e_pow 100
#define precyzyjnosc_sin 100
#define precyzyjnosc_atan 1000

struct timeval GetTimeStamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}
long double power_int(double x, unsigned long int pow)
{
    long double res = 1;
    for (size_t i = 1; i <= pow; i++)
    {
        res *= x;
    }

    return res;
}
double calc(double temp_x)
{
    double sum = 0;
    for (size_t i = 1; i < precyzyjnosc_ln; i++)
    {
        sum += power_int(temp_x - 1, i) / i * ((i % 2) ? 1 : (-1));
    }
    return sum;
}
double ln_my(double x)
{
    if (!x)
        return 1;
    int eX = 0;
    double temp_x = x;
    while (temp_x > 1.7)
    {
        temp_x /= e;
        eX++;
    }
    while (temp_x < 0.6)
    {
        temp_x *= e;
        eX--;
    }
    return eX + calc(temp_x);
}
double logx_y(double x, double y)
{
    return ln_my(y) / ln_my(x);
}
double e_pow(double x)
{
    double term = 1.0;
    double sum = 1.0;
    for (int i = 1; i < precyzyjnosc_e_pow; i++)
    {
        term *= x / i;
        sum += term;
    }
    return sum;
}
double sqrt_my(double x)
{
    return e_pow(ln_my(x) / 2);
}
double pow_my(double x, double pow)
{
    if (((int)pow) == pow)
        return power_int(x, pow);
    return e_pow(ln_my(x) * pow);
}
unsigned long long int factorial(unsigned long int i)
{
    unsigned long long int temp = 1;
    for (size_t j = 2; j <= i; j++)
    {
        temp *= j;
    }
    return temp;
}

double deg2rad(double deg)
{
    deg = (deg - ((int)deg)) + ((int)deg) % 360;
    return deg / 180.0 * pi;
}
double rad2deg(double rad)
{
    rad /= pi;
    rad = (rad - ((int)rad)) + ((int)rad) % 2;
    return rad * 180;
}
double sin_my(double rad)
{
    double term = rad; // pierwszy wyraz
    double sum = rad;
    for (size_t n = 1; n < precyzyjnosc_sin; n++)
    {
        term *= -rad * rad / ((2.0 * n) * (2.0 * n + 1));
        sum += term;
    }
    return sum;
}
double cos_my(double rad)
{
    return sin_my(deg2rad((rad / pi) * 180 + 90));
}
double tan_my(double rad)
{
    return sin_my(rad) / cos_my(rad);
}
double atan_my(double x)
{
    // dla |x| > 1 redukujemy do mniejszego argumentu
    if (x > 1.0)
        return pi / 2 - atan_my(1.0 / x);
    if (x < -1.0)
        return -pi / 2 - atan_my(1.0 / x);

    double sum = 0.0;
    for (size_t i = 0; i < precyzyjnosc_atan; i++)
    {

        double term = power_int(x, 2 * i + 1) / (2 * i + 1);
        if (i % 2 != 0)
            term = -term;
        sum += term;
    }
    return sum;
}
double atan2_my(double y, double x)
{
    if (x > 0)
        return atan_my(y / x);
    if (x < 0 && y >= 0)
        return atan_my(y / x) + pi;
    if (x < 0 && y < 0)
        return atan_my(y / x) - pi;
    if (x == 0 && y > 0)
        return pi / 2;
    if (x == 0 && y < 0)
        return -pi / 2;
    return 0.0; // x=0, y=0 → kąt 0
}

struct Complex
{
    double real;
    double i;
};
struct ComplexAngle
{
    double mod;
    double deg;
};
typedef struct Complex Complex_t;
typedef struct ComplexAngle ComplexAngle_t;
double moduleCompex(Complex_t complex)
{
    return sqrt_my(complex.real * complex.real + complex.i * complex.i);
}
double realComplex(Complex_t complex)
{
    return complex.real;
}
double iComplex(Complex_t complex)
{
    return complex.i;
}
Complex_t sumComplex(Complex_t a, Complex_t b)
{
    Complex_t res;
    res.real = a.real + b.real;
    res.i = a.i + b.i;
    return res;
}
Complex_t angle2Complex(ComplexAngle_t complex)
{
    Complex_t res = {.real = cos_my(deg2rad(complex.deg)) * complex.mod,
                     .i = sin_my(deg2rad(complex.deg)) * complex.mod};
    return res;
}
ComplexAngle_t complex2Angle(Complex_t complex)
{

    boolean is_dodatnie;
    ComplexAngle_t res = {mod : moduleCompex(complex),
                          deg :
                              rad2deg(atan2_my(complex.i, complex.real))};
    return res;
}
Complex_t multiComplexAngle(ComplexAngle_t a, ComplexAngle_t b)
{
    ComplexAngle_t preReturn = {
        .mod = a.mod * b.mod,
        .deg = a.deg + b.deg};
    Complex_t res = angle2Complex(preReturn);
    return res;
}
Complex_t multiComplex(Complex_t a, Complex_t b)
{
    ComplexAngle_t a1 = complex2Angle(a);
    ComplexAngle_t b1 = complex2Angle(b);
    return multiComplexAngle(a1, b1);
}
Complex_t sumComplexAngle(ComplexAngle_t a, ComplexAngle_t b)
{
    Complex_t a1 = angle2Complex(a);
    Complex_t b1 = angle2Complex(b);
    return sumComplex(a1, b1);
}

int main(int argc, char *argv[])
{

    struct timeval t1 = GetTimeStamp();
    unsigned long time_in_micros = GetTimeStamp().tv_usec;

    ComplexAngle_t E1 = {mod : 13, deg : 120};
    ComplexAngle_t E2 = {mod : 14, deg : 110};
    ComplexAngle_t E3 = {mod : 15, deg : 300};
    for (size_t i = 0; i < 1; i++)
    {
        ComplexAngle_t E_sum = complex2Angle(
            sumComplexAngle(complex2Angle(sumComplexAngle(E1, E2)), E3));

        printf("U_N_RMS:%f U_N_DEG:%f\n", E_sum.mod, E_sum.deg);
    }

    unsigned long time_in_micros2 = GetTimeStamp().tv_usec;
    printf("%d us", time_in_micros2 - time_in_micros);
}