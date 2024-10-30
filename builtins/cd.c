/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:25:42 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/30 16:52:18 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_cd(char *name, char *value, t_var *var)
{
	int		i;
	int		j;
	char	*tmp;
	char	*new_var;
	char	**new_environ;

	tmp = ft_strjoin(name, "=");
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	i = unset(name, ft_strlen(name), var);
	new_environ = malloc((i + 2) * sizeof(char *));
	if (new_environ == NULL)
	{
		free(new_var);
		return ;
	}
	j = -1;
	while (j++ < i)
		new_environ[j] = var->envp[j];
	new_environ[i] = new_var;
	new_environ[i + 1] = NULL;
	var->envp = new_environ;
}

static int	check_cd(t_token **token, int i, char **new_path)
{
	if (!token[i] || is_builtins(token[i]->value) != 0)
	{
		*new_path = getenv("HOME");
		if (!(*new_path))
			return (1);
		return (2);
	}
	else if (access(token[i]->value, F_OK) != 0 && token[i]->value[0] != '$')
	{
		ft_putstr_fd("cd: No such file or directory\n", 2);
		return (1);
	}
	else if (token[i + 1] && token[i + 1]->type == 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	else if (token[i]->value[0] == '$')
	{
		*new_path = getenv(token[i]->value + 1);
		if (!(*new_path))
			return (1);
		return (2);
	}
	return (0);
}

static int	expand_home_path(t_token **token, int i, char **new_path)
{
	char	*home;
	char	expanded_path[260];

	if (token[i + 1] && token[i + 1]->type == 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	if (ft_strncmp(token[i]->value, "~/", 2) == 0)
	{
		home = getenv("HOME");
		if (home)
		{
			ft_strlcpy(expanded_path, home, sizeof(expanded_path));
			ft_strlcat(expanded_path, token[i]->value + 1, sizeof(expanded_path));
			*new_path = ft_strdup(expanded_path);
		}
		if (access(*new_path, F_OK) != 0)
		{
			ft_putstr_fd("cd: No such file or directory\n", 2);
			return (1);
		}
	}
	else
		return (check_cd(token, i, new_path));
	return (2);
}
//check if we can use the function set_environment_variable in export instead of export_cd

int	handle_cd(t_token **token_group, t_var *var)
{
	char	new_cwd[260];
	char	old_path[260];
	char	*new_path;
	int		i;
	int		result;

	if (!getcwd(old_path, sizeof(old_path)))
		return (1);
	i = find_command_index(token_group, "cd");
	result = expand_home_path(token_group, i + 1, &new_path);
	if (result == 0)
		new_path = token_group[i + 1]->value;
	else if (result == 1)
		return (1);
	if (new_path && chdir(new_path) == 0)
	{
		if (!getcwd(new_cwd, sizeof(new_cwd)))
			return (1);
		export_cd("OLDPWD", old_path, var);
		export_cd("PWD", new_cwd, var);
		//free (new_path);
	}
	else
		return (1);
	return (0);
}
