#include <windows.h>
#include <stdio.h>
#include <time.h>

static int monthtab[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

int mkdays_noleap(int year, int month, int day)
{
    return year * 365 + monthtab[month - 1] + day - 1;
}

int mkdays(int year, int month, int day)
{
    return year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + monthtab[month - 1] + day - 1 + ((month > 2 && (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0)) ? 1 : 0);
}

double fractional_day(SYSTEMTIME st)
{
    double totalSeconds = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    return (totalSeconds / 86400) + (double)(st.wMilliseconds / 86400000);
}
int main()
{
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    char portName[] = "\\\\.\\COM16";
    char buffer[1];
    DWORD bytesRead;
    char ones1 = 0;

    FILE *logfile = fopen("log.csv", "a");
    if (!logfile)
    {
        printf("Nie można otworzyć pliku log.csv\n");
        return 1;
    }

    hSerial = CreateFileA(
        portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        printf("Nie można otworzyć portu COM16\n");
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        printf("Błąd GetCommState\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Błąd SetCommState\n");
        CloseHandle(hSerial);
        return 1;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Błąd SetCommTimeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    printf("Oczekiwanie na dane z COM16...\n");

    while (1)
    {
        if (!ones1)
        {
            SYSTEMTIME st;
            GetLocalTime(&st);
            int today = mkdays(st.wYear, st.wMonth, st.wDay);
            int kiedys = mkdays(1900, 1, 0);

            int datePart = today - kiedys + 1;
            double timePart = fractional_day(st) + datePart;
            char floatStr[64];
            snprintf(floatStr, sizeof(floatStr), "%.10f", timePart);
            for (int i = 0; floatStr[i]; ++i)
            {
                if (floatStr[i] == '.')
                {
                    floatStr[i] = ',';
                    break;
                }
            }

            fprintf(logfile, "%s;", floatStr);
            fflush(logfile);
            ones1 = 1;
        }

        int received = 0;

        while (ReadFile(hSerial, buffer, 1, &bytesRead, NULL) && bytesRead > 0)
        {
            if (buffer[0] == '.')
                buffer[0] = ',';
            printf("%c", buffer[0]);
            fflush(stdout); // wypisuj natychmiast
            fprintf(logfile, "%c", buffer[0]);
            fflush(logfile);
            received = 1;
        }

        if (received)
        {
            fprintf(logfile, "%s", "\n");
            fflush(logfile);
            printf("\n");
            fflush(stdout);
            ones1 = 0;
        }
    }

    CloseHandle(hSerial);
    return 0;
}