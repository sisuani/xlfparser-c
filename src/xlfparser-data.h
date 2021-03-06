#ifndef XLFPARSERDATA
#define XLFPARSERDATA

typedef enum _XlfNodeType {
    XLF_NODE_UNKNOWN = 0,

    XLF_NODE_LAYOUT,
    XLF_NODE_REGION,
    XLF_NODE_MEDIA,
    XLF_NODE_OPTION,

    XLF_NODE_LAYOUT_TAG,
    XLF_NODE_MEDIA_RAW,

    XLF_NODE_REGION_OPTION,
    XLF_NODE_MEDIA_OPTION,

    XLF_NODE_KEYVAL

} NodeType;


typedef struct _XlfNode {
    void *data;

    enum _XlfNodeType _type;
    unsigned int _id;
    struct _XlfNode *first;
    struct _XlfNode *next;
    struct _XlfNode *prev;
    struct _XlfNode *last;
} xlfNode;


typedef struct _XlfKeyVal {
    char *key;
    char *value;
} KeyVal;


typedef struct _XlfLayoutTag {
    char *tag;
} LayoutTag;

typedef struct _XlfLayout {
    int width;
    int height;
    char *bgcolor;
    char *background;

    xlfNode *tags;
    xlfNode *regions;

    xlfNode *_node;
} Layout;

typedef struct _XlfKeyVal RegionOption;

typedef struct _XlfRegion {
    char *id;
    int width;
    int height;
    int left;
    int top;

    xlfNode *media;
    xlfNode *options;

    xlfNode *_node;
} Region;

typedef struct _XlfKeyVal MediaOption;
typedef struct _XlfKeyVal MediaRaw;

typedef struct _XlfMedia {
    char *id;
    char *type;
    int duration;
    char *render;

    xlfNode *options;
    xlfNode *raws;

    xlfNode *_node;
} Media;

#endif /* XLFPARSERDATA */


