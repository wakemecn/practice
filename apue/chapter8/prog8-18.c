/*
 * Filename:      prog8-18.c
 * Author:        Junwei Wang(wakemecn@gmail.com)
 * Last Modified: 2012-02-29 18:58
 * Description:
 *
 */
#include "apue.h"
#include <sys/times.h>

static void pr_times(clock_t,	struct tms *, struct tms *);
static void do_cmd(char *);

int main(int argc, char *argv[])
{
	int	i;

	setbuf(stdout, NULL);
	for (i = 1; i < argc; i++)
		do_cmd(argv[i]);
	exit(0);
}

static void do_cmd(char *cmd) 
{
	struct tms	tms_start, tms_end;
	clock_t		start, end;
	int		status;

	printf("\ncommand: %s\n", cmd);
	
	if ((start = time(&tms_start)) == -1)
		err_sys("times error");
	if ((status = system(cmd)) < 0)
		err_sys("system() error");
	if ((end = time(&tms_end)) == -1)
		err_sys("times error");

	pr_times(end - start, &tms_start, &tms_end);
	pr_exit(status);
}

static void pr_times(clock_t real, struct tms *start, struct tms *end)
{
	static long	clktck = 0;

	if (clktck == 0)
		if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
			err_sys("sysconf error");

	printf("  real:  %7.2f\n", real/(double)clktck);
	printf("  user:  %7.2f\n", 
			(end->tms_utime-start->tms_utime) / (double)clktck);
	printf("  sys:  %7.2f\n", 
			(end->tms_stime-start->tms_stime) / (double)clktck);
	printf("  child user:  %7.2f\n", 
			(end->tms_cutime-start->tms_cutime) / (double)clktck);
	printf("  child sys:  %7.2f\n", 
			(end->tms_cstime-start->tms_cstime) / (double)clktck);
}
