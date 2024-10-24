/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:38:02 by arissane          #+#    #+#             */
/*   Updated: 2024/10/24 14:39:03 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_double_quotes(char *input, int *k, char *str, int i)
{
	int	len;

	str[(*k)++] = input[i++];
	while (input[i] && input[i] != '\"')
	{
		if (input[i] == '$' && check_symbols(input[i + 1]) == 0)
		{
			len = (check_env(input, i));
			if (len > 0)
				i += len;
			else
				str[(*k)++] = input[i++];
		}
		else
			str[(*k)++] = input[i++];
	}
	if (input[i] == '\"')
		str[(*k)++] = input[i++];
	else
	{
		printf("unclosed quotes\n");
		return (-1);
	}
	return (i);
}

static int	process_special_characters3(char *input, char *str, int *i, int *k)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		if (validate_heredoc_input(&input[*i + 2]) == 1)
		{
			printf("heredoc syntax\n");
			return (1);
		}
		copy2_with_space(input, str, i, k);
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
		copy2_with_space(input, str, i, k);
	else if (input[*i] == '<' || input[*i] == '>')
		copy1_with_space(input, str, i, k);
	else if (input[*i] == '|')
		copy1_with_space(input, str, i, k);
	else
		str[(*k)++] = input[(*i)++];
	return (0);
}

static int	process_special_characters2(char *input, char *str, int *i, int *k)
{
	if (input[*i] == '\"')
	{
		*i = (process_double_quotes(input, k, str, *i));
		if (*i == -1)
			return (1);
	}
	else if (input[*i] == '\'')
	{
		str[(*k)++] = input[(*i)++];
		while (input[*i] && input[*i] != '\'')
			str[(*k)++] = input[(*i)++];
		if (input[*i] == '\'')
			str[(*k)++] = input[(*i)++];
		else
		{
			printf("unclosed quotes\n");
			return (1);
		}
	}
	else if (process_special_characters3(input, str, i, k) == 1)
		return (1);
	return (0);
}

static int	process_special_characters(char *input, char *str)
{
	int	i;
	int	k;
	int	len;

	i = 0;
	k = 0;
	while (input[i])
	{
		if (input[i] == '$' && check_symbols(input[i + 1]) == 0)
		{
			len = check_env(input, i);
			if (len > 0)
				i += len;
			else
				str[k++] = input[i++];
		}
		else if (input[i] == '\"' && input[i + 1] == '\"')
			i += 2;
		else if (input[i] == '\'' && input[i + 1] == '\'')
			i += 2;
		else if (process_special_characters2(input, str, &i, &k) == 1)
			return (1);
	}
	str[k] = '\0';
	return (0);
}

char	*split_redirections(char *input)
{
	int		i;
	int		k;
	char	*str;

	i = 0;
	k = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			k += 2;
		i++;
		k++;
	}
	str = malloc(sizeof(char) * (k + 1));
	if (!str)
		return (NULL);
	if (process_special_characters(input, str) == 1)
	{
		free(str);
		return (NULL);
	}
	return (str);
}
