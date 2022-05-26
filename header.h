//
// Created by tomas on 05/04/2021.
//

#ifndef DSA_ZADANIE2_HEADER_H
#define DSA_ZADANIE2_HEADER_H

#define ABS(N) ((N<0)?(-N):(N))
#define MAXstr 100
//velkost tabulky Hash-LinearProbing    treba upravit pre zmenu testovania
#define TABLE_SIZE 500000

//  my SPLAY TREE
typedef struct stnode{
    char* prichut;
    int cena;
    struct stnode *left;
    struct stnode *right;
    struct stnode *parent;
}STnode;

//  AVL TREE
struct AVLNode
{
    int key;
    char* strKey;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
};

//  My hash
typedef struct hashsnode{
    char* prichut;
    int cena;
    struct hashsnode *next;
}HASHnode;

typedef struct nodeLP{
    char* prichut;
    int cena;
}LPnode;




#endif //DSA_ZADANIE2_HEADER_H
