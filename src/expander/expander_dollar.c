/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:16:33 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 11:36:25 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * WARNING: NOT FULLY IMPLEMENTED - exit code hardcoded
 * 
 * @brief Expands a dollar sign followed by the exit code in a string.
 * 
 * It replaces the exit code placeholder with the exit status of the most
 * recently executed foreground pipeline.
 * 
 * It breaks down the input string into 3 parts:
 * part_1: up until, but not including, the dollar sign
 * part_2: the exit code
 * part_3: after, not including, the question mark that follows the dollar sign
 * 
 * Then it constructs the final string in 2 steps, first joining part_1 and
 * part_2 into tmp_join and then joining tmp_join and part_3 into final.
 * 
 * Finally, the appropriate string in the 2D input array is pointed to the final
 * string. All of the dynamically allocated memory is then freed with the
 * exception of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * @param 	cmd		array of strings containing input commands
 * @param	i_cmd	index of the command string in the array to modify
 */

void	expand_exit_code(char **cmd, int i_cmd)
{
	char	*str;
	int		i;
	int		j;
	t_str	new_str;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	new_str.part_2 = ft_itoa(125);
	j = 2;
	new_str.part_3 = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	new_str.tmp_join = ft_strjoin(new_str.part_1, new_str.part_2);
	new_str.final = ft_strjoin(new_str.tmp_join, new_str.part_3);
	cmd[i_cmd] = new_str.final;
	free(str);
	free_struct_str(&new_str);
}

/**
 * @brief	Expands a found environment variable represented by a dollar sign.
 * 
 * This is a helper function to the expand_dollar function that helps with
 * constructing the final string based on the different parts of the input
 * string and the value of the env variable that was found.
 * 
 * @param	new_str	pointer to a t_str structure containing parsed string parts
 */

void	expand_dollar_found(t_str *new_str)
{
	new_str->content = (t_env *) new_str->env_found->content;
	new_str->tmp_join = ft_strjoin(new_str->part_1, new_str->content->value);
	new_str->final = ft_strjoin(new_str->tmp_join, new_str->part_3);
}

/**
 * @brief	Expands a dollar sign followed by an environment variable.
 * 
 * The function identifies the environment variable in the input string and
 * replaces it with its value from the provided environment list.
 * 
 * It breaks down the input string into 3 parts:
 * part_1: up until, but not including, the dollar sign
 * part_2: up until, and including, the first space present in the rest of
 * the string or if there is no space, the end of the string
 * part_3: after the space following the dollar sign, potentially NULL if there
 * is no space in the string after the dollar sign
 * 
 * Then it constructs the final string, which can happen in 2 ways depending on
 * whether or not the env variable was found in the env list.
 * If the env variable is found, the join happens in the expand_dollar_found
 * function. Otherwise, the part_2 is skipped in the join and the final str
 * includes only the part_1 and part_3.
 * 
 * Finally, the appropriate string in the 2D input array is pointed to the final
 * string. All of the dynamically allocated memory is then freed with the
 * exception of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * @param	cmd			array of strings containing input commands
 * @param	i_cmd		index of the command string in the array to modify
 * @param	env_list	linked list containing environment variables
 */

void	expand_dollar(char **cmd, int i_cmd, t_list *env_list)
{
	char	*str;
	int		i;
	int		j;
	t_str	new_str;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	j = 0;
	while (str[i + j] && str[i + j] != ' ')
		j++;
	new_str.part_2 = ft_substr(str, i + 1, j - 1);
	new_str.part_3 = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	new_str.env_found = env_find(env_list, new_str.part_2);
	if (new_str.env_found)
		expand_dollar_found(&new_str);
	else
		new_str.final = ft_strjoin(new_str.part_1, new_str.part_3);
	cmd[i_cmd] = new_str.final;
	free(str);
	free_struct_str(&new_str);
}

/**
 * @brief	Checks the context of the dollar sign in a string.
 * 
 * This function analyzes the context of a dollar sign ('$') in a string 'str'
 * at position 'j'. It examines nearby characters to determine the context
 * of the dollar sign within the string.
 * 
 * @param	str	input string to check
 * @param	j	index of the dollar sign in the string
 * @return	int	returns a flag representing the context:
 *              4:	the dollar sign is enclosed in single quotes
 *              3:	the dollar sign is followed by a space, a quote (' or ")
 * 					or there is nothing else following it
 *              2:	the dollar sign is followed by a question mark (?)
 *              1: 	the dollar sign appears in a context that can be expanded
 *              0:	the dollar sign doesn't match any defined context
 */

int	checker_dollar(char *str, int j)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\''))
		return (4);
	else if (!str[j + 1] || str[j + 1] == ' ' || str[j + 1] == '\''
		|| str[j + 1] == '"')
		return (3);
	else if (str[j + 1] == '?')
		return (2);
	else if (j != 0 && str[j - 1] != '\\' && str[j - 1] != '\''
		&& str[j + 1] != ' ')
		return (1);
	else if (str[j + 1] && str[j + 1] != '\'' && str[j + 1] != '"'
		&& str[j + 1] != '\\' && str[j + 1] != ' ')
		return (1);
	return (0);
}

/**
 * @brief	Expands dollar signs in a command string based on specific conditions.
 * 
 * This function iterates through a command string 'content->cmd[i]', determines
 * whether or not to expand a dollar sign based on the predefined conditions
 * represented by the checker_dollar function and does so where appropriate.
 * 
 * @param	content		pointer to t_simple_cmds struct
 * @param	i			index of the command string in the array to process
 * @param	env_list	linked list containing environment variables
 */

void	expander_loop_dollar(t_simple_cmds *content, int i, t_list *env_list)
{
	int	j;
	int	dollar_flag;

	j = 0;
	while (content->cmd[i][j])
	{
		if (content->cmd[i][j] == '$')
		{
			dollar_flag = checker_dollar(content->cmd[i], j);
			delete_quotes(content->cmd, i);
			if (dollar_flag == 4)
				break ;
			else if (dollar_flag == 2)
				expand_exit_code(content->cmd, i);
			else if (dollar_flag == 1)
				expand_dollar(content->cmd, i, env_list);
		}
		j++;
	}
}
