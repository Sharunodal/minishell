/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:31:07 by arissane          #+#    #+#             */
/*   Updated: 2024/10/24 14:37:00 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_env(char *input, int i)
{
	int		j;
	int		k;
	int		len;
	char	*temp;
	char	*envp;

	i++;
	j = i - 1;
	k = 0;
	len = 0;
	while (input[j++] && input[j] != ' '
		&& input[j] != '\"' && input[j] != '\'')
		len++;
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (1);
	while (k < len)
		temp[k++] = input[i++];
	temp[len] = '\0';
	envp = getenv(temp);
	free(temp);
	if (!envp)
		return (len + 1);
	return (0);
}

int	validate_heredoc_input(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != ' ')
	{
		if (input[i] == '<' || input[i] == '>')
			return (1);
		i++;
	}
	return (0);
}

int	check_symbols(char c)
{
	if (!c)
		return (1);
	else if (c == '\"')
		return (1);
	else if (c == '\'')
		return (1);
	else if (c == ' ')
		return (1);
	else if (c == '?')
		return (1);
	else
		return (0);
}

void	copy1_with_space(char *input, char *str, int *i, int *k)
{
	str[(*k)++] = ' ';
	str[(*k)++] = input[(*i)++];
	str[(*k)++] = ' ';
}

void	copy2_with_space(char *input, char *str, int *i, int *k)
{
	str[(*k)++] = ' ';
	str[(*k)++] = input[(*i)++];
	str[(*k)++] = input[(*i)++];
	str[(*k)++] = ' ';
}
