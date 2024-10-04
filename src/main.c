#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <log.h/log.h>

#include "json.h"
#include "parser.h"
#include "pprint.h"

#define MAX_BUFFER_SIZE 4096

LogLevel current_log_level;

typedef enum { COMMAND_PP } Command;

char *read_stdin(size_t buffer_size) {
  size_t position = 0;
  char  *buffer   = malloc(buffer_size);
  if (!buffer) {
    fprintf(stderr, "Error: Unable to allocate memory for input buffer.\n");
    exit(EXIT_FAILURE);
  }

  int c;
  while ((c = fgetc(stdin)) != EOF) {
    // Resize buffer if necessary
    if (position + 1 >= buffer_size) { // +1 for null terminator
      buffer_size *= 2;
      char *new_buffer = realloc(buffer, buffer_size);
      if (!new_buffer) {
        free(buffer);
        fprintf(stderr,
                "Error: Unable to reallocate memory for input buffer.\n");
        exit(EXIT_FAILURE);
      }
      buffer = new_buffer;
    }
    buffer[position++] = (char)c;
  }

  buffer[position] = '\0';

  return buffer;
}

int main(int argc, char **argv) {
  int opt;
  set_log_level(LOG_LEVEL_WARNING);

  const struct option longopts[] = {
      {"verbose", no_argument, 0, 'v'},
      {"quiet", no_argument, 0, 'q'},
      {"help", no_argument, 0, 'h'},
  };

  while ((opt = getopt_long(argc, argv, "vqh", longopts, NULL)) != -1) {
    switch (opt) {
      case 'v':
        if (get_log_level() > LOG_LEVEL_TRACE) {
          set_log_level(current_log_level - 1);
        }
        break;
      case 'q':
        if (get_log_level() < LOG_LEVEL_ERROR) {
          set_log_level(current_log_level + 1);
        }
        break;
      case 'h':
        printf("🐦 Jay - JSON utils\n");
        printf("\n");
        printf("Usage: %s [OPTIONS] [COMMAND]\n", argv[0]);
        printf("\n");
        printf("Commands:\n");
        printf("  pp\t\tPretty print (default)");
        printf("\n");
        printf("Options:\n");
        printf("  -v, --verbose\tIncrease logging verbosity\n");
        printf("  -q, --quiet\tDecrease logging verbosity\n");
        printf("  -h, --help\tPrint this message\n");
        exit(EXIT_SUCCESS);
      case '?':
        exit(EXIT_FAILURE);
      default:
        abort();
    }
  }

  Command command = COMMAND_PP;

  for (int i = optind; i < argc; i++) {
    // NOTE: when more commands are added handle here
    if (strcmp("pp", argv[i]) != 0) {
      error("Invalid command: %s", argv[i]);
      exit(EXIT_FAILURE);
    }
  }

  char     *json_input = read_stdin(MAX_BUFFER_SIZE);
  JsonNode *root       = parse_json(json_input);

  switch (command) {
    case COMMAND_PP:
      pprint(root, 0);
      break;
  }

  json_free(root);

  return 0;
}
