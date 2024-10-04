#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <log.h/log.h>

#include "lexer.h"

void lexer_init(Lexer *lexer, const char *input) {
  debug("Initializing lexer");
  lexer->input = input;
  lexer->pos   = 0;
  lexer->lenth = strlen(input);
}

char lexer_peek(Lexer *lexer) {
  if (lexer->pos > lexer->lenth)
    return '\0';
  return lexer->input[lexer->pos];
}

char lexer_next(Lexer *lexer) {
  if (lexer->pos > lexer->lenth)
    return '\0';
  return lexer->input[lexer->pos++];
}

void lexer_skip_whitespace(Lexer *lexer) {
  while (isspace(lexer_peek(lexer))) {
    lexer_next(lexer);
  }
}

char *lexer_parse_string(Lexer *lexer) {
  if (lexer_next(lexer) != '\"')
    return NULL;

  size_t buffer_size = 256;
  size_t buffer_pos  = 0;
  char  *buffer      = malloc(buffer_size);
  if (!buffer)
    return NULL;

  while (lexer->pos < lexer->lenth) {
    char c = lexer_next(lexer);
    if (c == '\"') {
      buffer[buffer_pos] = '\0';
      return buffer;
    }
    if (c == '\\') {
      c = lexer_next(lexer);
      switch (c) {
        case '\"':
          buffer[buffer_pos++] = '\"';
          break;
        case '\\':
          buffer[buffer_pos++] = '\\';
          break;
        case '/':
          buffer[buffer_pos++] = '/';
          break;
        case 'b':
          buffer[buffer_pos++] = '\b';
          break;
        case 'f':
          buffer[buffer_pos++] = '\f';
          break;
        case 'n':
          buffer[buffer_pos++] = '\n';
          break;
        case 'r':
          buffer[buffer_pos++] = '\r';
          break;
        case 't':
          buffer[buffer_pos++] = '\t';
          break;
        case 'u':
          for (int i = 0; i < 4; i++) {
            if (!isxdigit(lexer_peek(lexer))) {
              free(buffer);
              return NULL;
            }
            lexer_next(lexer);
          }
          break;
        default:
          free(buffer);
          return NULL;
      }
    } else {
      buffer[buffer_pos++] = c;
    }

    if (buffer_pos >= buffer_size - 1) {
      buffer_size <<= 1;
      char *new_buffer = realloc(buffer, buffer_size);
      if (!new_buffer) {
        free(buffer);
        return NULL;
      }
      buffer = new_buffer;
    }
  }
  free(buffer);
  return NULL;
}

char *lexer_parse_number(Lexer *lexer) {
  size_t start = lexer->pos;
  if (lexer_peek(lexer) == '-')
    lexer_next(lexer);

  if (lexer_peek(lexer) == '0') {
    lexer_next(lexer);
  } else if (isdigit(lexer_peek(lexer))) {
    while (isdigit(lexer_peek(lexer)))
      lexer_next(lexer);
  } else {
    return NULL;
  }

  if (lexer_peek(lexer) == '.') {
    lexer_next(lexer);
    if (!isdigit(lexer_peek(lexer)))
      return NULL;
    while (isdigit(lexer_peek(lexer)))
      lexer_next(lexer);
  }

  if (lexer_peek(lexer) == 'e' || lexer_peek(lexer) == 'E') {
    lexer_next(lexer);
    if (lexer_peek(lexer) == '+' || lexer_peek(lexer) == '-')
      lexer_next(lexer);
    if (!isdigit(lexer_peek(lexer)))
      return NULL;
    while (isdigit(lexer_peek(lexer)))
      lexer_next(lexer);
  }

  size_t end    = lexer->pos;
  size_t len    = end - start;
  char  *number = malloc(len + 1);
  if (!number)
    return NULL;
  strncpy(number, lexer->input + start, len);
  number[len] = '\0';
  return number;
}

Token lexer_get_next_token(Lexer *lexer) {
  lexer_skip_whitespace(lexer);

  Token token;
  token.value = NULL;

  char c = lexer_peek(lexer);
  if (c == '\0') {
    token.type = TOKEN_EOF;
    return token;
  }

  switch (c) {
    case '{':
      lexer_next(lexer);
      token.type = TOKEN_LEFT_BRACE;
      break;
    case '}':
      lexer_next(lexer);
      token.type = TOKEN_RIGHT_BRACE;
      break;
    case '[':
      lexer_next(lexer);
      token.type = TOKEN_LEFT_BRACKET;
      break;
    case ']':
      lexer_next(lexer);
      token.type = TOKEN_RIGHT_BRACKET;
      break;
    case ',':
      lexer_next(lexer);
      token.type = TOKEN_COMMA;
      break;
    case ':':
      lexer_next(lexer);
      token.type = TOKEN_COLON;
      break;
    case '\"':
      token.type  = TOKEN_STRING;
      token.value = lexer_parse_string(lexer);
      if (!token.value) {
        token.type = TOKEN_ERROR;
      }
      break;
    default:
      if (c == '-' || isdigit(c)) {
        token.type  = TOKEN_NUMBER;
        token.value = lexer_parse_number(lexer);
        if (!token.value) {
          token.type = TOKEN_ERROR;
        }
      } else if (strncmp(lexer->input + lexer->pos, "true", 4) == 0) {
        lexer->pos += 4;
        token.type = TOKEN_TRUE;
      } else if (strncmp(lexer->input + lexer->pos, "false", 5) == 0) {
        lexer->pos += 5;
        token.type = TOKEN_FALSE;
      } else if (strncmp(lexer->input + lexer->pos, "null", 4) == 0) {
        lexer->pos += 4;
        token.type = TOKEN_NULL;
      } else {
        token.type = TOKEN_ERROR;
      }
      break;
  }

  return token;
}
