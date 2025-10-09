#include <windows.h>
#include <stdio.h>
#include <locale.h>

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
typedef struct transition
{
    double time;
    unsigned char state : 2;
    unsigned char channel : 6;
} transition_t;
typedef struct
{
    void *value;
    void *next;
} list_t;
#pragma pack(pop)

int main(int argc, char *argv[])
{
    size_t sizetablice = 0;
    list_t *data_out = malloc(sizeof(list_t));
    list_t *cur_element_in_tablice = data_out;
    list_t *startpoint[8];
    int elements[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    FILE *fptr;
    for (size_t channel = 0; channel < 9; channel++)
    {

        char filename[32];
        sprintf(filename, "digital_%d.bin", channel);
        fptr = fopen(filename, "rb");

        if (!fptr)
        {
            perror("Failed to open file");
            continue;
            ;
        }

        header_t header_file;
        if (fread(&header_file, sizeof(header_t), 1, fptr) != 1)
        {
            perror("Failed to read file");
            fclose(fptr);
            continue;
        };

        double data = 0;

        startpoint[channel] = cur_element_in_tablice;
        transition_t *temp = malloc(sizeof(transition_t));
        temp->channel = channel;
        temp->state = header_file.data.initial_state;
        temp->time = header_file.data.begin_time;
        cur_element_in_tablice->value = temp;
        cur_element_in_tablice->next = malloc(sizeof(list_t));
        cur_element_in_tablice = cur_element_in_tablice->next;
        sizetablice++;

        unsigned char state = !(header_file.data.initial_state);
        elements[channel] = header_file.data.num_transitions + 1;
        for (size_t i = 0; i < header_file.data.num_transitions; i++)
        {
            fread(&data, sizeof(double), 1, fptr);
            transition_t *temp = malloc(sizeof(transition_t));
            temp->channel = channel;
            temp->state = state;
            temp->time = data;
            cur_element_in_tablice->value = temp;
            cur_element_in_tablice->next = malloc(sizeof(list_t));
            cur_element_in_tablice = cur_element_in_tablice->next;
            sizetablice++;
            state = !state;
        }
        fclose(fptr);
    }

    // const double tik = 1.0 / 24000000.0;
    // list_t *cur_element = data_out;

    // for (size_t i = 0; i < sizetablice; i++)
    // {
    //     transition_t temp = (*((transition_t *)(cur_element->value)));
    //     printf("%.5f|%d|%d\n", (temp.time) * (1000000), temp.state, temp.channel);
    //     cur_element = cur_element->next;
    // }

    typedef struct merged_transition
    {
        double time;
        unsigned char state[8];
    } merged_transition_t;
    // wskaźniki bieżącej pozycji w listach kanałów
    list_t *cur_ptr[8];
    for (int ch = 0; ch < 8; ch++)
    {
        cur_ptr[ch] = startpoint[ch];
    }

    // inicjalizacja stanu kanałów - zakładamy, że pierwszy element w liście to aktualny stan na początku
    unsigned char current_state[8];
    for (int ch = 0; ch < 8; ch++)
    {
        if (elements[ch] > 0)
        {
            transition_t *t = (transition_t *)cur_ptr[ch]->value;
            current_state[ch] = t->state;
        }
        else
        {
            current_state[ch] = 0; // lub inna wartość domyślna
        }
    }

    // tworzymy nową listę wynikową
    list_t *merged_head = malloc(sizeof(list_t));
    list_t *merged_cur = merged_head;
    size_t merged_count = 0;

    // dopóki są jeszcze zdarzenia
    while (1)
    {
        double min_time = 0;
        int min_channel = -1;

        // znajdź najbliższe zdarzenie w czasie spośród wszystkich kanałów
        for (int ch = 0; ch < 8; ch++)
        {
            if (elements[ch] > 0 && cur_ptr[ch] != NULL)
            {
                transition_t *t = (transition_t *)cur_ptr[ch]->value;
                if (min_channel == -1 || t->time < min_time)
                {
                    min_time = t->time;
                    min_channel = ch;
                }
            }
        }

        // brak kolejnych zdarzeń
        if (min_channel == -1)
            break;

        // zaktualizuj stan dla kanału
        transition_t *event = (transition_t *)cur_ptr[min_channel]->value;
        current_state[min_channel] = event->state;

        // dodaj rekord do listy wynikowej
        merged_transition_t *mt = malloc(sizeof(merged_transition_t));
        mt->time = min_time;
        for (int ch = 0; ch < 8; ch++)
            mt->state[ch] = current_state[ch];

        merged_cur->value = mt;
        merged_cur->next = malloc(sizeof(list_t));
        merged_cur = merged_cur->next;
        merged_count++;

        // przejdź do kolejnego zdarzenia w tym kanale
        cur_ptr[min_channel] = cur_ptr[min_channel]->next;
        elements[min_channel]--;
    }
    for (size_t i = 0; i < (sizetablice + 1); i++)
    {
        list_t *next = data_out->next;
        free(data_out->value);
        free(data_out);
        data_out = next; // na końcu przypisanie
    }
    sizetablice = 0;

    // zapis do pliku
    FILE *fp = fopen("output.txt", "w");
    if (!fp)
    {
        perror("output.txt");
        return 1;
    }

    merged_cur = merged_head;
    for (size_t i = 0; i < merged_count; i++)
    {
        merged_transition_t *mt = (merged_transition_t *)merged_cur->value;
        fprintf(fp, "%.5f", mt->time * 1000000); // mikrosekundy
        for (int ch = 0; ch < 8; ch++)
        {
            fprintf(fp, "|%d", mt->state[ch]);
        }
        fprintf(fp, "\n");
        merged_cur = merged_cur->next;
    }

    fclose(fp);
    // // wypisanie nowej listy
    // merged_cur = merged_head;
    // for (size_t i = 0; i < merged_count; i++)
    // {
    //     merged_transition_t *mt = (merged_transition_t *)merged_cur->value;
    //     printf("%.5f", mt->time * 1000000); // w mikrosekundach
    //     for (int ch = 0; ch < 8; ch++)
    //     {
    //         printf("|%d", mt->state[ch]);
    //     }
    //     printf("\n");
    //     merged_cur = merged_cur->next;
    // }
    system("pause");
}