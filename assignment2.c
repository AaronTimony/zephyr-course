#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdbool.h>

LOG_MODULE_REGISTER(demo, LOG_LEVEL_DBG);

#define STACK_SIZE 1024

#define THREAD_PRIO 5

#define INCREMENTS 1000000

static volatile uint32_t counter;

K_SEM_DEFINE(done_sem, 0, 2);

static K_MUTEX_DEFINE(counter_mutex);

void counter_func(void *p1, void *p2, void *p3) {


  for (int i = 0; i < INCREMENTS; i++) {
    k_mutex_lock(&counter_mutex, K_FOREVER);
    counter++;
    k_mutex_unlock(&counter_mutex);
  }

  LOG_INF("Finished counting on this thread");

  k_sem_give(&done_sem);
}

K_THREAD_DEFINE(worker_a, STACK_SIZE, counter_func, NULL, NULL, NULL, THREAD_PRIO, 0, 0);
K_THREAD_DEFINE(worker_b, STACK_SIZE, counter_func, NULL, NULL, NULL, THREAD_PRIO, 0, 0);

int main(void) {
  int64_t time = k_uptime_get();

  LOG_INF("Expected final val: %d", INCREMENTS * 2);

  k_sem_take(&done_sem, K_FOREVER);
  k_sem_take(&done_sem, K_FOREVER);

  /* Wait for both workers to complete */
  LOG_INF("Actual  final value: %u", counter);

  if (counter == INCREMENTS * 2) {
    LOG_WRN("No race this run");
  } else {
    LOG_ERR("Race condition confirmed: lost %d updates",
            (INCREMENTS * 2) - counter);
  }

  LOG_INF("Execution time: %lld ms", k_uptime_delta(&time));

  return 0;
}

