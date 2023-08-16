//#include "minishell_bonus.h"
//
//void	init_parse_tree(t_parse_tree *parse_tree)
//{
//	parse_tree->pair->connector = EMPTY;
//	parse_tree->pair->left = NULL;
//	parse_tree->pair->right = NULL;
//	parse_tree->pair = NULL;
//	parse_tree->next = NULL;
//}
//
//t_pair	*allocate_pair(void)
//{
//	t_pair *pair;
//
//	pair = ft_calloc(1, sizeof pair);
//	if (!pair)
//	{
//		ft_dprintf(2, "Error allocating memory to parse tree\n");
//		exit(GENERAL_ERROR);
//	}
//	return (pair);
//}
//
//t_command	*allocate_node(void)
//{
//	t_command *node;
//
//	node = ft_calloc(1, sizeof node);
//	if (!node)
//	{
//		ft_dprintf(2, "Error allocating memory to parse tree\n");
//		exit(GENERAL_ERROR);
//	}
//	return (node);
//}
