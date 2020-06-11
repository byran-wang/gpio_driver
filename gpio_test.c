#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

 /****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

#define DBG_PRINT (printf("simba: %s,%u \t", __FILE__, __LINE__), printf) 

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);

    return 0;
}

// /****************************************************************
//  * gpio_unexport
//  ****************************************************************/
// int gpio_unexport(unsigned int gpio)
// {
//     int fd, len;
//     char buf[MAX_BUF];

//     fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
//     if (fd < ) {
//         perror("gpio/export");
//         return fd;
//     }

//     len = snprintf(buf, sizeof(buf), "%d", gpio);
//     write(fd, buf, len);
//     close(fd);
//     return ;
// }

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf),"/sys/class/gpio/gpio%d/direction", gpio);
    DBG_PRINT("\n");
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/direction");
        return fd;
    }
    DBG_PRINT("\n");

    if (out_flag)
        write(fd, "out", sizeof("out"));
    else
        write(fd, "in", sizeof("in") );

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf),  "/sys/class/gpio/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-value");
        return fd;
    }

    if (value)
        write(fd, "1", sizeof("1"));
    else
        write(fd, "0", sizeof("0"));

    close(fd);
    return 0;
}

// /****************************************************************
//  * gpio_get_value
//  ****************************************************************/
// int gpio_get_value(unsigned int gpio, unsigned int *value)
// {
//     int fd, len;
//     char buf[MAX_BUF];
//     char ch;

//     len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

//     fd = open(buf, O_RDONLY);
//     if (fd < ) {
//         perror("gpio/get-value");
//         return fd;
//     }

//     read(fd, &ch, );

//     if (ch != '0') {
//         *value = ;
//     } else {
//         *value = ;
//     }

//     close(fd);
//     return ;
// }


// /****************************************************************
//  * gpio_set_edge
//  ****************************************************************/

// int gpio_set_edge(unsigned int gpio, char *edge)
// {
//     int fd, len;
//     char buf[MAX_BUF];

//     len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

//     fd = open(buf, O_WRONLY);
//     if (fd < ) {
//         perror("gpio/set-edge");
//         return fd;
//     }

//     write(fd, edge, strlen(edge) + );
//     close(fd);
//     return ;
// }

// /****************************************************************
//  * gpio_fd_open
//  ****************************************************************/

// int gpio_fd_open(unsigned int gpio)
// {
//     int fd, len;
//     char buf[MAX_BUF];

//     len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

//     fd = open(buf, O_RDONLY | O_NONBLOCK );
//     if (fd < ) {
//         perror("gpio/fd_open");
//     }
//     return fd;
// }

// /****************************************************************
//  * gpio_fd_close
//  ****************************************************************/

// int gpio_fd_close(int fd)
// {
//     return close(fd);
// }

/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp)
{
    // struct pollfd fdset[2];
    // int nfds = ;
    // int gpio_fd, timeout, rc;
    // char *buf[MAX_BUF];
    unsigned int gpio;
    // int len;



    // if (argc < ) {
    //     printf("Usage: gpio-int <gpio-pin>\n\n");
    //     printf("Waits for a change in the GPIO pin voltage level or input on stdin\n");
    //     exit(-1);
    // }

    // gpio = atoi(argv[]);
    gpio = 17;

    gpio_export(gpio);
    gpio_set_dir(gpio, 1);
    
    while (1)
    {
        gpio_set_value(gpio,1);
        usleep(1000);
        gpio_set_value(gpio,0);
        usleep(49000);
    }
    // gpio_set_edge(gpio, "rising");
    // gpio_fd = gpio_fd_open(gpio);

    // timeout = POLL_TIMEOUT;

    // while () {
    //     memset((void*)fdset, , sizeof(fdset));

    //     fdset[].fd = STDIN_FILENO;
    //     fdset[].events = POLLIN;

    //     fdset[].fd = gpio_fd;
    //     fdset[].events = POLLPRI;

    //     rc = poll(fdset, nfds, timeout);     

    //     if (rc < ) {
    //         printf("\npoll() failed!\n");
    //         return -1;
    //     }

    //     if (rc == ) {
    //         printf(".");
    //     }

    //     if (fdset[].revents & POLLPRI) {
    //         len = read(fdset[].fd, buf, MAX_BUF);
    //         printf("\npoll() GPIO %d interrupt occurred\n", gpio);
    //     }

    //     if (fdset[].revents & POLLIN) {
    //         (void)read(fdset[].fd, buf, );
    //         printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[]);
    //     }

    //     fflush(stdout);
    // }

    // gpio_fd_close(gpio_fd);
    return 0;
}