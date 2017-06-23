#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int fd;
    void *addr;
    unsigned long *gpiocon, *gpiodata;

    if(argc != 2) {
        printf("Usage: %s <on|off>\n", argv[0]);
        return -1;
    }

    fd = open("/dev/myled", O_RDWR);
    if (fd < 0)
        return -1;

    //��LED�������ַӳ���û��������ַ��
    //Ӧ��mmap->...->����mmap
    //addr(vm_start)<----->0xE0200000
    addr = mmap(0, 0x1000, PROT_READ|PROT_WRITE,
                    MAP_SHARED, fd, 0);
    //����Ĵ��������ַ��Ӧ���û������ַ
    gpiocon = (unsigned long *)(addr + 0x80);
    gpiodata = (unsigned long *)(addr + 0x84);

    //����Ϊ�����
    *gpiocon &= ~((0xf << 12) | (0xf << 16));
    *gpiocon |= ((1 << 12) | (1 << 16));
   
    //����״̬
    if(!strcmp(argv[1], "on"))
        *gpiodata |= ((1 << 3) | (1 << 4));
    else
        *gpiodata &= ~((1 << 3) | (1 << 4));

    munmap(addr, 0x1000);
    close(fd);
    return 0;
}






