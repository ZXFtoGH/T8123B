/*
 * Search/navigation functions for Mini-XML, a small XML file parsing library.
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
 * 'mxmlFindElement()' - Find the named element.
 *
 * The search is constrained by the name, attribute name, and value; any
 * @code NULL@ names or values are treated as wildcards, so different kinds of
 * searches can be implemented by looking for all elements of a given name
 * or all elements with a specific attribute. The descend argument determines
 * whether the search descends into child nodes; normally you will use
 * @code MXML_DESCEND_FIRST@ for the initial search and @code MXML_NO_DESCEND@
 * to find additional direct descendents of the node. The top node argument
 * constrains the search to a particular node's children.
 */

mxml_node_t *				/* O - Element node or @code NULL@ */
mxmlFindElement(mxml_node_t *node,	/* I - Current node */
                mxml_node_t *top,	/* I - Top node */
                const char  *element,	/* I - Element name or @code NULL@ for any */
		const char  *attr,	/* I - Attribute name, or @code NULL@ for none */
		const char  *value,	/* I - Attribute value, or @code NULL@ for any */
		int         descend)	/* I - Descend into tree - @code MXML_DESCEND@, @code MXML_NO_DESCEND@, or @code MXML_DESCEND_FIRST@ */
{
  const char	*temp;			/* Current attribute value */


 /*
  * Range check input...
  */

  if (!node || !top || (!attr && value))
    return (NULL);

 /*
  * Start with the next node...
  */

  node = mxmlWalkNext(node, top, descend);

 /*
  * Loop until we find a matching element...
  */

  while (node != NULL)
  {
   /*
    * See if this node matches...
    */

    if (node->type == MXML_ELEMENT &&
        node->value.element.name &&
	(!element || !strcmp(node->value.element.name, element)))
    {
     /*
      * See if we need to check for an attribute...
      */

      if (!attr)
        return (node);			/* No attribute search, return it... */

     /*
      * Check for the attribute...
      */

      if ((temp = mxmlElementGetAttr(node, attr)) != NULL)
      {
       /*
        * OK, we have the attribute, does it match?
	*/

	if (!value || !strcmp(value, temp))
	  return (node);		/* Yes, return it... */
      }
    }

   /*
    * No match, move on to the next node...
    */

    if (descend == MXML_DESCEND)
      node = mxmlWalkNext(node, top, MXML_DESCEND);
    else
      node = node->next;
  }

  return (NULL);
}


/*
 * 'mxmlFindPath()' - Find a node with the given path.
 *
 * The "path" is a slash-separated list of element names. The name "*" is
 * considered a wildcard for one or more levels of elements.  For example,
 * "foo/one/two", "bar/two/one", "*\/one", and so forth.
 *
 * The first child node of the found node is returned if the given node has
 * children and the first child is a value node.
 *
 * @since Mini-XML 2.7@
 */

mxml_node_t *				/* O - Found node or @code NULL@ */
mxmlFindPath(mxml_node_t *top,		/* I - Top node */
	     const char  *path)		/* I - Path to element */
{
  mxml_node_t	*node;			/* Current node */
  char		element[256];		/* Current element name */
  const char	*pathsep;		/* Separator in path */
  int		descend;		/* mxmlFindElement option */


 /*
  * Range check input...
  */

  if (!top || !path || !*path)
    return (NULL);

 /*
  * Search each element in the path...
  */

  node = top;
  while (*path)
  {
   /*
    * Handle wildcards...
    */

    if (!strncmp(path, "*/", 2))
    {
      path += 2;
      descend = MXML_DESCEND;
    }
    else
      descend = MXML_DESCEND_FIRST;

   /*
    * Get the next element in the path...
    */

    if ((pathsep = strchr(path, '/')) == NULL)
      pathsep = path + strlen(path);

    if (pathsep == path || (pathsep - path) >= sizeof(element))
      return (NULL);

    memcpy(element, path, pathsep - path);
    element[pathsep - path] = '\0';

    if (*pathsep)
      path = pathsep + 1;
    else
      path = pathsep;

   /*
    * Search for the element...
    */

    if ((node = mxmlFindElement(node, node, element, NULL, NULL,
                                descend)) == NULL)
      return (NULL);
  }

 /*
  * If we get this far, return the node or its first child...
  */

  if (node->child && node->child->type != MXML_ELEMENT)
    return (node->child);
  else
    return (node);
}


/*
 * 'mxmlWalkNext()' - Walk to the next logical node in the tree.
 *
 * The descend argument controls whether the first child is considered
 * to be the next node.  The top node argument constrains the walk to
 * the node's children.
 */

mxml_node_t *				/* O - Next node or @code NULL@ */
mxmlWalkNext(mxml_node_t *node,		/* I - Current node */
             mxml_node_t *top,		/* I - Top node */
             int         descend)	/* I - Descend into tree - @code MXML_DESCEND@, @code MXML_NO_DESCEND@, or @code MXML_DESCEND_FIRST@ */
{
  if (!node)
    return (NULL);
  else if (node->child && descend)
    return (node->child);
  else if (node == top)
    return (NULL);
  else if (node->next)
    return (node->next);
  else if (node->parent && node->parent != top)
  {
    node = node->parent;

    while (!node->next)
      if (node->parent == top || !node->parent)
        return (NULL);
      else
        node = node->parent;

    return (node->next);
  }
  else
    return (NULL);
}


/*
 * 'mxmlWalkPrev()' - Walk to the previous logical node in the tree.
 *
 * The descend argument controls whether the previous node's last child
 * is considered to be the previous node.  The top node argument constrains
 * the walk to the node's children.
 */

mxml_node_t *				/* O - Previous node or @code NULL@ */
mxmlWalkPrev(mxml_node_t *node,		/* I - Current node */
             mxml_node_t *top,		/* I - Top node */
             int         descend)	/* I - Descend into tree - @code MXML_DESCEND@, @code MXML_NO_DESCEND@, or @code MXML_DESCEND_FIRST@ */
{
  if (!node || node == top)
    return (NULL);
  else if (node->prev)
  {
    if (node->prev->last_child && descend)
    {
     /*
      * Find the last child under the previous node...
      */

      node = node->prev->last_child;

      while (node->last_child)
        node = node->last_child;

      return (node);
    }
    else
      return (node->prev);
  }
  else if (node->parent != top)
    return (node->parent);
  else
    return (NULL);
}
