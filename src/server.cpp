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
#include <filesystem>
#include <fstream>

const char *Not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
const char *JUST_found = "HTTP/1.1 200 OK\r\n\r\n";
std::string get_check = "GET /user-agent HTTP/1.1";

void handle_Person(int client_id, std::string file_path)
{
  char Yehbufferhai[1024];
  memset(Yehbufferhai, 0, sizeof Yehbufferhai);
  const char *message;
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

  size_t isitPost = cheker.find("POST");
  size_t found = cheker.find("echo");
  size_t Filefound = cheker.find("files");

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

    if (flag == 1)
    {
      message = response.c_str();
      int bytes_sent = send(client_id, message, strlen(message), 0);

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

    if (flag == 1)
    {
      message = response.c_str();
      int bytes_sent = send(client_id, message, strlen(message), 0);

      return;
    }
  }
  else if (Filefound != std::string::npos)
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
      else if (cheker[i - 1] == 's' && cheker[i] == '/')
      {

        flag = 1;
      }
    }

    if (isitPost != std::string::npos)
    {

      std::ofstream new_file(file_path + "/" + s);
      std::string buff = v1[v1.size() - 1];
      new_file << buff;
      new_file.close();
      std::string response = "HTTP/1.1 201 Created\r\n";
      response += "Content-Length: 0\r\n\r\n";
      const char *message = response.c_str();
      int bytes_sent = send(client_id, message, strlen(message), 0);

      return;
    }
    else
    {
      std::string full_path = file_path += '/';
      full_path += (s);

      std::ifstream file(full_path);
      if (file.is_open())
      {
        std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::cout << file_content;

        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: application/octet-stream\r\n";
        response += "Content-Length: ";
        response += std::to_string(file_content.length());
        response += "\r\n\r\n";
        response += file_content;
        response += "\r\n";

        if (flag == 1)
        {
          message = response.c_str();
          int bytes_sent = send(client_id, message, strlen(message), 0);

          return;

          file.close();
        }
      }
      else
      {
        const char *response = "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
        int bytes_sent = send(client_id, response, strlen(response), 0);

        return;
      }
      file.close();
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

    message = (flag2 == 1) ? JUST_found : Not_found;
  }

  int bytes_sent = send(client_id, message, strlen(message), 0);

  return;
}

int main(int argc, char **argv)
{
  std::string file_path = "LMAO";
  if (argc == 3 && strcmp(argv[1], "--directory") == 0)
  {
    file_path = argv[2];
  }
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

    CLIENTS.emplace_back(handle_Person, client_id, file_path);
  }
  for (auto &it : CLIENTS)
  {
    it.join();
  }
  close(server_fd);

  return 0;
}
