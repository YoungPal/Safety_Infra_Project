#Server Compile
gcc main_server.c -D_REENTRANT -o server -lphtread

#Server Run
./server <Port>

#Client Compile
gcc main_client.c -o client -pthread

#Client Run
./client <hostIP> <Port> <ID>

