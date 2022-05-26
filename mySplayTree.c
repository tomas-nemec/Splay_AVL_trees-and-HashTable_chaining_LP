#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

int pocetRotacii = 0;

//pomocne funkcie pre generovanie stringov a int pre vstupy

char *stringGenerator()         //zdroj: https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
{
    char* prichut;
    char abeceda[]="qwertzuiopasdfghjklyxcvbnmQWERTZUIOPASDFGHJKLYXCVBNM";
    int stringSIZE = 1 + (rand() % (MAXstr-1));

    prichut = (char *)malloc(sizeof(char)*MAXstr);

    for(int i = 0;i<stringSIZE;i++)   //stringSIZE
    {
        int a = rand() % (int) (sizeof(abeceda)-1);

        prichut[i] = abeceda[a];

    }
    prichut[stringSIZE] = '\0';

    return prichut;
}

int intGenerator(){
    int a = rand() % 100  ;
    return a;

}

//////////////////////////////////////////////////


STnode *createSTnode(int price,char* taste, STnode *rodic)       //vytvori mi novy node v strome ak je potreba
{
    STnode *new = (STnode*)malloc(sizeof(STnode));
    new->cena = price;
    new->prichut = strdup(taste);
    new->right = NULL;
    new->left = NULL;
    new->parent = rodic;
    return new;
}

STnode *rightRotacia(STnode *akt)
{
    STnode *a = akt->left;      //pomocny ukazovatel ktory si udrzi left_childa ktory budeme nastavovat ako vrchol
    akt->left = a->right;       //odlozenie right_childa "a"  pretoze po rotacii bude "a" na vrchu akt bude jej right child a predosly right_child sa stane LEFT_childom akt
    a->right = akt;             //priamo nastavenia uz AKT ako pravy child a "a" je na vrchole

    if(akt->parent != NULL)     //ak to nie je root celeho stromu
    {
        if(akt->parent->left == akt)            //ak AKT bol left child parenta, musim nastavit tento pointer aby ukazoval na novy vrchol "a"
        {
            akt->parent->left = a;
        }
        else
        {
            akt->parent->right = a;             //to iste ale ak bolo AKT right child jeho parenta
        }
    }

    a->parent = akt->parent;    //nastavim uz ako vrh pyramidy
    akt->parent = a;

    if(akt->left != NULL)
    {
        akt->left->parent = akt;
    }
    pocetRotacii++;
    return a;
}

STnode *leftRotacia(STnode *akt)
{
    STnode *a = akt->right;     //pomocny ukazovatel ktory si udrzi right_childa ktory budeme nastavovat ako vrchol
    akt->right = a->left;       //odlozenie left_childa "a"  pretoze po rotacii bude "a" na vrchu akt bude jej left child a predosly left_child sa stane right_childom akt
    a->left = akt;              //priamo nastavenia uz AKT ako pravy child a "a" je na vrchole

    if(akt->parent != NULL)     //nie je to root
    {
        if(akt->parent->left == akt)        //ak AKT bol left child parenta, musim nastavit tento pointer aby ukazoval na novy vrchol "a"
        {
            akt->parent->left = a;
        }
        else
        {
            akt->parent->right = a;
        }
    }

    a->parent = akt->parent;    //nastavim uz ako vrh pyramidy
    akt->parent = a;

    if(akt->right != NULL)
    {
        akt->right->parent = akt;
    }

    pocetRotacii++;
    return a;
}

STnode *splay(STnode *root, STnode *vloz) //splay = urob INSERTED novym korenom stromu
{
    if(strcmp(root->prichut,vloz->prichut) == 0 && root->cena == vloz->cena)       //searched alebo inserted je rootom
    {
        return root;
    }

    while(vloz->parent != NULL)
    {
        if(vloz->parent == root)        //child rootu, potrebna iba jedna rotacia
        {
            if(vloz == vloz->parent->left)
            {
                rightRotacia(vloz->parent);
            }
            else
            {
                leftRotacia(vloz->parent);
            }
        }
        else                //je grand child a nizsie
        {
            STnode *p = vloz->parent;       //nastavim si parenta a grand parenta nodu kotry idem posuvat na vrch
            STnode  *g = p->parent;
           if(vloz == vloz->parent->left && p == p->parent->left)   //2*RightRotation
           {
               rightRotacia(g);
               rightRotacia(p);
           }
           else if(vloz == vloz->parent->right && p == p->parent->right)   //2*LeftRotation
           {
                leftRotacia(g);
                leftRotacia(p);
           }
           else if(vloz == vloz->parent->right && p == p->parent->left)   //2*LeftRotation
           {
               leftRotacia(p);
               rightRotacia(g);
           }
           else if(vloz == vloz->parent->left && p == p->parent->right)   //2*LeftRotation
           {
               rightRotacia(p);
               leftRotacia(g);
           }
        }
    }

    return vloz;
}

