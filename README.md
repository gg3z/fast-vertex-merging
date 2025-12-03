# fast-vertex-merging
[--- IN PROGRESS / Please check later this evening or in the morning ---]

An optimal 3D vertex merging implementation based on grid hashing (O(n+f) where n = number of vertices and f = number of triangles).
The underlying algorithm uses a grid hash based on Morton-like codes.
This code focuses on consolidating the vertices and their indices - the faces are not modified at all.

Rationale: repeated vertices are a common occurence in scanned meshes, and especially in STL files, where repetition is guaranteed because of the format itself.
          Vertex consolidation is important:
          - to improve performance for downstream mesh operations
          - to establish the expected topology and detect any topological issues

Example 1: a simple 2D mesh with repeated vertex locations, so that the simplified mesh can be validated visually.

Example 2: a large randomly generated 3D mesh containing vertex clusters with a predefined diameter.
            When this mesh is simplified using a tolerance slghtly larger than the above-mentioned diameter, each cluster will be reduced to one vertex.
