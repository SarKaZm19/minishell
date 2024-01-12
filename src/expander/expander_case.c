/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_case.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 02:39:34 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/12 02:52:32 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_no_quotes(char *cmd, char *new_cmd, int *start_i, int *parts)
{
	char	*sub;
	int		j;
	int		k;
	int		mem_start;

	j = 0;
	k = 0;
	mem_start = *start_i;
	sub = expand_var(new_cmd, cmd, start_i);
	if (sub)
	{
		if (cmd[mem_start] == '$' || cmd[mem_start] == '*')
		{
			if (sub[k] && !is_space(sub[k]))
				*parts += 1;
			while (sub[k] && !is_space(sub[k]))
				k++;
			while (sub[k] && is_space(sub[k]))
				k++;
		}
		else
			*parts += 1;
	}
	*start_i += j;
	printf("start_i no quotes incr =  %d\n", *start_i);
	return (sub);
	// si * dans une var, pas de substitution du *, interprete litteralement
	// voir si on peut pas lire tout d un coup et tout mettre dans un tableau apres dans expand
}

char	*expand_d_quote(char *cmd, char *new_cmd, int *start_i)
{
	int		j;
	int		i;
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	int		test = 0;

	*start_i += 1;
	j = 0;
	while (cmd[*start_i + j] && cmd[*start_i + j] != '\"')
		j++;
	tmp = ft_substr(cmd, *start_i, j);
	// prot tmp + free
	printf("tmp = .%s., start_i = %d, j = %d\n", tmp, *start_i, j);
	i = 0;
	while (tmp[i])
	{
		test++;
		if (test == 10)
		{
			printf("limit reached\n");
			return (NULL);
		}
		printf("tmp[%d] = %c\n", i, tmp[i]);
		if (tmp[i] == '$')
			tmp2 = expand_var(new_cmd, tmp, &i);
		else
			tmp2 = expand_var(new_cmd, tmp, &i);
		printf("d_quote_expand_var = %s\n", tmp2);
		if (new_cmd)
		{
			printf("new_cmd exists --> join\n");
			printf("new_cmd = %s\n", new_cmd);
			tmp3 = ft_strdup(new_cmd);
			free(new_cmd);
			new_cmd = NULL;
			new_cmd = ft_strjoin(tmp3, tmp2);
			free(tmp3);
			tmp3 = NULL;
		}
		else
		{
			printf("new_cmd doesnt exists --> dup\n");
			new_cmd = ft_strdup(tmp2);
		}
		if (tmp2)
			free(tmp2);
		printf("tmp_i before loop = %d\n", i);
	}
	printf("new_cmd quote_expand = .%s.\n", new_cmd);
	*start_i += j + 1;
	return (new_cmd);
}

char	*expand_s_quote(char *cmd, char *new_cmd, int *start_i)
{
	int		j;
	char	*s_quote_expand;
	char	*tmp;

	*start_i += 1;
	j = 0;
	while (cmd[*start_i + j] && cmd[*start_i + j] != '\'')
		j++;
	tmp = ft_substr(cmd, *start_i, j);
	printf("s_quote_substr = .%s., start_i = %d, j = %d\n", tmp, *start_i, j);
	*start_i += j + 1;
	if (!new_cmd)
		s_quote_expand = ft_strdup(tmp);	
	else
	{
		s_quote_expand = ft_strjoin(new_cmd, tmp);
		free(new_cmd);
		new_cmd = NULL;
	}
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	printf("s_quote_expand = .%s.\n", s_quote_expand);
	return (s_quote_expand);
}
