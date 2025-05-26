
Fonctions autorisees



access:		#include <unistd.h> 	(?#include <fcntl.h>?)

		Checks if the calling process(the user who use this fonction) can
		access the file pathname (and if it exists), according to mode

			int	access(const char *pathname, int mode);

				*pathname:	pathname to check
				mode:		R_OK / W_OK / X_OK (read / write / execute)

			return values:	0		= OK
							-1	 	= ERROR

			usage exemple:

		{...
			if (access(arg, R_OK) == 0)
				printf("%s read -> OK\n", arg);
			else
				printf("%s read -> KO\n", arg);
		...}


add_history:	a voir



chdir:	#include <unistd.h>

		go to the directory specified in path	

				int	chdir(const char *path);

			return values:	0		= OK
							-1		= ERROR



close:	#include <unistd.h>

		close a file descriptor



dup, dup2:	#include <unistd.h>

		create a copy of the file descriptor (oldfd) in:
			dup:	the lowest-numbered unused file descriptor for the new
				descriptor
			dup2:	the file descriptor number specified in newfd

			int	dup(int oldfd);
			int	dup2(int oldfd, int newfd);

		return values:	-1		= ERROR



execve:		#include <unistd.h>

		executes the program referred to by pathname

			int	execve(const char *pathname, const char **argv, const char **envp);

			*pathname:	program (function) reference
			**argv:		strings passed to the new program as its command-line
						arguments (argv[0] should be the filename associated with
						the file being executed)
			**envp:		environment of the new program

		return value:	nothing		= OK
						-1			= ERROR

		additional:
			- 	all threads other than the calling are destroyed during an execve.
				mutexes, condition variables and other pthreads are not preserved

		usage exemple:

		{...
			 if (execve(cmd_path, args, envp) == -1)
			 {
				free(cmd_path);
				error_exit(...);
			 }
		...}



exit:..



fork:		#include <sys/types.h>, <unistd.h>

		create a new process (child) by duplicating the calling process, running 
		in a separate memory space, but at the start with the sames values

			pid_t	fork(void);

		return values: 	0 and PID to parent	== OK
						-1					== ERROR



free:..



getcwd:		#include <unistd.h>

		copies the absolute pathname of the current working directory to
		the array pointed to by buf, wich is of lenght size

			char	*getcwd(char *buf, size_t size);

		return values:	pointer to the string, value same as buf	== OK
						NULL										== ERROR




getenv:		#include <stdlib.h>

		searches the environment list to find the environment variable name

			char	*getenv(const char *name);
		
		return values:	pointer to the value in the environment	== OK
						NULL									== ERROR



isatty:		#include <unistd.h>

		tests wheter fd is an open file descriptor referring to a terminal

			int	isatty(int fd);
		
		return values:	1		== OK
						0		== ERROR
	
	

ttyname:	#include <unistd.h>

		return a pointer to the null-terminated pathname of the terminal device
		that is open on the file descriptor fd.

			char	**ttyname(int fd);
		
		return values:	pointer to a pathname		== OK
						NULL + errno (check man)	== ERROR



ttyslot:	#include <unistd.h>

		returns the index of the current users entry in a file

			int	ttyslot(void);

		detail a voir



ioctl:		#include <sys/ioctl.h>

		detail a voir

			int	ioctl(int fd, unsigned long request, ...);



kill:	<>

		kill a process
		check man for options (-s, -q, -l, -L)

		detail a voir



malloc: ...



open: ...



opendir:	#include <sys/types.h>

		open a directory stream corresponding to the directory name, and return
		a pointer to the directory stream

			DIR	*opendir(const char *name);

		return values:	pointer to the directory stream		== OK
						NULL + errno						== ERROR



closedir:	#include <sys/types.h>

		close the directory stream associated with dirp, if successful, also
		closes the underlying file descriptor associated with dirp.

			int	closedir(DIR *dirp);
		
		return values:	0			== OK
						-1 + errno	== ERROR



pipe:	#include <unistd.h>

		create a pipe, pipefd[0] refers to the read end of the pipe
		pipefd[1] refers to the write end of the pipe

			int	pipe(int pipefd[2]);

		return values:	0			== OK
						-1 + errno	== ERROR



printf: ...



read: <>

		read from the standard input into shell variables



readline:	#include <stdio.h>, <readline/readline.h>, <readline/history.h>

		read a line from the terminal and return it. the line returned is
		allocated with malloc, the caller must free it

			char	*readline(const char *prompt);

		return values:	the text of the line read	== OK
						if EOF: newline



rl_clear_history:	

		detail a voir



rl_on_new_line:

		detail a voir



rl_redisplay:

		detail a voir



rl_replace_line:

		detail a voir



(sigaction, sigaddset, sigemptyset, signal)



