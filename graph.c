#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "graph.h"

vertex * createVertex(char label) {
    vertex *r = (vertex *) malloc(sizeof (vertex));
    if (r) {
        r->label = label;
        r->status = NOT_VISITED;
        r->d = DBL_MAX;
        r->position = -1;
    }
    return r;
}

edge * createEdge(vertex *v1, vertex *v2, char label[]) {
    edge *r = (edge *) malloc(sizeof (edge));
    if (r) {
        r->v1 = v1;
        r->v2 = v2;
        strncpy(r->label, label, sizeof(r->label));
        r->type = UNSET;
    }
    return r;
}

graph * createGraph(int vertexListSize, int edgeListSize) {
    graph *r = (graph *) malloc(sizeof (graph));
    if (r) {
        r->vertices = (vertex **) malloc(vertexListSize * sizeof (vertex *));
        if (r->vertices) {
            r->vertexListSize = vertexListSize;
            r->numVertices = 0;

            r->edges = (edge **) malloc(edgeListSize * sizeof (edge *));
            if (r->edges) {
                r->edgeListSize = edgeListSize;
                r->numEdges = 0;
            } else {
                fprintf(stderr, "No memory for edge list allocation");
                free(r->vertices);
                free(r);
                r = NULL;
            }
        } else {
            fprintf(stderr, "No memory for vertex list allocation");
            free(r);
            r = NULL;
        }
    }
    return r;
}

int addVertex(graph *g, vertex *v) {
    int r = 0;
    if (g->numVertices < g->vertexListSize) {
        g->vertices[g->numVertices++] = v;
    } else {
        vertex **temp = (vertex **) realloc(g->vertices, 2 * g->vertexListSize * sizeof (vertex *));
        if (temp) {
            g->vertices = temp;
            g->vertexListSize *= 2;
            addVertex(g, v);
        } else {
            fprintf(stderr, "Unable to allocate memory for new vertex");
            r = -1;
        }
    }
    return r;
}

int addEdge(graph *g, edge *e) {
    int r = 0;
    if (g->numEdges < g->edgeListSize) {
        g->edges[g->numEdges++] = e;
    } else {
        edge **temp = (edge **) realloc(g->edges, 2 * g->edgeListSize * sizeof (edge *));
        if (temp) {
            g->edges = temp;
            g->edgeListSize *= 2;
            addEdge(g, e);
        } else {
            fprintf(stderr, "Unable to allocate memory for new edge");
            r = -1;
        }
    }
    return r;
}

int numVertices(graph *g) {
    return g->numVertices;
}

int numEdges(graph *g) {
    return g->numEdges;
}

vertex ** vertices(graph *g) {
    return g->vertices;
}

edge ** edges(graph* g) {
    return g->edges;
}

vertex * opposite(edge *e, vertex *v) {
    vertex *r = NULL;
    if (e->v1 == v) {
        r = e->v2;
    } else if (e->v2 == v) {
        r = e->v1;
    }
    return r;
}

vertex ** adjacentVertices(graph *g, vertex *v) {
    vertex **r = (vertex **) malloc((g->numVertices + 1) * sizeof (vertex *));
    if (r) {
        vertex *temp = NULL;
        int i = 0, j = 0;
        for (i = 0; i < g->numEdges; i++) {
            if (temp = opposite(g->edges[i], v)) {
                r[j++] = temp;
            }
        }
        // NULL terminated list
        r[j] = NULL;
    }
    return r;
}

edge ** incidentEdges(graph *g, vertex *v) {
    edge **r = (edge **)malloc((g->numEdges + 1) * sizeof (edge *));
    if (r) {
        int i = 0, j = 0;
        for (i = 0; i < g->numEdges; i++) {
            if (opposite(g->edges[i], v)) {
                r[j++] = g->edges[i];
            }
        }
        // NULL terminated list
        r[j] = NULL;
    }
    return r;
}

