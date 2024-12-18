#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXEC 0
#define PIPE 1
#define LIST 2

#define MAXARGS 10

typedef struct s_cmd {
  int type;
  struct s_cmd *left;
  struct s_cmd *right;
  char *argv[MAXARGS];
} t_cmd;

size_t ft_strlen(char *s) {
  size_t i = 0;
  if (!s)
    return 0;
  while (s[i])
    i++;
  return i;
}

void print_error(char *s1, char *s2) {
  if (s1)
    write(2, s1, ft_strlen(s1));
  if (s2)
    write(2, s2, ft_strlen(s2));
  write(2, "\n", 1);
}

void fatal_error(void) {
  print_error("error: fatal", 0);
  exit(1);
}

t_cmd *init_cmd(int type, t_cmd *left, t_cmd *right) {
  t_cmd *cmd = malloc(sizeof(t_cmd));
  if (!cmd)
    fatal_error();
  cmd->type = type;
  cmd->left = left;
  cmd->right = right;
  for (int i = 0; i < MAXARGS; i++)
    cmd->argv[i] = 0;
  return cmd;
}

int xfork(void) {
  int pid = fork();
  if (pid == -1)
    fatal_error();
  return pid;
}

t_cmd *parse_exec(int *i, char **argv) {
  t_cmd *cmd = init_cmd(EXEC, 0, 0);
  int argc = 0;
  while (argv[*i] && strcmp(argv[*i], "|") && strcmp(argv[*i], ";")) {
    cmd->argv[argc++] = argv[(*i)++];
  }
  return cmd;
}

t_cmd *parse_pipe(int *i, char **argv) {
  t_cmd *cmd = parse_exec(i, argv);
  if (argv[*i] && !strcmp(argv[*i], "|")) {
    (*i)++;
    cmd = init_cmd(PIPE, cmd, parse_pipe(i, argv));
  }
  return cmd;
}

t_cmd *parse_list(int *i, char **argv) {
  t_cmd *cmd = parse_pipe(i, argv);
  if (argv[*i] && !strcmp(argv[*i], ";")) {
    (*i)++;
    cmd = init_cmd(LIST, cmd, parse_list(i, argv));
  }
  return cmd;
}

void builtin_cd(t_cmd *cmd) {
  if (!cmd->argv[1] || cmd->argv[2]) {
    print_error("error: cd: bad arguments", 0);
  } else if (chdir(cmd->argv[1]) != 0) {
    print_error("error: cd: cannot change directory to ", cmd->argv[1]);
  }
}

void run_cmd(t_cmd *cmd, char **envp) {
  if (!cmd)
    return;
  if (cmd->type == EXEC && cmd->argv[0]) {
    if (strcmp(cmd->argv[0], "cd") == 0) {
      builtin_cd(cmd);
      return;
    }
    if (xfork() == 0) {
      execve(cmd->argv[0], cmd->argv, envp);
      print_error("error: cannot execute ", cmd->argv[0]);
      exit(1);
    }
    wait(0);
  } else if (cmd->type == PIPE) {
    int p[2];
    pipe(p);
    if (xfork() == 0) {
      dup2(p[1], STDOUT_FILENO);
      close(p[0]);
      close(p[1]);
      run_cmd(cmd->left, envp);
      exit(1);
    }
    if (xfork() == 0) {
      dup2(p[0], STDIN_FILENO);
      close(p[0]);
      close(p[1]);
      run_cmd(cmd->right, envp);
      exit(1);
    }
    close(p[0]);
    close(p[1]);
    wait(0);
    wait(0);
  } else if (cmd->type == LIST) {
    run_cmd(cmd->left, envp);
    run_cmd(cmd->right, envp);
  }
}

void free_cmd(t_cmd *cmd) {
  if (!cmd)
    return;
  if (cmd->type == PIPE || cmd->type == LIST) {
    free_cmd(cmd->left);
    free_cmd(cmd->right);
  }
  free(cmd);
}

int main(int argc, char **argv, char **envp) {
  int index = 1;
  t_cmd *cmd = parse_list(&index, argv);
  if (index < argc)
    print_error("syntax", 0);
  run_cmd(cmd, envp);
  free_cmd(cmd);
  return 0;
}
