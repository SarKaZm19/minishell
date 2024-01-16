#include "minishell.h"

char	*expand_no_quotes(t_expander *exp, int *start_i)
{
	int		i;
	int		j;
	char	*expand;

	j = 0;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\"')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	exp->cmd_part_len = j;
	printf("cmd_part = %s, len = %d\n", exp->cmd_part, exp->cmd_part_len);
	i = 0;
	expand = NULL;
	while (exp->cmd_part[i])
	{
		j = i;
		while (exp->cmd_part[i] && exp->cmd_part[i] != '$')
			i++;
		exp->op_index = i;
		if (exp->cmd_part[i + j] == '$')
			expand = expand_var(exp, &i);
		else
			expand = ft_substr(exp->cmd_part, j, i - j);
		printf("expand = %s\n", expand);
		if (expand)
		{
			if (exp->new_cmd)
				exp->new_cmd = ft_strjoin(exp->new_cmd, expand);
			else
				exp->new_cmd = ft_strdup(expand);
		}
		if (expand)
		{
			free(expand);
			expand = NULL;
		}
	}
	*start_i += (i - j) + 1;
	return (exp->new_cmd);
	//si * dans une var, pas de substitution du *, interprete litteralement
	//voir si on peut pas lire tout d un coup et tout mettre dans un tableau apres dans expand
}

char	*expand_var_in_d_quotes(t_expander *exp, int *tmp_i)
{
	int		j;

	*tmp_i += 1;
	j = 0;
	while (exp->cmd_part[*tmp_i + j] && !is_space(exp->cmd_part[*tmp_i + j]) && ft_isalnum(exp->cmd_part[*tmp_i + j]))
		j++;
	exp->var_to_sub = ft_substr(exp->cmd_part, *tmp_i, j);
	exp->var_to_sub_len = j;
	*tmp_i += exp->var_to_sub_len;
	printf("exp->var_to_sub d_quotes = .%s., len = %d\n", exp->var_to_sub, exp->var_to_sub_len);
	printf("new_tmp_i = %d\n", *tmp_i);
	exp->subbed_var = getenv(exp->var_to_sub);
	exp->subbed_var_len = ft_strlen(exp->subbed_var);
	printf("exp->subbed_var d_quotes = .%s., len = %d\n", exp->subbed_var, exp->subbed_var_len);
	return (exp->subbed_var);
}

char	*expand_d_quote(t_expander *exp, int *start_i)
{
	int		i;
	int		j;
	char	*d_quote_expand;

	*start_i += 1;
	j = 0;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\"')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	i = 0;
	while (exp->cmd_part[i])
	{
		j = i;
		while (exp->cmd_part[i] && exp->cmd_part[i] != '$')
			i++;
		exp->op_index = i;
		if (exp->cmd_part[i + j] == '$')
			d_quote_expand = expand_var_in_d_quotes(exp, &i);
		else
			d_quote_expand = ft_substr(exp->cmd_part, j, i - j);
		if (d_quote_expand)
		{
			if (exp->new_cmd)
				exp->new_cmd = ft_strjoin(exp->new_cmd, d_quote_expand);
			else
				exp->new_cmd = ft_strdup(d_quote_expand);
		}
		if (d_quote_expand)
		{
			free(d_quote_expand);
			d_quote_expand = NULL;
		}
	}
	*start_i += (i - j) + 1;
	return (exp->new_cmd);
}

char	*expand_s_quote(t_expander *exp, int *start_i)
{
	int		j;
	char	*s_quote_expand;

	*start_i += 1;
	j = 0;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\'')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	*start_i += j + 1;
	if (!exp->new_cmd)
		s_quote_expand = ft_strdup(exp->cmd_part);	
	else
		s_quote_expand = ft_strjoin(exp->new_cmd, exp->cmd_part);
	return (s_quote_expand);
}
