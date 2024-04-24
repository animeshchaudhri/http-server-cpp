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
#include <thread>

void handle_Person(int client_id)
{
  char Yehbufferhai[1024];
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

  char *token = strtok(Yehbufferhai, "\r\n");

  std::vector<std::string> v1;

  std::string ISitok;
  while (token != NULL)
  {
    ISitok += token;
    if (token = strtok(NULL, "\r\n"))
    {
      v1.push_back(ISitok);
      ISitok.clear();
    }
  }
  v1.push_back(ISitok);
  ISitok.clear();
  const char *Not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
  const char *JUST_found = "HTTP/1.1 200 OK\r\n\r\n";

  std::string cheker = v1[0];
  std::string User_agent;
  for (auto it : v1)
  {
    if (it[0] == 'U')
    {
      User_agent = it;
    }
  }

  int flag2 = 0;
  std::string get_check = "GET /user-agent HTTP/1.1";
  size_t found = cheker.find("echo");

  if (found != std::string::npos)
  {
    int flag = 0;
    std::string s;
    for (int i = 1; i < cheker.length() - 1; i++)
    {
      if (flag == 1)
      {
        if (cheker[i] == ' ')
          break;
        s += cheker[i];
      }
      else if (cheker[i - 1] == 'o' && cheker[i] == '/')
      {

        flag = 1;
      }
    }
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: ";
    response += std::to_string(s.length());
    response += "\r\n\r\n";
    response += s;
    response += "\r\n";
    const char *message = response.c_str();
    if (flag == 1)
    {
      int bytes_sent = send(client_id, message, strlen(message), 0);
      if (bytes_sent < 0)
      {
        std::cerr << "Failed to send response";
      }
      else
      {
        std::cout << "OK Response sent";
      }
      return;
    }
  }
  else if (get_check == cheker)
  {

    int flag = 0;
    std::string s;
    for (int i = 1; i < User_agent.length(); i++)
    {
      if (flag == 1)
      {

        s += User_agent[i];
      }
      else if (User_agent[i - 1] == ':' && User_agent[i] == ' ')
      {

        flag = 1;
      }
    }
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: ";
    response += std::to_string(s.length());
    response += "\r\n\r\n";
    response += s;
    response += "\r\n";
    const char *message = response.c_str();
    std::cout << s << std::endl;

    if (flag == 1)
    {
      int bytes_sent = send(client_id, message, strlen(message), 0);
      if (bytes_sent < 0)
      {
        std::cerr << "Failed to send response";
      }
      else
      {
        std::cout << "OK Response sent";
      }
      return;
    }
  }
  else
  {

    for (int i = 1; i < cheker.length(); i++)
    {
      if (cheker[i] == '/' && cheker[i + 1] == ' ')
      {

        flag2 = 1;
        break;
      }
    }

    if (flag2 == 1)
    {
      int bytes_sent = send(client_id, JUST_found, strlen(JUST_found), 0);
      if (bytes_sent < 0)
      {
        std::cerr << "Failed to send response";
      }
      else
      {
        std::cout << "OK Response sent";
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
      return;
    }
  }
  return;
}

int main(int argc, char **argv)
{

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
  {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

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

  std::vector<std::thread> CLIENTS;
  while (true)
  {

    std::cout << "Waiting for a client to connect...\n";

    int client_id = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
    std::cout << "Client connected\n";
    if (client_id < 0)
    {
      std::cout << "Error connecting\n";
      break;
    }

    CLIENTS.emplace_back(handle_Person, client_id);
  }
  for (auto &it : CLIENTS)
  {
    it.join();
  }
  close(server_fd);

  return 0;
}
