#include <stdio.h>

#include "json.h"

#define COLOR_BLUE         "\033[0;34m"
#define COLOR_YELLOW_BOLD  "\033[1;33m"
#define COLOR_MAGENTA_BOLD "\033[1;35m"
#define COLOR_RESET        "\033[0m"

void pprint(const JsonNode *node, int indent) {
  if (!node)
    return;

  for (int i = 0; i < indent; i++)
    printf("  ");

  switch (node->type) {
    case JSON_NULL:
      printf(COLOR_YELLOW_BOLD "null" COLOR_RESET "\n");
      break;
    case JSON_BOOL:
      printf(COLOR_YELLOW_BOLD "%s" COLOR_RESET "\n",
             node->data.bool_value ? "true" : "false");
      break;
    case JSON_NUMBER:
      printf(COLOR_YELLOW_BOLD "%lf" COLOR_RESET "\n", node->data.number_value);
      break;
    case JSON_STRING:
      printf(COLOR_MAGENTA_BOLD "\"%s\"" COLOR_RESET "\n",
             node->data.string_value);
      break;
    case JSON_ARRAY: {
      printf("[\n");
      JsonList *current = node->data.array_elements;
      while (current) {
        pprint(current->node, indent + 1);
        current = current->next;
      }
      for (int i = 0; i < indent; i++)
        printf("  ");
      printf("]\n");
      break;
    }
    case JSON_OBJECT: {
      printf("{\n");
      JsonList *current = node->data.object_members;
      while (current) {
        JsonKeyValuePair *pair = (JsonKeyValuePair *)current->node;
        for (int i = 0; i < indent + 1; i++)
          printf("  ");
        printf(COLOR_BLUE "\"%s\"" COLOR_RESET ": ", pair->key);
        switch (pair->value->type) {
          case JSON_NULL:
            printf(COLOR_YELLOW_BOLD "null" COLOR_RESET "\n");
            break;
          case JSON_BOOL:
            printf(COLOR_YELLOW_BOLD "%s" COLOR_RESET "\n",
                   pair->value->data.bool_value ? "true" : "false");
            break;
          case JSON_NUMBER:
            printf(COLOR_YELLOW_BOLD "%lf" COLOR_RESET "\n",
                   pair->value->data.number_value);
            break;
          case JSON_STRING:
            printf(COLOR_MAGENTA_BOLD "\"%s\"" COLOR_RESET "\n",
                   pair->value->data.string_value);
            break;
          case JSON_ARRAY:
            printf("[\n");
            pprint(pair->value, indent + 2);
            for (int i = 0; i < indent + 1; i++)
              printf("  ");
            printf("]\n");
            break;
          case JSON_OBJECT:
            printf("{\n");
            pprint(pair->value, indent + 2);
            for (int i = 0; i < indent + 1; i++)
              printf("  ");
            printf("}\n");
            break;
        }
        current = current->next;
      }
      for (int i = 0; i < indent; i++)
        printf("  ");
      printf("}\n");
      break;
    }
    default:
      printf("Unknown type\n");
      break;
  }
}
