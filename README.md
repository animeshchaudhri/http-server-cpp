# README.md

## Description

This is a simple C++ server program that listens for incoming connections and handles HTTP requests. It parses HTTP requests, extracts specific information, and sends appropriate responses based on the request content.

## Requirements

- C++ compiler
- POSIX-compliant operating system (e.g., Linux)

## Dependencies

- `iostream`
- `cstdlib`
- `string`
- `cstring`
- `unistd.h`
- `sys/types.h`
- `sys/socket.h`
- `arpa/inet.h`
- `netdb.h`
- `vector`
- `thread`

## Usage

1. Compile the program using a C++ compiler:
   ```bash
   g++ server.cpp -o server
   ```
2. Run the compiled executable:
   ```bash
   ./server
   ```
3. The server will start listening for incoming connections on port 4221 by default.

## Functionality

- The server listens for incoming connections and handles each client connection in a separate thread.
- It reads HTTP requests sent by clients and parses them to extract relevant information.
- The server supports handling HTTP GET requests for `/user-agent`.
- It sends appropriate HTTP responses based on the type of request received:
  - If the request contains "echo" in the URL, it echoes back the specified content.
  - If the request is a GET request for `/user-agent`, it sends back the User-Agent header value.
  - For any other request, it sends a 404 Not Found response.

## License

This project is licensed under the [MIT License](LICENSE).
