#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h> //mmap

//vmaָ���ں˴������������������ڴ���������Զ���
static int led_mmap(struct file *file,
                        struct vm_area_struct *vma)
{
    //0.��cache���ܹر�,��֤�����ܹ���ʱ����Ĵ���
    vma->vm_page_prot = 
        pgprot_noncached(vma->vm_page_prot);
    
    //1.ֻ������һ���£���ַӳ��
    remap_pfn_range(vma, //����ָ��
                    vma->vm_start,//�û�������ʼ��ַ
                    0xE0200000>>12,//�����ַ
                    0x1000, //ӳ����ڴ��С
                    vma->vm_page_prot //��������
                    );
    return 0;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .mmap = led_mmap //��ַӳ��
};

static struct miscdevice led_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "myled",
    .fops = &led_fops
};

static int led_init(void)
{
    misc_register(&led_misc);
    return 0;
}

static void led_exit(void)
{
    misc_deregister(&led_misc);
}
module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
