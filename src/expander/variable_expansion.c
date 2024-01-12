/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 02:37:41 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/12 13:57:37 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(char *tmp, int *tmp_i)
{
	char	*var;
	int		j;

	j = 0;
	if (tmp[*tmp_i + j] == '$' || tmp[*tmp_i + j] == '*')
	{
		j += 1;
		while (tmp[*tmp_i + j] && !is_space(tmp[*tmp_i + j]) && tmp[*tmp_i + j] != '\"' && tmp[*tmp_i + j] != '\'' && tmp[*tmp_i + j] != '$' && tmp[*tmp_i + j] != '*')
			j++;
	}
	else
	{
		while (tmp[*tmp_i + j] && tmp[*tmp_i + j] != '\"' && tmp[*tmp_i + j] != '\'' && tmp[*tmp_i + j] != '$' && tmp[*tmp_i + j] != '*')
			j++;
	}
	var = ft_substr(tmp, *tmp_i, j);
	return (var);
}

char	*get_var_sub(char *var, int *tmp_i)
{
	char	*var_sub;
	char	*tmp;
	char	*var_env;
	int		i;

	i = 0;
	while (var[i])
	{
		if (is_space(var[i]))
			break ;
		i++;
	}
	tmp = ft_substr(var, 0, i);
	var_env = getenv(tmp);
	if (var_env)
		var_sub = ft_strdup(var_env);
	else
		var_sub = NULL;
	return (var_sub);
}
