#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

char Yehbufferhai[1024];

int main(int argc, char **argv)
{
  // You can use print statements as follows for debugging, they'll be visible when running tests.
  std::cout << "Logs from your program will appear here!\n";

  // Uncomment this block to pass the first stage
  //
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
  {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  // Since the tester restarts your program quite often, setting REUSE_PORT
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0)
  {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0)
  {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  int client_id = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
  std::cout << "Client connected\n";
  if (client_id < 0)
  {
    std::cout << "Error connecting\n";
  }

  memset(Yehbufferhai, 0, sizeof Yehbufferhai);

  int checking = read(client_id, Yehbufferhai, 1024);
  if (checking < 0)
  {
    std::cout << "error reading buffer\n";
  }
  else
  {
    std::cout << Yehbufferhai << "\n";
  }

  // const char *response = "HTTP/1.1 200 OK\r\n\r\n";

  char *token = strtok(Yehbufferhai, "\r\n");

  std::vector<std::string> v1;

  std::string ISitok;
  while (token != NULL)
  {
    ISitok += token;
    if (token = strtok(NULL, "-"))
    {
      v1.push_back(ISitok);
      ISitok.clear();
    }
  }
  const char *Not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
  const char *JUST_found = "HTTP/1.1 200 OK\r\n\r\n";
  std::string cheker = v1[0];
  int flag = 0;
  int flag2 = 0;

  std::string s;

  for (int i = 1; i < cheker.length() - 1; i++)
  {
    if (cheker[i - 1] == 'o' && cheker[i] == '/')
    {

      flag = 1;
    }
    else if (flag == 1)
    {
      s += cheker[i];
      if (cheker[i] == ' ')
        break;
    }
  }
  for (int i = 1; i < cheker.length(); i++)
  {
    if (cheker[i] == '/' && cheker[i + 1] == ' ')
    {

      flag2 = 1;
      break;
    }
    
  }

  std::string response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/plain\r\n";
  response += "Content-Length: ";
  response += std::to_string(s.length());
  response += "\r\n\r\n";
  response += s;
  response += "\r\n";

  if (flag == 1)
  {
    int bytes_sent = send(client_id, response.c_str(), strlen(response.c_str()), 0);
    if (bytes_sent < 0)
    {
      std::cerr << "Failed to send response\n";
    }
    else
    {
      std::cout << "akj Response sent\n";
    }
  }
  else if (flag2 == 1)
  {
    int bytes_sent = send(client_id, JUST_found, strlen(JUST_found), 0);
    if (bytes_sent < 0)
    {
      std::cerr << "Failed to send response\n";
    }
    else
    {
      std::cout << "OK Response sent\n";
    }
  }
  else
  {
    int bytes_sent = send(client_id, Not_found, strlen(Not_found), 0);
    if (bytes_sent < 0)
    {
      std::cerr << "Failed to send response\n";
    }
    else
    {
      std::cout << "OK Response sent\n";
    }
  }

  close(server_fd);

  return 0;
}
