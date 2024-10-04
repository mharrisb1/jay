#include <stdlib.h>
#include <string.h>

#include <log.h/log.h>

#include "lexer.h"
#include "parser.h"

void parser_init(Parser *parser, const char *input) {
  debug("Initializing parser");
  lexer_init(&parser->lexer, input);
  parser->current_token = lexer_get_next_token(&parser->lexer);
}

void parser_advance(Parser *parser) {
  if (parser->current_token.type == TOKEN_STRING ||
      parser->current_token.type == TOKEN_NUMBER) {
    free(parser->current_token.value);
  }
  parser->current_token = lexer_get_next_token(&parser->lexer);
}

void parser_error(const char *message) {
  error("Parser error: %s", message);
  exit(EXIT_FAILURE);
}

JsonNode *parse_value(Parser *parser) {
  JsonNode *node = NULL;
  switch (parser->current_token.type) {
    case TOKEN_LEFT_BRACE:
      node = parse_object(parser);
      break;
    case TOKEN_LEFT_BRACKET:
      node = parse_array(parser);
      break;
    case TOKEN_STRING:
      node = json_create_string(parser->current_token.value);
      parser_advance(parser);
      break;
    case TOKEN_NUMBER: {
      double num = atof(parser->current_token.value);
      node       = json_create_number(num);
      parser_advance(parser);
      break;
    }
    case TOKEN_TRUE:
      node = json_create_bool(1);
      parser_advance(parser);
      break;
    case TOKEN_FALSE:
      node = json_create_bool(0);
      parser_advance(parser);
      break;
    case TOKEN_NULL:
      node = json_create_null();
      parser_advance(parser);
      break;
    default:
      parser_error("Unexpected token while parsing value");
  }
  return node;
}

JsonNode *parse_object(Parser *parser) {
  if (parser->current_token.type != TOKEN_LEFT_BRACE) {
    parser_error("Expected '{' at the beginning of object");
  }
  parser_advance(parser);

  JsonNode *object = json_create_object();

  if (parser->current_token.type == TOKEN_RIGHT_BRACE) {
    parser_advance(parser);
    return object;
  }

  while (1) {
    if (parser->current_token.type != TOKEN_STRING) {
      parser_error("Expected string as key in object");
    }
    char *key = strdup(parser->current_token.value);
    parser_advance(parser);

    if (parser->current_token.type != TOKEN_COLON) {
      free(key);
      parser_error("Expected ':' after key in object");
    }
    parser_advance(parser);

    JsonNode *value = parse_value(parser);
    json_object_add(object, key, value);
    free(key);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser_advance(parser);
      continue;
    } else if (parser->current_token.type == TOKEN_RIGHT_BRACE) {
      parser_advance(parser);
      break;
    } else {
      parser_error("Expected ',' or '}' in object");
    }
  }

  return object;
}

JsonNode *parse_array(Parser *parser) {
  if (parser->current_token.type != TOKEN_LEFT_BRACKET) {
    parser_error("Expected '[' at the beginning of array");
  }
  parser_advance(parser);

  JsonNode *array = json_create_array();

  if (parser->current_token.type == TOKEN_RIGHT_BRACKET) {
    parser_advance(parser);
    return array;
  }

  while (1) {
    JsonNode *element = parse_value(parser);
    json_array_add(array, element);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser_advance(parser);
      continue;
    } else if (parser->current_token.type == TOKEN_RIGHT_BRACKET) {
      parser_advance(parser);
      break;
    } else {
      parser_error("Expected ',' or ']' in array");
    }
  }

  return array;
}

JsonNode *parse_json(const char *input) {
  Parser parser;
  parser_init(&parser, input);
  JsonNode *root = parse_value(&parser);

  if (parser.current_token.type != TOKEN_EOF) {
    json_free(root);
    parser_error("Unexpected data after JSON value");
  }

  return root;
}
