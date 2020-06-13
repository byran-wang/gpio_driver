#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define GPIO(X)   X
#define GPIO_IOC_MAGIC 'g'

/* general APIs - GPIO_IOC_MAGIC */
enum {
    IOC_OUTPUT_CLR,
    IOC_OUTPUT_SET,
    IOC_SET_INPUT,
};

#define GPIO_IOC_OUTPUT_LOW        _IOW(GPIO_IOC_MAGIC, IOC_OUTPUT_CLR, unsigned int)
#define GPIO_IOC_OUTPUT_HIG        _IOW(GPIO_IOC_MAGIC, IOC_OUTPUT_SET, unsigned int)
#define GPIO_IOC_INPUT            _IOR(GPIO_IOC_MAGIC, IOC_SET_INPUT, unsigned int)

#define DBG_PRINT (printf("simba: %s,%u \t", __FILE__, __LINE__), printf) 

int main(int argc, char **argv)
{
    int gpiofd = 0, gpio = 0;
    int gpio_state = 0;
    printf("argc = %d\n",argc);
    for (int i =0; i < 3; i++)
    {
        printf("argv[%d] = %s\n",i,argv[i]);
    }
    
    if (argc != 3) { 
        printf("Usage: gpio-pin <on/off>\n\n"); 
        printf("gpio test\n"); 
        exit(-1); 
    } 

    gpio = atoi(argv[1]);

    if ((gpiofd = open("/dev/gpiodrv", O_RDWR)) < 0) {
        perror("open");
        return -1;
    }
    DBG_PRINT("\n");
    DBG_PRINT("GPIO_OUT_LOW %ld\n",GPIO_OUT_LOW);
    DBG_PRINT("GPIO_OUT_HIG %ld\n",GPIO_OUT_HIG);
    DBG_PRINT("GPIO_INPUT %ld\n",GPIO_INPUT);
    if(strcmp(argv[2],"on")== 0)
    {
        DBG_PRINT("\n");
        gpio_state = GPIO_IOC_OUTPUT_HIG;
    }
    else if(strcmp(argv[2],"off")== 0)
    {
        DBG_PRINT("\n");
        gpio_state = GPIO_IOC_OUTPUT_LOW;
    }
    else
    {
        DBG_PRINT("\n");
        gpio_state = GPIO_IOC_INPUT;
    }

    DBG_PRINT("gpio_state = %d\n",gpio_state);
    if ((gpio_state = ioctl(gpiofd, gpio_state, gpio)) < 0) {
        DBG_PRINT("gpio_state = %d\n",gpio_state);
        perror("ioctl err");
        DBG_PRINT("gpio_state = %d\n",gpio_state);
        return -1;
    }
    DBG_PRINT("\n");

    printf("GPIO state:%d\n", gpio_state);
    close(gpiofd);

    return 0;
}