stat, lstat, fstat:	#include <sys/types.h>, <sys/stat.h>, <unistd.h>

		hese functions return information about a file, in the buffer pointed 
		to by statbuf. No permissions are requiered on the file itself, but for
		stat and lstat, permiession is required on all of the directories
		in pathname that leadd to the file.

			int	stat(const har *pathname, struct stat *statbuf);
			int	fstat(int fd, struct stat *statbuf);
			int	lstat(const char *pathname, struct stat *statbuf);

		stat: 	retrieve information about the file pointed to by pathname
		lstat:	identical to stat, but if pathname is a symbolic link,
				then returns information about the link itself, not the file that
				link refers to.
		fstat:	identical to stat, except that the informations of the file
				is retrieved in the file descriptor fd.

		they retrun a stat struct: (man lstat for details)

			struct stat
			{
            	dev_t     st_dev;         /* ID of device containing file */
               	ino_t     st_ino;         /* Inode number */
              	mode_t    st_mode;        /* File type and mode */
              	nlink_t   st_nlink;       /* Number of hard links */
              	uid_t     st_uid;         /* User ID of owner */
               	gid_t     st_gid;         /* Group ID of owner */
               	dev_t     st_rdev;        /* Device ID (if special file) */
               	off_t     st_size;        /* Total size, in bytes */
               	blksize_t st_blksize;     /* Block size for filesystem I/O */
               	blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

               	struct timespec st_atim;  /* Time of last access */
               	struct timespec st_mtim;  /* Time of last modification */
               	struct timespec st_ctim;  /* Time of last status change */

           		#define st_atime st_atim.tv_sec  /* Backward compatibility */
           		#define st_mtime st_mtim.tv_sec
           		#define st_ctime st_ctim.tv_sec
           	};



strerror, perror



tcsetattr, tcgetattr

		detail a voir



tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

		detail a voir



unlink

		remove the specified FILE

			unlink FILE



wait, waitpid:	#include <sys/types.h>, <sys/wait.h>

		/*  All  of these system calls are used to wait for state changes
			in a child of the calling process, and obtain information about
			the child whose state has changed.   A  state  change  is 
			considered to be: the child terminated; the child was stopped
			by a signal; or the child was resumed by  a  signal. In the  case
			of a terminated child, performing a wait allows the system to
			release the resources associated with the child; if a wait is not
			performed, then the terminated child remains in a "zombie" state.

       		If  a child has already changed state, then these calls return 
			immediately. Otherwise, they block until either a child changes
			state or a  signal  handler  interrupts the call (assuming that
			system calls are not automatically restarted using
			the SA_RESTART flag of sigaction(2)). In the remainder of this
			page, a child whose state has changed and which has not yet been
			waited upon by one of these system calls is termed waitable. */

		------
		wait:

				pid_t	wait(int *wstatus);

			suspends execution of the calling thread until one of its children
			terminates.
				
		waitpid:

				pid_t	waitpid(pid_t pid, int *wstatus, int options);

			suspends execution of the calling thread until a child specified
			by pid argument has changed state. (man waitpid for options argument)
		------

		return values:	process ID of the terminated child	== OK
						-1									== ERROR

		wait(&wstatus) == waitpid(-1, &wstatus, 0);



wait3, wait4:	#include <sys/types.h>, <sys/time.h>,
				#include <sys/resource.h>, sys/wait.h>

		they are similoar to waitpid, but return additionally the resource usage
		informations about the child in the struct pointed to by rusage.

		--------
		wait3: (wait with options and struct)

				pid_t	wait3(int *wsatus, int options, struct rusage *rusage);

			wait3(wstatus, options, rusage) == waitpid(-1, wstatus, options)


		wait4: (waitpid with options and struct)

				pid_t	wait4(pid_t pid, int *wstatus, int options,
							struct rusage *rusage);
			
			wait4(pid, wstatus, options, rusage) == waitpid(pid, wstatus, options)
		--------

		struct rusage
		{
			struct timeval	ru_utime; 		/* user CPU time used */
            struct timeval	ru_stime; 		/* system CPU time used */
            long			ru_maxrss;      /* maximum resident set size */
            long			ru_ixrss;       /* integral shared memory size */
            long			ru_idrss;       /* integral unshared data size */
            long			ru_isrss;       /* integral unshared stack size */
            long			ru_minflt;      /* page reclaims (soft page faults)*/
            long			ru_majflt;      /* page faults (hard page faults) */
            long			ru_nswap;       /* swaps */
            long			ru_inblock;     /* block input operations */
            long			ru_oublock;     /* block output operations */
            long			ru_msgsnd;      /* IPC messages sent */
            long			ru_msgrcv;      /* IPC messages received */
            long			ru_nsignals;	/* signals received */
            long			ru_nvcsw;       /* voluntary context switches */
            long			ru_nivcsw;      /* involuntary context switches */
		}

		resource usage metrics are preserved across an execve

		return values:	0				== OK
						-1 + errno		== ERROR



write: ...