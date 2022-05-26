#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXsize 100000
#define ABS(N) ((N<0)?(-N):(N))
#define MAXstr 100




int size = 7;
HASHnode **resize(HASHnode **zac);



void nextPrimeNumber()          //najdenie dalsieho prvocisla - pomocnik pre funckiu resize
{
    int pocitadlo;
    for(int i=2*size;i<MAXsize;i++)     //zvacsim o polovicu a najdem hned dalsie prvocislo
    {
        pocitadlo=0;
        for(int j=1;j<=i;j++)
        {
            if(i%j==0)
            {
                pocitadlo++;
            }
        }
        if(pocitadlo==2)        //je delitelne 1kou a sebou
        {
            size = i;
            break;
        }
    }
}


int hash(char *string,int cena)     //hash - polynomialna akumulacia pre urcenie specifickeho cisla pre string
{
    int i, len = strlen(string), h=0;

    for(i=0;i<len;i++)
    {
        h = 41 * h + string[i];
    }

    return ABS(h)+(cena*41);

}



HASHnode **insertMyHash(HASHnode **zac, char* taste, int price)      //, int price
{
    HASHnode **akt = NULL;

    HASHnode *novy = NULL;
    HASHnode *skok = NULL;
    akt = zac;

    int hlbka = 0;
    int index = hash(taste,price)%size;

    if(akt[index] == NULL)      //ak na danom indexe este nie je spajany zoznam, vytvor jeho zaciatok
    {
        akt[index] = malloc(sizeof(HASHnode));
        akt[index]->cena = price;        //price
        akt[index]->prichut = strdup(taste);
        akt[index]->next = NULL;

        return zac;
    }
    else            //ak tam uz zoznam je, prejdi na jeho koniec ale zaroven davaj pozor ci tam nie je uz totozny prvok vlozeny
    {
      skok = akt[index];
      hlbka++;      //pocitam dlzku zoznamu aby som vedel kedy resize, aby nevznikali dlhe spajane zoznamy

      while(skok->next != NULL)
      {
          if(!(strcmp(taste,skok->prichut)) && skok->cena == price)            //skok->cena == price &&
          {
              return zac;       //nerob ziadne zmeny ak sa uz rovanky jogurt nachadza
          }
          hlbka++;
          skok = skok->next;
      }

      if(!(strcmp(taste,skok->prichut)) && skok->cena == price )     //kontrola posledneho prvku v zozname ci sa nerovna inserted
      {
          return zac;
      }
      //ak nie tak vloz na koniec node
      novy = malloc(sizeof(HASHnode));
      novy->cena = price;       //price
      novy->prichut = strdup(taste);
      novy->next = NULL;

      skok->next = novy;
      hlbka++;
    }




    if(hlbka > size/2)      //kontrola pre udrzanie spajanych zoznamov prijatelne dlhych
    {
        zac = resize(zac);
    }


    return zac;



}


int searchMyHash(HASHnode **zac, char* searched, int searchedINT)
{
    int index;

    index = hash(searched,searchedINT)%size;            //vypocitaj index na ktorom by sa mal hladany prvok nachadzat
    HASHnode *akt = zac[index];

    while (akt != NULL)         //prechadzanie spajanaho zoznamu na indexe a hladanie prvku
    {
        if(!(strcmp(searched,akt->prichut)) && akt->cena == searchedINT)
        {
            return 1;
        }
        akt = akt->next;
    }

    return 0;
}


HASHnode **resize(HASHnode **zac)       //funckia pre zvacsenie pola
{
    int oldSize = size;
    nextPrimeNumber();      //zmena pola = nastavi novu velkost

    HASHnode **newTable = (HASHnode **)malloc(sizeof(HASHnode *) * size);   //alokujem si novu tabulku do kotrej budem prehashovavat staru tabulku


    for(int i = 0; i<size;i++)
    {
        newTable[i] = NULL;
    }

    /***    ZACIATOK RESIZE     ***/
    HASHnode *skok = NULL;

   for(int i = 0; i < oldSize;i++)      // prejdem kazde policko a vsetky stare prehashujem do noveho
   {
       if(zac[i] != NULL)           //ak sa nerovna NULL, prejdem cely spajany zoznam a prehashujem
       {
           skok = zac[i];
           while(skok != NULL)
           {
               newTable = insertMyHash(newTable,skok->prichut,skok->cena);
               skok= skok->next;
           }

       }
   }

   //uvolnit staru tabulku
    HASHnode *uvolnovac = NULL;
    for(int i = 0; i < oldSize;i++)      // uvolnenie starej hashovacej tabulky ktoru uz nepotrebujeme
    {
        if(zac[i] == NULL)
        {
           free(zac[i]);
        }
        else
        {
            skok = zac[i];
            while(skok != NULL)
            {
                uvolnovac = skok;
                skok = skok->next;
                free(uvolnovac);
            }
        }
    }

    return newTable;
}


void testovacMyHASH (HASHnode **zac,int pocetINPUTOV,int castINPUTOV)
{
    for(int i=0;i<size;i++) //nastavit na NULL vsetky pointre
    {
        zac[i] = NULL;
    }
    HASHnode *inputs = (HASHnode*)malloc(sizeof(HASHnode) * pocetINPUTOV);

    /*
     * Testovanie insert x prvkov, search vsetkych, search prvych vlozebnych y prvkov, search poslednych vlozenych y prvkov, search y random prvkov
     */


    printf("\n\tHash Table - Chaining:");
    clock_t begin1 = clock();

    for(int i = 0;i<pocetINPUTOV;i++)
    {
        inputs[i].prichut = strdup(stringGenerator());
        inputs[i].cena=intGenerator();
        zac = insertMyHash(zac,inputs[i].prichut,inputs[i].cena);
    }
    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;

    printf("\nINSERT: execution time: %Lf",time_spent1);


    clock_t begin2 = clock();
    for(int i = 0;i<pocetINPUTOV;i++)
    {
        searchMyHash(zac,inputs[i].prichut,inputs[i].cena);
    }
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("\nSearch(all INSERTED): execution time: %Lf",time_spent2);



    //  TEST 2 - SEARCH specific numb
    clock_t begin3 = clock();
    for(int i = 0;i<castINPUTOV;i++)
    {
        searchMyHash(zac,inputs[i].prichut,inputs[i].cena);
    }
    clock_t end3 = clock();
    double time_spent3 = (double)(end3 - begin3) / CLOCKS_PER_SEC;
    printf("\nSearch(First %d INSERTED): execution time: %Lf",castINPUTOV,time_spent3);


    //  TEST 3
    clock_t begin4 = clock();

    for(int i = pocetINPUTOV-1;i >= pocetINPUTOV-castINPUTOV;i--)
    {
        searchMyHash(zac,inputs[i].prichut,inputs[i].cena);
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
        searchMyHash(zac,inputs[j].prichut,inputs[j].cena);

    }

    clock_t end5 = clock();
    double time_spent5 = (double)(end5 - begin5) / CLOCKS_PER_SEC;
    printf("\nSearch( %d random ): execution time: %Lf",castINPUTOV,time_spent5);

    printf("\nSize of Table at the end: %d",size);



    printf("\n//////////////////////////////////////////");
}



