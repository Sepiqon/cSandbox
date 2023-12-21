typedef unsigned char BOOLEAN;
/* Konwertuje liczbe na ciąg znaków */
char *const numToString(double *num);
/* Wyciag cyfre z liczby calkowitej zaczynajac od 1 */
char getCyfraFromNum(long long int a, int id);
/* Konvertuje cyfre to znaku */
char cyfrToChar(char c);
/* Wyciaga liczbe pozycji przed przecinkiem -> np. 999,9 -> 3*/
char przedPrzecinkiemVoid(double *num);
/* Wyciaga liczby pozycji po przecinku np. 9,999 -> 3 */
char poPrzecinkuVoid(double *num);
/* Liczba bezwgledna */
double bezw(double num);
/* Naprawia niedoskonałości liczb zmienno przecinkowych */
double skoryguj(double num);