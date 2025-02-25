# OS_ELA/ Team:5
OS Unix Domain and Internet Domain Chat Applications
# Chat Application using UNIX & Internet Domain Sockets

This repository contains **two implementations** of a **simple chat application** using **IPC sockets**:
- **UNIX Domain Sockets (`AF_UNIX`)** → For local inter-process communication.
- **Internet Domain Sockets (`AF_INET`)** → For communication over LAN/internet.

## Overview
## 1.UNIX Domain Sockets (`AF_UNIX`)
📍 **Used for:** Local IPC (Inter-Process Communication) on the same system.  
📍 **How it works:** Uses a **file path** (`/tmp/chat_socket`) for communication.  

### Running the UNIX Domain Chat
1. **Compile the server and client**
   ```bash
   gcc -o unix_server unix_server.c
   gcc -o unix_client unix_client.c

This chat system follows a **client-server model**, where:
- The **server** listens for incoming connections.
- The **client** connects to the server and exchanges messages.

2. **Run the server in one terminal**
   ```bash
   ./unix_server
   
3. **Run the client in another terminal**
   ```bash
   ./unix_client

4. **Start Chatting**
- Type a message in the client, and it appears on the server.
- Type a message in the server, and it appears on the client.
- Type "exit" to close the chat.

## 2.Internet Domain Sockets (`AF_INET`)
📍 **Used for:** Local IPC (Inter-Process Communication) on the same system.  
📍 **How it works:** Uses a **file path** (`/tmp/chat_socket`) for communication.  

### Running the Internet Domain Chat
1. **Compile the server and client**
   ```bash
   gcc -o internet_server internet_server.c
   gcc -o internet_client internet_client.c

This chat system follows a **client-server model**, where:
- The **server** listens for incoming connections.
- The **client** connects to the server and exchanges messages.

2. **Run the server in one terminal**
   ```bash
   ./internet_server
   
3. **Run the client in another terminal**
   ```bash
   ./internet_client

4. **Start Chatting**
- Type a message in the client, and it appears on the server.
- Type a message in the server, and it appears on the client.
- Type "exit" to close the chat.

### Contributors
- NISHANTH ROY KEERTHI
- KOLLA VIVEK SAGAR
- MALLIALA SRINIVAS
- MALLUPEDDI VAMSI KRISHNA
