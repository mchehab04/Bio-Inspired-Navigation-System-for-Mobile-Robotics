#include <Laser.h>
#include "tools/include/local_var.h"
/*#define DEBUG*/

static inline float normalize360(float a) {
    while (a < 0.0f)    a += 360.0f;
    while (a >= 360.0f) a -= 360.0f;
    return a;
}

/* LDS packet layout (44 bytes):
 * [0]   = 0x54
 * [1]   = length (commonly 0x2C)
 * [2..3]  speed (LSB..MSB, /64 deg/s)
 * [4..5]  startAngle (LSB..MSB) in 0.01 deg
 * [6..41]  12 * { distance(2B, LSB..MSB, in mm), confidence(1B) }
 * [42..43] endAngle (LSB..MSB) in 0.01 deg
 * [44..?]  timestamp, checksum (not strictly needed here)
 */

void laser_get_values(Laser *laser)
{
    int fd = laser->fd;
    if (fd < 0) return;

    unsigned char buf[44];
    //int got = readLMSdata(fd, buf);    /* now returns exactly 44 on success */
    int got = 0;    //now returns exactly 44 on success 
    for (int tries=0; tries<5 && got!=44; ++tries){
        got = readLMSdata(laser->fd,buf);
        usleep(2000);
        
    }
    if (got != 44) return;

    /* Angles in hundredths of degrees */
    float start_angle = (float)((buf[4]  | (buf[5]  << 8))) / 100.0f;
    float end_angle   = (float)((buf[42] | (buf[43] << 8))) / 100.0f;

    /* Shortest angular delta (handle wraparound) */
    float delta = end_angle - start_angle;
    if (delta < -180.0f) delta += 360.0f;
    if (delta >  180.0f) delta -= 360.0f;

    /* 12 samples per packet: write into degree bins (0..359) in **cm**.
     * (Laser.h documents values as "valeur en cm".)
     */
    for (int j = 0; j < 12; ++j) {
        int off = 6 + j * 3;
        unsigned short dist_mm = (unsigned short)(buf[off] | (buf[off + 1] << 8));
        float t = (float)j / 11.0f;                 /* 0..1 interpolation along arc */
        //float ang = normalize360(start_angle + delta * t);
        float ang = start_angle + (delta/11.0f)*j;
        if (ang<0.0f) ang+=360.0f;
        if (ang>=360.0f) ang-=360.0f;

        int idx = (int)(ang + 0.5f);                /* nearest integer degree */

        if (idx >= 0 && idx < laser->nb_values && laser->values) {
            long dist_cm = (long)((dist_mm + 5) / 10); /* round mm→cm */
            laser->values[idx] = dist_cm;
        }

#ifdef DEBUG
        /* printf("j=%d ang=%.2f idx=%d dist_mm=%u\n", j, ang, idx, dist_mm); */
#endif
    }
}
