#include <stdio.h>
#include <time.h>
#include <windows.h>

#define e 2.718281828459045
#define pi 3.141592653
#define precyzyjnosc_ln 100
#define precyzyjnosc_e_pow 100
#define precyzyjnosc_sin 100
#define precyzyjnosc_atan 1000

struct timeval GetTimeStamp()
{
    struct timeval tv;
    clock_t now = clock();
    tv.tv_sec = now / CLOCKS_PER_SEC;
    tv.tv_usec = (now % CLOCKS_PER_SEC) * (1000000L / CLOCKS_PER_SEC);
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
struct ComplexAll
{
    Complex_t complex;
    ComplexAngle_t angle;
};
typedef struct ComplexAll ComplexAll_t;
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
    ComplexAngle_t res = {mod : moduleCompex(complex),
                          deg :
                              rad2deg(atan2_my(complex.i, complex.real))};
    return res;
}
Complex_t multiComplexA(ComplexAngle_t a, ComplexAngle_t b)
{
    ComplexAngle_t preReturn = {
        .mod = a.mod * b.mod,
        .deg = a.deg + b.deg};
    Complex_t res = angle2Complex(preReturn);
    return res;
}
Complex_t divComplexA(ComplexAngle_t a, ComplexAngle_t b)
{
    ComplexAngle_t preReturn = {
        .mod = a.mod / b.mod,
        .deg = a.deg - b.deg};
    Complex_t res = angle2Complex(preReturn);
    return res;
}

Complex_t multiComplex(Complex_t a, Complex_t b)
{
    ComplexAngle_t a1 = complex2Angle(a);
    ComplexAngle_t b1 = complex2Angle(b);
    return multiComplexA(a1, b1);
}
Complex_t divComplex(Complex_t a, Complex_t b)
{
    ComplexAngle_t a1 = complex2Angle(a);
    ComplexAngle_t b1 = complex2Angle(b);
    return divComplexA(a1, b1);
}
Complex_t sumComplexA(ComplexAngle_t a, ComplexAngle_t b)
{
    Complex_t a1 = angle2Complex(a);
    Complex_t b1 = angle2Complex(b);
    return sumComplex(a1, b1);
}
Complex_t subComplexA(ComplexAngle_t a, ComplexAngle_t b)
{
    ComplexAngle_t b1 = b;
    b1.deg -= 180;
    return sumComplexA(a, b1);
}
Complex_t subComplex(Complex_t a, Complex_t b)
{
    Complex_t res = {
        .real = a.real - b.real,
        .i = a.i - b.i};
    return res;
}
Complex_t double2Complex(double a)
{
    Complex_t res = {
        .real = a,
        .i = 0};
    return res;
}
ComplexAngle_t double2ComplexA(double a)
{
    ComplexAngle_t res = {
        .mod = ((a < 0) ? -a : a),
        .deg = (a < 0) ? 180 : 0};
    return res;
}

