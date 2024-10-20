For UBUNTU system:
1) Install OpenSSL (if not already installed):
    sudo apt-get install libssl-dev
2) Compile the code:
    gcc -o banking_system account.c customer.c employee.c feedback.c loan.c main.c manager.c transaction.c user.c admin.c -lssl -lcrypto -pthread
3) Start the server:
    ./banking_system
4) Connect to client using telnet:
    telnet localhost 8080
