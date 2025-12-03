#include "mesh.h"

#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define DEBUG 0

int Mesh::read(const string& filename)
{
    // read vertex locations and triangle indices from a file
    // return 0 if successful
    // return 1 if error reading vertices
    // return 2 if error reading faces
    // return -1 if file cannot be opened
    // existing mesh data will be cleared
    V.clear();
    F.clear();
    ifstream fin(filename);
    if (!fin)
    {
        return -1;
    }
    string line;
    int nV = 0, nF = 0;
    while (getline(fin, line))
    {
        if (line.empty() || line[0] == '#') continue;
        if( nV == 0 || nF == 0 )
        {
            stringstream ss(line);
            ss >> nV >> nF;
            break;
        }
    }
    // read vertices
    while( nV > 0 && getline(fin, line) )
    {
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        Vertex v;
        ss >> v.p[0] >> v.p[1] >> v.p[2];
        V.emplace_back(v);
        nV--;
    }
    if( nV != 0 ) return 1;
    // read faces
    F.reserve(nF);
    while( nF > 0 && getline(fin, line) )
    {
        if (line.empty() || line[0] == '#') continue;
        Face face;
        stringstream ss(line);
        ss >> face.v[0] >> face.v[1] >> face.v[2];
        F.emplace_back(face);
        nF--;
    }
    if( nF != 0 ) return 2;
    return 0;
}

bool Mesh::write(const string& filename) const
{
    ofstream fout(filename);
    if (!fout)
    {
        cerr << "Error: cannot open output file " << filename << endl;
        return false;
    }
    // write vertices
    fout << "# Cleaned mesh\n";
    fout << V.size() << " " << F.size() << "\n";
    fout << "# Vertices\n";
    for( const auto& v : V )
    {
        fout << v.p[0] << " " << v.p[1] << " " << v.p[2] << "\n";
    }
    // write faces
    fout << "# Faces\n";
    for( const auto& f : F )
    {
        fout << f.v[0] << " " << f.v[1] << " " << f.v[2] << "\n";
    }
    return true;
}

int Mesh::mergeDupVerts(double tol)
{
    vector<uint32_t> o2n;
    int nNewInds = consolidate(V, tol, o2n);
#if DEBUG
    int iv = 0;
    for( const auto &v : V )
    {
        cout << "(" << v.p[0] << "," << v.p[1] << "," << v.p[2] << ")\n";
    }
    for( auto n : o2n )
    {
        cout << iv++ << " --> " << n << endl;
    }
#endif
    // update face indices
    for( auto &f : F )
    {
        f.v[0] = o2n[f.v[0]];
        f.v[1] = o2n[f.v[1]];
        f.v[2] = o2n[f.v[2]];
    }
    return nNewInds;
}

int consolidate(vector<Vertex>& V, double eps, vector<uint32_t>& old2new)
{
    const double cell = max(1e-12, eps);
    GridHash gh(cell);
    int n = (int)V.size();
    old2new.assign(n,-1);
    vector<int> neigh;
    int newCount=0;
    // vector<array<double,3>> NV; NV.reserve(n);
    vector<Vertex> NV; NV.reserve(n);
    for(int i=0;i<n;++i){
        const auto& p = V[i].p;
        gh.neighbors(p, neigh);
        int match=-1;
        for(int j: neigh){
            if(old2new[j]==-1) continue; // neighbor not mapped yet? skip
            int rep = old2new[j];
            const array<double,3>& q = NV[rep].p;
            double dx=p[0]-q[0], dy=p[1]-q[1], dz=p[2]-q[2];
            if(dx*dx+dy*dy+dz*dz <= eps*eps){ match = rep; break; }
        }
        if(match==-1){
            match = newCount++;
            Vertex v;
            v.p = p;
            NV.push_back(v);
        }
        old2new[i]=match;
        gh.insert(p,i);
    }
    V.swap(NV);
    return newCount;
}
