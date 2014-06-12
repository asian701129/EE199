/*
 * EE202C Assignment 2: Introduction to Linux Kernel Modules
 *
 * Creates      /proc/fib
 * Description:-
 *              1) Creates the proc entry '/proc/fib'
 *              2) Calculates the nth fibonacci number
 *              3) The value of 'n' is provided using echo n > /proc/fib
 *              4) Output is printed to the kernel log
 *
 * Author:      Digvijay Singh (digvijay@ucla.edu)
 *
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <asm/uaccess.h>

#define PROC_FILE_ENTRY "fib"

int fib_rec( int n)
{
        int i,fib1=1,fib2=1,tmp;

        for(i=1;i<n;i++)
        {
                tmp = fib1;
                fib1 = fib2;
                fib2 = tmp + fib2;
        }

return fib1;
}


/* Calculate the nth fibonacci number and print it to the kernel log */
static int fib_calculate(struct file *file, const char *user_buffer,
        unsigned long buf_count, void *data)
{
        int n, i, fib1=0, fib2=1, tmp;
        char kernel_buf[10];                    /* Expect max 10 chars */
        char *endp;

        if (buf_count > sizeof(kernel_buf))
                return -EINVAL;

        /* Copy data from user-space via proc file entry */
        if (copy_from_user(kernel_buf, user_buffer, buf_count))
        {
                printk ("Copy from user error\n");
                return -EFAULT;
        }

        n  = (int)(simple_strtoul(kernel_buf, &endp, 10));
        if (*endp != '\n')
        {
                return -EINVAL;
        }

        if(n<=0)
                printk("Invalid value entered\n");
        else
        {
                fib1=fib_rec(n);
                printk("The %dth Fibonacci number is: %d\n",n,fib1);

        }

return buf_count;
}

/* Initialize the kernel module by creating relevant proc file entry */
static int __init fib_init(void)
{
        struct proc_dir_entry *proc_fib;

        proc_fib = create_proc_entry(PROC_FILE_ENTRY, 0644, NULL);
        if (!proc_fib)
        {
                printk(KERN_DEBUG "Cannot create /proc/fib\n");
                return -ENOMEM;
        }

        proc_fib->write_proc = fib_calculate;
        printk("Fibonacci calculation module initialized\n");
return 0;
}

/* Remove kernel module and proc file entry */
static void __exit fib_exit(void)
{
        remove_proc_entry(PROC_FILE_ENTRY, 0);
        printk("Fibonacci calculation module removed\n");
}

module_init(fib_init);
module_exit(fib_exit);
MODULE_LICENSE("GPL");
