/*
 * Attribute support code for Mini-XML, a small XML file parsing library.
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


/*
 * Local functions...
 */

static int	mxml_set_attr(mxml_node_t *node, const char *name, char *value);


/*
 * 'mxmlElementDeleteAttr()' - Delete an attribute.
 *
 * @since Mini-XML 2.4@
 */

void
mxmlElementDeleteAttr(mxml_node_t *node,/* I - Element */
                      const char  *name)/* I - Attribute name */
{
  int		i;			/* Looping var */
  _mxml_attr_t	*attr;			/* Cirrent attribute */


#ifdef DEBUG
  fprintf(stderr, "mxmlElementDeleteAttr(node=%p, name=\"%s\")\n",
          node, name ? name : "(null)");
#endif /* DEBUG */

 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT || !name)
    return;

 /*
  * Look for the attribute...
  */

  for (i = node->value.element.num_attrs, attr = node->value.element.attrs;
       i > 0;
       i --, attr ++)
  {
#ifdef DEBUG
    printf("    %s=\"%s\"\n", attr->name, attr->value);
#endif /* DEBUG */

    if (!strcmp(attr->name, name))
    {
     /*
      * Delete this attribute...
      */

      free(attr->name);
      free(attr->value);

      i --;
      if (i > 0)
        memmove(attr, attr + 1, i * sizeof(_mxml_attr_t));

      node->value.element.num_attrs --;

      if (node->value.element.num_attrs == 0)
        free(node->value.element.attrs);
      return;
    }
  }
}


/*
 * 'mxmlElementGetAttr()' - Get an attribute.
 *
 * This function returns @code NULL@ if the node is not an element or the
 * named attribute does not exist.
 */

const char *				/* O - Attribute value or @code NULL@ */
mxmlElementGetAttr(mxml_node_t *node,	/* I - Element node */
                   const char  *name)	/* I - Name of attribute */
{
  int		i;			/* Looping var */
  _mxml_attr_t	*attr;			/* Cirrent attribute */


#ifdef DEBUG
  fprintf(stderr, "mxmlElementGetAttr(node=%p, name=\"%s\")\n",
          node, name ? name : "(null)");
#endif /* DEBUG */

 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT || !name)
    return (NULL);

 /*
  * Look for the attribute...
  */

  for (i = node->value.element.num_attrs, attr = node->value.element.attrs;
       i > 0;
       i --, attr ++)
  {
#ifdef DEBUG
    printf("    %s=\"%s\"\n", attr->name, attr->value);
#endif /* DEBUG */

    if (!strcmp(attr->name, name))
    {
#ifdef DEBUG
      printf("    Returning \"%s\"!\n", attr->value);
#endif /* DEBUG */
      return (attr->value);
    }
  }

 /*
  * Didn't find attribute, so return NULL...
  */

#ifdef DEBUG
  puts("    Returning NULL!\n");
#endif /* DEBUG */

  return (NULL);
}


/*
 * 'mxmlElementGetAttrByIndex()' - Get an element attribute by index.
 *
 * The index ("idx") is 0-based.  @code NULL@ is returned if the specified index
 * is out of range.
 *
 * @since Mini-XML 2.11@
 */

const char *                            /* O - Attribute value */
mxmlElementGetAttrByIndex(
    mxml_node_t *node,                  /* I - Node */
    int         idx,                    /* I - Attribute index, starting at 0 */
    const char  **name)                 /* O - Attribute name */
{
  if (!node || node->type != MXML_ELEMENT || idx < 0 || idx >= node->value.element.num_attrs)
    return (NULL);

  if (name)
    *name = node->value.element.attrs[idx].name;

  return (node->value.element.attrs[idx].value);
}


/*
 * 'mxmlElementGetAttrCount()' - Get the number of element attributes.
 *
 * @since Mini-XML 2.11@
 */

