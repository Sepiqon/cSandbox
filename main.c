#include <stdio.h>
#include <stdlib.h
#include <time.h>
const char *BASE64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int roundUp(int val, int dev)
{
    int temp = val / dev;
    if (val % dev)
        temp++;
    return temp;
}

char *base64(const char *str)
{
    if (!(*str))
        return 0;
    char *adressStart = (char *)str;
    unsigned int sizeText = 0, stay, element = 0, currentGroup, groups;
    unsigned char firstByte, secondByte, thirdByte, aByte, bByte, cByte, dByte;
    unsigned char tempOutput[5] = {0, 0, 0, 0, 0};
    while (*(adressStart + sizeText))
    {
        sizeText++;
    }
    stay = sizeText;
    groups = roundUp(sizeText, 3);

    char *temp = malloc((groups * 4) + 1);
    temp[groups * 4] = 0;

    tempOutput[4] = 0;
    for (currentGroup = 0; currentGroup < (groups - 1); currentGroup++)
    {
        firstByte = *(adressStart + (element));
        secondByte = *(adressStart + (element + 1));
        thirdByte = *(adressStart + (element + 2));

        aByte = (firstByte) >> 2;
        bByte = (((unsigned char)(firstByte << 6)) >> 2) + ((secondByte) >> 4);
        cByte = (((unsigned char)(secondByte << 4)) >> 2) + (thirdByte >> 6);
        dByte = ((unsigned char)(thirdByte << 2)) >> 2;

        tempOutput[0] = BASE64Chars[aByte];
        tempOutput[1] = BASE64Chars[bByte];
        tempOutput[2] = BASE64Chars[cByte];
        tempOutput[3] = BASE64Chars[dByte];
        ((int *)temp)[currentGroup] = ((int *)tempOutput)[0];
        element += 3;
        stay -= 3;
    }
    // LAST GROUP
    firstByte = *(adressStart + (element));
    tempOutput[2] = 0;
    tempOutput[3] = 0;
    if (stay > 1)
    {
        secondByte = *(adressStart + (element + 1));
    }
    else
    {
        secondByte = 0;
        tempOutput[2] = 61;
    }
    if (stay > 2)
    {
        thirdByte = *(adressStart + (element + 2));
    }
    else
    {
        thirdByte = 0;
        tempOutput[3] = 61;
    }
    aByte = (firstByte) >> 2;
    bByte = (((unsigned char)(firstByte << 6)) >> 2) + ((secondByte) >> 4);
    tempOutput[0] = BASE64Chars[aByte];
    tempOutput[1] = BASE64Chars[bByte];
    if (!tempOutput[2])
    {
        cByte = (((unsigned char)(secondByte << 4)) >> 2) + (thirdByte >> 6);
        tempOutput[2] = BASE64Chars[cByte];
    }
    if (!tempOutput[3])
    {
        dByte = ((unsigned char)(thirdByte << 2)) >> 2;
        tempOutput[3] = BASE64Chars[dByte];
    }
    ((int *)temp)[groups - 1] = ((int *)tempOutput)[0];
    return temp;
}
int main(int argc, char *argv[])
{
    const char *const text = "CZEŚĆ";

    printf("Teskt: \"%s\" został zaszyfrowany na: \"%s\"", text, base64(text));
    return 0;
}