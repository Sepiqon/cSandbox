
//  gcc  -o prog  -LC:\Users\RyZen\Desktop\ch431 CH341WriteI2C.c -lCH341DLL
#include <windows.h>
#include <stdio.h>
#define YEAR_FROM 2020
#define PIERWSZY_DZIEŃ 3
enum day
{
    poniedziałek = 1,
    wtorek,
    środa,
    czwartek,
    piątek,
    sobota,
    niedziela
};
enum miesiac
{
    styczeń = 1,
    luty,
    marzec,
    kwiecien,
    maj,
    czerwiec,
    lipiec,
    sierpień,
    wrzesień,
    październik,
    listopad,
    grudzień
};
typedef enum day day_t;
typedef enum miesiac miesiac_t;
struct Date
{
    union
    {
        struct
        {
            day_t day_name : 3;
            unsigned char day_number : 5;
            miesiac_t miesiac_number : 4;
            unsigned char year_number : 4;
        };
        unsigned short val;

    } data;
};
typedef struct Date Date_t;
unsigned char miesiac2day_number(miesiac_t miesiac, char year)
{
    if (miesiac == 2)
    {
        return (30 + (-1 - (((YEAR_FROM + year) % 4) != 0)));
    }
    if (miesiac > 7)
    {
        return (30 + ((miesiac + 1) % 2));
    }
    else
    {
        return (30 + (miesiac % 2));
    }
};

unsigned short year2day_number(char year)
{
    return 365 + (((YEAR_FROM + year) % 4) == 0);
};

unsigned short date2number(Date_t date)
{
    unsigned char year_number = date.data.year_number;
    unsigned char miesiac_number = date.data.miesiac_number;
    unsigned short temp = 0;
    for (size_t i = year_number; i > 0; i--)
    {
        temp += year2day_number(i - 1);
    }
    for (size_t i = (miesiac_number - 1); i > 0; i--)
    {
        temp += miesiac2day_number(i, date.data.year_number);
    }
    temp += date.data.day_number;
    return temp;
};
Date_t number2Date(unsigned short number)
{
    unsigned short temp_number = number;
    Date_t temp;
    temp.data.day_name = (((number % 7) + (PIERWSZY_DZIEŃ + 6)) % 7);
    temp.data.year_number = 0;
    temp.data.miesiac_number = 1;
    temp.data.day_number = 0;

    unsigned char year_flag = 1;
    unsigned char miesiac_flag = 1;

    for (size_t i = 0; year_flag == 1; i++)
    {
        unsigned short temp_days_in_year = year2day_number(i);
        if (temp_number >= temp_days_in_year)
        {
            temp.data.year_number++;
            temp_number -= temp_days_in_year;
        }
        else
        {
            year_flag = 0;
        }
    }

    for (size_t i = 1; miesiac_flag == 1; i++)
    {
        unsigned short temp_days_in_miesiac = miesiac2day_number(i, temp.data.year_number);
        if (temp_number >= temp_days_in_miesiac)
        {
            temp.data.miesiac_number++;
            temp_number -= temp_days_in_miesiac;
        }
        else
        {
            miesiac_flag = 0;
        }
    }
    temp.data.day_number = temp_number;
    return temp;
}
int main(int argc, char *argv[])
{
    Date_t startDate;
    startDate.data.day_number = 30;
    startDate.data.miesiac_number = 5;
    startDate.data.year_number = 4;

    Date_t endDate;
    endDate.data.day_number = 19;
    endDate.data.miesiac_number = 6;
    endDate.data.year_number = 5;

    unsigned short dev = (date2number(endDate) - date2number(startDate));
    Date_t new_Date = number2Date(dev);
    printf("%d dni czyli %d lat %d miesiecy i %d dni\n", dev, new_Date.data.year_number, new_Date.data.miesiac_number, new_Date.data.day_number);
    return 0;
}