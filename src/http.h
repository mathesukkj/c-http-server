struct http_fields parse_http_message(char* message);

struct http_fields {
  char method[7];
  char path[2048];
  char version[9];

  char host[2000];
  char user_agent[1024];
  char accept[512];
  char content_type[255];
  char content_length[64];
  char authorization[1024];
  char connection[64];
  char accept_encoding[256];
  char cache_control[256];
  char cookie[2048];

  char body[32768];
};