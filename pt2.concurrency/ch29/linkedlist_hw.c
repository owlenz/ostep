#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *next;
  pthread_mutex_t lock;
} node;

typedef struct list {
  node *root;
  pthread_mutex_t lock;
} list;

struct lol {
  int *arr;
  int num;
};

static list llist;

void init(list *list) {
  pthread_mutex_init(&list->lock, NULL);
  list->root = NULL;
}

void insert(list *list, int value) {
  node *new = malloc(sizeof(node));
  new->value = value;
  pthread_mutex_init(&new->lock, NULL);
	new->next = list->root;
	pthread_mutex_lock(&list->lock);
	list->root = new;
	pthread_mutex_unlock(&list->lock);
}

node *search(list *list, int value) {
  pthread_mutex_lock(&list->lock);
  node *current = list->root;
  if (current == NULL) {
    pthread_mutex_unlock(&list->lock);
    return 0;
  }
  pthread_mutex_lock(&current->lock);
  pthread_mutex_unlock(&list->lock);
  while (current != NULL) {
    if (current->value == value) {
      pthread_mutex_unlock(&current->lock);
      return current;
    }
    node *old = current;
    current = current->next;
    if (current != NULL) {
      pthread_mutex_lock(&current->lock);
    }
    pthread_mutex_unlock(&old->lock);
  }
  return NULL;
}

void *worker(void *value) {
  struct lol *lmao = (struct lol *)value;
  for (int i = 0; i < 1000; i++) {
    int x = lmao->num * i;
    if (x > 1000)
      break;
    /*printf("%d\n", lmao->arr[lmao->num * i]);*/
    insert(&llist, lmao->arr[lmao->num * i]);
  }
  return NULL;
}

int main() {
  init(&llist);
  pthread_t a, b, c;
  FILE *file = fopen("./test.txt", "r");
  int *x = malloc(sizeof(int) * 1000);
  int i = 0;
  while (fscanf(file, "%d", &x[i]) == 1) {
    /*printf("%d\n", x[i]);*/
    i++;
  }

  struct lol y = {x, 1}, z = {x, 2}, k = {x, 3};
  pthread_create(&a, NULL, worker, (void *)&y);
  pthread_create(&b, NULL, worker, (void *)&z);
  pthread_create(&c, NULL, worker, (void *)&k);
  pthread_join(a, NULL);
  pthread_join(b, NULL);
  pthread_join(c, NULL);
  node *current = llist.root;
  for (int i = 0; i < 1000; i++) {
    printf("%d\n", current->value);
    if (current->next == NULL)
      break;
    current = current->next;
  }
}
