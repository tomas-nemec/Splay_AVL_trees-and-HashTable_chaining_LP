/// zdroj: http://enthusiaststudent.blogspot.com/2017/03/hashing-using-linear-probing-c-program.html

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"



int found = 0;

LPnode* h[TABLE_SIZE] = {NULL};

void insertHashLP(char* input,int a)    //funkcia na vlozenie prvku do tabulky
{

    int index,i,hkey;

    hkey=hash(input,a)%TABLE_SIZE;      //vypocitam si hash pre dany prvok
    for(i=0;i<TABLE_SIZE;i++)           //iteracia pre prechadzanie tabulky
    {

        index=(hkey+i)%TABLE_SIZE;      //pohyb v poli a hladanie volneho miesta

        if(h[index] == NULL)        // ak je policko an danom indexe prazdne, vozim tam prvok
        {
            h[index] = (LPnode*)malloc(sizeof(LPnode));
            h[index]->prichut = strdup(input);
            h[index]->cena = a;
            break;
        }
        else if((strcmp(h[index]->prichut,input) == 0) && h[index]->cena == a){ //uz sa tam nachadza totozny prvok
            break;  //already in
        }

    }

    if(i == TABLE_SIZE)     //v pripade ze sa vklada viac prvkov ako sa zmesti do tabulky
        printf("\nelement cannot be inserted\n");
}
int searchHashLP(char* searched, int searchedINT) {

    int index, i, hkey;

    hkey = hash(searched,searchedINT) % TABLE_SIZE;     //vypocita sa hash pre prvok a zacne sa prehladavat tabulka
    for (i = 0; i < TABLE_SIZE; i++) {                  //iteracia pre prechadzanie tabulky
        index = (hkey + i) % TABLE_SIZE;
        if ((strcmp(h[index]->prichut, searched) == 0) && h[index]->cena == searchedINT ) {     //ak sa najde zhodny prvok tak return 1
            return 1;
        }
    }
    if (i == TABLE_SIZE)
        return 0;
}



void testovacHashLinearProbing (int pocetINPUTOV, int castINPUTOV)
{
    LPnode *inputs = (LPnode*)malloc(sizeof(LPnode) * pocetINPUTOV);


    printf("\n\tHash Table - Linear Probing:");
    clock_t begin1 = clock();

    for(int i = 0;i<pocetINPUTOV;i++)
    {
        inputs[i].prichut = strdup(stringGenerator());
        inputs[i].cena = intGenerator();
        insertHashLP(inputs[i].prichut,inputs[i].cena);
    }
    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;

    printf("\nINSERT: execution time: %Lf",time_spent1);

    //  TEST2 - SEARCH ALL
    clock_t begin2 = clock();
    for(int i = 0;i<pocetINPUTOV;i++)
    {
        found += searchHashLP(inputs[i].prichut,inputs[i].cena);

    }
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("\nSearch(all INSERTED): execution time: %Lf",time_spent2);



    //  TEST 2 - SEARCH specific numb
    clock_t begin3 = clock();
    for(int i = 0;i<castINPUTOV;i++)
    {
        found += searchHashLP(inputs[i].prichut,inputs[i].cena);
    }
    clock_t end3 = clock();
    double time_spent3 = (double)(end3 - begin3) / CLOCKS_PER_SEC;
    printf("\nSearch(First %d INSERTED): execution time: %Lf",castINPUTOV,time_spent3);


    //  TEST 3
    clock_t begin4 = clock();

    for(int i = pocetINPUTOV-1;i >= pocetINPUTOV-castINPUTOV;i--)
    {
        found += searchHashLP(inputs[i].prichut,inputs[i].cena);
    }

    clock_t end4 = clock();
    double time_spent4 = (double)(end4 - begin4) / CLOCKS_PER_SEC;
    printf("\nSearch(Last %d INSERTED): execution time: %Lf",castINPUTOV,time_spent4);

    //  TESt 4
    int j;
    clock_t begin5 = clock();

    for(int i = 0;i < castINPUTOV;i++)
    {
        j = rand() % pocetINPUTOV;
        found += searchHashLP(inputs[j].prichut,inputs[j].cena);
    }

    clock_t end5 = clock();
    double time_spent5 = (double)(end5 - begin5) / CLOCKS_PER_SEC;
    printf("\nSearch( %d random ): execution time: %Lf",castINPUTOV,time_spent5);


    printf("\n//////////////////////////////////////////");

}




