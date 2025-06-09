
Minishell.h


===============================================================================
main.c:

int	main(int ac, char **av, char **envp)


===============================================================================

parsing - Alberto

 --> execute(cmds);

===============================================================================
execute.c:


void	execute_command(t_cmd *cmd)
{
	char	*path = find_command_path(cmd->args[0])
	execve(path, cmd->args, cmd->g_env);
	error_exit();
}


void	execute(t_cmd *cmds)
{
	t_cmd	*cmd = cmds;
	int		fd[2];
	int		in_fd = 0;
	pid_t	pid;

	if (...) 
		error_exit();
	if (pid == 0)
		setup_redirections(cmd, in_fd, fd);
		if (is_builtin(cmd))
			execute_builtin(cmd);
		else
			execute_command(cmd);
	else
		cleanup_parent(cmd, &in_fd, fd);
}

===============================================================================
redirection.c:


void	setup_redirections(t_cmd *cmd, int in_fd, int pipe_fd[2])
{
	int	fd_in;
	int	flags;
	int	fd_out;

	if (cmd->infile)
		fd_in = open(cmd->infile, O_RDONLY);
		dup2(fd_in, STDIN_FILENO);
	else if (in_fd != 0)
		dup2(in_fd, STDIN_FILENO);

	if (cmd->outfile)
		flags = O_CREAT | O_WR.. | (cmd->append ? O_APPEND : O_TRUNC);
		fd_out = open(cmd->outfile, flags, 0644);
		dup2(fd_out, STDOUT_FILENO);
	else if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
}



===============================================================================
check_builtin.c:


int	execute_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (ft_echo(cmd->args));
	if (!ft_strcmp(cmd->args[0], "*builtin*"))
		return (ft_*builtin*(cmd->args));
	if (...)
		return (...);
	return (1);
}

int	is_builtin(t_cmd *cmd)
{
	char	*name = cmd->args[0];

	if (!ft_strcmp(name, "echo"))
		return (1);
	if (!ft_strcmp(name, "*builtin*"))
		return (1);
	if (...)
		return (1);
}



===============================================================================
builtin_1.c:

int	ft_echo(char **args)
{
	while (args[i])
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
}

int	ft_cd(char **args)
{
	char	*path;

	if (!args[1])
		path = get_env("HOME");
	else
		path = args[1];
}

int	ft_pwd(char **args)
{

}

int	ft_env(char **args)
{

}



===============================================================================




===============================================================================




===============================================================================