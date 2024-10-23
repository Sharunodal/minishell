/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:57:00 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/22 10:11:38 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**find_path(void)
{
	char	*path_env;
	char	*path_cpy;
	char	**full_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_cpy = ft_strdup(path_env);
	if (!path_cpy)
		return (NULL);
	full_path = ft_split(path_cpy, ':');
	free(path_cpy);
	if (!full_path)
		return (NULL);
	return (full_path);
}

char	*build_cmd_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*cmd_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	cmd_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (cmd_path);
}

char	*find_cmd_path(char *cmd, int i)
{
	char	**full_path;
	char	*cmd_path;

	full_path = find_path();
	if (full_path == NULL)
		return (NULL);
	while (full_path[i] != NULL)
	{
		cmd_path = build_cmd_path(full_path[i], cmd);
		if (!cmd_path)
		{
			free_list(full_path);
			return (NULL);
		}
		if (access(cmd_path, X_OK) == 0)
		{
			free_list(full_path);
			return (cmd_path);
		}
		i++;
		free(cmd_path);
	}
	free_list(full_path);
	return (NULL);
}
