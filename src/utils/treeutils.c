/*!
 * \file treeutils.c
 * \brief binary tree implementation
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Implementation of an AVL binary tree
 */


#include "config.h"
#include "defines.h"

#include <stdlib.h>

#include "memoryutils.h"
#include "treeutils.h"



/*!
 * private function to do left-rotation if tree is
 * unbalanced
 * \param \c n The subtree root node
 */
static void cf_tree_rotate_left(cf_tree_node_t **n) {
  cf_tree_node_t *tmp = *n;

  *n = (*n)->right;
  tmp->right = (*n)->left;
  (*n)->left = tmp;
}



/*!
 * private function to do right-rotation if tree
 * is unbalanced
 * \param \c n The subtree root node
 */
static void cf_tree_rotate_right(cf_tree_node_t **n) {
  cf_tree_node_t *tmp = *n;

  *n = (*n)->left;
  tmp->left = (*n)->right;
  (*n)->right = tmp;
}



/*!
 * private function to handle the case that the tree has
 * grown on the left side
 * \param \c n The subtree root node
 * \return Returns 0 or 1
 */
static int cf_tree_leftgrown(cf_tree_node_t **n) {
  switch((*n)->bal) {
    case CF_TREE_LEFT:
      if((*n)->left->bal == CF_TREE_LEFT) {
        (*n)->bal = (*n)->left->bal = CF_TREE_NONE;

        cf_tree_rotate_right(n);
      }
      else {
        switch((*n)->left->right->bal) {
          case CF_TREE_LEFT:
            (*n)->bal = CF_TREE_RIGHT;
            (*n)->left->bal = CF_TREE_NONE;
            break;

          case CF_TREE_RIGHT:
            (*n)->bal       = CF_TREE_NONE;
            (*n)->left->bal = CF_TREE_LEFT;
            break;

          default:
            (*n)->bal = CF_TREE_NONE;
            (*n)->left->bal = CF_TREE_NONE;
        }

        (*n)->left->right->bal = CF_TREE_NONE;

        cf_tree_rotate_left(&(*n)->left);
        cf_tree_rotate_right(n);
      }

      return 0;

    case CF_TREE_RIGHT:
      (*n)->bal = CF_TREE_NONE;
      return 0;

    default:
      (*n)->bal = CF_TREE_LEFT;
      return 1;
  }
}



/*!
 * private function to handle the case that the tree
 * has been grown to the right side
 * \param \c n Subtree root node
 * \return Returns 0 or 1
 */
static int cf_tree_rightgrown(cf_tree_node_t **n) {
  switch((*n)->bal) {
    case CF_TREE_LEFT:
      (*n)->bal = CF_TREE_NONE;
      return 0;

    case CF_TREE_RIGHT:
      if((*n)->right->bal == CF_TREE_RIGHT) {
        (*n)->bal = (*n)->right->bal = CF_TREE_NONE;
        cf_tree_rotate_left(n);
      }
      else {
        switch((*n)->right->left->bal) {
          case CF_TREE_RIGHT:
            (*n)->bal = CF_TREE_LEFT;
            (*n)->right->bal = CF_TREE_NONE;
            break;

          case CF_TREE_LEFT:
            (*n)->bal = CF_TREE_NONE;
            (*n)->right->bal = CF_TREE_RIGHT;
            break;

          default:
            (*n)->bal = CF_TREE_NONE;
            (*n)->right->bal = CF_TREE_NONE;
        }

        (*n)->right->left->bal = CF_TREE_NONE;
        cf_tree_rotate_right(& (*n)->right);
        cf_tree_rotate_left(n);
      }

      return 0;

    default:
      (*n)->bal = CF_TREE_RIGHT;
      return 1;
  }
}

/*!
 * private function to handle the case that the tree has
 * shrunken on the left side
 * \param \c n The subtree root node
 * \return Returns 0 or 1
 */
