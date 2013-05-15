#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const char BLACK = 'b';
const char RED = 'r';

struct node {
  int value;
  char color;
  struct node* lchild;
  struct node* rchild;
  struct node* parent;
};

int print_color(char c) {
  if (BLACK == c) {
    printf("%c", c);
  } else {
    printf("\033[0;1;31m%c\033[m", c);
  }
}

int print_recursion(const struct node* p) {
  if (NULL == p->parent) {
    printf("ROOT %d", p->value);
    return 0;
  }
  print_recursion(p->parent);
  if (p == p->parent->lchild) {
    printf(" -L-> ");
  } else {
    printf(" -R-> ");
  }
  printf("%d", p->value);
  print_color(p->color);
  return 0;
}

int print_path(const struct node* p) {
  if (NULL == p) {
    printf("NULL\n");
    return -1;
  }
  print_recursion(p);
  printf("\n");
  return 0;
}

struct node* find(const struct node* root, int key, bool parent = false) {
  const struct node* curr_node = root;
  struct node* last_node = NULL;
  while (NULL != curr_node) {
    last_node = (struct node*)curr_node;
    if (curr_node->value == key) {
      return (struct node*)curr_node;
    } else if (curr_node->value > key) {
      curr_node = curr_node->lchild;
    } else {
      curr_node = curr_node->rchild;
    }
  }
  if (parent) {
    return last_node;
  }
  return NULL;
}

int left_rotate(struct node* curr_node, struct node* parent) {
  parent->rchild = curr_node->lchild;
  if (NULL != curr_node->lchild) {
    curr_node->lchild->parent = parent;
  }

  curr_node->lchild = parent;
  curr_node->parent = parent->parent;

  if (NULL != parent->parent) {
    if (parent == parent->parent->lchild) {
      parent->parent->lchild = curr_node;
    } else {
      parent->parent->rchild = curr_node;
    }
  }
  parent->parent = curr_node;

  return 0;
}

int right_rotate(struct node* curr_node, struct node* parent) {
  parent->lchild = curr_node->rchild;
  if (NULL != curr_node->rchild) {
    curr_node->rchild->parent = parent;
  }

  curr_node->rchild = parent;
  curr_node->parent = parent->parent;

  if (NULL != parent->parent) {
    if (parent == parent->parent->lchild) {
      parent->parent->lchild = curr_node;
    } else {
      parent->parent->rchild = curr_node;
    }
  }
  parent->parent = curr_node;

  return 0;
}

int insert_fixup(struct node* curr_node) {
  struct node* parent = curr_node->parent;
  // 0. is root
  if (NULL == parent) {
    curr_node->color = BLACK;
    return 0;
  }

  // 1. parent is black
  if (parent->color == BLACK) {
    return 0;
  }

  // 2. parent is red

  // there must be a grandparent if parent is red
  struct node* grandparent = parent->parent;

  // get uncle
  struct node* uncle = NULL;
  if (parent == grandparent->lchild) {
    uncle = grandparent->rchild;
  } else {
    uncle = grandparent->lchild;
  }

  // 2.1 has uncle and uncle is red
  if (NULL != uncle && uncle->color == RED) {
    parent->color = BLACK;
    uncle->color = BLACK;
    grandparent->color = RED;
    insert_fixup(grandparent);
    return 0;
  }

  // 2.2 hasn't uncle or uncle is black
  // 2.2.1 gp.l->p
  if (parent == grandparent->lchild) {
    // 2.2.1.2 p.r->c, change to c.l->p
    if (curr_node == parent->rchild) {
      left_rotate(curr_node, parent);
      parent = curr_node;
    }
    // 2.2.1.1 p.l->c
    parent->color = BLACK;
    grandparent->color = RED;
    right_rotate(parent, grandparent);
  } else {
    // 2.2.2.2 p.l->c change to c.r->p
    if (curr_node == parent->lchild) {
      right_rotate(curr_node, parent);
      parent = curr_node;
    }
    parent->color = BLACK;
    grandparent->color = RED;
    left_rotate(parent, grandparent);
  }

  return 0;
}

struct node* insert(struct node * &root, int key) {
  struct node* parent = find(root, key, true);
  // printf("==== will insert %d under\n    ", key);
  // print_path(parent);
  // same node, insert failed
  if (NULL != parent && parent->value == key) {
    return NULL;
  }

