/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:59:52 by arissane          #+#    #+#             */
/*   Updated: 2024/10/22 11:23:19 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_new_string(char *str, int i, char *value, int end)
{
	size_t	len;
	char	*temp;
	int		k;
	int		j;

	len = ft_strlen(str) - (end - i) + ft_strlen(value);
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (NULL);
	k = 0;
	while (k < i)
	{
		temp[k] = str[k];
		k++;
	}
	j = 0;
	while (value[j])
		temp[k++] = value[j++];
	while (str[end])
		temp[k++] = str[end++];
	temp[k] = '\0';
	free(str);
	return (temp);
}

char	*get_env_value(t_var *var, char *str, int i)
{
	char	*value;
	char	*temp;
	int		end;

	end = i + 1;
	if (str[i + 1] == '?')
	{
		value = ft_itoa(var->exit_code);
		end = i + 2;
	}
	else
	{
		end = i + 1;
		while (str[end] && str[end] != ' '
			&& str[end] != '\"' && str[end] != '\'')
			end++;
		temp = ft_substr(str, i + 1, end - (i + 1));
		value = getenv(temp);
		free(temp);
	}
	return (create_new_string(str, i, value, end));
}

void	close_heredoc_fds(t_var *var)
{
	int	i;

	i = 0;
	if (var->heredoc_fds)
	{
		while (var->heredoc_fds[i] != -1)
		{
			close(var->heredoc_fds[i]);
			var->heredoc_fds[i] = -1;
			i++;
		}
		free(var->heredoc_fds);
	}
}
