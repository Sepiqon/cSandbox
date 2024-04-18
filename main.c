
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "main.h"
int main(int argc, char *argv[])
{
    double a, b, c, delta;

    // Zapytanie o a, b i c
    printf("\na(x^2) + bc + c = 0");
    printf("\nPodaj A: ");
    scanf("%lA", &a);
    printf("Podaj B: ");
    scanf("%lA", &b);
    printf("Podaj C: ");
    scanf("%lA", &c);

    // Wydrukowanie funkcji

    if (a && a != -1 && a != 1)
        printf("%s(x^2)", numToString(&a));
    if (a == -1)
        printf("-(x^2)");
    if (a == 1)
        printf("(x^2)");

    if (b > 0 && a)
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
    if (c > 0 && (a || b))
    {
        printf("+");
    }
    if (c && (a || b))
        printf(numToString(&c));
    if (a || b)
        printf("=0\n");

    // Liczenie delty
    delta = (b * b) - 4 * a * c;

    // delta ujemna
    if (delta < 0)
    {
        // wyswietlenie uzytkownikowi
        printf("\nRownanie nie ma rozwiazan\n");
        // return 0;
    }
    // delta rowna 0
    else if (delta == 0 && a)
    {
        // policzenie jednego x
        long double x = (-b) / (2 * a);
        double xd = x;
        //
        double a1 = skoryguj(a * pow(x, 2));
        double a2 = skoryguj(b * x);
        double a3 = c;
        BOOLEAN a2b = a2 > 0;
        BOOLEAN a3b = a3 > 0;
        // x
        printf("\n%s", numToString(&a1));
        if (a2b)
            printf("+");
        printf("%s", numToString(&a2));
        if (a3b)
            printf("+");
        printf("%s=0\n", numToString(&a3));
        // wyswietlenie uzytkownikowi
        printf("x = %s", numToString(&xd));
    }
    // delta dodatnia
    else if (delta > 0 && a)
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

        double a1 = skoryguj(a * pow(x1d, 2));
        double a2 = skoryguj(b * x1d);
        double a1_1 = skoryguj(a * pow(x2d, 2));
        double a2_1 = skoryguj(b * x2d);
        double a3 = c;
        BOOLEAN a2b = a2 >= 0;
        BOOLEAN a3b = a3 >= 0;
        BOOLEAN a2b_1 = a2_1 >= 0;
        // x1
        printf("\n%s", numToString(&a1));
        if (a2b)
            printf("+");
        printf("%s", numToString(&a2));
        if (a3b)
            printf("+");
        printf("%s=0\n", numToString(&a3));
        // x2
        printf("%s", numToString(&a1_1));
        if (a2b_1)
            printf("+");
        printf("%s", numToString(&a2_1));
        if (a3b)
            printf("+");
        printf("%s=0\n", numToString(&a3));
        //  wyswietlenie uzytkownikowi
        printf("x1 = %s | x2 = %s", numToString(&x1d), numToString(&x2d));
    }
    // funkcja liniowa
    else if (!a && b)
    {
        long double cd = c, bc = b, x;
        double x1;
        x = -(cd / bc);
        x1 = x;
        // wyswietlenie uzytkownikowi
        printf("\n x = %s", numToString(&x1));
    }
    // funkcja stala
    else if (!a && !b)
    {
        printf("\nf(x) = %s", numToString(&c));
    }
    // zatrzymanie programu
    getchar();
    getchar();
    return 0;
}

char *const numToString(double *num)
{
    // po przecinku np. 9,999 -> 3
    char poPrzecinku = poPrzecinkuVoid(num);
    if (poPrzecinku > 15)
        poPrzecinku = 15;
    // przed przecinkiem np. 999,9 -> 3
    char przedPrzecinkiem = przedPrzecinkiemVoid(num);
    // liczby zawierające 0 na poczatku np. -0,25 oraz 0.25
    BOOLEAN zero = (*num < 1) && (*num > -1);
    // liczby ze znakiem '-' na poczatku
    BOOLEAN ujemne = ((*num) < 0);
    // wszytskie cyfry liczby bez przecinka
    long long liczba = round(bezw(*num) * (pow(10, poPrzecinku)));
    if ((!(liczba % 10)) && poPrzecinku > 0)
    {
        poPrzecinku--;
        liczba /= 10;
    }
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