int main(int argc, char *argv[])
{
    struct timeval t1 = GetTimeStamp();
    unsigned long time_in_micros = GetTimeStamp().tv_usec / 1000;

    ComplexAngle_t E1, E2, E3;
    ComplexAngle_t G1, G2, G3;

    printf("Podaj wartości RMS napięć i kąty fazowe:\n");
    printf("E1 RMS [V]: ");
    scanf("%lf", &E1.mod);
    printf("E1 kąt [deg]: ");
    scanf("%lf", &E1.deg);
    printf("E2 RMS [V]: ");
    scanf("%lf", &E2.mod);
    printf("E2 kąt [deg]: ");
    scanf("%lf", &E2.deg);
    printf("E3 RMS [V]: ");
    scanf("%lf", &E3.mod);
    printf("E3 kąt [deg]: ");
    scanf("%lf", &E3.deg);

    double R1, R2, R3;
    printf("Podaj rezystancje R1, R2, R3 [ohm]:\n");
    printf("R1: ");
    scanf("%lf", &R1);
    printf("R2: ");
    scanf("%lf", &R2);
    printf("R3: ");
    scanf("%lf", &R3);

    // Konwersja rezystancji na przewodności
    G1.mod = 1.0 / R1;
    G1.deg = 0;
    G2.mod = 1.0 / R2;
    G2.deg = 0;
    G3.mod = 1.0 / R3;
    G3.deg = 0;

    // Reszta main() pozostaje dokładnie taka sama
    for (size_t i = 0; i < 2; i++)
    {
        if (!i)
        {
            printf("Obliczenia bez neutralnego:\n");
        }
        else
        {
            printf("Obliczenia z neutralnym:\n");
        }

        Complex_t a = multiComplexA(G1, E1);
        Complex_t b = multiComplexA(G2, E2);
        Complex_t c = multiComplexA(G3, E3);
        Complex_t sum = sumComplex(sumComplex(a, b), c);
        Complex_t sumw = sumComplexA(complex2Angle(sumComplexA(G1, G2)), G3);
        ComplexAngle_t UN = complex2Angle(divComplex(sum, sumw));
        if (i)
        {
            UN.deg = 0;
            UN.mod = 0;
        }
        ComplexAngle_t UR1 = complex2Angle(subComplexA(E1, UN));
        ComplexAngle_t UR2 = complex2Angle(subComplexA(E2, UN));
        ComplexAngle_t UR3 = complex2Angle(subComplexA(E3, UN));
        ComplexAngle_t i1 = complex2Angle(multiComplexA(UR1, G1));
        ComplexAngle_t i2 = complex2Angle(multiComplexA(UR2, G2));
        ComplexAngle_t i3 = complex2Angle(multiComplexA(UR3, G3));
        ComplexAngle_t in = complex2Angle(sumComplexA(complex2Angle(sumComplexA(i1, i2)), i3));

        ComplexAngle_t PR1 = complex2Angle(multiComplexA(complex2Angle(multiComplexA(UR1, UR1)), G1));
        ComplexAngle_t PR2 = complex2Angle(multiComplexA(complex2Angle(multiComplexA(UR2, UR2)), G2));
        ComplexAngle_t PR3 = complex2Angle(multiComplexA(complex2Angle(multiComplexA(UR3, UR3)), G3));

        printf("U_E1_RMS:%f V   U_E1_DEG:%f\n", E1.mod, E1.deg);
        printf("U_E2_RMS:%f V   U_E2_DEG:%f\n", E2.mod, E2.deg);
        printf("U_E3_RMS:%f V   U_E3_DEG:%f\n\n", E3.mod, E3.deg);

        printf("U_N_RMS:%f V    U_N_DEG:%f\n", UN.mod, UN.deg);
        printf("I_N_RMS:%f A    I_N_DEG:%f\n\n", in.mod, in.deg);

        printf("U_R1_RMS:%f V   U_R1_DEG:%f\n", UR1.mod, UR1.deg);
        printf("U_R2_RMS:%f V   U_R2_DEG:%f\n", UR2.mod, UR2.deg);
        printf("U_R3_RMS:%f V   U_R3_DEG:%f\n\n", UR3.mod, UR3.deg);

        printf("I_R1_RMS:%f A   I_R1_DEG:%f\n", i1.mod, i1.deg);
        printf("I_R2_RMS:%f A   I_R2_DEG:%f\n", i2.mod, i2.deg);
        printf("I_R3_RMS:%f A   I_R3_DEG:%f\n\n", i3.mod, i3.deg);

        printf("P_R1_RMS:%f W   P_R1_DEG:%f\n", PR1.mod, PR1.deg);
        printf("P_R2_RMS:%f W   P_R2_DEG:%f\n", PR2.mod, PR2.deg);
        printf("P_R3_RMS:%f W   P_R3_DEG:%f\n\n", PR3.mod, PR3.deg);

        printf("P_RMS:%f W\n\n", PR1.mod + PR2.mod + PR3.mod);
    }

    unsigned long time_in_micros2 = GetTimeStamp().tv_usec / 1000;
    printf("%d ms\n", time_in_micros2 - time_in_micros);
    system("pause");
}