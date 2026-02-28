#include <stdio.h>
double ln_my(double x)
{
    int k = 0;

    while (x > 1.4)
    {
        x /= 2.0;
        k++;
    }
    while (x < 0.7)
    {
        x *= 2.0;
        k--;
    }

    double t = (x - 1) / (x + 1);
    double sum = 0.0;
    double potega = t;
    int i;
    for (i = 1; i < 20; i += 2)
    {
        sum += potega / i;
        potega *= t * t;
    }

    return 2.0 * sum + k * 0.69314718055994530941723212145818;
}
double exp_my(double x)
{

    double y = x / 16.0;

    double term = 1.0;
    double sum = 1.0;
    int i;
    for (i = 1; i < 20; i++)
    {
        term *= y / i;
        sum += term;
    }
    sum = sum * sum;
    sum = sum * sum;
    sum = sum * sum;
    sum = sum * sum;

    return sum;
}
double log_xy_my(double x, double y)
{
    return ln_my(y) / ln_my(x);
}
double pow_my(double x, double y)
{
    return (exp_my(y * ln_my(x)));
}
double round_my_n(double x, int n)
{
    double p = 1.0;
    int i;
    for (i = 0; i < n; i++)
        p *= 10.0;

    if (x >= 0.0)
        return (double)((long long)(x * p + 0.5)) / p;
    else
        return (double)((long long)(x * p - 0.5)) / p;
}
int main(int argc, char *argv[])
{
    double x = exp_my(1.123);
    double y = x * x;
    if (y == 3)
    {
    }
    else
    {
    }

    printf("Siemaaa1\n");
    printf("Siemaaa2\n");
    getchar();
    return 0;
}