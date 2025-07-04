#ifndef SERVER_H
#define SERVER_H

#define MAX_EVENTS 16

#include "../context/app_context.h"

int init_server(app_context_t*, int);
void stop_server(app_context_t*);
void run_server(app_context_t*); 

#endif