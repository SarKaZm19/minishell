/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:21:41 by fvastena          #+#    #+#             */
/*   Updated: 2023/12/28 15:27:39 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	Lexical analysis (cf lexer.md).
 * 			Converts the input string into a list of tokens.
 *
 * @return	A list of tokens (type + content).
**/

t_list	*lexer(char *input, t_shell *sh)
{
	t_token			*token;
	t_token_type	type;
	t_list			*token_list;
	t_list			*new_token;
	size_t			i;
	size_t			len;

	i = 0;
	token_list = NULL;
	while (input[i])
	{
		len = 0;
		if (!is_space(input[i]))
		{
			type = scan_next_token(input + i, &len, sh);
			// fprintf(stderr, "type: %d\n", type);
			if (type == TK_ERROR)
				return (report_syntax_error(sh));
			token = create_token(type, input + i, len, sh);
			new_token = ft_lstnew(token);
			s_alloc(new_token, PROMPT, sh);
			ft_lstadd_back(&token_list, new_token);
		}
		else
			len++;
		i += len;
	}
	return (token_list);
}

t_token	*create_token(t_token_type type, char *value, size_t len, t_shell *sh)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	s_alloc(token, PROMPT, sh);
	token->type = type;
	token->value = ft_substr(value, 0, len);
	s_alloc(token->value, PROMPT, sh);
	return (token);
}
