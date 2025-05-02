#include "./http.h"

#include <stdio.h>
#include <string.h>

#define MAX_LEN 1024

char* parse_method_path_version(struct http_fields* fields, char* message);
char* parse_header(struct http_fields* fields, char* message_rest);
char* get_field_to_update(char* header_name, struct http_fields* fields, size_t* field_size);
void parse_body(struct http_fields* fields, char* message_rest);

char* read_line_until(char* message, char* line, char* delimiter) {
  char* delimiter_pos = strstr(message, delimiter);
  if (!delimiter_pos) return NULL;

  size_t len = delimiter_pos - message;
  if (len >= MAX_LEN) len = MAX_LEN - 1;

  strncpy(line, message, len);
  line[len] = '\0';

  return delimiter_pos + 2;
}

struct http_fields parse_http_message(char* message) {
  struct http_fields fields;
  memset(&fields, 0, sizeof(fields));

  char* rest = parse_method_path_version(&fields, message);
  while (rest != NULL) {
    rest = parse_header(&fields, rest);
    if (strncmp(rest, "\r\n", 2) == 0) {
      parse_body(&fields, rest);
      break;
    }
    if (rest == NULL) break;
  }

  return fields;
}

char* parse_method_path_version(struct http_fields* fields, char* message) {
  char line[MAX_LEN];
  char* rest = read_line_until(message, line, "\r\n");
  if (!rest) return NULL;
  if (strncmp(rest, "\r\n", 2) == 0) {
    return rest;
  }

  char* token = strtok(line, " ");
  if (token) strncpy(fields->method, token, sizeof(fields->method) - 1);

  token = strtok(NULL, " ");
  if (token) strncpy(fields->path, token, sizeof(fields->path) - 1);

  token = strtok(NULL, "");
  if (token) strncpy(fields->version, token, sizeof(fields->version) - 1);

  return rest;
}

char* parse_header(struct http_fields* fields, char* message_rest) {
  char line[MAX_LEN] = {0};
  char* rest = read_line_until(message_rest, line, "\r\n");
  if (rest == "") return rest;

  char* token = strtok(line, " ");
  size_t field_size;
  char* field_to_update = get_field_to_update(token, fields, &field_size);
  token = strtok(NULL, "");
  if (token) {
    strncpy(field_to_update, token, field_size - 1);
  }

  return rest;
}

void parse_body(struct http_fields* fields, char* message_rest) {
  char line[MAX_LEN] = {0};
  char* body = read_line_until(message_rest, line, "");

  strcpy(fields->body, body);
}

char* get_field_to_update(char* header_name, struct http_fields* fields, size_t* field_size) {
  if (strcmp(header_name, "Host:") == 0) {
    *field_size = sizeof(fields->host);
    return fields->host;
  } else if (strcmp(header_name, "User-Agent:") == 0) {
    *field_size = sizeof(fields->user_agent);
    return fields->user_agent;
  } else if (strcmp(header_name, "Accept:") == 0) {
    *field_size = sizeof(fields->accept);
    return fields->accept;
  } else if (strcmp(header_name, "Content-Type:") == 0) {
    *field_size = sizeof(fields->content_type);
    return fields->content_type;
  } else if (strcmp(header_name, "Content-Length:") == 0) {
    *field_size = sizeof(fields->content_length);
    return fields->content_length;
  } else if (strcmp(header_name, "Authorization:") == 0) {
    *field_size = sizeof(fields->authorization);
    return fields->authorization;
  } else if (strcmp(header_name, "Connection:") == 0) {
    *field_size = sizeof(fields->connection);
    return fields->connection;
  } else if (strcmp(header_name, "Accept-Encoding:") == 0) {
    *field_size = sizeof(fields->accept_encoding);
    return fields->accept_encoding;
  } else if (strcmp(header_name, "Cache-Control:") == 0) {
    *field_size = sizeof(fields->cache_control);
    return fields->cache_control;
  } else if (strcmp(header_name, "Cookie:") == 0) {
    *field_size = sizeof(fields->cookie);
    return fields->cookie;
  }

  *field_size = 0;
  return NULL;
}
