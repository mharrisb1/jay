#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum {
  TOKEN_LEFT_BRACE,    // {
  TOKEN_RIGHT_BRACE,   // }
  TOKEN_LEFT_BRACKET,  // [
  TOKEN_RIGHT_BRACKET, // ]
  TOKEN_COMMA,         // ,
  TOKEN_COLON,         // :
  TOKEN_STRING,        // "string"
  TOKEN_NUMBER,        // 123, -123, 1.23e10, etc.
  TOKEN_TRUE,          // true
  TOKEN_FALSE,         // false
  TOKEN_NULL,          // null
  TOKEN_EOF,           // End of input
  TOKEN_ERROR          // Error token
} TokenType;

typedef struct {
  TokenType type;
  char     *value;
} Token;

typedef struct {
  const char *input;
  size_t      pos;
  size_t      lenth;
} Lexer;

void lexer_init(Lexer *lexer, const char *input);
char lexer_peek(Lexer *lexer);
char lexer_next(Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);

char *lexer_parse_string(Lexer *lexer);
char *lexer_parse_number(Lexer *lexer);
Token lexer_get_next_token(Lexer *lexer);

#endif
