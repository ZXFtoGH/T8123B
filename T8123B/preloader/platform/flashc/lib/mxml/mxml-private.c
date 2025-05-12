/*
 * Private functions for Mini-XML, a small XML file parsing library.
 *
 * https://www.msweet.org/mxml
 *
 * Copyright ??2003-2019 by Michael R Sweet.
 *
 * Licensed under Apache License v2.0.  See the file "LICENSE" for more
 * information.
 */

/*
 * Include necessary headers...
 */
#if defined WIN32
#define _CRT_SECURE_NO_WARNINGS // for sscanf warning.
#endif
#include "mxml-private.h"


/*
 * Some crazy people think that unloading a shared object is a good or safe
 * thing to do.  Unfortunately, most objects are simply *not* safe to unload
 * and bad things *will* happen.
 *
 * The following mess of conditional code allows us to provide a destructor
 * function in Mini-XML for our thread-global storage so that it can possibly
 * be unloaded safely, although since there is no standard way to do so I
 * can't even provide any guarantees that you can do it safely on all platforms.
 *
 * This code currently supports AIX, HP-UX, Linux, Mac OS X, Solaris, and
 * Windows.  It might work on the BSDs and IRIX, but I haven't tested that.
 */

#if defined(__sun) || defined(_AIX)
#  pragma fini(_mxml_fini)
#  define _MXML_FINI _mxml_fini
#elif defined(__hpux)
#  pragma FINI _mxml_fini
#  define _MXML_FINI _mxml_fini
#elif defined(__GNUC__) /* Linux and Mac OS X */
#  define _MXML_FINI __attribute((destructor)) _mxml_fini
#else
#  define _MXML_FINI _fini
#endif /* __sun */


/*
 * 'mxml_error()' - Display an error message.
 */

void
mxml_error(const char *format,		/* I - Printf-style format string */
           ...)				/* I - Additional arguments as needed */
{
  va_list	ap;			/* Pointer to arguments */
  char		s[1024];		/* Message string */
  _mxml_global_t *global = _mxml_global();
					/* Global data */


 /*
  * Range check input...
  */

  if (!format)
    return;

 /*
  * Format the error message string...
  */

  va_start(ap, format);

  xml_vsnprintf(s, sizeof(s), format, ap);

  va_end(ap);

 /*
  * And then display the error message...
  */

  if (global->error_cb)
    (*global->error_cb)(s);
  else
  ;
    //fprintf(stderr, "mxml: %s\n", s);
}


/*
 * 'mxml_ignore_cb()' - Default callback for ignored values.
 */

mxml_type_t				/* O - Node type */
mxml_ignore_cb(mxml_node_t *node)	/* I - Current node */
{
  (void)node;

  return (MXML_IGNORE);
}


/*
 * 'mxml_integer_cb()' - Default callback for integer values.
 */

mxml_type_t				/* O - Node type */
mxml_integer_cb(mxml_node_t *node)	/* I - Current node */
{
  (void)node;

  return (MXML_INTEGER);
}


/*
 * 'mxml_opaque_cb()' - Default callback for opaque values.
 */

mxml_type_t				/* O - Node type */
mxml_opaque_cb(mxml_node_t *node)	/* I - Current node */
{
  (void)node;

  return (MXML_OPAQUE);
}


/*
 * 'mxml_real_cb()' - Default callback for real number values.
 */

mxml_type_t				/* O - Node type */
mxml_real_cb(mxml_node_t *node)		/* I - Current node */
{
  (void)node;

  return (MXML_REAL);
}

_mxml_global_t *			/* O - Global data */
_mxml_global(void)
{
  static _mxml_global_t	global =	/* Global data */
  {
    NULL,				/* error_cb */
    1,					/* num_entity_cbs */
    { _mxml_entity_cb },		/* entity_cbs */
    72,					/* wrap */
    NULL,				/* custom_load_cb */
    NULL				/* custom_save_cb */
  };


  return (&global);
}

