/*
 * Node get functions for Mini-XML, a small XML file parsing library.
 *
 * https://www.msweet.org/mxml
 *
 * Copyright ??2014-2019 by Michael R Sweet.
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


/*
 * 'mxmlGetCDATA()' - Get the value for a CDATA node.
 *
 * @code NULL@ is returned if the node is not a CDATA element.
 *
 * @since Mini-XML 2.7@
 */

const char *				/* O - CDATA value or @code NULL@ */
mxmlGetCDATA(mxml_node_t *node)		/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT ||
      strncmp(node->value.element.name, "![CDATA[", 8))
    return (NULL);

 /*
  * Return the text following the CDATA declaration...
  */

  return (node->value.element.name + 8);
}


/*
 * 'mxmlGetCustom()' - Get the value for a custom node.
 *
 * @code NULL@ is returned if the node (or its first child) is not a custom
 * value node.
 *
 * @since Mini-XML 2.7@
 */

const void *				/* O - Custom value or @code NULL@ */
mxmlGetCustom(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (NULL);

 /*
  * Return the integer value...
  */

  if (node->type == MXML_CUSTOM)
    return (node->value.custom.data);
  else if (node->type == MXML_ELEMENT &&
           node->child &&
	   node->child->type == MXML_CUSTOM)
    return (node->child->value.custom.data);
  else
    return (NULL);
}


/*
 * 'mxmlGetElement()' - Get the name for an element node.
 *
 * @code NULL@ is returned if the node is not an element node.
 *
 * @since Mini-XML 2.7@
 */

const char *				/* O - Element name or @code NULL@ */
mxmlGetElement(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT)
    return (NULL);

 /*
  * Return the element name...
  */

  return (node->value.element.name);
}


/*
 * 'mxmlGetFirstChild()' - Get the first child of an element node.
 *
 * @code NULL@ is returned if the node is not an element node or if the node
 * has no children.
 *
 * @since Mini-XML 2.7@
 */

mxml_node_t *				/* O - First child or @code NULL@ */
mxmlGetFirstChild(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT)
    return (NULL);

 /*
  * Return the first child node...
  */

  return (node->child);
}


/*
 * 'mxmlGetInteger()' - Get the integer value from the specified node or its
 *                      first child.
 *
 * 0 is returned if the node (or its first child) is not an integer value node.
 *
 * @since Mini-XML 2.7@
 */

int					/* O - Integer value or 0 */
mxmlGetInteger(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (0);

 /*
  * Return the integer value...
  */

  if (node->type == MXML_INTEGER)
    return (node->value.integer);
  else if (node->type == MXML_ELEMENT &&
           node->child &&
	   node->child->type == MXML_INTEGER)
    return (node->child->value.integer);
  else
    return (0);
}


/*
 * 'mxmlGetLastChild()' - Get the last child of an element node.
 *
 * @code NULL@ is returned if the node is not an element node or if the node
 * has no children.
 *
 * @since Mini-XML 2.7@
 */

mxml_node_t *				/* O - Last child or @code NULL@ */
mxmlGetLastChild(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT)
    return (NULL);

 /*
  * Return the node type...
  */

  return (node->last_child);
}


/*
 * 'mxmlGetNextSibling()' - Get the next node for the current parent.
 *
 * @code NULL@ is returned if this is the last child for the current parent.
 *
 * @since Mini-XML 2.7@
 */

mxml_node_t *
mxmlGetNextSibling(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (NULL);

 /*
  * Return the node type...
  */

  return (node->next);
}


/*
 * 'mxmlGetOpaque()' - Get an opaque string value for a node or its first child.
 *
 * @code NULL@ is returned if the node (or its first child) is not an opaque
 * value node.
 *
 * @since Mini-XML 2.7@
 */

