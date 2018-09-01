#ifndef EVENTTYPE_H
#define EVENTTYPE_H

enum EventType {
  ACCEPT_EVENT = 1,
  CONNECTED_EVENT = 2,
  READ_EVENT = 4,
  WRITE_EVENT = 8,
  RECV_EVENT = 16,
  SEND_EVENT = 32,
  CLOSE_EVENT = 64,
  ERROR_EVENT = 128,
  CONNECT = 256,
  ACCEPT = 512,
  SHUTDOWN = 1024
};

#endif