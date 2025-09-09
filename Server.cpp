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

    cout << "Server started. Waiting for messages...\n";

    while (true) {

        msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);
        string client_msg(message.msg_text);

        if (client_msg == "exit") {
            cout << "Client disconnected. Exiting...\n";
            break;
        }

        cout << "Client: " << client_msg << endl;

        cout << "You: ";
        string reply;
        getline(cin, reply);
        strcpy(message.msg_text, reply.c_str());
        message.msg_type = 2;
        msgsnd(msgid, &message, sizeof(message.msg_text), 0);

        if (reply == "exit") {
            cout << "Server exiting...\n";
            break;
        }
    }

    msgctl(msgid, IPC_RMID, NULL); 
    return 0;
}
