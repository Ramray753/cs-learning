 /* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and login ID here>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */
sigset_t mask, prev;

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/* Wrappers for Unix process control functions */
pid_t Fork(void);
pid_t Waitpid(pid_t pid, int *iptr, int options);
int Sigsuspend(const sigset_t *set);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
void Sigemptyset(sigset_t *set);
void Sigfillset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Setpgid(pid_t pid, pid_t pgid);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
            case 'h':             /* print help message */
                usage();
                break;
            case 'v':             /* emit additional diagnostic info */
                verbose = 1;
                break;
            case 'p':             /* don't print a prompt */
                emit_prompt = 0;  /* handy for automatic testing */
                break;
            default:
                usage();
	    }
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */ 
    char buf[MAXLINE]; /* Holds modified command line */ 
    int bg; /* Should the job run in bg or fg? */
    pid_t pid; /* Process id */
    
    // Initialize signal block mask
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL) 
        return; /* Ignore empty lines */
    
    if (!builtin_cmd(argv)) { 
        Sigprocmask(SIG_BLOCK, &mask, &prev); /* Block SIGCHLD */
        if ((pid = Fork()) == 0) { /* Child runs user job */ 
            Sigprocmask(SIG_SETMASK, &prev, NULL); /* Unblock SIGCHLD */
            Setpgid(0, 0); /* When you run your shell from the standard Unix shell, 
                              your shell is running in the foreground process group. 
                              If your shell then creates a child process, by default 
                              that child will also be a member of the foreground process 
                              group. Since typing ctrl-c sends a SIGINT to every process
                              in the foreground group, typing ctrl-c will send a SIGINT
                              to your shell, as well as to every process that your 
                              shell created, which obviously isn’t correct. */
            if (execve(argv[0], argv, environ) < 0) { 
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }
        // Initialize current job
        addjob(jobs, pid, bg ? BG : FG, cmdline); /* Add the child to the job list */
        Sigprocmask(SIG_SETMASK, &prev, NULL); /* Unblock SIGCHLD */
        /* Parent waits for foreground job to terminate */
        if (!bg) { 
            waitfg(pid);
        }
        else 
            printf("[%d] (%d) %s",pid2jid(pid), pid, cmdline);
    }
    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
                                /* array只会初始化一次，而且array始终指向这个数组的开头 */
                                /* argv储存的就是array里面的指针，由于被static修饰，会储存在Data区域，出栈后不会消失 */
    char *buf = array;          /* ptr that traverses command line */
                                /* 由于array是不变的，所以每一次buf都会重新指向数组开头 */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */
 
    strcpy(buf, cmdline);       /* strcpy会讲src的 '/0' 一起复制 */
                                /* 每一次都会复制到array开头 */
    buf[strlen(buf)-1] = ' ';   /* replace trailing '\0' with space 
                                   strlen的结果不会包含 '\0' */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	    buf++;                    /* 如果*buf是 '\0' 那么buf就停留在这个字符串 */
                                  /* 后面的判断也会都变成False */

    /* Build the argv list */
    /* 寻找下一个分隔符的指针，如果开头字符是单引号，那么中途的空格都会忽视，否则第一个出现的空格为分隔符 */
    /* 如果命令只有一个Token，那么就没有空格，delim会返回NULL */
    argc = 0;
    if (*buf == '\'') { 
        buf++; /* 单引号后面的字符才是命令本身 */
        delim = strchr(buf, '\'');
    }
    else {
	    delim = strchr(buf, ' ');
    }

    /* 如果出现了分隔符，那么寻找下一个分隔符 */
    while (delim) {
        argv[argc++] = buf;       /* 保存上一个Token的起始指针*/
        *delim = '\0';            /* 把分隔符替换成 '/0' */
        buf = delim + 1;          /* 从分隔符下一位开始找下一个Token */
	    
        while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;                     /* 如果没有Token， buf会指向'\0'  */

        if (*buf == '\'') {
            buf++;
            delim = strchr(buf, '\'');
        }
        else {
            delim = strchr(buf, ' ');
        }
    }

    argv[argc] = NULL;           /* argv是Null-terminated字符串数组 
                                    若果没有Token，argc=0，argv一开始就是NULL */
    
    if (argc == 0)  /* ignore blank line */
	    return 1;   /* 1 */

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL; /* 去掉& */
    }
    return bg; /* 1代表是后台，0代表不是后台 */
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) 
{
    if (strcmp(argv[0], "fg") == 0 || strcmp(argv[0], "bg") == 0){
        do_bgfg(argv);
        return 1;
    }
    if (strcmp(argv[0], "quit") == 0){ /* 需要结束所有进程后才能退出 */
        for (int i = 0; i < MAXJOBS; i++)
            if (jobs[i].pid)
                kill(-jobs[i].pid, SIGKILL);
        exit(0);
    }
    if (strcmp(argv[0], "jobs") == 0){
        listjobs(jobs);
        return 1;
    }
    return 0;     /* not a builtin command */
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    if(argv[1] == NULL){
        printf("%s command requires PID or %%jobid argument\n",argv[0]);
        return;
    }
    int id;
    int isJobID = 0;
    char *cp = argv[1];
    if (*cp == '%'){
        isJobID = 1;
        cp++;
    }
    id = atoi(cp);
    if (id == 0){
        printf("%s: argument must be a PID or %%jobid\n",argv[0]);
        return;
    }
    struct job_t *job;
    if (isJobID){
        job = getjobjid(jobs, id); 
    } else {
        job = getjobpid(jobs, id); 
    }
    if (job==NULL){
        printf("%s: No such %s\n", argv[1], isJobID ? "job" : "process");
        return;
    }
    kill(-job->pid, SIGCONT);
    if (argv[0][0] == 'b'){
        job->state = BG;
        printf("[%d] (%d) %s",job->jid, job->pid, job->cmdline);
    } else {
        job->state = FG;
        waitfg(-job->pid);
    }
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
    /* 所有的同步操作需要交给Handler，waitfg只需要判断是否reap成功 */
    /* It is simpler to do all reaping in the handler. */
    /* 传入的pid一定是前台任务，因此，只需要判断当前前台任务是否和pid一致即可*/ 
{
    /* Wait for SIGCHLD to be received */
    /* 由于使用pause会导致race：在判断和pause之前被SIGCHLD信号打断，导致pause会无限阻塞
        我们使用sigsuspend来避免race */
    Sigprocmask(SIG_BLOCK, &mask, &prev);
    while (pid == fgpid(jobs))
        Sigsuspend(&prev);
    Sigprocmask(SIG_SETMASK, &prev, NULL);
    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    int olderrno = errno;
    pid_t pid;
    int status;
    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0){
        /* 只要有子进程结束/暂停，SIGCHLD信号就会接收，但是waitpid方法默认
            1. 只能同步结束的进程
            2. 如果有没有结束的进程那就必须阻塞
            由于处理结束的进程必须使用while，这样会导致sigchld_handler一直等待所有子进程的结束
        解决方法：在waitpid方法中添加参数option
            1. WNOHANG：如果没有结束的进程，会结束阻塞。sigchld_handler被触发，
                    因此至少有一个子进程是结束的，所以其实至少同步一次
            2. WUNTRACED：除了结束的进程，暂停的进程也能结束阻塞。
            3. WNOHANG | WUNTRACED：上面两个特性同时生效
        在通过status判断子进程同步的类型：
            1. WIFEXITED(status)：正常结束。
            2. WIFSTOPPED(status)：暂停。 WSTOPSIG(status)返回被导致暂停的信号
            3. WIFSIGNALED(status)：被其他信号终止。 WTERMSIG(status)返回导致终止的信号
        */
        if (WIFEXITED(status)){
            deletejob(jobs, pid); // 正常结束，删除即可
                                  // 注意，对于正常推的子进程，不需要终止子子进程（应该交给子进程自行决定）
        }
        if (WIFSTOPPED(status)){ // 暂停，需要更改任务状态到ST
            struct job_t *job = getjobpid(jobs, pid);
            int jid = pid2jid(pid);
            printf("Job [%d] (%d) stopped by signal %d\n",jid, pid, WSTOPSIG(status)); // NOT SAFE!!!
            job->state = ST;
        } if(WIFSIGNALED(status)){
            int jid = pid2jid(pid);
            printf("Job [%d] (%d) terminated by signal %d\n",jid, pid, WTERMSIG(status)); // NOT SAFE!!!
            deletejob(jobs, pid);
        }
        
    }
    errno = olderrno;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
    /* sigint_handler只需要向子进程发送信号即可 
        不需要关心任务队列状态 */
{
    int olderrno = errno;
    pid_t fg_pid = fgpid(jobs);
    if (fg_pid){
        kill(-fg_pid,sig); /* 使用-fg_gid可以把信号发送给进程组内的所有进程
                            If pid is less than -1, then sig is sent to 
                            every process in the process group whose ID is -pid. */
    }
    errno = olderrno;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
    /* sigtstp_handler只需要向子进程发送信号即可 
        不需要关心任务队列状态 */
{
    int olderrno = errno;
    pid_t fg_pid = fgpid(jobs);
    if (fg_pid){
        kill(-fg_pid, sig); /* 使用-fg_gid可以把信号发送给进程组内的所有进程
                            If pid is less than -1, then sig is sent to 
                            every process in the process group whose ID is -pid. */
    }
    errno = olderrno;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid > max)
            max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		    nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
        }
        return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}


/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
	unix_error("Sigprocmask error");
    return;
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
	unix_error("Sigemptyset error");
    return;
}

void Sigfillset(sigset_t *set)
{ 
    if (sigfillset(set) < 0)
	unix_error("Sigfillset error");
    return;
}

void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
	unix_error("Sigaddset error");
    return;
}

pid_t Waitpid(pid_t pid, int *iptr, int options) 
{
    pid_t retpid;

    if ((retpid  = waitpid(pid, iptr, options)) < 0) 
	unix_error("Waitpid error");
    return(retpid);
}

int Sigsuspend(const sigset_t *set)
{
    int rc = sigsuspend(set); /* always returns -1 */
    if (errno != EINTR)
        unix_error("Sigsuspend error");
    return rc;
}

void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
	unix_error("Setpgid error");
    return;
}