  // init a new node
  struct node* new_node = (struct node*)malloc(sizeof(struct node));
  new_node->value = key;
  new_node->color = RED;
  new_node->lchild = NULL;
  new_node->rchild = NULL;
  new_node->parent= NULL;

  // connect new_node and parent
  new_node->parent = parent;
  if (parent != NULL) {
    if (parent->value > new_node->value) {
      parent->lchild = new_node;
    } else {
      parent->rchild = new_node;
    }
  }

  // printf(">>>> before adjust\n ");
  // print_path(new_node);

  // adjust color
  insert_fixup(new_node);

  // the root may be changed during adjust
  if (root == NULL) {
    root = new_node;
  } else {
    while (root->parent != NULL) {
      root = root->parent;
    }
  }

  printf("<<<< after insert %d and adjust color \n", key);
  print_path(new_node);

  return new_node;
}

struct node* find_max(struct node* p) {
  while (p->rchild != NULL) {
    p = p->rchild;
  }
  return p;
}

int remove_fixup(struct node* curr_node, struct node* parent) {
  if (NULL != curr_node) {
    // 0. is root
    if (NULL == parent) {
      curr_node->color = BLACK;
      return 0;
    }

    // 1. curr node is red
    if (curr_node->color == RED) {
      curr_node->color = BLACK;
      return 0;
    }
  }

  // 2. curr node is black, there must be a sibling
  struct node* sibling = NULL;
  if (curr_node == parent->lchild) {
    sibling = parent->rchild;
  } else {
    sibling = parent->lchild;
  }

  // 3. sibling is red
  //  and it also means parent must be black
  //  and there must be two children of sibling and are black
  if (sibling->color == RED) {
    parent->color = RED;
    sibling->color = BLACK;
    if (sibling == parent->lchild) {
      right_rotate(sibling, parent);
    } else {
      left_rotate(sibling, parent);
    }
  }

  // the following part are discuss sibling is black
  // 4. sibling's children are all black
  if ((sibling->lchild == NULL || sibling->lchild->color == BLACK) &&
      (sibling->rchild == NULL || sibling->rchild->color == BLACK)) {
    sibling->color = RED;
    remove_fixup(parent, parent->parent);
    return 0;
  }

  // 6. curr is lchild , sibling's is black and sibling->l is red
  if (curr_node == parent->lchild) {
    if (sibling->lchild != NULL && sibling->lchild->color == RED) {
      right_rotate(sibling->lchild, sibling);
      remove_fixup(curr_node, parent);
    }
    sibling->color = parent->color;
    sibling->rchild->color = BLACK;
    parent->color = BLACK;
    left_rotate(sibling, parent);
  } else {
    if (sibling->rchild != NULL && sibling->rchild->color == RED) {
      left_rotate(sibling->rchild, sibling);
      remove_fixup(curr_node, parent);
    }
    sibling->color = parent->color;
    sibling->lchild->color = BLACK;
    parent->color = BLACK;
  }

  return 0;
}

int remove_node(struct node* &root, struct node* p) {
  // have 2 child, change the max node of p's left subtree to instead p
  if (NULL != p->lchild && NULL != p->rchild) {
    struct node* lmax = find_max(p->lchild);
    p->value = lmax->value;
    remove_node(root, lmax);
    return 0;
  }

  struct node* parent = p->parent;

  // get child
  struct node* child = NULL;
  if (NULL != p->lchild) {
    child = p->lchild;
  } else {
    child = p->rchild;
  }

  if (NULL != parent) {
    if (p == parent->lchild) {
      parent->lchild = child;
    } else {
      parent->rchild = child;
    }
  } else {
    root = child;
  }
  if (NULL != child) {
    child->parent = parent;
  }

  if (p->color == BLACK) {
    remove_fixup(child, parent);
  }

  free(p);
  return 0;
}

int remove_by_key(struct node * &root, int key) {
  struct node* p = find(root, key);
  if (NULL == p) {
    return -1;
  }

  printf("<<<< after remove %d and adjust color \n", key);
  p = find(root, key, true);
  print_path(p);

  return remove_node(root, p);
}

int main(int argc, char* argv[]) {
  struct node* root = NULL;
  srand(time(NULL));

  for (int i = 0; i < 200; ++i) {
    insert(root, rand() % 1000);
    insert(root, rand() % 1000);
    insert(root, rand() % 1000);
    insert(root, rand() % 1000);
    insert(root, rand() % 1000);
    while(remove_by_key(root, rand() % 1000));
  }
  return 0;
}

