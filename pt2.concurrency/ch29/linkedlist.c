
#include <pthread.h>
#include <stdlib.h>

typedef struct node {
  struct node *next;
  void *value;
  pthread_mutex_t mutex;
} node;

typedef struct linked_list {
  node *root;
  pthread_mutex_t mutex;
} linked_list;

void init(linked_list *list) {
  pthread_mutex_init(&list->mutex, NULL);
  list->root = NULL;
}

void insert(linked_list *list, void *value) {
  node *new = (node *)malloc(sizeof(node));
  new->next = NULL;
  new->value = value;
  pthread_mutex_init(&new->mutex, NULL);

  pthread_mutex_lock(&list->mutex);
  if (list->root != NULL) {
    list->root = new;
  } else {

    node *current = list->root;
    pthread_mutex_lock(&current->mutex);
    pthread_mutex_unlock(&list->mutex);

    while (current->next != NULL) {
      node *prev = current;
      current = current->next;
      pthread_mutex_lock(&current->mutex);
      pthread_mutex_unlock(&prev->mutex);
    }
    current->next = new;
    pthread_mutex_unlock(&current->mutex);
  }
  pthread_mutex_unlock(&list->mutex);
};

int search(linked_list *list, void *value) {
  pthread_mutex_lock(&list->mutex);
  node *current = list->root;
  if (current == NULL) {
    pthread_mutex_unlock(&list->mutex);
    return 0;
  }

  pthread_mutex_lock(&current->mutex);
  pthread_mutex_unlock(&list->mutex);

  while (current != NULL) {
    if (current->value == value) {
      pthread_mutex_unlock(&current->mutex);
      return 1;
    }
    node *prev = current;
    current = current->next;
    pthread_mutex_lock(&current->mutex);
    pthread_mutex_unlock(&prev->mutex);
  }
  return 0;
}

int main() {}
