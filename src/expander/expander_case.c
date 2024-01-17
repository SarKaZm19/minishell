#include "minishell.h"

char	*expand_no_quotes(t_expander *exp, int *start_i)
{
	int		i;
	int		j;
	char	*expand;
	char	*tmp;

	j = 0;
	if (exp->cmd[*start_i] == '$')
		j++;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\"' && exp->cmd[*start_i + j] != '\'' && exp->cmd[*start_i + j] != '$')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	exp->cmd_part_len = j;
	printf("cmd_part = %s, len = %d\n", exp->cmd_part, exp->cmd_part_len);
	i = 0;
	expand = NULL;
	while (exp->cmd_part[i])
	{
		j = 0;
		if (exp->cmd_part[i] == '$')
			expand = expand_var(exp, &i);
		else
		{
			while (exp->cmd_part[i + j] && exp->cmd_part[i + j] != '$')
				j++;
			expand = ft_substr(exp->cmd_part, i, j);
		}
		printf("expand = %s\n", expand);
		i += j;
		if (expand)
		{
			if (exp->new_cmd)
			{
				tmp = ft_strdup(exp->new_cmd);
				free(exp->new_cmd);
				exp->new_cmd = ft_strjoin(tmp, expand);
				free(tmp);
				tmp = NULL;
			}
			else
				exp->new_cmd = ft_strdup(expand);
		}
	}
	*start_i += i;
	return (exp->new_cmd);
	//si * dans une var, pas de substitution du *, interprete litteralement
	//voir si on peut pas lire tout d un coup et tout mettre dans un tableau apres dans expand
}

char	*expand_var_in_d_quotes(t_expander *exp, int *tmp_i)
{
	int		j;
	char	*sub_env;

	sub_env = NULL;
	*tmp_i += 1;
	j = 0;
	while (exp->cmd_part[*tmp_i + j] && !is_space(exp->cmd_part[*tmp_i + j]) && ft_isalnum(exp->cmd_part[*tmp_i + j]))
		j++;
	exp->var_to_sub = ft_substr(exp->cmd_part, *tmp_i, j);
	exp->var_to_sub_len = j;
	*tmp_i += exp->var_to_sub_len;
	printf("exp->var_to_sub d_quotes = .%s., len = %d\n", exp->var_to_sub, exp->var_to_sub_len);
	printf("new_tmp_i = %d\n", *tmp_i);
	sub_env = getenv(exp->var_to_sub);
	if (sub_env)
		exp->subbed_var = ft_strdup(sub_env);
	exp->subbed_var_len = ft_strlen(exp->subbed_var);
	if (exp->var_to_sub)
	{
		free(exp->var_to_sub);
		exp->var_to_sub = NULL;
	}
	printf("exp->subbed_var d_quotes = .%s., len = %d\n", exp->subbed_var, exp->subbed_var_len);
	return (exp->subbed_var);
}

char	*expand_d_quote(t_expander *exp, int *start_i) // !!! "$test" --> Command '' not found donc $test sans var est une string vide
{
	int		i;
	int		j;
	char	*d_quote_expand;
	char	*tmp;

	*start_i += 1;
	j = 0;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\"')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	exp->cmd_part_len = j;
	printf("exp->cmd_part = %s, len = %d\n", exp->cmd_part, exp->cmd_part_len);
	i = 0;
	d_quote_expand = NULL;
	while (exp->cmd_part[i])
	{
		j = 0;
		while (exp->cmd_part[i + j] && exp->cmd_part[i + j] != '$')
			j++;
		printf("start $ or end = %d, .%c.\n", j, exp->cmd_part[i + j]);
		printf("start of cmd_part = i = %d, .%c.\n", i, exp->cmd_part[i]);
		if (exp->cmd_part[i] == '$')
			d_quote_expand = expand_var_in_d_quotes(exp, &i);
		else
			d_quote_expand = ft_substr(exp->cmd_part, i, j);
		printf("d_quote_expand = %s\n", d_quote_expand);
		i += j;
		if (d_quote_expand)
		{
			if (exp->new_cmd)
			{
				tmp = ft_strdup(exp->new_cmd);
				free(exp->new_cmd);
				exp->new_cmd = ft_strjoin(tmp, d_quote_expand);
				free(tmp);
				tmp = NULL;
			}
			else
				exp->new_cmd = ft_strdup(d_quote_expand);
		}
	}
	*start_i += i + 1;
	return (exp->new_cmd);
}

char	*expand_s_quote(t_expander *exp, t_shell *sh, int *start_i)
{
	int		j;
	char	*s_quote_expand;

	*start_i += 1;
	s_quote_expand = NULL;
	j = 0;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\'')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	s_alloc(exp->cmd_part, ONLY_CHECK, sh);
	*start_i += j + 1;
	if (!exp->new_cmd)
	{
		if (exp->cmd_part)
		{
			s_quote_expand = ft_strdup(exp->cmd_part);	
			free(exp->cmd_part);
			exp->cmd_part = NULL;
		}
	}
	else
	{
		if (exp->cmd_part)
		{
			s_quote_expand = ft_strjoin(exp->new_cmd, exp->cmd_part);
			free(exp->cmd_part);
			exp->cmd_part = NULL;
			free(exp->new_cmd);
			exp->new_cmd = NULL;
		}
	}
	return (s_quote_expand);
}