STnode *insertST(STnode *root, char* taste, int price)
{
    STnode *akt = root;
    STnode *parent = NULL;

    STnode *vloz = createSTnode(price,taste,NULL);


    while(akt != NULL)      //dostanem sa na miesto kde vkladam
    {
        parent = akt;
        if(strcmp(vloz->prichut,akt->prichut) < 0)
        {
            akt = akt->left;
        }
        else if(strcmp(vloz->prichut,akt->prichut) > 0)
        {
            akt = akt->right;
        }
        else{   //zhoduju sa mena, este skontrolujem cenu
            if(akt->cena == vloz->cena)
            {
                return root;        //already in
            }
            else if(akt->cena < vloz->cena)         //ak sa nezhoduju aj v cene tak  idem niezsie v strome- right alebo left podla ceny
            {
                akt = akt->right;
            }
            else
            {
                akt = akt->left;
            }

        }
    }
    if(parent == NULL)      //ak nemame nic v strome, tak vrat vloz ako root stromu
    {
        return vloz;
    }

    // tu si nastavim este parenta, aby som sa vedel hybat obomi smermi v strome
    else if(strcmp(vloz->prichut,parent->prichut) < 0 || ((strcmp(vloz->prichut,parent->prichut) == 0) && vloz->cena < parent->cena))
    {
        parent->left = vloz;
        vloz->parent = parent;
    }

    else
    {
        parent->right = vloz;
        vloz->parent = parent;
    }


    //TU UZ MAM KOREKTNY BINARNY STROM
    //volam SPLAY funckiu

    root = splay(root,vloz);

    return root;


}

STnode *searchST(STnode *root, char *searched, int searchedINT)
{
    STnode *akt = root;
    while(akt != NULL)
    {
        if(strcmp(akt->prichut,searched) == 0 )
        {
            if(akt->cena == searchedINT){
                root = splay(root,akt);     //nasiel som prvok, volam funkciu splay aby som dostal sarched node na vrchol stromu
                return root;
            }
            else if(akt->cena < searchedINT)        //ak nesedi aj searchedINT tak idem nizsie v strome
            {
                akt = akt->right;
            }
            else{
                akt = akt->left;
            }

        }

        else if(strcmp(akt->prichut,searched) > 0)          //porovnavam a idem bud do praveho_childu alebo laveho
            akt = akt->left;
        else if(strcmp(akt->prichut,searched) < 0)
            akt = akt->right;

    }

    return root;
}



/***                TESTOVACIA FAZA     */




void testovacST(STnode *root, int pocetINPUTOV, int castINPUTOV)
{
    //pole inputov
    STnode *inputs = (STnode*)malloc(sizeof(STnode) * pocetINPUTOV);


    //  TEST insert pocetINPUTOV(int) prvkov do stromu
    printf("\n\tSPLAY TREE:");
    clock_t begin1 = clock();

    for(int i =0;i<pocetINPUTOV;i++)
    {

        inputs[i].cena = intGenerator();
        inputs[i].prichut = strdup(stringGenerator());
        root = insertST(root,inputs[i].prichut,inputs[i].cena);    //,inputs[i].key

    }

    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;

    printf("\nINSERT: execution time: %Lf",time_spent1);
    int pocetRotPredSearchom = pocetRotacii;





    //     TEST search poslednych castINPUTOV(int) prvkov inserted do stromu
    clock_t begin3 = clock();

    for(int i = pocetINPUTOV-1;i>=pocetINPUTOV-castINPUTOV;i--)
    {
        root = searchST(root,inputs[i].prichut,inputs[i].cena);
    }


    clock_t end3 = clock();
    double time_spent3 = (double)(end3 - begin3) / CLOCKS_PER_SEC;
    printf("\nSearch(last 50 000 INSERTED): execution time: %Lf",time_spent3);



    //     TEST search prvych castINPUTOV(int) prvkov inserted do stromu
    clock_t begin4 = clock();

    for(int i = 0;i<castINPUTOV;i++)
    {
        root = searchST(root,inputs[i].prichut,inputs[i].cena);
    }


    clock_t end4 = clock();
    double time_spent4 = (double)(end4 - begin4) / CLOCKS_PER_SEC;
    printf("\nSearch(first 50 000 INSERTED): execution time: %Lf",time_spent4);


    clock_t begin2 = clock();

    for(int i = 0;i<pocetINPUTOV;i++)
    {
        root = searchST(root,inputs[i].prichut,inputs[i].cena);
    }


    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("\nSearch(all INSERTED): execution time: %Lf",time_spent2);





    printf("\nRotations after INSERT: %d",pocetRotPredSearchom);
    printf("\nTotal rotations: %d",pocetRotacii);




    printf("\n//////////////////////////////////////////");
}




