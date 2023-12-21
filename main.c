
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "main.h"
int main(int argc, char *argv[])
{
    double a, b, c, delta;

    // Zapytanie o a, b i c
    printf("\nPodaj A: ");
    scanf("%lA", &a);
    printf("\nPodaj B: ");
    scanf("%lA", &b);
    printf("\nPodaj C: ");
    scanf("%lA", &c);

    // Wydrukowanie funkcji

    printf("%s", (a == 1) ? "(x^2)" : ((a == -1) ? "-(x^2)" : (a ? strcat(numToString(&a), "(x^2)") : "")));

    if (b > 0)
    {
        printf("+");
    }
    if (b == -1)
    {
        printf("-");
    }
    if (b != 1 && b != -1 && b)
    {
        printf(numToString(&b));
    }
    if (b)
        printf("x");
    if (c > 0)
    {
        printf("+");
    }
    if (c)
        printf(numToString(&c));
    printf("=0\n");

    // Liczenie delty
    delta = (b * b) - 4 * a * c;

    // delta ujemna
    if (delta < 0)
    {
        // wyswietlenie uzytkownikowi
        printf("\nRownanie nie ma rozwiazan");
        return 0;
    }
    // delta rowna 0
    else if (delta == 0)
    {
        // policzenie jednego x
        double x = (-b) / (2 * a);
        // wyswietlenie uzytkownikowi
        printf("\n x = %s", numToString(&x));
    }
    // delta dodatnia
    else if (delta > 0)
    {
        // spierwiastkowanie delty
        long double x1, x2, pierwiastek = pow(delta, 0.5), bd = b, ad = a;
        // policzenie dwóch x odpowiednio x1 i x2
        x1 = ((-bd) - pierwiastek) / (2L * ad);
        x2 = ((-bd) + pierwiastek) / (2L * ad);
        // zmiejszenie dokładnosci z long double do double
        double x1d = x1, x2d = x2;
        // usuniecie niedoskonalosci licz zmiennoprzecinkowych
        x1d = skoryguj(x1d);
        x2d = skoryguj(x2d);
        // wyswietlenie uzytkownikowi
        printf("\n x1 = %s | x2 = %s", numToString(&x1d), numToString(&x2d));
    }
    // zatrzymanie programu
    getchar();
    return 0;
}

char *const numToString(double *num)
{
    // po przecinku np. 9,999 -> 3
    char poPrzecinku = poPrzecinkuVoid(num);
    // przed przecinkiem np. 999,9 -> 3
    char przedPrzecinkiem = przedPrzecinkiemVoid(num);
    // liczby zawierające 0 na poczatku np. -0,25 oraz 0.25
    BOOLEAN zero = (*num < 1) && (*num > -1);
    // liczby ze znakiem '-' na poczatku
    BOOLEAN ujemne = ((*num) < 0);
    // wszytskie cyfry liczby bez przecinka
    long long liczba = ceil(bezw(*num) * (pow(10, poPrzecinku)));
    // ilosc znakow po wypisaniu np. "-0,25" => 5
    char iloscZnakow = poPrzecinku + przedPrzecinkiem + (poPrzecinku ? 1 : 0) + zero + ujemne;
    // wyjsciowy ciag znakow
    char *temp = malloc(iloscZnakow + 1);
    // dodanie konca ciągu
    temp[iloscZnakow] = 0;
    // id cyfry z liczby zaczynajac od 1
    char cyfraId = 1;
    // zmienna dodatkowa
    BOOLEAN dodanyPrzecinek = 0;
    for (char i = (iloscZnakow - 1); i >= 0; i--)
    {
        // dodanie przecinku
        if ((cyfraId == (poPrzecinku + 1)) && (!dodanyPrzecinek) && poPrzecinku)
        {
            temp[i] = ',';
            dodanyPrzecinek = 1;
        }
        else
        {
            // dodanie znaku '-'
            if ((i == 0 && ujemne))
            {
                temp[i] = '-';
            }
            else
            {
                // dodanie cyfr liczby
                temp[i] = cyfrToChar(getCyfraFromNum(liczba, cyfraId));
            }

            cyfraId++;
        }
    }
    return temp;
}

char cyfrToChar(char c)
{
    return c + '0';
}
char getCyfraFromNum(long long int a, int id)
{

    if (id == 1)
    {
        return a % 10;
    }
    if (id < 2)
    {
        return 0;
    }
    long long int i1 = a % (long long int)(pow(10, (double)id));
    long long int i2 = a % (long long int)(pow(10, (double)(id - 2)));
    long long int i3 = i1 - i2;

    i3 /= pow(10, (double)(id - 1));
    return i3;
}
char poPrzecinkuVoid(double *num)
{
    char poPrzecinku = 0;
    while ((bezw(*num) * (pow(10, poPrzecinku))) != ceil(bezw(*num) * (pow(10, poPrzecinku))))
    {
        poPrzecinku++;
    }
    return poPrzecinku;
}
char przedPrzecinkiemVoid(double *num)
{
    char przedPrzecinkiem = 0;
    while (pow(10, przedPrzecinkiem) <= (bezw(*num)))
    {
        przedPrzecinkiem++;
    }
    return przedPrzecinkiem;
}
double bezw(double num)
{
    // jesli jest ujemna zwroci przeciwna do niej - dodatnia
    if (num < 0)
    {
        return -num;
    }
    // dodatnia jest zwracana odrazu
    return num;
}
double skoryguj(double num)
{
    // oryginalna liczba pozycji po przecinku
    const char poPOry = poPrzecinkuVoid(&num);
    // zapissanie oryginalnej liczby
    const double oryginalnaLiczba = num;
    // zakonczy jesli po przecinku jest mniej niz 10 pozycji
    if (poPOry < 10)
    {
        return num;
    }
    // zwiekszenie dokladnosci z double do long double
    long double lnum = num;
    // wartosc dodawana by przyblizyc sie do wartosci okrągłej
    const long double step = 0.0000000000000001L;
    // zakres -19E-16L do 19E-16L
    long double start = lnum - 0.0000000000000019L;

    for (int i = 0; i < 39; i++)
    {

        num = start;
        // jesli jest conajmniej 2 po przecinku mniej zakonczy program
        if (poPrzecinkuVoid(&num) < (poPOry - 1))
        {
            return num;
        }
        // szukanie liczby okrągłej
        start = start + step;
    }
    // zwracanie oryginalnej liczby jesli wyszukiwanie nie zwrocilo
    return oryginalnaLiczba;
}