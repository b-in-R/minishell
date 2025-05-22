#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	main(int ac, char **av)
{
	(void)ac;
	int	status;

	//child process
	if (fork() == 0)
		execvp(av[1], av + 1);

	wait(&status);

	return (EXIT_SUCCESS);
}