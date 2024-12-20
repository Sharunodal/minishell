/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 18:16:12 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/25 10:06:25 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_unset(char *name, size_t name_len, t_var *var)
{
	int	i;
	int	j;

	i = 0;
	while (var->envp[i] != NULL)
	{
		if (ft_strncmp(var->envp[i], name, name_len) == 0 \
			&& (var->envp[i][name_len] == '\0' \
			|| var->envp[i][name_len] == '='))
		{
			free(var->envp[i]);
			j = i;
			while (var->envp[j] != NULL)
			{
				var->envp[j] = var->envp[j + 1];
				j++;
			}
			continue ;
		}
		i++;
	}
	return (i);
}

int	handle_unset(t_token **token, t_var *var)
{
	int		i;

	i = find_command_index(token, "unset");
	if (i == -1)
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return (1);
	}
	if (token[i + 1] && token[i + 1]->value[0] == '-'
		&& token[i + 1]->value[1] && ft_isascii(token[i + 1]->value[1]))
	{
		ft_putstr_fd("unset: no option allowed\n", 2);
		return (2);
	}
	while (token[i + 1] && (token[i + 1]->type == 2 || token[i + 1]->type == 1))
	{
		ft_unset(token[i + 1]->value, ft_strlen(token[i + 1]->value), var);
		i++;
	}
	return (0);
}
