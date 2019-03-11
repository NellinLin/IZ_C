/*
ИЗ1 Шишова Анастасия АПО-12
Run ID: 1288

Составить программу для перевода чисел из системы счисления по основанию P в систему счисления по основанию Q, 
где 2 меньше или равно Q меньше P меньше или равно 36. 
Значения P, Q и строковое представление S числа в исходной системе счисления вводятся с клавиатуры в следующем
 порядке: 
P Q S 

S не выходит за границы size_t. 

Процедура перевода не может использовать соответствующей функции системной библиотеки и должна быть оформлена 
в виде отдельной функции, на вход которой подаются значения P, Q и S, размещенные в динамической памяти. 
На выход функция должна возвращать строковое представление S1 числа в целевой системе счисления. 

Программа должна уметь обрабатывать ошибки во входных данных. 
В случае возникновения ошибки нужно вывести в поток стандартного вывода сообщение "[error]" (без кавычек) и 
завершить выполнение программы. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERR -1
#define BUFLEN 256
#define BUFLEN2 8
#define N_LETTERS 26
#define DIFFERENCE 10
#define MAX_REMAINDER 9
#define MIN_LIMIT 2
#define MAX_LIMIT 36

// Структура с алфавитом A-Z и их значениями
typedef struct Symbols {
    char CH[N_LETTERS];
    int NUM[N_LETTERS];
    int SIZE;
}symbols_t;

// Методы для структуры Symbols
symbols_t assign_val_to_sym(symbols_t Sym);
int take_num(char ch, symbols_t Sym);
char take_symb(int n, symbols_t Sym);

// Функция считывания строки
char * Read_Long_Str(FILE *f);

// Функция возведения в степень
long long int exponentiation(int* P, int x);

// Перевод из данной системы счисления в 10
long long int Translation_from_SS_to_10_SS(int *Now_SS, char *S, size_t n, symbols_t Sym);

// Функция перевода из 10 СС в другую
char* Translation_10_SS_to_another_SS(int* Q, long long int num, symbols_t Sym);

// Функция перевода из СС P в СС Q
char* Trenslation_from_one_SS_to_another_SS(int* Now_SS, int* Another_SS, char *S);


int main(void){
    char buf1[BUFLEN2],buf2[BUFLEN2];
    int n_error = 0;
    int *P = NULL;
    int *Q = NULL;

    scanf("%s%s",buf1,buf2);
    if(atoi(buf1)==0||atoi(buf2)==0){
        printf("[error]");
        return 0;
    }

    if( !(P = (int*)malloc(1*sizeof(int))) || !(Q = (int*)malloc(1*sizeof(int)))) {
        n_error = ERR;
    }
    else{
        *P = atoi(buf1);
        *Q = atoi(buf2);
    }

     char *S = Read_Long_Str(stdin);

    if ((*Q < MIN_LIMIT) || (*Q > *P) || (*P > MAX_LIMIT) || (S==NULL)) {
        n_error = ERR;
    }

    if(n_error!=ERR) {
        for(int i = 0; S[i] !='\0'; i++){
            if(S[i]<'a' && S[i]>='A'){
                S[i]+='a'-'A';
            }
        }
        char *S1 = Trenslation_from_one_SS_to_another_SS(P,Q,S);

        if (S1==NULL){
            printf("[error]");
        }
        else {
            printf("%s",S1);
            free(S1);
        }
    }
    else {
        printf("[error]");
    }

    free(P);
    free(Q);
    free(S);
    return 0;
}



symbols_t assign_val_to_sym(symbols_t Sym) {
    Sym.SIZE = 'z' - 'a' + 1;
    char ch = 'a';
    for (int i = 0; i < Sym.SIZE; i++) {
        Sym.NUM[i] = i + DIFFERENCE;
        Sym.CH[i] = ch;
        ch++;
    }
    return Sym;
}

int take_num(char ch, symbols_t Sym) {
    int i = 0;
    for (; i < Sym.SIZE; i++) {
        if (Sym.CH[i] == ch) break;
    }
    return Sym.NUM[i];
}

char take_symb(int n, symbols_t Sym) {
    int i = 0;
    for (; i < Sym.SIZE; i++) {
        if (Sym.NUM[i] == n) break;
    }
    return Sym.CH[i];
}

char * Read_Long_Str(FILE *f) {
    char buf[BUFLEN] = "";
    char *resbuf = NULL;
    int curbuflen = 0;

    if (f==NULL) return NULL;
     
    while (fgets(buf,BUFLEN,f)) {
        if ( resbuf ) {
            resbuf = (char*)realloc(resbuf,curbuflen + BUFLEN);
            curbuflen = curbuflen + BUFLEN;
        } else {
            if(!(resbuf = (char*)malloc(BUFLEN))){
                return NULL;
            }
            curbuflen = BUFLEN;
            resbuf[0]=0;
        }
        strcat(resbuf,buf);
        if ( strlen(buf) < BUFLEN-1 ) break;

    }
    for (int i=0; i<(strlen(resbuf)-1);i++){
        resbuf[i]=resbuf[i+1];
    }
    resbuf[strlen(resbuf)-1]='\0';
    return resbuf; 
}

long long int exponentiation(int* P, int x) {
    long long int exp=1;
    for (size_t i = 1; i <= abs(x); i++) {
        exp *= *P;
    }
    return exp;
}

long long int Translation_from_SS_to_10_SS(int *Now_SS, char *S, size_t n, symbols_t Sym) {
    long long int result = 0;
    for (size_t i = 0; i < n; i++) {
        if ((S[i] >= 'a' && S[i] <= 'z') && (take_num(S[i],Sym) < (*Now_SS))) {
            result += take_num(S[i],Sym) * exponentiation(Now_SS, n - i - 1);
        }
        else if ((S[i] >= '0'&&S[i] <= '9') && ((S[i]-'0') < (*Now_SS))) {
            result += ((S[i])-'0') * exponentiation(Now_SS, n - i - 1); // значение цифры равно значению ее символа в таблице символов компьютера минус значение '0'
        }
        else {
            result = ERR;
               break;
        }
    }
    return result;
}

char* Translation_10_SS_to_another_SS(int* Q, long long int num, symbols_t Sym) {
    int max_st = 0;
    char *S1 = NULL;
    while (exponentiation(Q, max_st) <= num) {
        max_st++;
    }
    if(!(S1 = (char*)malloc((max_st+1)*sizeof(char)))){
        return NULL;
    }
    if (num == 0) {
        S1[0] = 0;
    }
    S1[max_st] = '\0';
    for (max_st--; num != 0; max_st--) {
        if (num%(*Q) > MAX_REMAINDER) {
            S1[max_st] = take_symb(num%(*Q), Sym);
        }
        else {
            S1[max_st] = ('0' + (num%(*Q)));
        }
        num = (num / (*Q));
    }
    return S1;
}

char* Trenslation_from_one_SS_to_another_SS(int* Now_SS, int* Another_SS, char *S){
    long long int translation = 0;
    char *S1 = NULL;
    symbols_t Sym;

    size_t n= strlen(S);
    Sym = assign_val_to_sym(Sym);
    translation = Translation_from_SS_to_10_SS(Now_SS, S, n, Sym);
    if(translation != ERR){
        S1 = Translation_10_SS_to_another_SS(Another_SS, translation, Sym);
    }
    else {
        return NULL;
    }

    return S1;
}