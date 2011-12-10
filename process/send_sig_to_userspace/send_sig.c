#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
// #include <linux/slab.h> /* kmalloc() */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/signal.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "sig2pid"

/**
 * 送signal 到pid去
 */
static int send_sig_to_pid(int sig, pid_t pid)
{
    struct siginfo info;

    info.si_signo = sig;
    info.si_errno = 0;
    info.si_code = SI_USER; // sent by kill, sigsend, raise
    info.si_pid = get_current()->pid; // sender's pid
    info.si_uid = current_uid(); // sender's uid

    return kill_proc_info(sig, &info, pid);
}

/**
 * /proc/sig2pid的write ops
 */
static int
sig2pid_proc_write(struct file *file, const char __user * buffer,
                     unsigned long count, void *data)
{
    int sig, pid, ret;
    char line[count];
    ret = copy_from_user(line, buffer, count);

    if (ret) {
        return -EFAULT;
    }
    sscanf(line, "%d %d", &pid, &sig);
    printk("%s(#%d): pid(%d), sig(%d)\n",
            __func__, __LINE__, pid, sig);
    send_sig_to_pid(sig, (pid_t) pid);
    return count;
}

/**
 * 建立/proc/sig2pid
 */
static int create_proc_file(void)
{
    struct proc_dir_entry *p;
    p = create_proc_entry(PROC_NAME, S_IFREG | S_IWUGO, NULL);
    if (!p) {
        printk("%s(#%d): create proc entry failed\n", __func__, __LINE__);
        return -EFAULT;
    }
    p->write_proc = sig2pid_proc_write;
    return 0;
}

int sig2pid_init_module(void)
{
    return create_proc_file();
}

void sig2pid_exit_module(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(sig2pid_init_module);
module_exit(sig2pid_exit_module);

