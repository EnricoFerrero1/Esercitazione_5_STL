#pragma once

#include <iostream>
#include <string>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary
{
    //dichiaro le funzioni per importare mesh, vertici, lati e poligoni
    bool ImportMesh(const string &filepath, PolygonalMesh& mesh);
    bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh);
    bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh);
    bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh);

    //dichiaro le funzioni di controllo
    void CheckSegmentLengths(const PolygonalMesh& mesh);
    void CheckPolygonAreas(const PolygonalMesh& mesh);
}
