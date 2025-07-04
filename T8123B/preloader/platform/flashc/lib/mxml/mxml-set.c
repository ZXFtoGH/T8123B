/*
 * Node set functions for Mini-XML, a small XML file parsing library.
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
#include "config.h"
#include "mxml-private.h"

#define XML_WRITE_FEATURE_ENABLE
#if defined XML_WRITE_FEATURE_ENABLE
/*
 * 'mxmlSetCDATA()' - Set the element name of a CDATA node.
 *
 * The node is not changed if it (or its first child) is not a CDATA element node.
 *
 * @since Mini-XML 2.3@
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetCDATA(mxml_node_t *node,		/* I - Node to set */
             const char  *data)		/* I - New data string */
{
  char	*s;				/* String pointer */


 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      strncmp(node->value.element.name, "![CDATA[", 8) &&
      node->child && node->child->type == MXML_ELEMENT &&
      !strncmp(node->child->value.element.name, "![CDATA[", 8))
    node = node->child;

  if (!node || node->type != MXML_ELEMENT || !data ||
      strncmp(node->value.element.name, "![CDATA[", 8))
    return (-1);

  if (data == (node->value.element.name + 8))
    return (0);

 /*
  * Allocate the new value, free any old element value, and set the new value...
  */

  s = _mxml_strdupf("![CDATA[%s", data);

  if (node->value.element.name)
    free(node->value.element.name);

  node->value.element.name = s;

  return (0);
}


/*
 * 'mxmlSetCustom()' - Set the data and destructor of a custom data node.
 *
 * The node is not changed if it (or its first child) is not a custom node.
 *
 * @since Mini-XML 2.1@
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetCustom(
    mxml_node_t              *node,	/* I - Node to set */
    void                     *data,	/* I - New data pointer */
    mxml_custom_destroy_cb_t destroy)	/* I - New destructor function */
{
 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      node->child && node->child->type == MXML_CUSTOM)
    node = node->child;

  if (!node || node->type != MXML_CUSTOM)
    return (-1);

  if (data == node->value.custom.data)
  {
    node->value.custom.destroy = destroy;
    return (0);
  }

 /*
  * Free any old element value and set the new value...
  */

  if (node->value.custom.data && node->value.custom.destroy)
    (*(node->value.custom.destroy))(node->value.custom.data);

  node->value.custom.data    = data;
  node->value.custom.destroy = destroy;

  return (0);
}


/*
 * 'mxmlSetElement()' - Set the name of an element node.
 *
 * The node is not changed if it is not an element node.
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetElement(mxml_node_t *node,	/* I - Node to set */
               const char  *name)	/* I - New name string */
{
 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT || !name)
    return (-1);

  if (name == node->value.element.name)
    return (0);

 /*
  * Free any old element value and set the new value...
  */

  if (node->value.element.name)
    free(node->value.element.name);

  node->value.element.name = strdup(name);

  return (0);
}


/*
 * 'mxmlSetInteger()' - Set the value of an integer node.
 *
 * The node is not changed if it (or its first child) is not an integer node.
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetInteger(mxml_node_t *node,	/* I - Node to set */
               int         integer)	/* I - Integer value */
{
 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      node->child && node->child->type == MXML_INTEGER)
    node = node->child;

  if (!node || node->type != MXML_INTEGER)
    return (-1);

 /*
  * Set the new value and return...
  */

  node->value.integer = integer;

  return (0);
}


/*
 * 'mxmlSetOpaque()' - Set the value of an opaque node.
 *
 * The node is not changed if it (or its first child) is not an opaque node.
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetOpaque(mxml_node_t *node,	/* I - Node to set */
              const char  *opaque)	/* I - Opaque string */
{
 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      node->child && node->child->type == MXML_OPAQUE)
    node = node->child;

  if (!node || node->type != MXML_OPAQUE || !opaque)
    return (-1);

  if (node->value.opaque == opaque)
    return (0);

 /*
  * Free any old opaque value and set the new value...
  */

  if (node->value.opaque)
    free(node->value.opaque);

  node->value.opaque = strdup(opaque);

  return (0);
}


