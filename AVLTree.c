//  zdroj: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
// An AVL tree node
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "header.h"
#define MAXstr 100

int pocetRotaciiAVL = 0;





// pomocna funkcia pre ziskanie vacsieho z dvoch prvkov
int max(int a, int b);

// pomocna funkcia pre ziskanie vysky stromu
int height(struct AVLNode *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// pomocna funkcia pre return vacsieho z 2 cisiel
int max(int a, int b)
{
    return (a > b)? a : b;
}

// pomocna funkcia pre vytvaranie novych nodes, kotre sa nasledne pridavaju do stromu
struct AVLNode* newAVLNode(char* a, int b)
{
    struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    node->strKey = strdup(a);
    node->key = b;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);

}

//  funkcia pre pravu rotaciu
struct AVLNode *rightRotate(struct AVLNode *y)
{
    struct AVLNode *x = y->left;       //  pointer pre node, ktory sa po rotacii stane vrcholom
    struct AVLNode *T2 = x->right;     //  pointer pre pravy podstrom Xka, ktory budem nasledne napajat ako lavy-podstrom Ycku

    // rotacie
    x->right = y;   //xko sa posunie na vrchol
    y->left = T2;   //napojim byvaly pravy-podstrom Xka ako lavy-child pre y

    // updatujem vysky pre nodes, vysky pouzivam na zistenie ci je node vyvazeny alebo nie je
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return novy root
    pocetRotaciiAVL++;
    return x;
}

//  funkcia pre lavu rotaciu
struct AVLNode *leftRotate(struct AVLNode *x)
{
    struct AVLNode *y = x->right;  //  pointer pre node, ktory sa po rotacii stane vrcholom
    struct AVLNode *T2 = y->left;  //  pointer pre lavy podstrom Ycka, ktory budem nasledne napajat ako pravy-podstrom Xku

    // rotacie
    y->left = x;        //  Ycko sa posunie na vrchol
    x->right = T2;      //  napojim byvaly lavy-podstrom ako right-child pre Xko, kedze Ycko ma uz ako lavy-child node X

    // updatujem vysky pre nodes, vysky pouzivam na zistenie ci je node vyvazeny alebo nie je
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return novy root
    pocetRotaciiAVL++;
    return y;
}

// zisti ci je vyvazeny
int getBalance(struct AVLNode *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);  // ak vyska: (L-node) - (R-node) = {-1,0,1}, potom je strom vyvazeny, inak nie je
}


//  rekurzivna funkcia pre vlozenie prvku do stromu
struct AVLNode* insertAVL(struct AVLNode* node, char* key, int a)
{
    // 1. klasicky insert do stromu na zaklade porovnavania klucov
    if (node == NULL)
        return(newAVLNode(key,a));

    if(strcmp(key,node->strKey) < 0)
        node->left  = insertAVL(node->left, key, a);
    else if (strcmp(key,node->strKey) > 0)
        node->right = insertAVL(node->right, key, a);
    else            // mam rovnaky string, este proovnam CENU ci sa jedna o duplikat
    {
        if(node->key == a){
            return node;        //already in - uz sa takyto prvok nachadza v strome
        }

        else if(node->key < a){     //cislo vkladaneho prvku je vacsie cize idem do praveho podstromu
            node->right = insertAVL(node->right, key,a);
        }
        else{                       //cislo vkladaneho prvku je mensie, idem do laveho podstromu
            node->left  = insertAVL(node->left, key,a);
        }
    }


    // 2. updatuje vysky nodes ktore su nad aktualnym
    node->height = 1 + max(height(node->left),
                           height(node->right));

    // 3. zisti ci je aktualny node vyvazeny, na zaklade tohto sa budu vykonavat rotacie ak budu potrebne
    int balance = getBalance(node);

    // ak je balance = 0 || 1 || -1 , je vyvazeny

    // pripad Left Left           - tu sa urcuje do ktorej strany sa bude rotovat + osetrenie ak je strKEY rovnaky a odlisny intKey (ten sa porovnava iba ak je strKEY rovnaky)
    if (balance > 1 && ((strcmp(key,node->left->strKey) < 0) || ((strcmp(key,node->left->strKey) == 0) && (a < node->left->key) )) )
        return rightRotate(node);

