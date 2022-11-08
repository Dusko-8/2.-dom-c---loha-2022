/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) { (*tree) = NULL; }

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bool ret = false;
  bst_node_t *tmp = tree;

  while (tmp != NULL) {
    // right subtree as new tree
    if (key > tmp->key) {
      tmp = tmp->right;
    }
    // left subtree as new tree
    else if (key < tmp->key) {
      tmp = tmp->left;
    } else {
      ret = true;
      *value = tmp->value;
      return ret;
    }
  }

  return ret;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *nNode = (bst_node_t *)malloc(sizeof(bst_node_t));

  nNode->key = key;
  nNode->value = value;
  nNode->right = NULL;
  nNode->left = NULL;

  bst_node_t *actual = *tree;
  bst_node_t *previus = NULL;

  while (actual != NULL) {

    previus = actual;
    if (key < actual->key) {
      actual = actual->left;
    } else if (key > actual->key) {
      actual = actual->right;
    } else {
      actual->value = value;
      return;
    }
  }

  if (*tree == NULL) // Tree is empty
  {
    *tree = nNode;
    return;
  }

  if (key < previus->key) {
    previus->left = nNode;
  }
  if (key > previus->key) {
    previus->right = nNode;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *moust_R = *tree;
  bst_node_t *previus_R = NULL;
  bst_node_t *actual = *tree;

  if (moust_R->left != NULL) {
    moust_R = moust_R->left;
  } else {
    return;
  }

  while (moust_R->right != NULL) {
    previus_R = moust_R;
    moust_R = moust_R->right;
  }

  while (actual != NULL) {
    if (target->key < actual->key) {
      actual = actual->left;
    } else if (target->key > actual->key) {
      actual = actual->right;
    } else {
      actual->value = moust_R->value;
      actual->key = moust_R->key;
      previus_R->right = NULL;
      free(moust_R);
      return;
    }
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

  bst_node_t *actual = *tree;
  bst_node_t *previus = NULL;

  while (actual != NULL) {

    previus = actual;
    if (key < actual->key) {

      actual = actual->left;
    } else if (key > actual->key) {
      actual = actual->right;
    }

    if (actual != NULL && key == actual->key) {

      if (actual->left == NULL && actual->right == NULL) {

        if (previus->key < actual->key) {

          previus->right = NULL;
        } else {
          previus->left = NULL;
        }

        free(actual);
        return;
      }
      if (actual->left != NULL && actual->right != NULL) {
        bst_replace_by_rightmost(actual, tree);
        return;
      } else {

        if (previus->key < actual->key) {

          if (actual->left == NULL) {
            previus->right = actual->right;
            free(actual);
            return;
          } else {
            previus->right = actual->left;
            free(actual);
            return;
          }
        } else {
          if (actual->left == NULL) {
            previus->left = actual->right;
            free(actual);
            return;
          } else {
            previus->left = actual->left;
            free(actual);
            return;
          }
        }
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  if (tree != NULL) {
    stack_bst_push(&stack, *tree);
  }

  while (!stack_bst_empty(&stack)) {
    bst_node_t *node = stack_bst_pop(&stack);
    if (node->left != NULL) {
      stack_bst_push(&stack, node->left);
    }
    if (node->left != NULL) {
      stack_bst_push(&stack, node->right);
    }
    free(node);
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  stack_bst_t *stack = to_visit;
  bst_node_t *ptr = tree;

  while (tree != NULL) {
    stack_bst_push(&stack, ptr);
    bst_print_node(ptr);
    ptr = ptr->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  bst_node_t *actual = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);

  LeftMostPre(tree, stack);
  while (!stack_bst_empty(&stack)) {
    actual = Top(&stack);
    stack_bst_pop(&stack);
    LeftMostPre(actual->right, stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *actual = tree;
  while (actual != NULL) {
    stack_bst_push(&to_visit, actual);
    actual = actual->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t *Stack;
  bst_node_t *actual = tree;
  InitStack(&Stack);
  bst_leftmost_inorder(tree,&Stack);
  while (!IsEmpty(&Stack)) {
    actual = Top(&Stack);
    stack_bst_pop(&Stack);
    //DLL_InsertLast(l, actual->data); // změna od PreOrder
    bst_leftmost_inorder(actual->right);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {}
