#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) { //LISTO
    TreeMap *mapa = (TreeMap *)malloc(sizeof(TreeMap)) ;
    if (mapa == NULL) return NULL ;

    mapa->root = NULL ;
    mapa->current = NULL ;
    mapa->lower_than = lower_than ;
    return mapa ;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) { //LISTO
    if (tree == NULL) return ;
    if (searchTreeMap(tree, key) != NULL) return ; //verifica que la clave ya exista

    TreeNode *padre = NULL ;
    TreeNode *explorador = tree->root ;

    while (explorador != NULL){ //Buscamos la posicion correcta para la insercion
        padre = explorador ;
        if (tree->lower_than(key, explorador->pair->key))
            explorador = explorador->left ; //buscamos por la izquierda
        else 
            explorador = explorador->right ; //buscamos por la derecha
    }

    TreeNode *nodito = createTreeNode(key, value) ; //creacion del nuevo nodo y enlazamiento
    if (nodito == NULL) return ;
    nodito->parent = padre ; //enlazamiento del nuevo nodo
    if (padre == NULL) tree->root = nodito ;
    else if (tree->lower_than(key, padre->pair->key))
        padre->left = nodito ; //se enlaza como hijo izquierdo
    else
        padre->right = nodito ; //se enlaza como hijo derecho
    
    tree->current = nodito ;
}

TreeNode * minimum(TreeNode * x){ //LISTO
    if (x == NULL) return NULL ;
    while (x->left != NULL) //mientras haya un hijo izq. seguimos buscando
        x = x->left ;
    return x ;
}

void removeNode(TreeMap * tree, TreeNode* node) { //LISTO
    if (tree == NULL || node == NULL) return ;

    TreeNode *padre = node->parent ;
    if (node->left == NULL && node->right == NULL){ //para Nodo SIN hijos
        if (padre == NULL) tree->root = NULL ;
        else if (padre->left == node) padre->left = NULL ;
        else padre->right = NULL ;

        free(node->pair) ;
        free(node) ;
    }
    else if (node->left == NULL || node->right == NULL){ //para Nodo con UN hijo
        TreeNode* hijito = (node->left != NULL) ? node->left : node->right ; //corroborar si el nodo esta a la izquierda o la derecha
        hijito->parent = padre ;

        if (padre == NULL) tree->root = hijito ;
        else if (padre->left == node) padre->left = hijito ;
        else padre->right = hijito ;
    }
    else { //para Nodo con DOS hijos
        TreeNode* siguiente = minimum(node->right) ;
        free(node->pair) ;
        node->pair = siguiente->pair ;
        removeNode(tree, siguiente) ;
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) { //LISTO
    if (tree == NULL || tree->root == NULL) return NULL ;
    TreeNode *nodito = tree->root ;

    while (nodito != NULL){
        if (is_equal(tree, key, nodito->pair->key)){ //se busca el nodo que tenga la clave buscada
            tree->current = nodito ;
            return nodito->pair ;}
        if (tree->lower_than(key, nodito->pair->key))
            nodito = nodito->left ; //si la clave es menor se busca en el sub-arbol izquierda
        else //si es mayor, en el sub-arbol derecho
            nodito = nodito->right ;
    }
    //si no se encuentra la clave
    tree->current = NULL ;
    return NULL ;
}

Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
