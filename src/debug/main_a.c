/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/20 21:50:10 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	check_input_null(char *input)
{
	if (input == NULL)
		return (0);
	else
		add_history(input);
	return (1);
}

int	check_enter_space(char *input)
{
	if (!ft_strncmp(input, " ", ft_strlen_custom(input)) || input[0] == '\0')
		return (0);
	return (1);
}

int	minishell_loop(t_data *data)
{
	data->input = readline((const char *)data->prompt);
	if (!check_input_null(data->input))
	{
		printf("exit\n");
		exit_minishell(NULL, 50);
	}
	if (!check_quotes(data->input) || !check_enter_space(data->input))
		exit_current_prompt(data);
	data->input_split = ft_split_minishell(data->input, ' ');
	input_arr_to_lexer_list(data->input_split, &data->lexer);
	lexer_to_simple_cmds(&data->lexer, &data->simple_cmds);
	expander(data);
	heredoc(data);
	print_simple_cmds(&data->simple_cmds);
	exit_current_prompt(data);
	return (1);
}

//int	minishell_loop(t_data *data)
//{
//	data->input = readline((const char *)data->prompt);
//	if (!check_input_null(data->input))
//	{
//		printf("exit\n");
//		exit_minishell(NULL, 50);
//	}
//	if (!check_quotes(data->input) || !check_enter_space(data->input))
//		exit_current_prompt(data);
//	/* Split S*/
//	data->input_split = ft_split_minishell(data->input, ' ');
//	printf("Minishell split output:\n");
//	print_input_split(data);
//	printf("----------------------\n");
//	/* Split E */
//	/* Lexer - Link list S */
//	printf("Minishell lexer output:\n");
//	input_arr_to_lexer_list(data->input_split, &data->lexer);
//	print_lexer(&data->lexer);
//	printf("----------------------\n");
//	/* Lexer - Link list E */
//	/* Parser - Link list S */
//	printf("Minishell parser output:\n");
//	printf("----------------------\n");
//	lexer_to_simple_cmds(&data->lexer, &data->simple_cmds);
//	printf("Rest of lexer: \n");
//	print_lexer(&data->lexer);
//	printf("----------------------\n");
//	printf("SIMPLE CMDS - before expander\n");
//	print_simple_cmds(&data->simple_cmds);
//	printf("----------------------\n");
//	printf("SIMPLE CMDS - after expander\n");
//	expander(data);
//	print_simple_cmds(&data->simple_cmds);
//	printf("----------------------\n");
//	printf("Heredoc output:\n");
//	heredoc(data);
//	printf("----------------------\n");
//	/* Parser - Link list E */
//	exit_current_prompt(data);
//	return (1); //should never reach this
//}

int	main(int argc, char **argv, char *env[])
{
	t_data	data;

	if (argc > 1 || argv[1])
	{
		ft_putstr_fd("Error: Minishell doesn't take any arguments.\n\n", 2);
		ft_putstr_fd("Correct usage: ./minishell\n\n", 2);
		return (0);
	}
	init_data(&data);
	env_init(env, &data);
	data.prompt = set_prompt(data.env_list);
	minishell_loop(&data);
	return (0);
}
