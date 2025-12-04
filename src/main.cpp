#include "mesh.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
  // cout << "argc = " << argc << endl;
  // cout << round(1.99) << " " << round(2.01) << " " << round(-1.01) << " " << round(-0.99) << endl;
  // test_merge("mesh_ex1.dat");
  test_randomize("mesh_ex2.dat");
  return 0;
}