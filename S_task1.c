/*
ИЗ1 Шишова Анастасия АПО-12
Run ID: 1196

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

// Структура с алфавитом A-Z и их значениями
struct Symbols {
	char CH[26];
	int NUM[26];
	int SIZE;
};

// Методы для структуры Symbols
struct Symbols fill_s(struct Symbols Sym) {
	Sym.SIZE = 'z' - 'a' + 1;
	char ch = 'a';
	for (int i = 0; i < Sym.SIZE; i++) {
		Sym.NUM[i] = i + 10;
		Sym.CH[i] = ch;
		ch++;
	}
    return Sym;
}

int take_num(char ch, struct Symbols Sym) {
	int i = 0;
	for (; i < Sym.SIZE; i++) {
		if (Sym.CH[i] == ch) break;
	}
	return Sym.NUM[i];
}

char take_symb(int n, struct Symbols Sym) {
	int i = 0;
	for (; i < Sym.SIZE; i++) {
		if (Sym.NUM[i] == n) break;
	}
	return Sym.CH[i];
}

// Функция считывания строки
char * ReadLongStr(FILE *f)
{
    char buf[BUFLEN];
    char *resbuf = 0;
    int curbuflen = 0;

    while (fgets(buf,BUFLEN,f)) {
        if ( resbuf ) {
            resbuf = (char*)realloc(resbuf,curbuflen + BUFLEN);
            curbuflen = curbuflen + BUFLEN;
        } else {
            resbuf = (char*)malloc(BUFLEN);
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

// Функция возведения в степенЬ
long long int exponentiation(int* P, int x) {
	long long int exp=1;
	for (size_t i = 1; i <= x; i++) {
		exp *= *P;
	}
	return exp;
}

// Перевод из данной системы счисления в 10
long long int Now_SS_to_10_SS(int *P, char *S, size_t n, struct Symbols Sym) {
	long long int result = 0;
	for (size_t i = 0; i < n; i++) {
		if ((S[i] >= 'a' && S[i] <= 'z') && (take_num(S[i],Sym) < (*P))) {
			result += take_num(S[i],Sym) * exponentiation(P, n - i - 1);
		}
		else if ((S[i] >= '0'&&S[i] <= '9') && ((S[i]-'0') < (*P))) {
			result += ((S[i])-'0') * exponentiation(P, n - i - 1); // значение цифры равно значению ее символа в таблице символов компьютера минус значение '0'
		}
		else {
            result = ERR;
			break;
		}
	}
	return result;
}

// Функция перевода из 10 СС в другую
char* SS_10_to_another_SS(int* Q, long long int num, struct Symbols Sym) {
	int max_st = 0;
	while (exponentiation(Q, max_st) <= num) {
		max_st++;
	}
	char *S1 = (char*)malloc((max_st+1)*sizeof(char));
	if (num == 0) S1[0] = 0;
	S1[max_st] = '\0';
	for (max_st--; num != 0; max_st--) {
		if (num%(*Q) > 9) {
			S1[max_st] = take_symb(num%(*Q), Sym);
		}
		else {
			S1[max_st] = ('0' + (num%(*Q)));
		}
		num = (num / (*Q));
	}
    return S1;
}

// Функция перевода из СС P в СС Q
char* sys_schisl(int* P, int* Q, char *S){
	long long int perevod = 0;
    char *S1;
    struct Symbols Sym;

    size_t n= strlen(S);
	Sym = fill_s(Sym);
	perevod = Now_SS_to_10_SS(P, S, n, Sym);
    if(perevod != ERR){
	    S1 = SS_10_to_another_SS(Q, perevod, Sym);
    }
    else {
		return NULL;
    }

    return S1;
}

int main(void){
    char buf1[5],buf2[5];

    scanf("%s%s",buf1,buf2);
    if(atoi(buf1)==0||atoi(buf2)==0){
        printf("[error]");
        return 0;
    }

    int* P = (int*)malloc(1*sizeof(int));
    int* Q = (int*)malloc(1*sizeof(int));
    *P = atoi(buf1);
    *Q = atoi(buf2);

	char *S=ReadLongStr(stdin);

	for(int i = 0; S[i] !='\0'; i++){
		if(S[i]<'a' && S[i]>='A'){
			S[i]+='a'-'A';
		}
	}

    if ((*Q < 2) || (*Q > *P) || (*P > 36)){
        printf("[error]");
        free(P);
        free(Q);
        free(S);
        return 0;
    }
    char *S1 = sys_schisl(P,Q,S);
    if (S1==NULL){
        printf("[error]");
    }
    else {
        printf("%s",S1);
		free(S1);
    }

    free(P);
    free(Q);
    free(S);
    return 0;
}