static int cf_tree_leftshrunk(cf_tree_node_t **n) {
  switch((*n)->bal) {
    case CF_TREE_LEFT:
      (*n)->bal = CF_TREE_NONE;

      return 1;

    case CF_TREE_RIGHT:
      if((*n)->right->bal == CF_TREE_RIGHT) {
        (*n)->bal = (*n)->right->bal = CF_TREE_NONE;
        cf_tree_rotate_left(n);

        return 1;
      }
      else if((*n)->right->bal == CF_TREE_NONE) {
        (*n)->bal = CF_TREE_RIGHT;
        (*n)->right->bal = CF_TREE_LEFT;
        cf_tree_rotate_left(n);

        return 0;
      }
      else {
        switch((*n)->right->left->bal) {
          case CF_TREE_LEFT:
            (*n)->bal = CF_TREE_NONE;
            (*n)->right->bal = CF_TREE_RIGHT;
            break;

          case CF_TREE_RIGHT:
            (*n)->bal = CF_TREE_LEFT;
            (*n)->right->bal = CF_TREE_NONE;
            break;

          default:
            (*n)->bal = CF_TREE_NONE;
            (*n)->right->bal = CF_TREE_NONE;
        }

        (*n)->right->left->bal = CF_TREE_NONE;
        cf_tree_rotate_right(&(*n)->right);
        cf_tree_rotate_left(n);
        return 1;
      }

    default:
      (*n)->bal = CF_TREE_RIGHT;
      return 0;
  }
}



/*!
 * Private function to handle the case that the tree has
 * shrunken on the right side
 * \param \c n The subtree root node
 * \return Returns 0 or 1
 */
static int cf_tree_rightshrunk(cf_tree_node_t **n) {
  switch((*n)->bal) {
    case CF_TREE_RIGHT:
      (*n)->bal = CF_TREE_NONE;
      return 1;

    case CF_TREE_LEFT:
      if((*n)->left->bal == CF_TREE_LEFT) {
        (*n)->bal = (*n)->left->bal = CF_TREE_NONE;
        cf_tree_rotate_right(n);

        return 1;
      }
      else if((*n)->left->bal == CF_TREE_NONE) {
        (*n)->bal = CF_TREE_LEFT;
        (*n)->left->bal = CF_TREE_RIGHT;
        cf_tree_rotate_right(n);

        return 0;
      }
      else {
        switch((*n)->left->right->bal) {
          case CF_TREE_LEFT:
            (*n)->bal = CF_TREE_RIGHT;
            (*n)->left->bal = CF_TREE_NONE;
            break;

          case CF_TREE_RIGHT:
            (*n)->bal = CF_TREE_NONE;
            (*n)->left->bal = CF_TREE_LEFT;
            break;

          default:
            (*n)->bal = CF_TREE_NONE;
            (*n)->left->bal = CF_TREE_NONE;
        }

        (*n)->left->right->bal = CF_TREE_NONE;

        cf_tree_rotate_left(&(*n)->left);
        cf_tree_rotate_right(n);

        return 1;
      }

    default:
      (*n)->bal = CF_TREE_LEFT;
      return 0;
  }
}



/*!
 * Private function to find the highest subtree
 * \param \c target Target node
 * \param \c n The subtree root node
 * \param \c res Result of this operation (height of the tree)
 * \return Returns 0 or 1
 */
static int cf_tree_findhighest(cf_tree_node_t *target,cf_tree_node_t **n,int *res) {
  cf_tree_node_t *tmp;

  *res = 1;
  if(!(*n)) {
    return 0;
  }

  if((*n)->right) {
    if(!cf_tree_findhighest(target,&(*n)->right,res)) {
      return 0;
    }
    if(*res == 1) {
      *res = cf_tree_rightshrunk(n);
    }

    return 1;
  }

  target->d = (*n)->d;
  tmp = *n;
  *n = (*n)->left;
  free(tmp);

  return 1;
}



/*!
 * Private function to find the lowest subtree
 * \param \c target The target node
 * \param \c n The subtree root node
 * \param \c res The result of this operation (height of the tree)
 * \return Returns 0 or 1
 */
