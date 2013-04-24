#include <linux/syscalls.h>
#include <linux/thread_info.h>
#include <linux/list.h>

extern struct task_struct *find_task_by_vpid(pid_t nr);

void get_counts(struct task_struct *cur_ts, int *counts) {
	struct task_struct *child_ts;

	counts[0] += cur_ts->fork_count;
	counts[1] += cur_ts->vfork_count;
	counts[2] += cur_ts->execve_count;
	counts[3] += cur_ts->clone_count;

	list_for_each_entry(child_ts, &(cur_ts->children), sibling) {
		get_counts(child_ts, counts);
	}
}

// counts[0] => fork counts
// counts[1] => vfork counts
// counts[2] => execve counts
// counts[3] => clone counts
SYSCALL_DEFINE2 (get_fork_calls, int, pid, int *, counts) {
	counts[0] = 0;
	counts[1] = 0;
	counts[2] = 0;
	counts[3] = 0;
	
	get_counts(find_task_by_vpid((pid_t) pid), counts);

	return 0;
}

