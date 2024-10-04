#ifndef JSON_H
#define JSON_H

typedef enum {
  JSON_NULL,
  JSON_BOOL,
  JSON_NUMBER,
  JSON_STRING,
  JSON_ARRAY,
  JSON_OBJECT,
} JsonType;

typedef struct JsonNode JsonNode;

typedef struct {
  char     *key;
  JsonNode *value;
} JsonKeyValuePair;

typedef struct JsonList {
  JsonNode        *node;
  struct JsonList *next;
} JsonList;

typedef struct JsonNode {
  JsonType type;
  union {
    int       bool_value;
    double    number_value;
    char     *string_value;
    JsonList *array_elements;
    JsonList *object_members;
  } data;
} JsonNode;

JsonNode *json_create_null();
JsonNode *json_create_bool(int bool_value);
JsonNode *json_create_number(double number_value);
JsonNode *json_create_string(const char *string_value);

JsonNode *json_create_array();
void      json_array_add(JsonNode *array, JsonNode *element);

JsonNode *json_create_object();
void      json_object_add(JsonNode *object, const char *key, JsonNode *value);

void json_free(JsonNode *node);

#endif
