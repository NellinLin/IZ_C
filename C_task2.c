/*
ИЗ2 Шишова Анастасия АПО-12

B-7: калькулятор векторов

Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над векторами. 
На стандартный поток ввода программы подается входное выражение, а результат вычислений программа должна вывести на стандартный поток вывода. 
Результат вычислений - это вектор. 

Вектор - это набор координат (разложение через базисные векторы), перечисленный через запятую и заключенный в фигурные скобки. Размерность произвольная, но минимум 2. 
Например, 
{1, 1}, {1, 2, 3}, {1, 2, 3, 4, 5, 99999} - векторы, 
{1}, (1, 2), {1, 2,3] - не векторы. 

Поддерживаемые операции: '+' - сложение, '-' - вычитание, '*' - умножение, '()' - задание приоритета вычислений. 
Складывать и вычитать можно только векторы! Иначе необходимо вывести "[error]" (без кавычек). 
Примеры: 
{1, 1, 1} + {2,2,2}, ответ - {3, 3, 3} 
{1, 2, 3} + 2: ошибка, необходимо вывести "[error]" 
4 + 6: ошибка, необходимо вывести "[error]" 

Складывать и вычитать можно векторы разных размерностей! При этом соответствующие координаты вектора меньшей размерности полагаются равными нулю. 
Например, 
{1, 1} + {2,2,2}, ответ - {3, 3, 2} 

Перемножать между собой можно только вектора и числа (в любом порядке)! Иначе необходимо вывести "[error]". 
Примеры: 
{1, 2, 3} * 3 ответ - {3, 6, 9} 
{1, 2, 3} * {1, 2, 3} или 4 * 9: ошибка, необходимо вывести "[error]" 
{1, 2, 3} * a - также ошибка. 
Произведение вектора на ноль равно вектору той же размерности, у которого все координаты - нули. 

Между операциями, векторами и внутри векторов между координатами может быть произвольное количество пробелов - их необходимо пропускать. 
Например, 
3 * {1, 1,         1}      * 2 - 3 *( {1,1} +       {1,1} ) - валидно, результат {0, 0, 6}

Однако, при выводе ответа пробелы не пишутся! То есть 
{0, 0, 6} - не правильно 
{0,0,6} - правильно! 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERR -1
#define BUFLEN 256
#define NUM_TOP 1
#define NUM_BEFORE 2
//#pragma warning(disable : 4996)

typedef struct Element {
    int *Value;
    int Num;
}element_t;

// Функции для работы со структурой
void Show_answer(element_t *Elem, int top);
int Add_elem(element_t *Elem, int top, char* S);


// Считывание с консоли строки
char * Read_long_str(FILE *f);

// Функция считывания строки и удаление пробелов
char* Getline_wthot_pr();

// Функция проверки введённой строки
int Check_str(char* S);

// Обратная польская запись
char* Polskaya(char* S);

// Подсчёт польской записи
int Answer(char* S);

// Функция калькулятор
int Calculate_vector();


int main(void) {
    Calculate_vector();
    return 0;
}


// Основная функция калькулятор
int Calculate_vector() {
    char *S = Getline_wthot_pr();

    if (S == NULL) {
        printf("[error]");
    }
    else {
        if (Check_str(S) == ERR) {
            printf("[error]");
        }
        else {
            char* Pol_zap = Polskaya(S);

            if (Answer(Pol_zap) == ERR) {
                printf("[error]");
            }
            free(Pol_zap);
        }
    }
    free(S);
    return 0;
}

// Вывести ответ
void Show_answer(element_t *Elem, int top) {
    top--;
    printf("{");
    for (int i = 0; i < Elem[top].Num - 1; i++)
        printf("%d,", Elem[top].Value[i]);
    printf("%d}", Elem[top].Value[Elem[top].Num - 1]);
}

// Добавление элемента
int Add_elem(element_t *Elem, int top, char* S) {
    int num = 0, count_of_elem = 0;

    if (S[0] != '{') {
        num = atoi(S);
        count_of_elem = 1;
        if (!(Elem[top].Value = (int*)malloc(count_of_elem * sizeof(int))))
            return ERR;
        Elem[top].Value[0] = num;
        Elem[top].Num = count_of_elem;
    }
    else {
        for (int i = 0; i<strlen(S); i++) {
            if (S[i] == ',') count_of_elem++;
        }
        count_of_elem++;

        if (!(Elem[top].Value = (int*)malloc(count_of_elem * sizeof(int))))
            return ERR;
        Elem[top].Num = count_of_elem;
        count_of_elem = 0;

        for (int i = 1; i < strlen(S) - 1; i++) {
            int index = i, k = 0;
            char *buffer = NULL;

            while (S[index] != ','&& index != strlen(S) - 1) {
                index++;
                k++;
            }
            
            if (!(buffer = (char*)malloc((k + 1) * sizeof(char))))
                return ERR;

            index = 0;
            for (int j = i; j<i + k; j++) {
                buffer[index] = S[j];
                index++;
            }
            buffer[index] = '\0';
            Elem[top].Value[count_of_elem] = atoi(buffer);
            free(buffer);
            count_of_elem++;
            i += k;
        }
    }
    return 0;
}

// Считывание с консоли строки
char * Read_long_str(FILE *f) {
    char buf[BUFLEN] = "";
    char *resbuf = NULL;
    int curbuflen = 0;

    if (f == NULL) return NULL;

    while (fgets(buf, BUFLEN, f)) {
        if (resbuf) {
            resbuf = (char*)realloc(resbuf, curbuflen + BUFLEN);
            curbuflen = curbuflen + BUFLEN;
        }
        else {
            if (!(resbuf = (char*)malloc(BUFLEN))) {
                return NULL;
            }
            curbuflen = BUFLEN;
            resbuf[0] = 0;
        }
        strcat(resbuf, buf);
        if (strlen(buf) < BUFLEN - 1) break;

    }
    resbuf[strlen(resbuf)] = '\0';
    return resbuf;
}

// Функция считывания с консоли
char* Getline_wthot_pr() {
    char *S = Read_long_str(stdin);
    char *N_S = NULL;

    if (!(N_S = (char*)realloc(N_S, (strlen(S) + 1)))) {
        return NULL;
    }

    int n = 0;
    for (int i = 0; i<strlen(S); i++) {
        if (S[i] != ' ') {
            N_S[n] = S[i];
            n++;
        }
    }
    N_S[n] = '\0';

    free(S);
    S = N_S;

    return S;
}

// Функция проверки введённой строки
int Check_str(char* S) {
    int len = strlen(S);
    if (len == 0) return ERR;

    // Проверка на символы
    for (int i = 0; i < len; i++) {
        if (!((S[i] >= '0'&& S[i] <= '9') || (S[i] >= '('&& S[i] <= '-') || (S[i] == '{') || (S[i] == '}'))) {
            return ERR;
        }
    }

    // Проверка на правильное кол-во скобок
    int parenthesis = 0;
    for (int i = 0; i < len; i++) {
        if (parenthesis<0) return ERR;
        if (S[i] == '(') parenthesis++;
        else if (S[i] == ')') parenthesis--;
    }
    if (parenthesis != 0) return ERR;

    // Провека некольких знаков подрят ++ (+ | [0] И [strlen-1]
    for (int i = 0; i < len - 1; i++) {
        if (S[i] == '+' || S[i] == '-' || S[i] == '*') {
            if (S[i + 1] == '+' || S[i + 1] == '-' || S[i + 1] == '*') {
                return ERR;
            }
        }
        else if (S[i] == '(') {
            if (S[i + 1] == '+' || S[i + 1] == '*') {
                return ERR;
            }
        }
        else if ((S[i] == '{'&&S[i + 1] == ',') || (S[i] == ','&&S[i + 1] == '}')) return ERR;
        else if (S[i] == '}'&&S[i + 1] == '{') return ERR;
        if (i != 0)
            if (S[i - 1] == '}'&&S[i] == '*'&&S[i + 1] == '{') return ERR;
    }
    if (S[0] == '+' || S[0] == '*') return ERR; // S[0] может быть -
    if (S[len - 1] == '+' || S[len - 1] == '-' || S[len - 1] == '*') return ERR;

    // Проверка  +)
    for (int i = 0; i < len; i++) {
        if (S[i] == ')' && i != 0) {
            if (S[i - 1] == '+' || S[i - 1] == '-' || S[i - 1] == '*') return ERR;
        }
    }

    // Проверка для вектора, наличие внутри верных знаков, невозможность написать {-2-2,2},больше одного значения
    parenthesis = 0;
    int point_minus = 0;
    int kol_num = 0;
    for (int i = 0; i<len; i++) {
        if (S[i] == '{') {
            if (parenthesis > 0) return ERR;
            else {
                parenthesis++;
                kol_num = 0;
            }
        }
        else if (S[i] == '}') {
            if (parenthesis > 0 && kol_num>0) parenthesis--;
            else return ERR;
        }
        else if (parenthesis > 0) {
            if (!((S[i] >= '0'&& S[i] <= '9') || (S[i] == ',') || (S[i] == '-'))) {
                return ERR;
            }
            if (S[i] == '-') {
                if (point_minus>0 || (S[i - 1] >= '0' && S[i - 1] <= '9')) return ERR;
                else point_minus++;
            }
            if (S[i] == ',') {
                if (point_minus != 0) point_minus--;
                kol_num++;
            }
        }
        else if (S[i] == ',') return ERR;
    }
    if (parenthesis > 0) return ERR;
    return 0;
}

// Обратная польская запись
char* Polskaya(char* S) {
    char* pol_zap = NULL;
    char* znak = NULL;

    if (!(pol_zap = (char*)malloc((strlen(S) * 2 + 1) * sizeof(char))))
        return NULL;
    if (!(znak = (char*)malloc((strlen(S) + 1) * sizeof(char))))
        return NULL;
   
    int pol_n = 0, znak_n = 0;
    int parenthesis = 0, vect_sk = 0;

    for (int i = 0; i<strlen(S); i++) {
        if (S[i] >= '('&& S[i] <= '-' && vect_sk == 0) {
            if ((S[i] == '*' || S[i] == '+' || S[i] == '-') && i != 0) {
                pol_zap[pol_n] = ' ';
                pol_n++;
            }
            if (S[i] == '+' || S[i] == '-') {
                if (znak_n != 0) {
                    if (znak[znak_n - 1] == '+' || znak[znak_n - 1] == '-' || znak[znak_n - 1] == '*') {
                        pol_zap[pol_n] = znak[znak_n - 1];
                        znak[znak_n - 1] = S[i];
                        pol_n++;
                        pol_zap[pol_n] = ' ';
                        pol_n++;
                    }
                    else {
                        znak[znak_n] = S[i];
                        znak_n++;
                    }
                }
                else {
                    znak[znak_n] = S[i];
                    znak_n++;
                }
            }
            if (S[i] == '(') {
                znak[znak_n] = S[i];
                znak_n++;
                parenthesis++;
            }
            if (S[i] == '*') {
                if (znak_n != 0) {
                    if (znak[znak_n - 1] == '*') {
                        pol_zap[pol_n] = znak[znak_n - 1];
                        znak[znak_n - 1] = S[i];
                        pol_n++;
                    }
                    else {
                        znak[znak_n] = S[i];
                        znak_n++;
                    }
                }
                else {
                    znak[znak_n] = S[i];
                    znak_n++;
                }
            }
            if (S[i] == ')' && parenthesis>0) {
                znak_n--;
                while (znak[znak_n] != '(') {
                    pol_zap[pol_n] = ' ';
                    pol_n++;
                    pol_zap[pol_n] = znak[znak_n];
                    znak_n--;
                    pol_n++;
                }
                parenthesis--;
            }
        }
        else if (S[i] == '{') {
            vect_sk++;
            pol_zap[pol_n] = S[i];
            pol_n++;
        }
        else if (S[i] == '}') {
            vect_sk--;
            pol_zap[pol_n] = S[i];
            pol_n++;
        }
        else {
            pol_zap[pol_n] = S[i];
            pol_n++;
        }
    }

    if (znak[0] != '(') {
        for (int i = znak_n - 1; i >= 0; i--) {
            pol_zap[pol_n] = ' ';
            pol_n++;
            pol_zap[pol_n] = znak[i];
            pol_n++;
        }
    }

    free(znak);
    pol_zap[pol_n] = '\0';
    return pol_zap;
}

// Подсчёт польской записи
int Answer(char* S) {
    if (S == NULL) {
        return ERR;
    }

    int len = strlen(S);
    int k = 0, count = 0;
    element_t *Buf_elements;

    for (int i = 0; i<len; i++) {
        if (S[i] == ' ') count++;
    }

    Buf_elements = (element_t*)malloc(count * sizeof(element_t));
    int top = 0, now_elem = 0, prev_elem = 0;

    for (int i = 0; i < len; i++) {
        if (S[i] != '+'&&S[i] != '-'&&S[i] != '*'&&S[i] != ' ') {
            int index = i;
            char *buffer = NULL;
            k = 0;

            while (S[index] != ' '&& index != len) {
                index++;
                k++;
            }

            buffer = (char*)malloc((k + 1) * sizeof(char));

            index = 0;
            for (int j = i; j < i + k; j++) {
                buffer[index] = S[j];
                index++;
            }
            buffer[index] = '\0';

            if (Add_elem(Buf_elements, top, buffer) == ERR)
                return ERR;

            free(buffer);
            i += k;
            top++;
        }
        else if (S[i] != ' ') {
            now_elem = top - 1;
            prev_elem = top - 2;

            if (Buf_elements[now_elem].Num == NUM_TOP && Buf_elements[prev_elem].Num == NUM_TOP) {
                return ERR;
            }
            if (S[i] == '+' || S[i] == '-') {
                if (Buf_elements[now_elem].Num == NUM_TOP || Buf_elements[prev_elem].Num == NUM_TOP) {
                    return ERR;
                }

                if (Buf_elements[prev_elem].Num < Buf_elements[now_elem].Num) {

                    int *buffer = NULL;
                    if (!(buffer = (int*)calloc((Buf_elements[now_elem].Num), sizeof(int))))
                        return ERR;
                    for (int i = 0; i < Buf_elements[prev_elem].Num; i++) {
                        buffer[i] = Buf_elements[prev_elem].Value[i];
                    }

                    free(Buf_elements[prev_elem].Value);

                    Buf_elements[prev_elem].Value = buffer;
                    Buf_elements[prev_elem].Num = Buf_elements[now_elem].Num;
                }
                if (S[i] == '+') {
                    for (int i = 0; i < Buf_elements[now_elem].Num; i++)
                        Buf_elements[prev_elem].Value[i] += Buf_elements[now_elem].Value[i];
                }
                else {
                    for (int i = 0; i < Buf_elements[now_elem].Num; i++)
                        Buf_elements[prev_elem].Value[i] -= Buf_elements[now_elem].Value[i];
                }
            }
            else if (S[i] == '*') {
                if (Buf_elements[now_elem].Num == 1) {
                    for (int i = 0; i < Buf_elements[prev_elem].Num; i++)
                        Buf_elements[prev_elem].Value[i] *= Buf_elements[now_elem].Value[0];
                }
                else {
                    for (int i = 0; i < Buf_elements[now_elem].Num; i++)
                        Buf_elements[now_elem].Value[i] *= Buf_elements[prev_elem].Value[0];

                    Buf_elements[prev_elem].Value = (int*)realloc(Buf_elements[prev_elem].Value, (Buf_elements[now_elem].Num) * sizeof(int));

                    for (int i = 0; i < Buf_elements[now_elem].Num; i++) {
                        Buf_elements[prev_elem].Value[i] = Buf_elements[now_elem].Value[i];
                    }
                    Buf_elements[prev_elem].Num = Buf_elements[now_elem].Num;
                }
            }
            free(Buf_elements[now_elem].Value);
            top--;
        }
    }

    Show_answer(Buf_elements, top);

    free(Buf_elements[top - 1].Value);
    free(Buf_elements);
    return 0;
}