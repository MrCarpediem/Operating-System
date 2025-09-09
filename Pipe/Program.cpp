#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>

using namespace std;

#define MESSAGE_SIZE 256

int main() {
    int parentToChildPipe[2];
    int childToParentPipe[2];
    pid_t processId;
    char message[MESSAGE_SIZE];

    if (pipe(parentToChildPipe) == -1 || pipe(childToParentPipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    processId = fork();

    if (processId == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (processId == 0) {  
        // Child process
        close(parentToChildPipe[1]); 
        close(childToParentPipe[0]); 

        for (int i = 0; i < 3; ++i) {
            int bytesRead = read(parentToChildPipe[0], message, MESSAGE_SIZE);
            if (bytesRead <= 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            message[bytesRead] = '\0'; 

            cout << "Child received message: " << message << endl;

            cout << "Writing message to parent: " << message << endl;
            int bytesWritten = write(childToParentPipe[1], message, strlen(message) + 1);
            if (bytesWritten <= 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(parentToChildPipe[0]);
        close(childToParentPipe[1]);
        exit(EXIT_SUCCESS);
    } else {  
        // Parent process
        close(parentToChildPipe[0]); 
        close(childToParentPipe[1]); 

        for (int i = 0; i < 3; ++i) {
            string msg = "Message " + to_string(i + 1);

            cout << "Writing message to child: " << msg << endl;
            int bytesWritten = write(parentToChildPipe[1], msg.c_str(), msg.size() + 1);
            if (bytesWritten <= 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            int bytesRead = read(childToParentPipe[0], message, MESSAGE_SIZE);
            if (bytesRead <= 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            message[bytesRead] = '\0'; 

            cout << "Parent received message from child: " << message << "\n" << endl;
        }

        close(parentToChildPipe[1]);
        close(childToParentPipe[0]);
        wait(NULL);  
        exit(EXIT_SUCCESS);
    }

    return 0;
}
