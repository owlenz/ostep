#define __XDD_H__
#ifdef __XDD_H__

typedef struct {
  int xturn;
  int gturn;
} lock_t;

static lock_t xdd;
static int gg = 0;

void lock_init(lock_t *lock);
void *worker(void *num);
void set_affinity_to_core(int core_id);

#endif
