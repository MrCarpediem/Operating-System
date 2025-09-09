#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <unistd.h>

using namespace std;

#define MAX 1024

// Message structure
struct msg_buffer {
    long msg_type;
    char msg_text[MAX];
};

int main() {
    key_t key = ftok("chat_key", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    msg_buffer message;

    cout << "Client started. Type messages (exit to quit)\n";

    while (true) {
    
        cout << "You: ";
        string input;
        getline(cin, input);
        strcpy(message.msg_text, input.c_str());
        message.msg_type = 1;
        msgsnd(msgid, &message, sizeof(message.msg_text), 0);

        if (input == "exit") {
            break;
        }

        msgrcv(msgid, &message, sizeof(message.msg_text), 2, 0);
        string server_reply(message.msg_text);
        cout << "Server: " << server_reply << endl;

        if (server_reply == "exit") {
            break;
        }
    }

    return 0;
}
