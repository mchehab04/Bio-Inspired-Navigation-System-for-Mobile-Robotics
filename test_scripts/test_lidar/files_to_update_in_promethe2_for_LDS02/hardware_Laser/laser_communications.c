#include <Laser.h>
#include "tools/include/local_var.h"

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define LDS_HEADER     0x54
#define LDS_PACKET_LEN 44

static struct termios OLDTERMIOS;

/* Small helper: open a serial port in blocking mode at 115200 8N1 */
static int open_serial_115200(const char *dev) {
    int fd = open(dev, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("open_serial_115200: open");
        return -1;
    }

    struct termios tio;
    if (tcgetattr(fd, &OLDTERMIOS) != 0) {
        perror("open_serial_115200: tcgetattr");
        close(fd);
        return -1;
    }
    memset(&tio, 0, sizeof(tio));
    cfsetispeed(&tio, B115200);
    cfsetospeed(&tio, B115200);

    tio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    tio.c_iflag = IGNPAR;
    tio.c_oflag = 0;
    tio.c_lflag = 0;
    tio.c_cc[VTIME] = 1;  /* 0.1s per byte */
    tio.c_cc[VMIN]  = 1;  /* block for at least one byte */

    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &tio) != 0) {
        perror("open_serial_115200: tcsetattr");
        close(fd);
        return -1;
    }
    return fd;
}

/* For LDS-02 we ignore LMS "modes" and simply open the device.
 * Signature retained to satisfy existing callers from laser_init_USE_SICK_LMS(). :contentReference[oaicite:0]{index=0}
 */
int connectToLMS(int range_mode, int res_mode, int unit_mode, char *port, int baud_sel)
{
    (void)range_mode; (void)res_mode; (void)unit_mode; (void)baud_sel;

    Laser *laser = laser_get_first_laser();
    if (!laser) {
        fprintf(stderr, "connectToLMS: no laser instance\n");
        return -1;
    }
    const char *dev = (laser->port && laser->port[0]) ? laser->port : "/dev/ttyUSB0";
    int fd = open_serial_115200(dev);
    if (fd < 0) return -1;
    return fd;
}

/* Read exactly ONE LDS-02 frame (44 bytes beginning with 0x54).
 * Returns 44 on success, 0 on failure. Called by laser_get_values(). :contentReference[oaicite:1]{index=1}
 */
int readLMSdata(int fd, unsigned char* buf)
{
    if (fd < 0 || !buf) return 0;

    unsigned char b;
    ssize_t n;
    int found = 0;
    size_t bytes_read = 0;

    /* Drain any stale bytes — ensures we always get the latest frame */
    tcflush(fd, TCIFLUSH);

    /* Hunt for 0x54 header */
    while (1) {
        n = read(fd, &b, 1);
        if (n <= 0) return 0;

        if (b == 0x54) {            /* Found header */
            buf[0] = b;
            found = 1;
            break;
        }
    }

    if (!found) return 0;

    /* Read the rest of the packet (43 bytes) */
    while (bytes_read < 43) {
        n = read(fd, buf + 1 + bytes_read, 43 - bytes_read);
        if (n <= 0) return 0;
        bytes_read += (size_t)n;
    }

    return 44;
}

/* LMS status handler existed in original code path; keep a no-op so callers still link. */
void chkstatus(unsigned char c) { (void)c; }
