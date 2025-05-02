#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "./http.h"
#include "./tcp.h"

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
  int new_socket = create_socket(PORT);
  if (new_socket < 0) {
    perror("failed to setup socket");
    exit(1);
  }

  char buf[BUFFER_SIZE];
  ssize_t val_read;

  while ((val_read = read_from_socket(new_socket, buf, sizeof(buf))) > 0) {
    struct http_fields fields;
    fields = parse_http_message(buf);
  }

  close(new_socket);

  return 0;
}