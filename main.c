#include <stdio.h>
#include "header.h"
#include "mySplayTree.c"
#include "AVLTree.c"
#include "MyHASH.c"
#include "HashLinearProbing.c"

int main() {
    /***    STROMY  ***/
    STnode *splaytree = NULL;
    struct AVLNode *AVLtree = NULL;

    /***    HASH tabulka ***/
    HASHnode **MyHashTable = (HASHnode **)malloc(sizeof(HASHnode *) * size);
    //  LP_tabulak je staticky definovana v HashLinearProbing.c

    /***    TESTY   ***/

    //pocetINPUTOV = kolko prvkov chceme vkladat do tabulky/stromu
    //castINPUTOV = iba pre dplnujuci test, nejake ciastocne vyhladavanie

    testovacST(splaytree,300000,100000);
    testovacAVL(AVLtree,300000,100000);
    testovacMyHASH(MyHashTable,500000,40000);
    testovacHashLinearProbing(500000,40000);     //v header.h je konstanta pre velkost tabulky, treba zmenit ak chceme vacsiu tabulku lebo je staticka

    
    return 0;
}
