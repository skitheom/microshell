
// #include <stdio.h> // debug printf用途

// void print_cmd(t_cmd *cmd) {
//   if (!cmd)
//     return;
//   switch (cmd->type) {
//   case EXEC:
//     printf("EXEC cmd: ");
//     for (int i = 0; cmd->argv[i]; i++) {
//       printf("%s,", cmd->argv[i]);
//     }
//     printf("\n");
//     break;
//   case PIPE:
//     printf("PIPE cmd: \n");
//     printf("---PIPE LEFT---\n");
//     print_cmd(cmd->left);
//     printf("---PIPE RIGHT---\n");
//     print_cmd(cmd->right);
//     break;
//   case LIST:
//     printf("LIST cmd: \n");
//     printf("---LIST LEFT---\n");
//     print_cmd(cmd->left);
//     printf("---LIST RIGHT---\n");
//     print_cmd(cmd->right);
//     break;
//   default:
//     printf("error\n");
//   }
// }
