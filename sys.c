// sys.c in kernel-4.14-14/kernel/sys.c
// add this function in sys.c

asmlinkage long sys_my_syscall(int pid){

    struct task_struct      *task_target = NULL;
    struct mm_struct        *mm;
    struct vm_area_struct   *vma;
    int count = 0;

    task_target = pid_task(find_vpid(pid), PIDTYPE_PID);
    mm = task_target->mm;

    printk("\nPID:%d Process Name:%s\n", pid, task_target->comm);
    printk("\nThis mm_struct has %d vmas.\n", mm->map_count);

    for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
            printk("\nVma number %d: \n", ++count);
            printk("  Starts: 0x%lx, Ends: 0x%lx, Size:%ld KB, Page_prot: 0x%lx， Flags: 0x%lx\n",
                      vma->vm_start, vma->vm_end, (vma->vm_end - vma->vm_start) / 1024,vma->vm_page_prot , vma->vm_flags);
    }

    unsigned long shared = get_mm_counter(mm, MM_FILEPAGES) + get_mm_counter(mm, MM_SHMEMPAGES);
    unsigned long text = (PAGE_ALIGN(mm->end_code) - (mm->start_code & PAGE_MASK)) >> (PAGE_SHIFT - 2);
    unsigned long data = mm->data_vm * 4;
    unsigned long resident = shared + get_mm_counter(mm, MM_ANONPAGES);
    //Resident file mapping pages + Resident shared memory pages + Resident anonymous pages

    printk("\nCode  Segment start = 0x%lx, end = 0x%lx, Size = %ld KB\n"
             "Data  Segment start = 0x%lx, end = 0x%lx, Size = %ld KB\n"
             "Heap  Segment Start = 0x%lx, end = 0x%lx, Size = %ld KB\n"
             "Stack Segment Start = 0x%lx, Size = %ld KB\n"
             "Total VM mapped pages = %ld, Size = %ld KB\n"
             "Total Physical mapped pages = %ld, Size = %ld KB\n"
             "PGD = 0x%lx\n",
             mm->start_code, mm->end_code, text,
             mm->start_data, mm->end_data, data,
             mm->start_brk, mm->brk, mm->brk - mm->start_brk,
             mm->start_stack, mm->stack_vm*4,
             mm->total_vm, mm->total_vm*4,
             resident, resident*4,
             mm->pgd);
    return 0;
}
