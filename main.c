#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *f = fopen("output.txt", "r");
    if (!f)
    {
        perror("output.txt");
        return 1;
    }

    double t;
    int scl, sda, dummy[6];
    int prev_scl = 1, prev_sda = 1;
    int bitcount = 0;
    int byte = 0;
    int in_frame = 0;

    while (fscanf(f, "%lf|%d|%d|%d|%d|%d|%d|%d|%d",
                  &t, &scl, &sda,
                  &dummy[0], &dummy[1], &dummy[2],
                  &dummy[3], &dummy[4], &dummy[5]) == 9)
    {
        // START condition
        if (prev_sda == 1 && sda == 0 && scl == 1)
        {
            printf("\n[START]\n");
            in_frame = 1;
            bitcount = 0;
            byte = 0;
        }

        // STOP condition
        if (prev_sda == 0 && sda == 1 && scl == 1 && in_frame)
        {
            printf("\n[STOP]\n");
            in_frame = 0;
        }

        // Capture data on rising edge of SCL
        if (in_frame && prev_scl == 0 && scl == 1)
        {
            bitcount++;

            if (bitcount <= 8)
            {
                byte = (byte << 1) | (sda & 1); // dane
            }
            else
            {
                // 9. bit = ACK/NACK
                printf("0x%02X %s\n", byte, sda == 0 ? "ACK" : "NACK");
                bitcount = 0;
                byte = 0;
            }
        }

        prev_scl = scl;
        prev_sda = sda;
    }

    fclose(f);
    return 0;
}