
#include <debug.h>
#include <printf.h>
#include <stdarg.h>

/***********************************
*                                       +
*  printf --->   _dvprintf   -----------+-------> vsnprintf
*                  /      |             +             /          \
*            dputs        |the same       +      vsprintf          sprintf        
*                 \       |               + 
*   dprintf   ---> _dprintf               +
*                                         +

*********************************/

void _panic(void *caller, const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);

   dprintf(FATAL, "panic (caller %p): ", caller);
   _dvprintf(fmt, ap);
   va_end(ap);
   while(1);
}

int _dprintf(const char *fmt, ...)
{
   char buf[512];
   int err;

   va_list ap;
   va_start(ap, fmt);
   err = vsnprintf(buf, sizeof(buf), fmt, ap);
   va_end(ap);

   _dputs((void *)buf);

   return err;
}

__WEAK void print(char *fmt, ...)
{
   char buf[512];
   //int err;

   va_list ap;
   va_start(ap, fmt);
   //err = vsnprintf(buf, sizeof(buf), fmt, ap);
   vsnprintf(buf, sizeof(buf), fmt, ap);
   va_end(ap);

   _dputs((void *)buf);

   return ;
}

int _dvprintf(const char *fmt, va_list ap)
{
   char buf[1024];
   int err;

   err = vsnprintf(buf, sizeof(buf), fmt, ap);

   _dputs((void *)buf);

   return err;
}
/* mark for build warning
static int islower(int c)
{
return ((c >= 'a') && (c <= 'z'));
}

static int isupper(int c)
{
return ((c >= 'A') && (c <= 'Z'));
}
static int isalpha(int c)
{
return isupper(c) || islower(c);
}
*/
void hexdump(const void *ptr, size_t len)
{

}

void hexdump8(const void *ptr, size_t len)
{

}


