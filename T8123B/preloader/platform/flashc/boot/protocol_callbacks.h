#pragma once

typedef int (*CB_OP_PROGRESS_REPORT)(void* user_arg, int progress, const char* msg);
struct progress_cb
{
   void* user_arg;
  CB_OP_PROGRESS_REPORT cb;
};
