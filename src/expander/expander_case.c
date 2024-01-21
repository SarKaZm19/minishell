#include "minishell.h"

char	*treat_no_quotes(t_expander *exp, t_shell *sh, int *start_i)
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
	i = 0;
	expand = NULL;
	while (exp->cmd_part[i])
	{
		j = 0;
		if (exp->cmd_part[i] == '$')
		{
			if (!exp->cmd_part[i + 1] || (exp->cmd_part[i + 1] && (exp->cmd_part[i + 1] == '\'' || is_space(exp->cmd_part[i + 1]))))
			{
				expand = ft_strdup("$");
				i += 1;
			}
			else if (exp->cmd_part[i + 1] == '?')
			{
				expand = ft_itoa(sh->last_prompt_exit_status);
				i += 2;
			}
			else
				expand = expand_var(exp, &i);

		}
		else
		{
			while (exp->cmd_part[i + j] && exp->cmd_part[i + j] != '$')
				j++;
			expand = ft_substr(exp->cmd_part, i, j);
		}
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
			free(expand);
			expand = NULL;
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
	sub_env = getenv(exp->var_to_sub);
	if (sub_env)
		exp->subbed_var = ft_strdup(sub_env);
	exp->subbed_var_len = ft_strlen(exp->subbed_var);
	if (exp->var_to_sub)
	{
		free(exp->var_to_sub);
		exp->var_to_sub = NULL;
	}
	return (exp->subbed_var);
}

/*
// Isolate the double quotes part inside the command
// treat it and return it as a string
// if the " contains a solo $ or $ is followed by ?, we execute special functions
// if "normal" dollar case, expand the var and add it to the " treated string
*/
char	*treat_d_quotes(t_expander *exp, t_shell *sh, int *start_i) // !!! "$test" --> Command '' not found donc $test sans var est une string vide
{
	int		i;
	int		j;
	char	*d_quote_expand;
	char	*tmp;

	(void)sh;
	*start_i += 1;
	j = 0;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\"')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	exp->cmd_part_len = j;
	i = 0;
	d_quote_expand = NULL;
	while (exp->cmd_part[i])
	{
		j = 0;
		if (exp->cmd_part[i] == '$')
		{
			if (!exp->cmd_part[i + 1] || (exp->cmd_part[i + 1] && (exp->cmd_part[i + 1] == '\'' || is_space(exp->cmd_part[i + 1]))))
			{
				d_quote_expand = ft_strdup("$");
				i += 1;
			}
			else if (exp->cmd_part[i + 1] == '?')
			{
				d_quote_expand = ft_itoa(sh->last_prompt_exit_status);
				i += 2;
			}
			else
				d_quote_expand = expand_var_in_d_quotes(exp, &i);

		}
		else
		{
			while (exp->cmd_part[i + j] && exp->cmd_part[i + j] != '$')
				j++;
			d_quote_expand = ft_substr(exp->cmd_part, i, j);
		}
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

/*
// Isolate the ' ' part of the cmd, and treat it
// return it as a string, we join or dup depending on the value of new_cmd
*/
char	*treat_s_quotes(t_expander *exp, t_shell *sh, int *start_i)
{
	int		j; // end_sequence
	char	*s_quote_expand;

	*start_i += 1; // start_sequence
	s_quote_expand = NULL;
	j = 0;
	while (exp->cmd[*start_i + j] && exp->cmd[*start_i + j] != '\'')
		j++;
	exp->cmd_part = ft_substr(exp->cmd, *start_i, j);
	s_alloc(exp->cmd_part, ONLY_CHECK, sh);
	*start_i += j + 1;
	if (!exp->new_cmd) // new_cmd is not a command: string ou sub_arg
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