static int cf_tree_findlowest(cf_tree_node_t *target,cf_tree_node_t **n,int *res) {
  cf_tree_node_t *tmp;

  *res = 1;
  if(!(*n)) return 0;

  if((*n)->left) {
    if(!cf_tree_findlowest(target,&(*n)->left,res)) {
      return 0;
    }
    if(*res == 1) {
      *res = cf_tree_leftshrunk(n);
    }

    return 1;
  }

  target->d = (*n)->d;
  tmp = *n;
  *n = (*n)->right;
  free(tmp);

  return 1;
}

/*!
 * Private function used to destroy all tree nodes (works recursively)
 * \param \c tree The tree
 * \param \c n Actual node
 */
static void cf_tree_destroy_nodes(cf_tree_t *tree,cf_tree_node_t *n) {
  if(n) {
    if(n->left) cf_tree_destroy_nodes(tree,n->left);
    if(n->right) cf_tree_destroy_nodes(tree,n->right);

    if(tree->destroy) {
      tree->destroy(n->d);
    }
    else {
      if(n->d->key)  free(n->d->key);
      if(n->d->data) free(n->d->data);
    }

    free(n->d);
    free(n);
  }
}


int cf_tree_insert(cf_tree_t *tree,cf_tree_node_t **n, cf_tree_dataset_t *d) {
  int tmp;

  if(!n) n = &tree->root;

  if(!(*n)) {
    *n = cf_alloc(NULL,1,sizeof(*tree->root),CF_ALLOC_CALLOC);

    (*n)->d   = memdup(d,sizeof(*d));
    (*n)->bal = 0;

    return 1;
  }

  if(tree->compare(d,(*n)->d) < 0) {
    if((tmp = cf_tree_insert(tree,&(*n)->left,d)) == 1) {
      return cf_tree_leftgrown(n);
    }

    return tmp;
  }
  else if(tree->compare(d,(*n)->d) > 0) {
    if((tmp = cf_tree_insert(tree,&(*n)->right,d)) == 1) {
      return cf_tree_rightgrown(n);
    }

    return tmp;
  }

  return -1;
}

int cf_tree_remove(cf_tree_t *tree,cf_tree_node_t **n,cf_tree_dataset_t *key) {
  int tmp = 1;

  if(!n) n = &tree->root;

  if(!(*n)) return -1;

  if(tree->compare(key,(*n)->d) < 0) {
    if((tmp = cf_tree_remove(tree,&(*n)->left, key)) == 1) {
      return cf_tree_leftshrunk(n);
    }

    return tmp;
  }
  else if(tree->compare(key, (*n)->d) > 0) {
    if((tmp = cf_tree_remove(tree,&(*n)->right,key)) == 1) {
      return cf_tree_rightshrunk(n);
    }

    return tmp;
  }

  if((*n)->left) {
    if(cf_tree_findhighest(*n, &((*n)->left), &tmp)) {
      if(tmp == 1) {
        tmp = cf_tree_leftshrunk(n);
      }

      return tmp;
    }
  }
  if((*n)->right) {
    if(cf_tree_findlowest(*n, &((*n)->right), &tmp)) {
      if(tmp == 1) {
        tmp = cf_tree_rightshrunk(n);
      }
      return tmp;
    }
  }

  free(*n);
  *n = NULL;

  return 1;
}

const cf_tree_dataset_t *cf_tree_find(cf_tree_t *tree,cf_tree_node_t *n, cf_tree_dataset_t *key) {
  if(!n) return NULL;

  if(tree->compare(key,n->d) < 0) {
    return cf_tree_find(tree,n->left,key);
  }
  else if(tree->compare(key,n->d) > 0) {
    return cf_tree_find(tree,n->right,key);
  }

  return n->d;
}



void cf_tree_init(cf_tree_t *tree,cf_tree_comparer_t compare,cf_tree_destroyer_t destroy) {
  tree->root    = NULL;
  tree->compare = compare;
  tree->destroy = destroy;
}

void cf_tree_destroy(cf_tree_t *tree) {
  cf_tree_destroy_nodes(tree,tree->root);
}


/* eof */
