#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int adjust_color(struct node* curr_node) {
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
    adjust_color(grandparent);
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
  printf("==== will insert %d under\n    ", key);
  print_path(parent);
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

  printf(">>>> before adjust\n ");
  print_path(new_node);

  // adjust color
  adjust_color(new_node);

  // the root may be changed during adjust
  while (root != NULL && root->parent != NULL) {
    root = root->parent;
  }

  printf("<<<< after adjust \n");
  print_path(new_node);

  return new_node;
}

int remove(struct node* root, int key) {
  struct node* remove_node = find(root, key);
  if (NULL == remove_node) {
    return -1;
  }
}

int main(int argc, char* argv[]) {
  struct node* root = NULL;

  root = insert(root, 64);
  struct node* p = NULL;
  
  // test root init
  print_path(root);

  p = insert(root, 72);

  p = insert(root, 32);

  p = insert(root, 90);

  p = insert(root, 74);

  p = insert(root, 86);

  p = insert(root, 23);

  p = insert(root, 13);

  p = insert(root, 12);

  p = insert(root, 9);

  p = insert(root, 3);

  return 0;
}

