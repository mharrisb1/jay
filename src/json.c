#include <stdlib.h>
#include <string.h>

#include "json.h"

JsonNode *json_create_null() {
  JsonNode *node = malloc(sizeof(JsonNode));
  node->type     = JSON_NULL;
  return node;
}

JsonNode *json_create_bool(int bool_value) {
  JsonNode *node        = malloc(sizeof(JsonNode));
  node->type            = JSON_BOOL;
  node->data.bool_value = bool_value ? 1 : 0;
  return node;
}

JsonNode *json_create_number(double number_value) {
  JsonNode *node          = malloc(sizeof(JsonNode));
  node->type              = JSON_NUMBER;
  node->data.number_value = number_value;
  return node;
}

JsonNode *json_create_string(const char *string_value) {
  JsonNode *node          = malloc(sizeof(JsonNode));
  node->type              = JSON_STRING;
  node->data.string_value = strdup(string_value);
  return node;
}

JsonNode *json_create_array() {
  JsonNode *node            = malloc(sizeof(JsonNode));
  node->type                = JSON_ARRAY;
  node->data.array_elements = NULL;
  return node;
}

void json_array_add(JsonNode *array, JsonNode *element) {
  if (array->type != JSON_ARRAY)
    return;

  JsonList *new_elem = malloc(sizeof(JsonList));
  new_elem->node     = element;
  new_elem->next     = NULL;

  if (!array->data.array_elements) {
    array->data.array_elements = new_elem;
  } else {
    JsonList *current = array->data.array_elements;
    while (current->next) {
      current = current->next;
    }
    current->next = new_elem;
  }
}

JsonNode *json_create_object() {
  JsonNode *node            = malloc(sizeof(JsonNode));
  node->type                = JSON_OBJECT;
  node->data.object_members = NULL;
  return node;
}

void json_object_add(JsonNode *object, const char *key, JsonNode *value) {
  if (object->type != JSON_OBJECT)
    return;

  JsonKeyValuePair *pair = malloc(sizeof(JsonKeyValuePair));
  pair->key              = strdup(key);
  pair->value            = value;

  JsonList *new_member = malloc(sizeof(JsonList));
  new_member->node     = (JsonNode *)pair;
  new_member->next     = NULL;

  if (!object->data.object_members) {
    object->data.object_members = new_member;
  } else {
    JsonList *current = object->data.object_members;
    while (current->next) {
      current = current->next;
    }
    current->next = new_member;
  }
}

void json_free(JsonNode *node) {
  if (!node)
    return;

  switch (node->type) {
    case JSON_STRING:
      free(node->data.string_value);
      break;
    case JSON_ARRAY: {
      JsonList *current = node->data.array_elements;
      while (current) {
        JsonList *next = current->next;
        json_free(current->node);
        free(current);
        current = next;
      }
      break;
    }
    case JSON_OBJECT: {
      JsonList *current = node->data.object_members;
      while (current) {
        JsonList         *next = current->next;
        JsonKeyValuePair *pair = (JsonKeyValuePair *)current->node;
        free(pair->key);
        json_free(pair->value);
        free(pair);
        free(current);
        current = next;
      }
      break;
    }
    default:
      break;
  }

  free(node);
}
