/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchene <jchene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 23:49:13 by jchene            #+#    #+#             */
/*   Updated: 2022/05/17 16:41:57 by jchene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	init_data(t_data *data, t_exec *exec, int argc)
{
	int		i;

	if (!nul((void **)&(data->ids), sizeof(pid_t) * ((argc - 3) + 1)))
		return (-1);
	if (!nul((void **)&(data->pipes), sizeof(int *) * ((argc - 3) + 1)))
	{
		free(data->ids);
		return (-1);
	}
	i = 0;
	while (i < argc - 3)
	{
		if (!nul((void **)&(data->pipes[i]), sizeof(int) * 2))
			free_tab((void **)data->pipes, i);
		data->pipes[i][READ] = -1;
		data->pipes[i][WRITE] = -1;
		data->ids[i] = -1;
		i++;
	}
	data->files_fds[0] = -1;
	data->files_fds[1] = -1;
	get_exec(exec);
	get_data(data);
	return (0);
}

int	set_input_fds(char **argv, int i)
{
	if (i == 0)
	{
		get_data(NULL)->files_fds[INFILE] = open(argv[1], O_RDONLY);
		if (get_data(NULL)->files_fds[INFILE] < 0)
			perror("pipex: open");
		get_exec(NULL)->in_fds[READ] = get_data(NULL)->files_fds[INFILE];
		get_exec(NULL)->in_fds[WRITE] = -1;
	}
	else
	{
		get_exec(NULL)->in_fds[READ] = get_data(NULL)->pipes[i - 1][READ];
		get_exec(NULL)->in_fds[WRITE] = get_data(NULL)->pipes[i - 1][WRITE];
	}
	return (0);
}

int	set_output_fds(int argc, char **argv, int i)
{
	if (i < argc - 3 - 1)
	{
		pipe(get_data(NULL)->pipes[i]);
		get_exec(NULL)->out_fds[READ] = get_data(NULL)->pipes[i][READ];
		get_exec(NULL)->out_fds[WRITE] = get_data(NULL)->pipes[i][WRITE];
	}
	else
	{
		if (!access(argv[argc - 1], W_OK))
			if (unlink(argv[argc - 1]))
				perror("pipex: unlink");
		get_data(NULL)->files_fds[OUTFILE] = open(argv[argc - 1],
			O_WRONLY | O_CREAT, 00777);
		if (get_data(NULL)->files_fds[OUTFILE] < 0)
			perror("pipex: open");
		get_exec(NULL)->out_fds[READ] = -1;
		get_exec(NULL)->out_fds[WRITE] = get_data(NULL)->files_fds[OUTFILE];
	}
	return (0);
}

int	init_exec(int argc, char **argv, char **envp, int i)
{
	set_input_fds(argv, i);
	set_output_fds(argc, argv, i);
	if (get_args(get_exec(NULL), argv, i) == -1)
	{
		close_fds(i);
		return (free_data(i, -1));
	}	
	if (get_path(&(get_exec(NULL)->path), get_exec(NULL)->args[0], envp) == -1)
	{
		close_fds(i);
		return (free_data(i, -1) + free_exec(0));
	}
	return (0);
}