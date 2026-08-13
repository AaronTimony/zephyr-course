#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(demo, LOG_LEVEL_DBG);

#define STACK_SIZE 1024

#define LOW_PRIO 7

#define MEDIUM_PRIO 5

#define HIGH_PRIO 3

#define COOP_PRIO -1

void t_low_fn (void *p1, void *p2, void *p3) {

    LOG_INF("LOW started.");

    for (int i = 0; i < 10; i++) {
        LOG_INF("T_LOW running step %d tick=%u", i, k_uptime_get_32());
        k_msleep(300);
    }

    LOG_INF("LOW Done.");
}

void t_coop_fn (void *p1, void *p2, void *p3) {

    LOG_INF("COOP started.");

    for (int i = 0; i < 5; i++) {
        LOG_INF("T_COOP running iteration %d tick=%u", i + 1, k_uptime_get_32());
        k_busy_wait(40000);
    }
    

    LOG_INF("Initial COOP loop done.");
    k_yield();

    LOG_INF("COOP Done.");
}


void t_med_fn(void *p1, void *p2, void *p3) {
    LOG_INF("MED started.");

    for (int i = 0; i < 8; i++) {
        LOG_INF("T_MED running  step %d tick=%u", i, k_uptime_get_32());
        k_msleep(200);

    }

    LOG_INF("MED Done.");
}

void t_high_fn(void *p1, void *p2, void *p3) {
    LOG_INF("HIGH started.");

    for (int i = 0; i < 8; i++) {
        LOG_INF("T_HIGH running  step %d tick=%u", i, k_uptime_get_32());
        k_msleep(100);

    }

    LOG_INF("HIGH Done.");
}

K_THREAD_DEFINE(time_low_fn, STACK_SIZE, t_low_fn, NULL, NULL, NULL, LOW_PRIO, 0, 0);
K_THREAD_DEFINE(time_med_fn, STACK_SIZE, t_med_fn, NULL, NULL, NULL, MEDIUM_PRIO, 0, 0);
K_THREAD_DEFINE(time_high_fn, STACK_SIZE, t_high_fn, NULL, NULL, NULL, HIGH_PRIO, 0, 0);
K_THREAD_DEFINE(time_coop_fn, STACK_SIZE, t_coop_fn, NULL, NULL, NULL, COOP_PRIO, 0, 0);

int main(void)
{
    LOG_INF("Begin Scheduling Competition");
    LOG_INF("COOP prio = %d LOW prio=%d HIGH prio=%d  MEDIUM prio=%d",
            COOP_PRIO, LOW_PRIO, HIGH_PRIO, MEDIUM_PRIO);
    return 0;
}
