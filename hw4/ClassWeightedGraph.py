class WeightedGraph(dict):
    def __init__(self):
        super().__init__(self)
        self._totalLengthUpperBound = 1
        
    def AddVertex(self, vertex):
        if vertex not in self:
            self[vertex] = dict()
            
    def AddEdge(self, vertex1, vertex2, length):
        if length > 0:
            self.AddVertex(vertex1)
            self.AddVertex(vertex2)
            if vertex2 not in self[vertex1]:
                self[vertex1][vertex2] = length
                self[vertex2][vertex1] = length
                self._totalLengthUpperBound += length
            
    def HasVertex(self, vertex):
        return vertex in self
    
    def HasEdge(self, vertex1, vertex2):
        return self.HasVertex(vertex1) and (vertex2 in self[vertex1])
    
    def EdgeLength(self, vertex1, vertex2):
        if self.HasVertex(vertex1):
            return self[vertex1].get(vertex2)

    #generator for all vertices of the graph
    def Vertices(self):
        return self.keys()

    #generator for all adjacent vertices of the given vertex
    def AdjacentVertices(self, vertex):
        if self.HasVertex(vertex):
            return self[vertex].keys()
        
    @property
    def totalLengthUpperBound(self):
        return self._totalLengthUpperBound
        
