# Operating-System# Shared Memory IPC

This project demonstrates **Inter-Process Communication (IPC)** using **Shared Memory** in C++.

## ⚙️ How to Compile & Run

```bash
g++ Server/Server.cpp -o server -lrt
g++ Client/Client.cpp -o client -lrt
./server    # Run in one terminal
./client    # Run in another terminal
