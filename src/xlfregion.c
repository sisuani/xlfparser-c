#include "xlfregion.h"

Node *region_new() {
    Node *node;
    Region *region;

    region = malloc(sizeof(Region));
    region->media = NULL;
    region->options = NULL;

    node = node_new();
    node->data = (Region *) region;
    node->_type = XLF_NODE_REGION;

    return node;
}

int region_delete_last(Node **node) {
    Node *last;
    Region *region;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    region = last->data;

    keyval_delete_all(&(region->options));
    media_delete_all(&(region->media));

    free(region->id);
    region->id = NULL;
    free(region);
    region = NULL;

    return node_delete_last(node);
}

int region_delete_all(Node **node) {
    while(*node) {
        region_delete_last(node);
    }

    return node_length(*node);
}

void region_add_media(Node **region_node, Node *media_node) {
    Region *region;

    region = (*region_node)->data;
    if(!region->media) {
        region->media = media_node;
    } else {
        node_append(&(region->media), media_node);
    }
}

void region_add_option(Node **region_node, const char *key, const char *value) {
    Region *region;

    region = (*region_node)->data;
    if(!region->options) {
        region->options = keyval_new(key, value);
    } else {
        keyval_append(&(region->options), key, value);
    }
}