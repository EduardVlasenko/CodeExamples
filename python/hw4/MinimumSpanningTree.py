from ClassWeightedGraph import *
import heapq

def GetMinimumSpanningTree(graph):
    spanning_tree = WeightedGraph()
    if not graph:
        return spanning_tree
    start_vertex = next(iter(graph.Vertices()))
    spanning_tree.AddVertex(start_vertex)
    if not graph.AdjacentVertices(start_vertex):
        return spanning_tree
    available_edges = [min([(graph.EdgeLength(start_vertex, x), start_vertex, x)
                            for x in graph.AdjacentVertices(start_vertex)], key=lambda x: x[0])]
    while len(spanning_tree) < len(graph):
        new_edge = heapq.heappop(available_edges)
        if not new_edge[2] in spanning_tree.Vertices():
            spanning_tree.AddVertex(new_edge[2])
            spanning_tree.AddEdge(new_edge[1], new_edge[2], new_edge[0])
            for visited_vertex in new_edge[1], new_edge[2]:
                min_edge = [graph.totalLengthUpperBound]
                for vertex in graph.AdjacentVertices(visited_vertex):
                    if vertex not in spanning_tree and min_edge[0] > graph.EdgeLength(visited_vertex, vertex):
                        min_edge = (graph.EdgeLength(visited_vertex, vertex), visited_vertex, vertex)
                if len(min_edge) == 3:
                    heapq.heappush(available_edges, min_edge)
        else:
            min_edge = [graph.totalLengthUpperBound]
            for vertex in graph.AdjacentVertices(new_edge[1]):
                if vertex not in spanning_tree and min_edge[0] > graph.EdgeLength(new_edge[1], vertex):
                    min_edge = (graph.EdgeLength(new_edge[1], vertex), new_edge[1], vertex)
            if len(min_edge) == 3:
                heapq.heappush(available_edges, min_edge)
    return spanning_tree