    // pripad Right Right           - podobne ako napisane vyssie, porovnanie klucov + osetrenie ako strKEY je rovnaky ale odlisny intKEY

    if (balance < -1 && ((strcmp(key,node->right->strKey) > 0) || ((strcmp(key,node->right->strKey) == 0) && (a > node->right->key) )  ))
        return leftRotate(node);

    // pripad Left Right
    if (balance > 1 && ( (strcmp(key,node->left->strKey) > 0) || ( (strcmp(key,node->left->strKey) == 0) && (a > node->left->key) ) ) )
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // pripad Right Left
    if (balance < -1 && ((strcmp(key,node->right->strKey) < 0) || ((strcmp(key,node->right->strKey) == 0) && (a < node->right->key) ) ) )
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}


int searchAVL(struct AVLNode* node, char* searched, int searchedINT)       //klasicky search pre binarny strom
{
    struct AVLNode *akt = node;
    while(akt != NULL)
    {
        if(strcmp(akt->strKey,searched) == 0)       //ak su kluce rovnake, este porovnam intKEY a na zaklade toho sa urci nasledovny krok
        {
            if(akt->key == searchedINT){
                return 1;
            }

            else if(akt->key < searchedINT){
                akt = akt->right;
            }
            else{
                akt = akt->left;
            }
        }
        else if(strcmp(akt->strKey,searched) > 0)   //ak je kluc mensi ako aktualny prvok, chod do laveho podstromu
            akt = akt->left;
        else if(strcmp(akt->strKey,searched) < 0)   //ak je kluc vacsi ako aktualny prvok, chod do praveho podstromu
            akt = akt->right;

    }
    return 0;
}




void testovacAVL(struct AVLNode *root, int pocetINPUTOV,int castINPUTOV)
{
    //pole inputov
    struct AVLNode *inputs = (struct AVLNode*)malloc(sizeof(struct AVLNode) * pocetINPUTOV);

    //  TEST insert all

    printf("\n\tAVL TREE:");
    clock_t begin1 = clock();

    for(int i = 0;i<pocetINPUTOV;i++)
    {
        inputs[i].key = intGenerator();
        inputs[i].strKey = strdup(stringGenerator());
        root = insertAVL(root,inputs[i].strKey,inputs[i].key);    //,inputs[i].key
    }

    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;

    printf("\nINSERT: execution time: %Lf",time_spent1);
    int pocetRotaciiPoINSERT = pocetRotaciiAVL;



    //  TEST search poslednych castINPUTOV(int) vlozenych do stromu

    clock_t begin3 = clock();

    for(int i = pocetINPUTOV-1;i >= pocetINPUTOV-castINPUTOV;i--)
    {
        searchAVL(root,inputs[i].strKey,inputs[i].key); //,inputs[i].key
    }

    clock_t end3 = clock();
    double time_spent3 = (double)(end3 - begin3) / CLOCKS_PER_SEC;
    printf("\nSearch(last 50 000 INSERTED): execution time: %Lf",time_spent3);

    clock_t begin4 = clock();

    for(int i = 0;i <castINPUTOV;i++)
    {
        searchAVL(root,inputs[i].strKey,inputs[i].key);   //,inputs[i].key
    }

    clock_t end4 = clock();
    double time_spent4 = (double)(end4 - begin4) / CLOCKS_PER_SEC;
    printf("\nSearch(first 50 000 INSERTED): execution time: %Lf",time_spent4);


    //  TEST search prvych castINPUTOV(int) vlozenych do stromu
    clock_t begin2 = clock();

    for(int i = 0;i<pocetINPUTOV;i++)
    {
        searchAVL(root,inputs[i].strKey,inputs[i].key);   //,inputs[i].key
    }

    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("\nSearch(all INSERTED): execution time: %Lf",time_spent2);


    printf("\nRotations after INSERT: %d",pocetRotaciiPoINSERT);
    printf("\nTotal rotations: %d",pocetRotaciiAVL);

    printf("\n//////////////////////////////////////////");







}
