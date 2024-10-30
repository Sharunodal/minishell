/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/10/30 10:47:38 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status = 0;

static void	initialise(t_var *variables)
{
	variables->input = NULL;
	variables->str = NULL;
	variables->cmd_list = NULL;
	variables->tokens = NULL;
	variables->token_groups = NULL;
	variables->commands = 0;
	variables->pipes = 0;
	variables->is_redirect = 0;
	variables->input_redir = 0;
	variables->output_redir = 0;
	variables->heredoc_count = 0;
	variables->heredoc_fds = NULL;
	init_signal();
}

static int	handle_commands(t_var *var)
{
	var->tokens = malloc(sizeof(t_token) * (count_cmd_list(var->cmd_list) + 1));
	if (!var->tokens)
	{
		var->exit_code = 1;
		return (1);
	}
	init_heredoc_fds(var);
	tokenize_cmd_list(var, var->tokens);
	if (handle_heredoc(var, var->tokens) != 1)
	{
		var->token_groups = split_tokens(var, var->tokens);
		if (var->pipes == 0 && var->is_redirect == 0)
			var->exit_code = run_command(var, var->token_groups[0]);
		else
		{
			var->nb_cmd = count_cmd(var->cmd_list);
			handle_pipe(var->token_groups, var->nb_cmd, var);
		}
	}
	close_heredoc_fds(var);
	return (0);
}

static int	parse_and_execute(t_var *var)
{
	if (parse(var) == 2)
	{
		var->exit_code = 2;
		printf("parse error");
	}
	else
		handle_commands(var);
	if (g_status == FAILURE)
	{
		var->exit_code = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	g_status = 0;
	return (0);
}

static int	run_shell(t_var *var)
{
	int		exit_code;

	while (1)
	{
		initialise(var);
		var->input = readline("prompt = ");
		if (var->input == NULL)
			var->input = ft_strdup("exit");
		add_history(var->input);
		parse_and_execute(var);
		if (var->exit_code == -2)
		{
			exit_code = my_exit(var->token_groups[0]);
			free_shell(var);
			write(1, "exit\n", 5);
			return (exit_code);
		}
		free_shell(var);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	int		exit_code;
	t_var	variables;

	(void)argc;
	(void)argv;
	exit_code = 1;
	variables.exit_code = 0;
	copy_env(&variables, envp);
	exit_code = run_shell(&variables);
	if (variables.envp)
		free_env(&variables.envp);
	rl_clear_history();
	return (exit_code);
}
