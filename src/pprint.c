#include <stdio.h>

#include "json.h"

#define COLOR_BLUE         "\033[0;34m"
#define COLOR_YELLOW_BOLD  "\033[1;33m"
#define COLOR_MAGENTA_BOLD "\033[1;35m"
#define COLOR_RESET        "\033[0m"

void pprint(const JsonNode *node, int indent) {
  if (!node)
    return;

  switch (node->type) {
    case JSON_NULL:
      printf(COLOR_YELLOW_BOLD "null" COLOR_RESET);
      break;
    case JSON_BOOL:
      printf(COLOR_YELLOW_BOLD "%s" COLOR_RESET,
             node->data.bool_value ? "true" : "false");
      break;
    case JSON_NUMBER:
      printf(COLOR_YELLOW_BOLD "%lf" COLOR_RESET, node->data.number_value);
      break;
    case JSON_STRING:
      printf(COLOR_MAGENTA_BOLD "\"%s\"" COLOR_RESET, node->data.string_value);
      break;
    case JSON_ARRAY: {
      printf("[\n");
      JsonList *current = node->data.array_elements;
      while (current) {
        for (int i = 0; i < indent + 1; i++)
          printf("  ");

        pprint(current->node, indent + 1);

        if (current->next)
          printf(",");
        printf("\n");

        current = current->next;
      }
      for (int i = 0; i < indent; i++)
        printf("  ");
      printf("]");
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
        pprint(pair->value, indent + 1);

        if (current->next)
          printf(",");
        printf("\n");

        current = current->next;
      }
      for (int i = 0; i < indent; i++)
        printf("  ");
      printf("}");
      if (indent == 0) {
        printf("\n");
      }
      break;
    }
    default:
      printf("Unknown type\n");
      break;
  }
}