void unVisitGraph(graph *g) {
    vertex **v = vertices(g);
    vertex **t = v;
    while (*v) {
        (*v++)->status = NOT_VISITED;
    }

    edge **e = edges(g);
    edge **u = e;
    while (*e) {
        (*e++)->type = UNSET;
    }
}

treeNode * createTreeNode(char key, int childrenListSize) {
    treeNode *r = (treeNode *) malloc(sizeof (treeNode));
    if (r) {
        r->children = (treeNode **) malloc(childrenListSize * sizeof (treeNode *));
        if (r->children) {
            r->childrenListSize = childrenListSize;
            r->childrenNum = 0;
            r->key = key;
        } else {
            free(r);
        }
    }
    return r;

}

int addChild(treeNode *n, treeNode *child) {
    int r = 0;
    if (n->childrenNum < n->childrenListSize) {
        n->children[n->childrenNum++] = child;
    } else {
        treeNode **temp = realloc(n->children, 2 * n->childrenListSize);
        if (temp) {
            n->children = temp;
            n->childrenListSize *= 2;
            r = addChild(n, child);
        } else {
            fprintf(stderr, "Unable to enlarge children list");
            r = -1;
        }
    }
    return r;
}

/*
treeNode * DFS(graph *g, vertex *v) {
    printf("%c ", v->key);
    v->status = VISITED;
    treeNode *r = createTreeNode(v->key, 8);
    if (r) {
        edge **ie = incidentEdges(g, v);
        while (*ie) {
            vertex *o = NULL;
            switch ((*ie)->type) {
                case UNSET:
                    o = opposite(*ie, v);
                    switch (o->status) {
                        case VISITED:
                            (*ie)->type = BACK;
                            break;
                        case NOT_VISITED:
                            (*ie)->type = DISCOVERY;
                            addChild(r, DFS(g, o));
                            break;
                        default:
                            fprintf(stderr, "Unexpected vertex status %d", o->status);
                            return NULL;
                    }
                    break;
                case DISCOVERY:
                case BACK:
                    break;
                default:
                    fprintf(stderr, "Unexpected edge type %d", (*ie)->type);
                    return NULL;
            }
            ie++;
        }
    }
    return r;
}
*/

/*
treeNode * BFS(graph *g, vertex *v) {
    queue *q = createQueue(16);
    treeNode *root = NULL;
    if (q) {
        queueElement *current = NULL;
        edge **ie = NULL;
        vertex *o = NULL;
        treeNode *n = NULL;
        enqueue(q, createQueueElement(v, NULL));
        v->status = VISITED;
        while (!queueIsEmpty(q)) {
            current = dequeue(q);
            n = createTreeNode(current->v->key, 8);
            if (current->parent) {
                addChild(current->parent, n);
            }
            else {
                root = n;
            }
            ie = incidentEdges(g, current->v);
            while (*ie) {
                switch ((*ie)->type) {
                    case UNSET:
                        o = opposite(*ie, current->v);
                        switch (o->status) {
                            case VISITED:
                                (*ie)->type = CROSS;
                                break;
                            case NOT_VISITED:
                                (*ie)->type = DISCOVERY;
                                enqueue(q, createQueueElement(o, n));
                                o->status = VISITED;
                                break;
                            default:
                                fprintf(stderr, "Unexpected vertex status %d", o->status);
                                return NULL;
                        }
                        break;
                    case DISCOVERY:
                    case CROSS:
                        break;
                    default:
                        fprintf(stderr, "Unexpected edge type %d", (*ie)->type);
                        return NULL;
                }
                ie++;
            }
        }
    } else {
        free(root);
        root = NULL;
    }

    return root;
}
*/

void TreeDFSPrint(treeNode *n, int level) {
    printf("%c (%d); ", n->key, level);
    int i = 0;
    for (i = 0; i < n->childrenNum; i++) {
        TreeDFSPrint(n->children[i], level + 1);
    }
}