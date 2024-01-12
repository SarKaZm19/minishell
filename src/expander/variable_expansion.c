/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 02:37:41 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/12 02:45:08 by fvastena         ###   ########.fr       */
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
	printf("get_var j = %d, tmp_i = %d\n", j, *tmp_i);
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
	printf("var_sub tmp = %s\n", tmp);
	var_env = getenv(tmp);
	printf("vr_env = %s\n", var_env);
	if (var_env)
		var_sub = ft_strdup(var_env);
	else
		var_sub = ft_strdup("");
	printf("var_sub = %s\n", var_sub);
	printf("size_var_sub_tmp = %zu\n", ft_strlen(tmp));
	printf("3new_tmp_i = %d\n", *tmp_i);

	return (var_sub);
}
