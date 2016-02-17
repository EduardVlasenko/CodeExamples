import unittest
from ClassWeightedGraph import *
from MinimumSpanningTree import *
import copy

# Decrease variable n (in setUp if it is set) if test case takes much time.

class TestSpanningTree(unittest.TestCase):
    # This test case is super for other test cases. In other test cases setUp(self) is redefined.
    # Don't forget to define all required parameter or skip tests using that parameters.
    # You can redefine tests to skip them this way:
    # @unittest.skip("Comment")
    # def test_(self):
    #    pass
    #
    # I haven't added skips case I don't know how will results be processed.
    # Message from skipped test is different from passed one.
    # This test case also covers empty graph.

    def setUp(self):
        self.graph = WeightedGraph()
        self.result_length_upper_bound = 1
        self.graph_copy = WeightedGraph()
        self.tree = GetMinimumSpanningTree(self.graph)

    def test_return_type(self):
        self.assertEqual(type(self.tree), WeightedGraph)

    def test_graph_has_not_changed(self):
        self.assertEqual(self.graph, self.graph_copy)

    def test_vertices_set(self):
        self.assertEqual(set(self.tree.Vertices()), set(self.graph.Vertices()), "Wrong vertices")

    def test_edges_length(self):
        for x in self.tree.Vertices():
            for y in self.tree.AdjacentVertices(x):
                self.assertEqual(self.tree.EdgeLength(x, y), self.graph.EdgeLength(x, y), "Wrong edge length")

    def test_tree(self):
        if not self.tree:
            return
        available_vertexes = [next(iter(self.tree.Vertices()))]
        double_number_of_edges = 0
        for i in range(len(self.tree)):
            if i < len(available_vertexes):
                for vertex in self.tree.AdjacentVertices(available_vertexes[i]):
                    double_number_of_edges += 1
                    if vertex not in available_vertexes:
                        available_vertexes.append(vertex)
            else:
                self.fail("'Tree' is not connective")
        self.assertEqual(double_number_of_edges, 2 * (len(self.tree) - 1), "Not a tree, too many edges")

    def test_minimal(self):
        self.assertEqual(self.tree.totalLengthUpperBound, self.result_length_upper_bound, "Not minimal")


class TestSpanningTreeOneVertex(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        self.graph.AddVertex(57.57)
        self.result_length_upper_bound = 1
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


class TestSpanningTreeTwoVertices(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        self.graph.AddVertex("57")
        self.graph.AddVertex("fifty seven")
        self.graph.AddEdge("57", "fifty seven", 1)
        self.result_length_upper_bound = 2
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


class TestSpanningTree1(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        self.graph.AddVertex(5)
        for i in range(5):
            self.graph.AddVertex(i)
            self.graph.AddEdge(i, 5, 2)
        self.graph.AddEdge(1, 3, 1)
        self.graph.AddEdge(2, 3, 1)
        self.graph.AddEdge(1, 3, 1)
        self.result_length_upper_bound = 9
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


class TestSpanningTree2(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        n = 1000
        self.graph.AddVertex(n)
        for i in range(n):
            self.graph.AddVertex(i)
            self.graph.AddEdge(i, n, i+1)
        self.result_length_upper_bound = (n + 1) * n / 2 + 1
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


class TestSpanningTree3(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        n = 1000
        for i in range(n):
            self.graph.AddVertex(i)
            for j in range(i):
                self.graph.AddEdge(i, j, i - j)
        self.result_length_upper_bound = n
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


class TestSpanningTree4(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        n = 500
        for i in range(n):
            self.graph.AddVertex(i)
        for i in range(n):
            for j in range(n):
                self.graph.AddEdge(i, j, 0.5)
        self.result_length_upper_bound = (n - 1) * 0.5 + 1
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


class TestSpanningTree5(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        for i in range(5):
            self.graph.AddVertex(i+1)
        self.graph.AddEdge(1, 2, 1)
        self.graph.AddEdge(1, 3, 2)
        self.graph.AddEdge(1, 4, 3)
        self.graph.AddEdge(1, 6, 3.5)
        self.graph.AddEdge(2, 3, 1)
        self.graph.AddEdge(3, 4, 1)
        self.graph.AddEdge(4, 5, 5)
        self.graph.AddEdge(5, 6, 6)
        self.result_length_upper_bound = 12.5
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


class TestSpanningTree6(TestSpanningTree):
    def setUp(self):
        self.graph = WeightedGraph()
        n = 1000
        for i in range(n):
            self.graph.AddVertex(i)
        for i in range(n-1):
            self.graph.AddEdge(i, i + 1, 1)
        self.result_length_upper_bound = n
        self.graph_copy = copy.deepcopy(self.graph)
        self.tree = GetMinimumSpanningTree(self.graph)


if __name__ == '__main__':
    unittest.main()
