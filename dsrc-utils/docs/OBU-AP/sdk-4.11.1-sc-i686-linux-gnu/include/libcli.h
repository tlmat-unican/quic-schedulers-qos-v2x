#ifndef __LIBCLI_H__
#define __LIBCLI_H__

#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   libcli API
*/

#define CLI_OK 0
#define CLI_ERROR (-1)
#define CLI_QUIT (-2)
#define CLI_ERROR_ARG (-3)

#define PRIVILEGE_UNPRIVILEGED 0

#define MODE_ANY (-1)
#define MODE_EXEC 0

struct cli_def;
struct cli_command;

struct cli_command *
cli_register_command(struct cli_def *cli, struct cli_command *parent,
                     const char *command,
                     int (*callback)(struct cli_def *,
                                     const char *, char **, int), int privilege,
                     int mode, const char *help);
void cli_print(struct cli_def *cli, const char *format, ...)
  __attribute__((format (printf, 2, 3)));
void cli_set_banner(struct cli_def *cli, const char *banner);
void cli_set_hostname(struct cli_def *cli, const char *hostname);
struct cli_def *cli_init(void);
void cli_telnet_protocol(struct cli_def *cli, int telnet_protocol);
void cli_regular_interval(struct cli_def *cli, int seconds);
void cli_set_idle_timeout_callback(struct cli_def *cli,
                                   unsigned int seconds,
                                   int (*callback)(struct cli_def *));
void cli_set_auth_callback(struct cli_def *cli,
                           int (*auth_callback)(const char *, const char *));
void cli_set_enable_callback(struct cli_def *cli,
                             int (*enable_callback)(const char *));
void cli_print_callback(struct cli_def *cli,
                        void (*callback)(struct cli_def *, const char *));
int cli_file(struct cli_def *cli, FILE *fh, int privilege, int mode);
void cli_set_context(struct cli_def *cli, void *context);
void *cli_get_context(struct cli_def *cli);
int cli_loop(struct cli_def *cli, int sockfd);
int cli_done(struct cli_def *cli);

#ifdef __cplusplus
}
#endif

#endif /* __LIBCLI_H__ */
