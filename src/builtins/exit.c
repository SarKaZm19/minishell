#include "minishell.h"

int	exit_builtin(t_ast_command *cmd, t_shell *sh)
{
	if (cmd && cmd->cmd_exe && count_args(cmd->cmd_exe) >= 2)
	{
		if (!is_valid_64bit_signed_integer(cmd->cmd_exe[1]))
		{
			s_write_fd("minishell: exit: ", STDERR_FILENO, sh);
			s_write_fd(cmd->cmd_exe[1], STDERR_FILENO, sh);
			s_write_fd(": numeric argument required\n", STDERR_FILENO, sh);
			quit_shell(255, sh);
		}
		else if (count_args(cmd->cmd_exe) > 2)
		{
			s_write_fd("minishell: exit: too many arguments\n", STDERR_FILENO,
				sh);
			return (EXIT_FAILURE);
		}
		else
			quit_shell((uint8_t)ft_atoi_long_long(cmd->cmd_exe[1]), sh);
	}
	quit_shell(sh->last_prompt_exit_status, sh);
	return (EXIT_SUCCESS);
}

// todo: rename to strs_len ? could be use in other situations ?
int	count_args(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

bool	is_number(const char *str)
{
	if (!str || *str == '\0')
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

bool	is_valid_64bit_signed_integer(const char *str)
{
	const long long	MAX_DIV_10 = LLONG_MAX / 10;
	long long		value;
	int				negative_flag;
	int				digit;

	if (!is_number(str))
		return (false);
	negative_flag = 0;
	if (*str == '-' || *str == '+')
	{
		negative_flag = (*str == '-');
		str++;
	}
	value = 0;
	while (*str)
	{
		digit = *str - '0';
		if (value > MAX_DIV_10 || (value == MAX_DIV_10 && digit > (7
					+ negative_flag)))
			return (false);
		value = value * 10 + digit;
		str++;
	}
	return (true);
}

long long	ft_atoi_long_long(const char *str)
{
	long long	value;
	int			sign;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	value = 0;
	while (ft_isdigit(*str))
		value = value * 10 + (*str++ - '0');
	return (sign * value);
}
