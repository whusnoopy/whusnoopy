#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define MAX_CHILDREN 26

const char BASE_CHAR = 'a';
const int MAX_CHILDREN = 26;

struct node {
  bool is_word;
  char ch;
  int num;
  struct node* parent;
  struct node* children[MAX_CHILDREN];
};

inline int char_idx(char c) {
  return c - BASE_CHAR;
}

struct node* init_node(struct node* parent_node, char c) {
  struct node* new_node = (struct node*)malloc(sizeof(struct node));
  new_node->is_word = false;
  new_node->ch = c;
  new_node->num = 0;
  for (int i = 0; i < MAX_CHILDREN; ++i) {
    new_node->children[i] = NULL;
  }
  if (NULL == parent_node) {
    // init root
    new_node->parent = NULL;
  } else {
    // point to parent
    new_node->parent = parent_node;
    parent_node->children[char_idx(c)] = new_node;
  }
  return new_node;
}

struct node* insert(struct node* root, const char* word, int len) {
  struct node* curr_node = root;
  int idx = 0;
  for (int i = 0; i < len; ++i) {
    idx = char_idx(word[i]);
    if (NULL != curr_node->children[idx]) {
      curr_node = curr_node->children[idx];
    } else {
      curr_node = init_node(curr_node, word[i]);
    }
    curr_node->num++;
  }
  curr_node->is_word = true;
  return curr_node;
}

struct node* find(const struct node* root, const char* word, int len) {
  const struct node* curr_node = root;
  int idx = 0;
  for (int i = 0; i < len; ++i) {
    idx = char_idx(word[i]);
    if (NULL != curr_node->children[idx]) {
      curr_node = curr_node->children[idx];
    } else {
      return NULL;
    }
  }
  if (curr_node->is_word) {
    return (struct node*)curr_node;
  }
  return NULL;
}

int remove(struct node* root, const char* word, int len) {
  struct node* last = find(root, word, len);
  if (NULL == last) {
    return -1;
  }
  last->is_word = false;
  struct node* p = last->parent;
  while (NULL != p) {
    last->num--;
    if (0 == last->num) {
      int idx = char_idx(last->ch);
      p->children[idx] = NULL;
      free(last);
    }
    last = p;
    p = last->parent;
  }
  return 0;
}

int print_recursion(const struct node* p) {
  if (NULL == p->parent) {
    printf("ROOT (%d)", p->num);
    return 0;
  }
  print_recursion(p->parent);
  printf("-> %c (%c, %d)", p->ch, p->is_word ? 't' : 'f', p->num);
  return 0;
}

int print(const struct node* p) {
  if (NULL == p) {
    printf("NULL\n");
    return -1;
  }
  print_recursion(p);
  printf("\n");
  return 0;
}

int main(int argc, char* argv[]) {
  struct node* root = init_node(NULL, 0);
  struct node* p = NULL;
  p = insert(root, "hello", 5);
  print(p);
  p = insert(root, "helloworld", 10);
  print(p);
  p = insert(root, "halo", 4);
  print(p);
  p = find(root, "world", 5);
  print(p);
  p = find(root, "halo", 4);
  print(p);
  remove(root, "hello", 5);
  p = find(root, "hello", 5);
  print(p);
  return 0;
}

