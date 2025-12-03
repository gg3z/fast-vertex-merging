#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <cstdlib>
#include <memory>

using namespace std;

using Id = uint32_t;

struct Vertex {
    array<double,3> p{0,0,0};
};

struct Face {
    array<Id, 3> v;
};

struct GridHash
{
    unordered_map<long long, vector<int>> buckets;
    double s; // cell size
    static long long pack(array<long long,3> ixyz){
        // simple 3D Morton-like pack
        const long long B=73856093; // coprime-ish
        return ixyz[0]*B ^ ixyz[1]*19349663LL ^ ixyz[2]*83492791LL;
    }
    GridHash(double cell):s(cell){}
    
    void insert(const array<double,3>& p, int idx){
        array<long long,3> ixyz = {llround(floor(p[0]/s)), llround(floor(p[1]/s)), llround(floor(p[2]/s))};
        buckets[pack(ixyz)].push_back(idx);
    }
    void neighbors(const array<double,3>& p, vector<int>& out) const {
        out.clear();
        array<long long,3> ixyz = {llround(floor(p[0]/s))-1, llround(floor(p[1]/s))-1, llround(floor(p[2]/s))-1};
        for( long long k = 0; k < 27; k++ )
        {
            array<long long,3> neighbor = { ixyz[0]+k%3, ixyz[1]+(k/3)%3, ixyz[2]+k/9 };
            auto it = buckets.find(pack(neighbor));
            if(it!=buckets.end()) out.insert(out.end(), it->second.begin(), it->second.end());
        }
    }
};

struct Mesh {
    vector<Vertex> V;
    vector<Face>   F; 
    int read(const string& filename);
    int readSTL(const string& filename);
    bool write(const string& filename) const;
    int mergeDupVerts(double tol = 1.e-10);
};


int consolidate(vector<Vertex>&, double tol, vector<uint32_t> &old_to_new);
