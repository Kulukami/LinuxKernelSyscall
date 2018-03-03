# LinuxKernelSyscall
Adding linux kernel system call based on kernel-4.14.14

## Section 1: A way to add syscall in kernel-4.14.14.
### 1-1 Download kernel source code from kernel.org using aria2
```bash
sudo dnf install aria2
aria2c -x9 https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.14.14.tar.xz
```
### 1-2 Add syscall entry on system call tables.
```bash
vim arch/x86/entry/syscalls/syscall_64.tbl
#[ADD] 338	64	my_syscall	sys_my_syscall
```
### 1-3 Add syscall function at sys.c.
*I write a syscall function here,you can download sys.c and copy the function into kernel/sys.c*
```bash
cat sys.c >> kernel/sys.c
```
*Or you can write your own sys_my_syscall*
```c
asmlinkage long sys_my_syscall(int pid);
```
### 1-4 Compile and install the new kernel
```bash
make oldconfig
make -j12
sudo make modules_install
sudo make install
sudo reboot
```
*Choice linux-kernel-4.14.14 after reboot.*
### 1-5 Use the new syscall 338
```bash
gcc use.c -o use
./use
```
*Input a pid and see the result.*

## Section 2: What does my system call do?
### 2-1 Get the task_struct by pid
```c
struct task_struct *pid_task(struct pid *pid, enum pid_type type) {
	struct task_struct *result = NULL;
	if (pid) {
		struct hlist_node *first;
		first = rcu_dereference_check(hlist_first_rcu(&pid->tasks[type]),
		lockdep_tasklist_lock_is_held());
		if (first) result = hlist_entry(first, struct task_struct, pids[(type)].node);
	}
	return result;
}
```
### 2-2 Get the mm_struct infomation
```c
printk("\nCode Segment start = 0x%lx, end = 0x%lx, Size = %ld B\n"
	"Data Segment start = 0x%lx, end = 0x%lx, Size = %ld B\n"
	"Heap Segment Start = 0x%lx, end = 0x%lx, Size = %ld B\n"
	"Stack Segment start = 0x%lx\n"
	"Total physical pages maped = %ld, physical memory size = %ld KB\n"
	"PGD = 0x%lx\n",
	mm->start_code, mm->end_code, text,
	mm->start_data, mm->end_data, mm->data,
	mm->start_brk, mm->brk, mm->brk – mm→start_brk,
	mm->start_stack, mm→stack_vm*4,
	resident, resident*4,
	mm→pgd);
```
