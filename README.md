# fast-vertex-merging

This small c++ project implements an optimal 3D vertex merging algorithm (O(v+f) where v = number of vertices and f = number of triangles).
The underlying algorithm uses a grid hash based on Morton-like codes.

This code focuses on consolidating the vertices and their indices - the faces are not modified though their vertex indices will be updated.

Rationale: repeated vertices are a common occurence in scanned meshes, and especially in STL files, where repetition is guaranteed due to the format itself.
          Vertex consolidation is important:
          
          - to improve performance for downstream mesh operations
          - to facilitate further mesh cleanup operations, e.g. edge collapse, degenerate face removal
          - to reliably derive the expected topology and detect any topological issues

Example 1: a simple 2D mesh with repeated vertex locations, so that the simplified mesh can be validated visually.

After launching:


'./build/polyclean mesh_ex1.dat'


the result should be:


'merged_mesh_ex1.dat'


Example 2: (in progress) a large randomly generated 3D mesh containing vertex clusters with a predefined diameter d.
            When this mesh is simplified using a tolerance slghtly larger than the diameter d, each cluster will be reduced to one vertex.
