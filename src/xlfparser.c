#include "xlflayout.h"
#include "xlfparser.h"

#ifdef XLFPARSER_USE_LIBROXML
#include "xlfparser-roxml.h"
#endif

#ifdef XLFPARSER_USE_LIBXML2
#include "xlfparser-xml2.h"
#endif

xlfNode *xlfparser_parse_file(const char *filename, int *nr, int *nt) {
    return parse_layout(filename, nr, nt);
}

Layout *xlfparser_get_layout(xlfNode *xlf_node) {
    if(xlf_node == NULL) {
        return NULL;
    }
    return (Layout *) xlf_node->data;
}

int xlfparser_region_length(Layout *layout) {

    if(layout == NULL) {
        return -1;
    }

    return node_length(layout->regions);
}

Region *xlfparser_get_region(Layout *layout, int index, int *no, int *nm) {
    xlfNode *node;

    *no = *nm = -1;

    if(layout == NULL) {
        return NULL;
    }

    node = node_get(layout->regions, index);
    if(node) {
        *no = node_length(((Region *)node->data)->options);
        *nm = node_length(((Region *)node->data)->media);
        return node->data;
    }

    return NULL;
}

int xlfparser_region_option_length(Region *region) {
    if(region == NULL) {
        return -1;
    }
    return node_length(region->options);
}

RegionOption *xlfparser_get_region_option(Region *region, int index, const char *key) {
    xlfNode *node;
    RegionOption *opt;
    int i, len;

    if(region == NULL) {
        return NULL;
    }
    if(key == NULL) {
        node = node_get(region->options, index);
        return node ? node->data : NULL;
    }

    len = xlfparser_region_option_length(region);
    for(i = 0; i < len; i++) {
        opt = xlfparser_get_region_option(region, i, NULL);
        if(!strcmp(opt->key, key)) {
            return opt;
        }
    }

    return NULL;

}

int xlfparser_tag_length(Layout *layout) {
    if(layout == NULL) {
        return -1;
    }

    return node_length(layout->tags);
}

int xlfparser_media_length(Region *region) {
    if(region == NULL) {
        return -1;
    }
    return node_length(region->media);
}

char *xlfparser_get_tag(Layout *layout, int index) {
    xlfNode *node;

    if(layout == NULL) {
        return NULL;
    }
    node = node_get(layout->tags, index);
    return node ? ((LayoutTag *)(node)->data)->tag : NULL;
}

Media *xlfparser_get_media(Region *region, int index) {
    xlfNode *node;
    if(region == NULL) {
        return NULL;
    }
    node = node_get(region->media, index);
    return node ? node->data : NULL;
}

int xlfparser_media_option_length(Media *media) {
    if(media == NULL) {
        return -1;
    }
    return node_length(media->options);
}

MediaOption *xlfparser_get_media_option(Media *media, int index, const char *key) {
    xlfNode *node;
    MediaOption *opt;
    int i, len;
    if(media == NULL) {
        return NULL;
    }
    if(key == NULL) {
        node = node_get(media->options, index);
        return node ? node->data : NULL;
    }

    len = xlfparser_media_option_length(media);
    for(i = 0; i < len; i++) {
        opt = xlfparser_get_media_option(media, i, NULL);
        if(!strcmp(opt->key, key)) {
            return opt;
        }
    }

    return NULL;
}

int xlfparser_media_raw_length(Media *media) {
    if(media == NULL) {
        return -1;
    }
    return node_length(media->raws);
}

MediaRaw *xlfparser_get_media_raw(Media *media, int index) {
    xlfNode *node;

    if(media == NULL) {
        return NULL;
    }
    node = node_get(media->raws, index);
    return node ? node->data : NULL;
}

int xlfparser_delete_layout(Layout *layout) {
    xlfNode *node;
    if(layout == NULL || layout->_node == NULL) {
        return -1;
    }
    node = layout->_node;
    return layout_delete_all(&node);
}

int xlfparser_node_length(xlfNode *xlf_node) {
    return node_length(xlf_node);
}

Layout *xlfparser_layout_dup(Layout *src) {
    xlfNode *dst;
    int i, len, no, nm;
    Layout *layout;
    Region *region;
    char *tag;

    dst = layout_new();
    layout = dst->data;

    layout->background = _xlf_strcpy(src->background);
    layout->bgcolor = _xlf_strcpy(src->bgcolor);
    layout->height = src->height;
    layout->width = src->width;

    len = xlfparser_node_length(src->regions);
    len = len > -1 ? len : 0;
    for(i = 0; i < len; i++) {
        region = xlfparser_region_dup(xlfparser_get_region(src, i, &no, &nm));
        layout_add_region(&dst, region->_node);
    }

    len = xlfparser_node_length(src->tags);
    len = len > -1 ? len : 0;
    for(i = 0; i < len; i++) {
        tag = xlfparser_get_tag(src, i);
        layout_add_tag(&dst, tag);
    }
    return layout;
}

Region *xlfparser_region_dup(Region *src) {
    xlfNode *dst;
    int i, len;
    Region *region;
    RegionOption *opt;
    Media *media;

    dst = region_new();
    region = dst->data;

    region->id = _xlf_strcpy(src->id);
    region->left = src->left;
    region->top = src->top;
    region->width = src->width;
    region->height = src->height;

    len = xlfparser_node_length(src->options);
    len = len > -1 ? len : 0;
    for(i = 0; i < len; i++) {
        opt = xlfparser_get_region_option(src, i, NULL);
        region_add_option(&dst, opt->key, opt->value);
    }

    len = xlfparser_node_length(src->media);
    len = len > -1 ? len : 0;
    for(i = 0; i < len; i++) {
        media = xlfparser_media_dup(xlfparser_get_media(src, i));
        region_add_media(&dst, media->_node);
    }

    return region;
}

int xlfparser_delete_region(Region *region) {
    xlfNode *node;
    if(region == NULL || region->_node == NULL) {
        return -1;
    }
    node = region->_node;
    return region_delete_all(&node);
}

Media *xlfparser_media_dup(Media *src) {
    xlfNode *dst;
    MediaOption *opt;
    MediaRaw *raw;
    Media *media;
    int len;
    int i;

    dst = media_new();
    media = dst->data;

    media->id = _xlf_strcpy(src->id);
    media->type = _xlf_strcpy(src->type);
    media->render = _xlf_strcpy(src->render);
    media->duration = src->duration;


    len = xlfparser_media_option_length(src);
    len = len > -1 ? len : 0;
    for(i = 0; i < len; i++) {
        opt = xlfparser_get_media_option(src, i, NULL);
        media_add_option(&dst, opt->key, opt->value);
    }

    len = xlfparser_media_raw_length(src);
    len = len > -1 ? len : 0;
    for(i = 0; i < len; i++) {
        raw = xlfparser_get_media_raw(src, i);
        media_add_raw(&dst, raw->key, raw->value);
    }

    return dst->data;
}

int xlfparser_delete_media(Media *media) {
    xlfNode *node;
    if(media == NULL || media->_node == NULL) {
        return -1;
    }
    node = media->_node;
    return media_delete_all(&node);
}
