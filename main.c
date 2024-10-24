/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/10/24 11:14:39 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status = 0;

static void	initialise(t_var *variables)
{
	variables->input = NULL;
	variables->cmd_list = NULL;
	variables->commands = 0;
	variables->pipes = 0;
	variables->is_redirect = 0;
	variables->input_redir = 0;
	variables->output_redir = 0;
	variables->heredoc_count = 0;
	variables->heredoc_fds = NULL;
	init_signal();
}

static int	count_cmd(char **cmd_list)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (cmd_list[i])
	{
		if (ft_strcmp(cmd_list[i], "|") == 0)
			count++;
		i++;
	}
	return (count + 1);
}

int	main(int argc, char **argv, char **envp)
{
	int		exit_code;
	t_var	variables;
	t_token	*tokens;
	t_token	***token_groups;

	(void)argc;
	(void)argv;
	variables.exit_code = 0;
	copy_env(&variables, envp);
	while (1)
	{
		initialise(&variables);
		variables.input = readline("prompt = ");
		if (variables.input == NULL)
			variables.input = ft_strdup("exit");
		add_history(variables.input);
		if (parse(&variables) == 2)
		{
			variables.exit_code = 2;
			printf("parse error");
		}
		else
		{
			init_heredoc_fds(&variables);
			tokens = malloc(sizeof(t_token) * (count_cmd_list(variables.cmd_list) + 1));
			if (!tokens)
				return (1);
			tokenize_cmd_list(&variables, tokens);
			if (handle_heredoc(&variables, tokens) != 1)
			{
				token_groups = split_tokens(&variables, tokens);
				if (variables.pipes == 0 && variables.is_redirect == 0)
					variables.exit_code = run_command(&variables, token_groups[0]);
				else
				{
					variables.nb_cmd = count_cmd(variables.cmd_list);
					handle_pipe(token_groups, variables.nb_cmd, &variables);
				}
			}
		}
		close_heredoc_fds(&variables);
		if (g_status == FAILURE)
		{
			variables.exit_code = 130;
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		g_status = 0;
		if (variables.exit_code == -2)
		{
			exit_code = my_exit(token_groups[0]);
			free_shell(&variables, tokens, token_groups);
			write(1, "exit\n", 5);
			break ;
		}
		free_shell(&variables, tokens, token_groups);
		
	}
	if (variables.envp)
		free_env(&variables.envp);
	rl_clear_history();
	exit (exit_code);
	return (0);
}
