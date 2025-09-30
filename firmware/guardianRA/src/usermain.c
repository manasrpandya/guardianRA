#include <tk/tkernel.h>
#include <tm/tmonitor.h>

extern void monitor_init(void);
extern void monitor_start(void);
#include "version.h"

LOCAL void app_task(INT stacd, void *exinf);
LOCAL ID app_tskid;
LOCAL T_CTSK ctsk = {
    .itskpri = 10,
    .stksz   = 4096,
    .task    = app_task,
    .tskatr  = TA_HLNG | TA_RNG3,
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
