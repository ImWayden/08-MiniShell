/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:36:47 by wayden            #+#    #+#             */
/*   Updated: 2023/11/03 23:15:13 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main(int argc, char *argv[], char *envp[]) 
{
    char *input;

	sget_env(envp);
    while (1) {
        input = readline(RED "minishell:>" RESET); //display prompt
        if (input)
            add_history(input); //working history
		//treatment
        free(input);
    }
	clear_history();
    return 0;
}


