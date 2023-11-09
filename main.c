#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char snum[5];
char *wyrazenie;
#define PIERWIASTEK3 1.7320508

#define SREDNICADEF 50;
#define SZEROKOSCDEF 500;
#define DLUGOSCDEF 500;
#define GRUBOSCDEF 4;
int SREDNICA = 0;
int SZEROKOSC = 0;
int DLUGOSC = 0;
int GRUBOSC = 0;
const char *s1 = "<div id = \"circleblue\" style = \"margin-left: ";
const char *s11 = "<div id = \"circlered\" style = \"margin-left: ";
const char *s2 = "px; margin-top: ";
const char *s3 = "px;\"></div>\n";
char pominRysowanie = 0;

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char *itoa(int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36)
    {
        *result = '\0';
        return result;
    }

    char *ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';

    // Reverse the string
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
void wypisz(int left, int top, char blue)
{
    if (pominRysowanie)
    {
        return;
    }
    if (blue)
    {
        strcpy(wyrazenie, s1);
    }
    else
    {
        strcpy(wyrazenie, s11);
    }

    itoa(left + GRUBOSC, snum, 10);
    strcat(wyrazenie, snum);

    strcat(wyrazenie, s2);

    itoa(top + GRUBOSC, snum, 10);
    strcat(wyrazenie, snum);

    strcat(wyrazenie, s3);

    printf(wyrazenie);
    strcpy(wyrazenie, "");
}
void generuj()
{
    pominRysowanie = 0;
    printf("\n Podaj srednice kulki: ");
    if (scanf("%d", &SREDNICA) != 1)
        exit(1);

    printf("\n Podaj szerokosc (prostokata w ktorym maja byc kulki)<-->: ");
    if (scanf("%d", &SZEROKOSC) != 1)
        exit(1);
    printf("\n Podaj dlugosc (prostokatna w ktorym maja byc kulki): ");
    if (scanf("%d", &DLUGOSC) != 1)
        exit(1);

    char t3 = 11;
    printf("\n Pominac rysowanie? [T/N]: ");
    scanf("%s", &t3);

    if (t3 == 'T')
    {
        pominRysowanie = 1;
        printf("\nSREDNICA: %d, SZEROKOSC <-->: %d, DLUGOSC: %d", SREDNICA, SZEROKOSC, DLUGOSC);
    }
    else
    {
        printf("\n Podaj punkt startu rysowania [px]: ");
        if (scanf("%d", &GRUBOSC) != 1)
            exit(1);
        printf("\nSREDNICA: %d, SZEROKOSC <-->: %d, DLUGOSC: %d, GRUBOSC: %d\n", SREDNICA, SZEROKOSC, DLUGOSC, GRUBOSC);
    }

    printf("\nSTART BODY\n");
    int wierszyNiebieskich = SREDNICA ? (((DLUGOSC - SREDNICA) / (SREDNICA * PIERWIASTEK3 * 0.5)) + 1) : 0;
    int wierszyCzerwonych = SREDNICA ? (DLUGOSC / SREDNICA) : 0;
    int kolumn = SREDNICA ? (SZEROKOSC / SREDNICA) : 0;
    int iloscCzerwonych = 0;
    int ilosccNiebieskich = 0;
    for (size_t in = 0; in < wierszyNiebieskich; in++)
    {
        if (in % 2)
        {

            for (size_t i = 0; i < (kolumn - 1); i++)
            {
                wypisz((i * SREDNICA) + (SREDNICA / 2), (PIERWIASTEK3 / 2.0) * SREDNICA * in, 1);
                ilosccNiebieskich++;
            }
        }
        else
        {

            for (size_t i = 0; i < kolumn; i++)
            {
                wypisz(i * SREDNICA, (PIERWIASTEK3 / 2.0) * SREDNICA * (in ? in : 0), 1);
                ilosccNiebieskich++;
            }
        }
    }
    getchar();
    for (size_t in = 0; in < wierszyCzerwonych; in++)
    {
        for (size_t i = 0; i < kolumn; i++)
        {
            wypisz(i * SREDNICA, in * SREDNICA, 0);
            iloscCzerwonych++;
        }
    }

    printf("\nNiebieskich %d Czerwonych  %d\n", ilosccNiebieskich, iloscCzerwonych);

    printf("\nChcesz powtorzyc? [T/N]: ");
    char t = 0;
    scanf("%c", &t);
    if (t == 'T')
    {
        generuj();
    }
}
int main(int argc, char *argv[])
{
    wyrazenie = malloc(100 * sizeof(char));
    generuj();

    return 0;
}