const char *				/* O - Opaque string or @code NULL@ */
mxmlGetOpaque(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (NULL);

 /*
  * Return the integer value...
  */

  if (node->type == MXML_OPAQUE)
    return (node->value.opaque);
  else if (node->type == MXML_ELEMENT &&
           node->child &&
	   node->child->type == MXML_OPAQUE)
    return (node->child->value.opaque);
  else
    return (NULL);
}


/*
 * 'mxmlGetParent()' - Get the parent node.
 *
 * @code NULL@ is returned for a root node.
 *
 * @since Mini-XML 2.7@
 */

mxml_node_t *				/* O - Parent node or @code NULL@ */
mxmlGetParent(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (NULL);

 /*
  * Return the node type...
  */

  return (node->parent);
}


/*
 * 'mxmlGetPrevSibling()' - Get the previous node for the current parent.
 *
 * @code NULL@ is returned if this is the first child for the current parent.
 *
 * @since Mini-XML 2.7@
 */

mxml_node_t *				/* O - Previous node or @code NULL@ */
mxmlGetPrevSibling(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (NULL);

 /*
  * Return the node type...
  */

  return (node->prev);
}


/*
 * 'mxmlGetReal()' - Get the real value for a node or its first child.
 *
 * 0.0 is returned if the node (or its first child) is not a real value node.
 *
 * @since Mini-XML 2.7@
 */

double					/* O - Real value or 0.0 */
mxmlGetReal(mxml_node_t *node)		/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (0.0);

 /*
  * Return the integer value...
  */

  if (node->type == MXML_REAL)
    return (node->value.real);
  else if (node->type == MXML_ELEMENT &&
           node->child &&
	   node->child->type == MXML_REAL)
    return (node->child->value.real);
  else
    return (0.0);
}


/*
 * 'mxmlGetText()' - Get the text value for a node or its first child.
 *
 * @code NULL@ is returned if the node (or its first child) is not a text node.
 * The "whitespace" argument can be @code NULL@.
 *
 * Note: Text nodes consist of whitespace-delimited words. You will only get
 * single words of text when reading an XML file with @code MXML_TEXT@ nodes.
 * If you want the entire string between elements in the XML file, you MUST read
 * the XML file with @code MXML_OPAQUE@ nodes and get the resulting strings
 * using the @link mxmlGetOpaque@ function instead.
 *
 * @since Mini-XML 2.7@
 */

const char *				/* O - Text string or @code NULL@ */
mxmlGetText(mxml_node_t *node,		/* I - Node to get */
            int         *whitespace)	/* O - 1 if string is preceded by whitespace, 0 otherwise */
{
 /*
  * Range check input...
  */

  if (!node)
  {
    if (whitespace)
      *whitespace = 0;

    return (NULL);
  }

 /*
  * Return the integer value...
  */

  if (node->type == MXML_TEXT)
  {
    if (whitespace)
      *whitespace = node->value.text.whitespace;

    return (node->value.text.string);
  }
  else if (node->type == MXML_ELEMENT &&
           node->child &&
	   node->child->type == MXML_TEXT)
  {
    if (whitespace)
      *whitespace = node->child->value.text.whitespace;

    return (node->child->value.text.string);
  }
  else
  {
    if (whitespace)
      *whitespace = 0;

    return (NULL);
  }
}


/*
 * 'mxmlGetType()' - Get the node type.
 *
 * @code MXML_IGNORE@ is returned if "node" is @code NULL@.
 *
 * @since Mini-XML 2.7@
 */

mxml_type_t				/* O - Type of node */
mxmlGetType(mxml_node_t *node)		/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (MXML_IGNORE);

 /*
  * Return the node type...
  */

  return (node->type);
}


/*
 * 'mxmlGetUserData()' - Get the user data pointer for a node.
 *
 * @since Mini-XML 2.7@
 */

void *					/* O - User data pointer */
mxmlGetUserData(mxml_node_t *node)	/* I - Node to get */
{
 /*
  * Range check input...
  */

  if (!node)
    return (NULL);

 /*
  * Return the user data pointer...
  */

  return (node->user_data);
}
