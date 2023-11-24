/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 09:59:29 by wayden            #+#    #+#             */
/*   Updated: 2023/11/24 04:38:29 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void afficher_scmd(const t_scmd *cmd) {
    printf("Commande : %s\n", cmd->cmd);
	printf("%d",cmd->is_builtin);
    printf("Arguments : ");
    if (cmd->args != NULL) {
        char **arg = cmd->args;
        while (*arg != NULL) {
            printf("%s ", *arg);
            arg++;
        }
    }
    printf("\n");
    printf("Est une commande intégrée : %d\n", cmd->is_builtin);
}

void serialize(t_cmd *cmd)
{
	int file;
	char *itoa;
	int i;

	i = 1;
	file = p_open(".serializedcmd", O_CREAT | O_TRUNC | O_RDWR, 0666);
	itoa = ft_itoa(cmd->is_builtin);
	write(file, cmd->cmd, ft_strlen(cmd->cmd));
	write(file, "\n", 1);
	write(file, itoa, ft_strlen(itoa));
	free(itoa);
	write(file, "\n", 1);
	while(cmd->args[i] != NULL)
	{
		write(file, cmd->args[i], ft_strlen(cmd->args[i]) + 1);
		write(file, "\n", 1);
		i++;
	}
	close(file);
}

t_scmd *unserialize(void)
{
	t_scmd *cmd;
	char *line;
	int file;
	int i;
	
	i = 1;
	cmd = (t_scmd *)malloc(sizeof(t_scmd));
	cmd->args = NULL;
	file = open(".serializedcmd", O_CREAT | O_RDWR, 0666);
	if(file == -1)
		return(NULL);
	cmd->cmd = get_next_line(file);
	if(!cmd->cmd)
		return(NULL);
	line = get_next_line(file);
	cmd->is_builtin = ft_atoi(line);
	free(line);
	line = get_next_line(file);
	cmd->args = NULL;
	while(line)
	{
		//printf("\nline  == %s\n",line); //debug
		cmd->args = insert_args_in_tab(cmd->args, line);
		line = get_next_line(file);
	}
	close(file);
	return(cmd);
}


t_scmd *sget_scmd(void)
{
	static t_scmd *cmd;	
	static bool is_init = FALSE;
	if(!sget_init(SCMD, NOP) && sget_init(SCMD, SET))
	{
		cmd = unserialize();
		if(!cmd)
			return(NULL);
		//afficher_scmd(cmd);
	}
	return(cmd);
}