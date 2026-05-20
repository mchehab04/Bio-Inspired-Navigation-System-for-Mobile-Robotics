/*
 * Map a requested angle (deg) to the latest LDS-02 distance in cm.
 * f_laser calls this with deg in [-180, +180] for nb_values neurons. The older
 * LMS path used +90 offset for a 180° scanner; LDS-02 is 360°, so use +180. :contentReference[oaicite:6]{index=6}
 */

#include <Laser.h>
#include "tools/include/local_var.h"
/*#define DEBUG*/
#include <math.h>
#include <net_message_debug_dist.h>

long  laser_get_intensity_by_degres(Laser * laser,float deg)
{
    if (!laser || !laser->values || laser->nb_values <= 0) return 0;

    /* Convert [-180,180] → [0,360) */
    int idx = (int)lroundf(deg + 180.0f);
    idx %= 360;
    if (idx < 0) idx += 360;

    if (idx >= 0 && idx < laser->nb_values) {
        return laser->values[idx];
    }
    return 0;
}
