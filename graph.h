#ifndef GRAPH_H
#define GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

    enum v_status {
        NOT_VISITED,
        VISITED
    };

    struct s_vertex {
        char label;     // label of the vertex
        double d;       // pseudo-distance for Dijkstra
        int position;   // position in the PQ
        enum v_status status;
    };

    typedef struct s_vertex vertex;

    enum e_type {
        UNSET,
        DISCOVERY,
        BACK,
        CROSS
    };

    struct s_edge {
        vertex *v1;
        vertex *v2;
        char label[10];
        enum e_type type;
    };

    typedef struct s_edge edge;

    struct s_graph {
        int numVertices;
        int numEdges;
        int vertexListSize;
        int edgeListSize;
        vertex **vertices;
        edge **edges;
    };

    typedef struct s_graph graph;

    vertex * createVertex(char key);
    edge * createEdge(vertex *v1, vertex *v2, char label[]);
    graph * createGraph(int vertexListSize, int edgeListSize);

    int addVertex(graph *g, vertex *v);
    int addEdge(graph *g, edge *e);
    int numVertices(graph *g);
    int numEdges(graph *g);

    vertex ** vertices(graph *g);
    edge ** edges(graph* g);

    vertex * opposite(edge *e, vertex *v);

    vertex ** adjacentVertices(graph *g, vertex *v);

    edge ** incidentEdges(graph *g, vertex *v);

    void unVisitGraph(graph *g);

//    // -----------------------------------------
//
//    struct s_treeNode {
//        char key;
//        int level;
//        struct s_treeNode **children;
//        int childrenListSize;
//        int childrenNum;
//    };
//
//    typedef struct s_treeNode treeNode;
//
//    treeNode * createTreeNode(char key, int childrenListSize);
//    int addChild(treeNode *n, treeNode *child);
//
//    //---------------------------------------------------------------
//    
//    
//    // --------------------------------------------
//
//    treeNode * DFS(graph *g, vertex *v);
//    treeNode * BFS(graph *g, vertex *v);
//    void TreeDFSPrint(treeNode *n, int level);

#ifdef __cplusplus
}
#endif

#endif /* GRAPH_H */