int                                     /* O - Number of attributes */
mxmlElementGetAttrCount(
    mxml_node_t *node)                  /* I - Node */
{
  if (node && node->type == MXML_ELEMENT)
    return (node->value.element.num_attrs);
  else
    return (0);
}


/*
 * 'mxmlElementSetAttr()' - Set an attribute.
 *
 * If the named attribute already exists, the value of the attribute
 * is replaced by the new string value. The string value is copied
 * into the element node. This function does nothing if the node is
 * not an element.
 */

void
mxmlElementSetAttr(mxml_node_t *node,	/* I - Element node */
                   const char  *name,	/* I - Name of attribute */
                   const char  *value)	/* I - Attribute value */
{
  char	*valuec;			/* Copy of value */


#ifdef DEBUG
  fprintf(stderr, "mxmlElementSetAttr(node=%p, name=\"%s\", value=\"%s\")\n",
          node, name ? name : "(null)", value ? value : "(null)");
#endif /* DEBUG */

 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT || !name)
    return;

  if (value)
    valuec = strdup(value);
  else
    valuec = NULL;

  if (mxml_set_attr(node, name, valuec))
    free(valuec);
}


/*
 * 'mxmlElementSetAttrf()' - Set an attribute with a formatted value.
 *
 * If the named attribute already exists, the value of the attribute
 * is replaced by the new formatted string. The formatted string value is
 * copied into the element node. This function does nothing if the node
 * is not an element.
 *
 * @since Mini-XML 2.3@
 */

void
mxmlElementSetAttrf(mxml_node_t *node,	/* I - Element node */
                    const char  *name,	/* I - Name of attribute */
                    const char  *format,/* I - Printf-style attribute value */
		    ...)		/* I - Additional arguments as needed */
{
  va_list	ap;			/* Argument pointer */
  char		*value;			/* Value */


#ifdef DEBUG
  fprintf(stderr,
          "mxmlElementSetAttrf(node=%p, name=\"%s\", format=\"%s\", ...)\n",
          node, name ? name : "(null)", format ? format : "(null)");
#endif /* DEBUG */

 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT || !name || !format)
    return;

 /*
  * Format the value...
  */

  va_start(ap, format);
  value = _mxml_vstrdupf(format, ap);
  va_end(ap);

  if (!value)
    mxml_error("Unable to allocate memory for attribute '%s' in element %s!",
               name, node->value.element.name);
  else if (mxml_set_attr(node, name, value))
    free(value);
}


/*
 * 'mxml_set_attr()' - Set or add an attribute name/value pair.
 */

static int				/* O - 0 on success, -1 on failure */
mxml_set_attr(mxml_node_t *node,	/* I - Element node */
              const char  *name,	/* I - Attribute name */
              char        *value)	/* I - Attribute value */
{
  int		i;			/* Looping var */
  _mxml_attr_t	*attr;			/* New attribute */


 /*
  * Look for the attribute...
  */

  for (i = node->value.element.num_attrs, attr = node->value.element.attrs;
       i > 0;
       i --, attr ++)
    if (!strcmp(attr->name, name))
    {
     /*
      * Free the old value as needed...
      */

      if (attr->value)
        free(attr->value);

      attr->value = value;

      return (0);
    }

 /*
  * Add a new attribute...
  */

  if (node->value.element.num_attrs == 0)
    attr = malloc(sizeof(_mxml_attr_t));
  else
    attr = realloc(node->value.element.attrs,
                   (node->value.element.num_attrs + 1) * sizeof(_mxml_attr_t));

  if (!attr)
  {
    mxml_error("Unable to allocate memory for attribute '%s' in element %s!",
               name, node->value.element.name);
    return (-1);
  }

  node->value.element.attrs = attr;
  attr += node->value.element.num_attrs;

  if ((attr->name = strdup(name)) == NULL)
  {
    mxml_error("Unable to allocate memory for attribute '%s' in element %s!",
               name, node->value.element.name);
    return (-1);
  }

  attr->value = value;

  node->value.element.num_attrs ++;

  return (0);
}
