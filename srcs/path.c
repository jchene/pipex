/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchene <jchene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 13:07:11 by jchene            #+#    #+#             */
/*   Updated: 2022/05/19 16:22:57 by jchene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*get_env(const char *key, char **envp)
{
	unsigned int	i;

	i = 0;
	while (envp[i])
	{
		if (!basic_cmp(key, envp[i]))
			return (envp[i]);
		i++;
	}
	return (NULL);
}

int	get_args(t_exec *exec, char **argv, int i)
{
	exec->args = split(argv[i + 2], " \t");
	if (!exec->args)
		return (-1);
	return (0);
}

int	ft_lanorme(char **struc_path, char *cmd)
{
	if (!ft_strdup(cmd, struc_path))
		return (-1);
	return (0);
}

int	get_path(char **struc_path, char *cmd, char **envp)
{
	char			**dirs;
	char			*tmp;
	unsigned int	i;

	if (!access(cmd, X_OK))
		return (ft_lanorme(struc_path, cmd));
	dirs = split(&(get_env("PATH", envp)[strl("PATH=")]), ":");
	i = 0;
	while (dirs[i])
	{
		if (!nul((void **)&tmp, sizeof(char) * (strl(dirs[i]) + strl(cmd) + 2)))
			return (-1);
		ft_strcpyl(dirs[i], tmp, strl(dirs[i]));
		ft_strcpyl("/", &(tmp[strl(tmp)]), 1);
		ft_strcpyl(cmd, &(tmp[strl(tmp)]), strl(cmd));
		free(dirs[i]);
		dirs[i] = tmp;
		if (!access(dirs[i], X_OK))
			break ;
		i++;
	}
	if (!ft_strdup(dirs[i], struc_path))
		return (-1);
	free_tab((void **)dirs, get_nword(":", &(get_env("PATH", envp)[5])) + 1);
	return (0);
}
