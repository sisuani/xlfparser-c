#ifndef XLFPARSER_ROXML_H
#define XLFPARSER_ROXML_H
#ifdef XLFPARSER_USE_LIBROXML
#include <roxml.h>
#include "xlflayout.h"

Node *parse_layout(const char *xlf_file);
#else
void xlfparser_roxml_dummy();
#endif /* XLFPARSER_USE_LIBROXML */
#endif /* XLFPARSER_ROXML_H */
