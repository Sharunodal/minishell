/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/10/24 14:41:24 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmd_list(t_var *var)
{
	int	i;

	i = 0;
	while (var->cmd_list[i])
	{
		if (is_builtins(var->cmd_list[i]) >= 9)
			var->is_redirect = 1;
		if (is_builtins(var->cmd_list[i]) == 9)
		{
			if ((var->cmd_list[i + 1] && var->cmd_list[i + 1][0] == '|')
				|| !var->cmd_list[i + 1])
				return (2);
			var->pipes++;
		}
		if (is_builtins(var->cmd_list[i]) == 10)
			var->heredoc_count++;
		i++;
	}
	return (0);
}

int	parse(t_var *var)
{
	char	*input;

	input = split_redirections(var->input);
	if (!input)
		return (2);
	var->cmd_list = split_input(input, ' ');
	free(input);
	if (!var->cmd_list)
		return (1);
	return (parse_cmd_list(var));
}
