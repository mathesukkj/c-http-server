#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int create_socket(int port) {
  int server_fd;  // file descriptor
  int opt = 1;

  // creates a socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("failed to create socket");
    exit(1);
  }

  // set socket opts
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("failed to set socket opts");
    exit(1);
  }

  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  address.sin_family = AF_INET;          // ipv4 family
  address.sin_port = htons(port);        // port
  address.sin_addr.s_addr = INADDR_ANY;  // allows the server to accept any incoming conns

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("failed binding socket to address and port");
    exit(1);
  }

  if (listen(server_fd, 3) < 0) {
    perror("failed to listen for connections");
    exit(1);
  }

  int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
  if (new_socket < 0) {
    perror("failed to accept connections");
    exit(1);
  }

  printf("new conn on port %d\n", port);

  return new_socket;
}

ssize_t read_from_socket(int socket_fd, char *buffer, size_t buffer_size) {
  memset(buffer, 0, buffer_size);
  ssize_t bytes_read = read(socket_fd, buffer, buffer_size);
  if (bytes_read < 0) {
    perror("read failed");
  }

  return bytes_read;
}