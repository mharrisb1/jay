#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <log.h/log.h>

#include "json.h"
#include "pprint.h"

typedef enum { COMMAND_PP } Command;

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
        if (current_log_level > LOG_LEVEL_TRACE) {
          set_log_level(current_log_level - 1);
        }
        break;
      case 'q':
        if (current_log_level < LOG_LEVEL_ERROR) {
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

  // TODO: remove
  JsonNode *root = json_create_object();

  json_object_add(root, "name", json_create_string("John Doe"));

  json_object_add(root, "age", json_create_number(30));

  json_object_add(root, "ethnicity", json_create_null());

  json_object_add(root, "is_student", json_create_bool(0));

  JsonNode *courses = json_create_array();
  json_array_add(courses, json_create_string("Math"));
  json_array_add(courses, json_create_string("Science"));
  json_object_add(root, "courses", courses);

  JsonNode *address = json_create_object();
  json_object_add(address, "street", json_create_string("123 Main St"));
  json_object_add(address, "city", json_create_string("Anytown"));
  json_object_add(root, "address", address);
  // TODO: remove

  Command command = COMMAND_PP;

  for (int i = optind; i < argc; i++) {
    // NOTE: when more commands are added handle here
    if (strcmp("pp", argv[i]) != 0) {
      error("Invalid command: %s", argv[i]);
      exit(EXIT_FAILURE);
    }
  }

  switch (command) {
    case COMMAND_PP:
      pprint(root, 0);
      break;
  }

  json_free(root);

  return 0;
}
