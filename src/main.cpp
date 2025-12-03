#include "mesh.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>

using namespace std;

int main(int argc, char* argv[])
{
  Mesh mesh;
  if( mesh.read(argv[1]) != 0 )
  {
    cerr << "Error: failed to read mesh from file " << argv[1] << endl;
    return -1;
  }
  string vmerged("merged_");
  vmerged += string(argv[1]);
  cout << "Initial vertices = " << mesh.V.size() << "\n";
  const auto start = std::chrono::steady_clock::now();
  mesh.mergeDupVerts();
  const auto end = std::chrono::steady_clock::now();
  cout << "Time to merge = " << end - start << endl;
  cout << "Resulting vertices = " << mesh.V.size() << "\n";
  mesh.write(vmerged);
  return 0;
}