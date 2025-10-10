#include <tk/tkernel.h>
#include <tm/tmonitor.h>

extern void monitor_init(void);
extern void monitor_start(void);
#include "version.h"

/* Fallbacks in case attributes are not defined by the port headers */
#ifndef TA_USERSTACK
#define TA_USERSTACK 0
#endif
#ifndef TA_FPU
#define TA_FPU 0
#endif

#ifdef TA_RNG0
#define MSENTINEL_TASK_ATTR (TA_HLNG | TA_RNG0 | TA_USERSTACK | TA_FPU)
#else
#define MSENTINEL_TASK_ATTR (TA_HLNG | TA_RNG3 | TA_USERSTACK | TA_FPU)
#endif

/* Explicit stack for the monitor task (8 KiB), 8-byte aligned */
static UB __attribute__((aligned(8))) app_stack[8192];

LOCAL void app_task(INT stacd, void *exinf);
LOCAL ID app_tskid;
LOCAL T_CTSK ctsk = {
    .itskpri = 10,
    .stksz   = sizeof(app_stack),
    .stkptr  = app_stack,
    .task    = app_task,
    /* Task attributes chosen based on platform: RNG0 if available, else RNG3 */
    .tskatr  = MSENTINEL_TASK_ATTR,
};

LOCAL void app_task(INT stacd, void *exinf) {
    tm_printf((UB*)"\n[%s %s] monitor task starting...\n", (UB*)MSENTINEL_NAME, (UB*)MSENTINEL_VERSION);

    monitor_init();
    monitor_start();

    /* Sleep forever; monitor runs its own periodic work */
    tk_slp_tsk(TMO_FEVR);
}

EXPORT INT usermain(void) {
    tm_printf((UB*)"%s %s built %s %s\n", (UB*)MSENTINEL_NAME, (UB*)MSENTINEL_VERSION, (UB*)MSENTINEL_BUILD_DATE, (UB*)MSENTINEL_BUILD_TIME);
    tm_printf((UB*)"\xB5T-Kernel Started (guardianRA)\n");

    app_tskid = tk_cre_tsk(&ctsk);
    tk_sta_tsk(app_tskid, 0);

    tk_slp_tsk(TMO_FEVR);
    return 0;
}
