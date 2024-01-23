#include "minishell.h"

char	*trim_white_spaces(t_expander *exp, char *sub_env)
{
	char	*res;
	int		i;
	int		size;
	int		j;

	i = 0;
	size = 0;
	while (is_space(sub_env[i]))
		i++;
	while (i < exp->subbed_var_len)
	{
		if (!is_space(sub_env[i]))
			size++;
		i++;
	}
	res = malloc(sizeof(char) * (size + exp->nb_split_parts + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (sub_env[i] && is_space(sub_env[i]))
		i++;
	while (i < exp->subbed_var_len)
	{
		if (!is_space(sub_env[i]))
		{
			res[j] = sub_env[i];
			j++;
			i++;
		}
		else
		{
			while (i < exp->subbed_var_len && is_space(sub_env[i]))
				i++;
			if (i < exp->subbed_var_len)
			{
				res[j] = ' ';
				j++;
			}
		}
	}
	res[j] = '\0';
	return (res);
}

int		count_parts(char *subbed)
{
	int	i;
	int	nb_w;

	i = 0;
	nb_w = 0;
	while (is_space(subbed[i]))
		i++;
	while (subbed[i])
	{
		if (!is_space(subbed[i]))
		{
			nb_w++;
			while (subbed[i] && !is_space(subbed[i]))
				i++;
		}
		else
		{
			while (subbed[i] && is_space(subbed[i]))
			i++;
		}
	}
	printf("nb_w = %d\n", nb_w);
	return (nb_w);
}

char	*expand_var(t_expander *exp, int *tmp_i)
{
	int		j;
	char	*sub_env;

	*tmp_i += 1;
	j = 0;
	while (exp->arg_part[*tmp_i + j] && ft_isalnum(exp->arg_part[*tmp_i + j]))
		j++;
	exp->var_to_sub = ft_substr(exp->arg_part, *tmp_i, j);
	exp->var_to_sub_len = j;
	*tmp_i += exp->var_to_sub_len;
	sub_env = getenv(exp->var_to_sub);
	if (!sub_env)
	{
		exp->subbed_var = NULL;
		exp->subbed_var_len = 0;
		exp->nb_split_parts = 0;
		return (NULL);
	}
	else
	exp->subbed_var = ft_strdup(sub_env);
	exp->subbed_var_len = ft_strlen(exp->subbed_var);
	exp->nb_split_parts = count_parts(exp->subbed_var);
	exp->subbed_var = trim_white_spaces(exp, exp->subbed_var);
	return (exp->subbed_var);
}

char	*no_quotes_dollar_expand(t_expander *exp, t_shell *sh, int *part_i)
{
	char	*nq_dollar_expand;

	if (!exp->arg_part[*part_i + 1] ||
		(exp->arg_part[*part_i + 1] &&
		(exp->arg_part[*part_i + 1] == '\'' || is_space(exp->arg_part[*part_i + 1]))))
	{
		nq_dollar_expand = ft_strdup("$");
		*part_i += 1;
	}
	else if (exp->arg_part[*part_i + 1] == '?')
	{
		nq_dollar_expand = ft_itoa(sh->last_prompt_exit_status);
		*part_i += 2;
	}
	else
		nq_dollar_expand = expand_var(exp, part_i);
	s_alloc(nq_dollar_expand, EXPANDER, sh);
	return (nq_dollar_expand);
}

static char	*update_arg(t_shell *sh, char *var1, char *var2)
{
	char	*updated;

	updated = NULL;
	if (!var1)
		updated = ft_strdup(var2);
	else
		updated = ft_strjoin(var1, var2);
	s_alloc(updated, EXPANDER, sh);
	return (updated);
}

char	*no_quotes_expand(t_expander *exp, t_shell *sh)
{
	char	*expand_part;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	expand_part = NULL;
	tmp = NULL;
	while (exp->arg_part[i])
	{
		j = 0;
		if (exp->arg_part[i] == '$')
			expand_part = no_quotes_dollar_expand(exp, sh, &i);
		else
		{
			while (exp->arg_part[i + j] && exp->arg_part[i + j] != '$')
				j++;
			expand_part = ft_substr(exp->arg_part, i, j);
			printf("expand_part = .%s.\n", expand_part);
			s_alloc(expand_part, EXPANDER, sh);
			printf("......\n");
		}
		tmp = update_arg(sh, tmp, expand_part);
		i += j;
	}
	return (update_arg(sh, exp->new_arg, tmp));
}

char	*treat_no_quotes(t_expander *exp, t_shell *sh, int *start_i)
{
	int		i;
	char	*ret;

	i = 0;
	if (exp->arg[*start_i] == '$')
		i++;
	while (exp->arg[*start_i + i] && exp->arg[*start_i + i] != '\"' && exp->arg[*start_i + i] != '\'' && exp->arg[*start_i + i] != '$')
		i++;
	exp->arg_part = ft_substr(exp->arg, *start_i, i);
	s_alloc(exp->arg_part, ONLY_CHECK, sh);
	printf("exp->arg_part = %s\n", exp->arg_part);
	exp->arg_part_len = i;
	ret = no_quotes_expand(exp, sh);
	*start_i += i;
	free(exp->arg_part);
	exp->arg_part = NULL;
	printf("ret = %s\n", ret);
	return (ret);
}
