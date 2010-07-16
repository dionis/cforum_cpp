/*!
 * \file treeutils.h
 * \brief Interface to an AVL binary tree implementation
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * Interface to an implementation of an AVL binary tree
 */

#ifndef CF_TREE_UTILS_H
#define CF_TREE_UTILS_H

/*!
 * A tree node data
 */
typedef struct cf_tree_dataset_s {
  void *key; /*!< Key member */
  void *data; /*!< Data member */
} cf_tree_dataset_t;

/*!
 * balancing types. NONE means balanced, LEFT means
 * left subtree is overbalanced and right means right
 * subtree is overbalanced.
 */
enum cf_tree_balance_e {
  CF_TREE_NONE, /*!< balanced */
  CF_TREE_LEFT, /*!< Left overbalanced */
  CF_TREE_RIGHT /*!< Right overbalanced */
};

/*!
 *  AVL tree node structure
 */
typedef struct s_cf_tree_node {
  struct s_cf_tree_node *left,  /*!< left subtree */
                        *right; /*!< right subtree */

  cf_tree_dataset_t *d; /*!< Dataset member */

  enum cf_tree_balance_e bal; /*!< balance */
} cf_tree_node_t;

/*! comparer function prototype */
typedef int (*cf_tree_comparer_t)(cf_tree_dataset_t *,cf_tree_dataset_t *);

/*! destroyer function prototype */
typedef void (*cf_tree_destroyer_t)(cf_tree_dataset_t *);

/*!
 * AVL tree structure
 */
typedef struct s_cf_tree {
  /*!
   * Comparing function. Has to return -1 if first dataset is smaller
   * than the second, +1 if first dataset is greater than the second
   * and 0 if both are equal.
   */
  cf_tree_comparer_t compare;

  /*!
   * 'Destructor' function for a tree dataset
   */
  cf_tree_destroyer_t destroy;

  cf_tree_node_t *root; /*!< root node */
} cf_tree_t;

/**
 * This function initializes a new tree object
 * \param tree The tree object structure
 * \param compare The comparing function
 * \param destroy The destructor function for tree elements
 */
void cf_tree_init(cf_tree_t *tree,cf_tree_comparer_t compare,cf_tree_destroyer_t destroy);

/**
 * This function is used to destroy a tree
 * \param tree The tree object
 */
void cf_tree_destroy(cf_tree_t *tree);

/**
 * This function inserts a tree node into a tree with the data of dataset d (d will be copied)
 * \param tree The tree object structure
 * \param n Set always to NULL
 * \param d The dataset structure. Will be copied
 * \return CF_TREE_NONE, CF_TREE_LEFT or CF_TREE_RIGHT
 */
int cf_tree_insert(cf_tree_t *tree,cf_tree_node_t **n, cf_tree_dataset_t *d);

/**
 * Removes a node from a tree (expensive!)
 * \param tree The tree object structure
 * \param n Set always to NULL
 * \param key A dataset structure with a valid key set
 * \return CF_TREE_NONE, CF_TREE_LEFT or CF_TREE_RIGHT
 */
int cf_tree_remove(cf_tree_t *tree,cf_tree_node_t **n, cf_tree_dataset_t *key);

/**
 * Looks up a dataset
 * \param tree The tree object structure
 * \param n Set to tree->root
 * \param key A dataset structure with a valid key
 * \return NULL if element could not be found, element reference if element has been found
 */
const cf_tree_dataset_t *cf_tree_find(cf_tree_t *tree,cf_tree_node_t *n, cf_tree_dataset_t *key);

#endif

/* eof */
