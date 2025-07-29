/***************************************************************************//**

  @file         main.c

  @author       Utkarsh Dabral

  @date         29 July 2025

  @brief        USH - Utkarsh's SHell (inspired by Stephen Brennan's LSH)

*******************************************************************************/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Built-in command declarations
int ush_cd(char **args);
int ush_help(char **args);
int ush_exit(char **args);
char **ush_split_line(char *line);  

// Built-in command lookup
char *internal_cmds[] = {
  "cd",
  "help",
  "exit"
};

int (*internal_funcs[]) (char **) = {
  &ush_cd,
  &ush_help,
  &ush_exit
};

int ush_num_builtins() {
  return sizeof(internal_cmds) / sizeof(char *);
}

// Change directory command
int ush_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "ush: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("ush");
    }
  }
  return 1;
}

// Help command
int ush_help(char **args)
{
  printf("Welcome to USH - Utkarsh's Shell\n");
  printf("Available built-in commands:\n");

  for (int i = 0; i < ush_num_builtins(); i++) {
    printf("  %s\n", internal_cmds[i]);
  }

  printf("Use system commands as you would in any UNIX shell.\n");
  return 1;
}

// Exit command
int ush_exit(char **args)
{
  return 0;
}

// Launch non-built-in programs
int ush_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("ush");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("ush");
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// Execute a single command (built-in or external)
int ush_run_command(char **args)
{
  if (args[0] == NULL) {
    return 1;
  }

  for (int i = 0; i < ush_num_builtins(); i++) {
    if (strcmp(args[0], internal_cmds[i]) == 0) {
      return (*internal_funcs[i])(args);
    }
  }

  return ush_launch(args);
}

// Handle chaining commands with && and ||
int ush_handle_logical_ops(char *line)
{
  char *and_sep = strstr(line, "&&");
  char *or_sep = strstr(line, "||");

  if (and_sep != NULL) {
    *and_sep = '\0';
    char *first = line;
    char *second = and_sep + 2;

    char **args1 = ush_split_line(first);
    int status = ush_run_command(args1);
    free(args1);

    if (status == 1) {
      char **args2 = ush_split_line(second);
      status = ush_run_command(args2);
      free(args2);
    }
    return status;
  }

  if (or_sep != NULL) {
    *or_sep = '\0';
    char *first = line;
    char *second = or_sep + 2;

    char **args1 = ush_split_line(first);
    int status = ush_run_command(args1);
    free(args1);

    if (status != 1) {
      char **args2 = ush_split_line(second);
      status = ush_run_command(args2);
      free(args2);
    }
    return status;
  }

  // If no logical operators, run normally
  char **args = ush_split_line(line);
  int status = ush_run_command(args);
  free(args);
  return status;
}

// Read user input
char *ush_read_input(void)
{
#define USH_BUFSIZE 1024
  int bufsize = USH_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "ush: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();
    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position++] = c;
    }

    if (position >= bufsize) {
      bufsize += USH_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "ush: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

// Tokenize input by spaces, tabs, etc.
char **ush_split_line(char *line)
{
#define USH_TOK_BUFSIZE 64
#define USH_TOK_DELIM " \t\r\n\a"

  int bufsize = USH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "ush: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, USH_TOK_DELIM);
  while (token != NULL) {
    tokens[position++] = token;

    if (position >= bufsize) {
      bufsize += USH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "ush: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, USH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

// The main loop of the shell
void ush_main_loop(void)
{
  char *line;
  int status;

  do {
    printf("ush> ");
    line = ush_read_input();
    status = ush_handle_logical_ops(line);
    free(line);
  } while (status);
}

// Entry point
int main(int argc, char **argv)
{
  printf("Starting USH - Utkarsh's Shell\n");
  ush_main_loop();
  return EXIT_SUCCESS;
}