/*
 * 'mxmlSetOpaquef()' - Set the value of an opaque string node to a formatted string.
 *
 * The node is not changed if it (or its first child) is not an opaque node.
 *
 * @since Mini-XML 2.11@
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetOpaquef(mxml_node_t *node,	/* I - Node to set */
               const char  *format,	/* I - Printf-style format string */
	       ...)			/* I - Additional arguments as needed */
{
  va_list	ap;			/* Pointer to arguments */
  char		*s;			/* Temporary string */


 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      node->child && node->child->type == MXML_OPAQUE)
    node = node->child;

  if (!node || node->type != MXML_OPAQUE || !format)
    return (-1);

 /*
  * Format the new string, free any old string value, and set the new value...
  */

  va_start(ap, format);
  s = _mxml_vstrdupf(format, ap);
  va_end(ap);

  if (node->value.opaque)
    free(node->value.opaque);

  node->value.opaque = s;

  return (0);
}


/*
 * 'mxmlSetReal()' - Set the value of a real number node.
 *
 * The node is not changed if it (or its first child) is not a real number node.
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetReal(mxml_node_t *node,		/* I - Node to set */
            double      real)		/* I - Real number value */
{
 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      node->child && node->child->type == MXML_REAL)
    node = node->child;

  if (!node || node->type != MXML_REAL)
    return (-1);

 /*
  * Set the new value and return...
  */

  node->value.real = real;

  return (0);
}


/*
 * 'mxmlSetText()' - Set the value of a text node.
 *
 * The node is not changed if it (or its first child) is not a text node.
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetText(mxml_node_t *node,		/* I - Node to set */
            int         whitespace,	/* I - 1 = leading whitespace, 0 = no whitespace */
	    const char  *string)	/* I - String */
{
 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      node->child && node->child->type == MXML_TEXT)
    node = node->child;

  if (!node || node->type != MXML_TEXT || !string)
    return (-1);

  if (string == node->value.text.string)
  {
    node->value.text.whitespace = whitespace;
    return (0);
  }

 /*
  * Free any old string value and set the new value...
  */

  if (node->value.text.string)
    free(node->value.text.string);

  node->value.text.whitespace = whitespace;
  node->value.text.string     = strdup(string);

  return (0);
}


/*
 * 'mxmlSetTextf()' - Set the value of a text node to a formatted string.
 *
 * The node is not changed if it (or its first child) is not a text node.
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetTextf(mxml_node_t *node,		/* I - Node to set */
             int         whitespace,	/* I - 1 = leading whitespace, 0 = no whitespace */
             const char  *format,	/* I - Printf-style format string */
	     ...)			/* I - Additional arguments as needed */
{
  va_list	ap;			/* Pointer to arguments */
  char		*s;			/* Temporary string */


 /*
  * Range check input...
  */

  if (node && node->type == MXML_ELEMENT &&
      node->child && node->child->type == MXML_TEXT)
    node = node->child;

  if (!node || node->type != MXML_TEXT || !format)
    return (-1);

 /*
  * Free any old string value and set the new value...
  */

  va_start(ap, format);
  s = _mxml_vstrdupf(format, ap);
  va_end(ap);

  if (node->value.text.string)
    free(node->value.text.string);

  node->value.text.whitespace = whitespace;
  node->value.text.string     = s;

  return (0);
}


/*
 * 'mxmlSetUserData()' - Set the user data pointer for a node.
 *
 * @since Mini-XML 2.7@
 */

int					/* O - 0 on success, -1 on failure */
mxmlSetUserData(mxml_node_t *node,	/* I - Node to set */
                void        *data)	/* I - User data pointer */
{
 /*
  * Range check input...
  */

  if (!node)
    return (-1);

 /*
  * Set the user data pointer and return...
  */

  node->user_data = data;
  return (0);
}

#endif