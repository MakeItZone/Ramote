#ifndef status_h
#define status_h

enum class Status { boot, disconnected, running };

void updateStatus(Status status);

#endif