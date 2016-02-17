import unittest
from ClassWeightedGraph import *
from MinimumSpanningTree import *


class TestSpanningTree(unittest.TestCase):

    def setUp(self):
        self.graph = WeightedGraph()
        N = 100000
        for i in range(41, N):
            self.graph.AddEdge(i - 41, i, 41)
            if i > 57:
                self.graph.AddEdge(i - 58, i, 58)
      
    def test_prim(self):
        tree = GetMinimumSpanningTree(self.graph)
        self.assertEqual(len(tree.Vertices()), len(self.graph.Vertices()))
        self.assertEqual(tree.totalLengthUpperBound, 40 * 58 + (len(self.graph) - 41) * 41 + 1)


if __name__ == '__main__':                      
    unittest.main()
