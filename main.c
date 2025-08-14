#include <windows.h>
#include <stdio.h>
#pragma pack(push, 1)

typedef struct header
{
    union
    {
        struct
        {
            char identifier[8];
            INT32 version;
            INT32 type;
            INT32 initial_state;
            double begin_time;
            double end_time;
            INT32 num_transitions;
            INT32 res;
        };
    } data;
} header_t;
#pragma pack(pop)

int main(int argc, char *argv[])
{
    FILE *fptr = fopen("digital_1.bin", "rb"); // use binary mode
    if (!fptr)
    {
        perror("Failed to open file");
        return 1;
    }

    header_t header_file;
    if (fread(&header_file, sizeof(header_t), 1, fptr) != 1)
    {
        perror("Failed to read file");
        fclose(fptr);
        return 1;
    };
    double *data = malloc(sizeof(double) * header_file.data.num_transitions);
    fread(data, sizeof(double), header_file.data.num_transitions, fptr);
    char state = header_file.data.initial_state;
    const double tik = 1.0 / 24000000.0;

    printf("%.5f|%d\n", header_file.data.begin_time * 1000000, state);

    for (size_t i = 0; i < header_file.data.num_transitions; i++)
    {
        printf("%.5f|%d\n", data[i] * 1000000, state);
        state = !state;
    }
    fclose(fptr);
    return 0;
}