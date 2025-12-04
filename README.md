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

After launching  './build/polyclean mesh_ex1.dat'


the result should be  'merged_mesh_ex1.dat'


Example 2: a study of the execution time to merge increasingly large meshes. These meshes are randomly generated and contain vertex clusters with a predefined diameter d. The grid hashing algorithm reduces each cluster to one vertex.
The code in this repository produces the following output (times are in microseconds)

Vertices merged from 16 to 8 ; Time to merge = 105 us
Vertices merged from 32 to 8 ; Time to merge = 179 us
Vertices merged from 64 to 8 ; Time to merge = 326 us
Vertices merged from 128 to 8 ; Time to merge = 515 us
Vertices merged from 256 to 9 ; Time to merge = 3598 us
Vertices merged from 576 to 9 ; Time to merge = 2277 us
Vertices merged from 1152 to 9 ; Time to merge = 4684 us
Vertices merged from 2304 to 10 ; Time to merge = 9562 us
Vertices merged from 5120 to 14 ; Time to merge = 21720 us
Vertices merged from 14336 to 14 ; Time to merge = 68710 us
Vertices merged from 28672 to 12 ; Time to merge = 156295 us
Vertices merged from 49152 to 12 ; Time to merge = 243646 us

The relationship between initial vertices and execution time is almost linear, as expected.

TBD:
- random locations should be generated more "randomly"
- there is a bug in the merging of vertices (the final number of vertices should always be 8)
- performance can likely be improved
