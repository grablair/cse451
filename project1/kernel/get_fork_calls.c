#include <linux/syscalls.h>
#include <linux/thread_info.h>
#include <linux/list.h>

extern struct task_struct *find_task_by_vpid(pid_t nr);
extern struct task_struct *find_task_by_pid_ns(pid_t nr,
		struct pid_namespace *ns);

void get_counts(struct task_struct *cur_ts, unsigned long *counts) {
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
SYSCALL_DEFINE1 (get_fork_calls, unsigned long *, counts) {
	counts[0] = 0;
	counts[1] = 0;
	counts[2] = 0;
	counts[3] = 0;
	
	get_counts(current, counts);

	return 0;
}

