/*
 * EE202C Assignment 3: Effect of Cache on Energy
 *
 * Creates      /proc/mem_cpy
 * Description:
 *              1) Creates the proc entry '/proc/mem_cpy'
 *              2) Performs a memory copy using a 'n'KB data block
 *              3) The value of 'n' is provided using echo n > /proc/mem_cpy
 *              4) Energy calipers are used to measure the energy of the copy
 *
 * Author:      Digvijay Singh (digvijay@ucla.edu)
 *
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/msr.h>                    /* machine specific registers */
#include <asm/smp.h>                    /* for SMP/SMT support */
#include <linux/slab.h>                 /* kmalloc() and kfree() for kernel memory */


#define PROC_FILE_ENTRY "mem_cpy"

void * mem_src;                         /* Source memory for memory copy */
void * mem_dest;                        /* Destination of memory copy */

/* Time-Stamp Counter (TSC) values */
int long long tsc_start, tsc_end;

/* Repeatedly perform a memory copy using a 'n'KB block for a total of 1GB */
static int mem_copy(struct file *file, const char *user_buffer,
        unsigned long buf_count, void *data)
{
        int n, i, core;
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

        if(n<=0||n>1024)
                printk("Invalid value for block size\n");
        else
        {
                /* Get needed kernel memory with high-priority */
                mem_src = kmalloc(n*1024, GFP_ATOMIC);
                if(!mem_src)
                {
                        printk(KERN_DEBUG "Couldn't allocate kernel memory\n");
                        return -ENOMEM;
                }
                mem_dest = kmalloc(n*1024, GFP_ATOMIC);
                if(!mem_dest)
                {
                        kfree(mem_src);
                        printk(KERN_DEBUG "Couldn't allocate kernel memory\n");
                        return -ENOMEM;
                }

                /* Get CPU core number (necessary for multi-core architectures) */
                core = smp_processor_id();

                /* Read current TSC value - STARTING CALIPER */
                rdtscll(tsc_start);

                /* Memory-copy loop */
                for(i=0; i<(1024*1024)/n; i++)
                {
                        memcpy(mem_dest,mem_src,n*1024);
                }

                /* Read current TSC value - ENDING CALIPER */
                rdtscll(tsc_end);

                /* Report Kernel caliper TSC values */
                printk(KERN_DEBUG "Kernel_Calipers Core = %d TSC_Start = %llu TSC_End = %llu\n", core, tsc_start, tsc_end);

                /* Free kernel memory */
                kfree(mem_src);
                kfree(mem_dest);
        }

return buf_count;
}

/* Initialize the kernel module by creating relevant proc file entry */
static int __init mem_init(void)
{
        struct proc_dir_entry *proc_mem;

        proc_mem = create_proc_entry(PROC_FILE_ENTRY, 0644, NULL);
        if (!proc_mem)
        {
                printk(KERN_DEBUG "Cannot create /proc/mem_cpy\n");
                return -ENOMEM;
        }

        proc_mem->write_proc = mem_copy;
        printk("Memory copy module initialized\n");
return 0;
}

/* Remove kernel module and proc file entry */
static void __exit mem_exit(void)
{
        remove_proc_entry(PROC_FILE_ENTRY, 0);
        printk("Memory copy module removed\n");
}

module_init(mem_init);
module_exit(mem_exit);
MODULE_LICENSE("GPL");
