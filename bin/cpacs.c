
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
  if (argc < 2) return 120;

  char req[512], path[256];
  snprintf(path, sizeof(path),
    "/geiger-games/cpacs/main/%s", argv[1]);

  snprintf(req, sizeof(req),
    "GET %s HTTP/1.0\r\n"
    "Host: raw.githubusercontent.com\r\n"
    "\r\n", path);

  struct addrinfo hints = {0}, *res;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("raw.githubusercontent.com", "80", &hints, &res);
  int s = socket(res->ai_family, res->ai_socktype, 0);
  connect(s, res->ai_addr, res->ai_addrlen);

  write(s, req, strlen(req));

  char outpath[256];
  snprintf(outpath, sizeof(outpath), "/bin/%s", argv[1]);
  FILE *f = fopen(outpath, "w");

  char buf[1024];
  int n, hdr = 1;
  while ((n = read(s, buf, sizeof(buf))) > 0) {
    if (hdr) {
      char *p = strstr(buf, "\r\n\r\n");
      if (p) {
        fwrite(p + 4, 1, n - (p + 4 - buf), f);
        hdr = 0;
      }
    } else {
      fwrite(buf, 1, n, f);
    }
  }

  fclose(f);
  close(s);
  return 0;
}