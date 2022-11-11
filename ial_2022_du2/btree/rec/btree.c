/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
  bool ret = false;
  bst_node_t *actual = tree;

  if (actual == NULL)
  {
    return false;
  }
  if (actual->key == key)
  {
    *value = actual->value;
    return true;
  }

  if (actual->key < key)
  {
    return bst_search(actual->right, key, value);
  }

  if (actual->key > key)
  {
    return bst_search(actual->left, key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
  bst_node_t *actual = *tree;

  if (*tree == NULL)
  {
    bst_node_t *new = (bst_node_t *)malloc(sizeof(bst_node_t));

    new->key = key;
    new->value = value;
    new->left = NULL;
    new->right = NULL;

    *tree = new;
    return;
  }

  if (actual->key == key)
  {
    actual->value = value;
    return;
  }

  if (actual->key < key)
  {
    if (actual->right == NULL)
    {
      bst_node_t *new = (bst_node_t *)malloc(sizeof(bst_node_t));

      new->key = key;
      new->value = value;
      new->left = NULL;
      new->right = NULL;
      actual->right = new;

      return;
    }

    bst_insert(&actual->right, key, value);
  }

  if (actual->key > key)
  {

    if (actual->left == NULL)
    {
      bst_node_t *new = (bst_node_t *)malloc(sizeof(bst_node_t));

      new->key = key;
      new->value = value;
      new->left = NULL;
      new->right = NULL;
      actual->left = new;

      return;
    }
    bst_insert(&actual->left, key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *actual = *tree;
  if (actual->right == NULL)
  {
    target->value = actual->value;
    target->key = actual->key;

    if (actual->left == NULL)
    {
      target->left = NULL;
    }
    else
    {
      target->left = target->left;
    }
    free(actual);
    return;
  }
  else if (actual->right->right == NULL)
  {
    bst_node_t *temp = actual->right;

    target->value = actual->right->value;
    target->key = actual->right->key;

    if (actual->right->left != NULL)
    {
      actual->right = actual->right->left;
    }
    else if (actual->right->left == NULL)
    {
      actual->right = NULL;
    }
    free(temp);
    return;
  }

  bst_replace_by_rightmost(target, &actual->right);
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *actual = *tree;
  bst_node_t *delete = NULL;
  if (*tree == NULL)
  {
    return;
  }

  if (actual->key < key)
  {
    // syn je klucom
    if (actual->right != NULL &&actual->right->key == key)
    {
      delete = actual->right;
      // syn nema ziadnych potomkov
      if (actual->right->left == NULL && actual->right->right == NULL)
      {
        actual->right = NULL;
        free(delete);
        return;
      }

      // syn ma laveho potomka
      if (actual->right->left != NULL && actual->right->right == NULL)
      {
        actual->right = actual->right->left;
        free(delete);
        return;
      }

      // syn ma praveho potomka
      if (actual->right->left == NULL && actual->right->right != NULL)
      {
        actual->right = actual->right->right;
        free(delete);
        return;
      }

      // syn ma 2 potomkov
      if (actual->right->left != NULL && actual->right->right != NULL)
      {
        bst_replace_by_rightmost(actual->right, &actual->right->left);
        return;
      }
    }

    bst_delete(&actual->right, key);
    return;
  }

  else if (actual->key > key)
  {
    // syn je klucom
    if (actual->left != NULL && actual->left->key == key)
    {
      delete = actual->left;
      // syn nema ziadnych potomkov
      if (actual->left->left == NULL && actual->left->right == NULL)
      {
        actual->left = NULL;
        free(delete);
        return;
      }

      // syn ma laveho potomka
      if (actual->left->left != NULL && actual->left->right == NULL)
      {
        actual->left = actual->left->left;
        free(delete);
        return;
      }

      // syn ma praveho potomka
      if (actual->left->left == NULL && actual->left->right != NULL)
      {
        actual->left = actual->left->right;
        free(delete);
        return;
      }

      // syn ma 2 potomkov
      if (actual->left->left != NULL && actual->left->right != NULL)
      {
        bst_replace_by_rightmost(actual->left, &actual->left->left);
        return;
      }
    }

    bst_delete(&actual->left, key);
    return;
  }

  if (actual->key == key)
  {

    delete = *tree;
    if (actual->right == NULL && actual->right == NULL)
    {
      free(delete);
      *tree = NULL;
      return;
    }

    if (actual->left != NULL && actual->right == NULL)
    {
      *tree = actual->left;
      free(delete);
      return;
    }

    // syn ma praveho potomka
    if (actual->left == NULL && actual->right != NULL)
    {
      *tree = actual->right;
      free(delete);
      return;
    }
    // syn ma 2 potomkov
    if (actual->left != NULL && actual->right != NULL)
    {
      bst_replace_by_rightmost(*tree, &actual->left);
      return;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{
  // bst_node_t *actual = *tree;
  if (*tree == NULL)
  {
    return;
  }

  bst_dispose(&(*tree)->left);
  bst_dispose(&(*tree)->right);

  free(*tree);
  *tree = NULL;
  return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
  if (tree != NULL)
  {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
  if (tree != NULL)
  {

    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
  if (tree != NULL)
  {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
