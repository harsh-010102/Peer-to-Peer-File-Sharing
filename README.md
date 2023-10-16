# Task: running a Tracker & Clients communication with Tracker

## Functionality

Created a trackerSocket wich will listen to multiple clients.

Each clientConnection created in a new thread so all clients can communicate parallely to the trackerSocket.

client can send as many messages as he want, server will reply to each message.

when client wants to close a connection, it just need to send "quit" string to the server.

On recieve of "quit" tracker will close the connection of that client and join the thread to the parent thread.

## How to run

open terminal to the root directory of project.

```bash
    cd /tracker
    make# Task: running a Tracker & Clients communication with Tracker

## Functionality

Created a trackerSocket wich will listen to multiple clients.

Each clientConnection created in a new thread so all clients can communicate parallely to the trackerSocket.

client can send as many messages as he want, server will reply to each message.

when client wants to close a connection, it just need to send "quit" string to the server.

On recieve of "quit" tracker will close the connection of that client and join the thread to the parent thread.

## How to run

open terminal to the root directory of project.

```bash
    cd /tracker
    make
    ./tracker <port_no>
```

It will create a trackerSocket on localhost with given <port_no>. 

Tracker, now accept every clientConnection on given <port_no>.

now run following commands to create multiple client connections with tracker

```bash
    telnet localhost <port_no>
```

Now you can send message to tracker directly by writng on terminal. on every message you will recieve "Recieved your msg." Acknowledgement from server if msg is recieved by server.

Type "quit" on client's side to close the connection from server. Client will recieve "Bye." Acknowledgement as closed connection by server.


    ./tracker <port_no>
```

It will create a trackerSocket on localhost with given <port_no>. 

Tracker, now accept every clientConnection on given <port_no>.

now run following commands to create multiple client connections with tracker

```bash
    telnet localhost <port_no>
```

Now you can send message to tracker directly by writng on terminal. on every message you will recieve 

Type "quit" on client's side to close the connection from server. Client will recieve "Bye." Acknowledgement as closed connection by server.

