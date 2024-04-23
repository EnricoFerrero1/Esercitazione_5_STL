#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;
    string filepath = "PolygonalMesh";

    if (!ImportMesh(filepath, mesh)) //controllo di importare correttamente la mesh
    {
        cerr << "Errore nell'importazione della mesh" << endl;
        return 1;
    }
    //controlli su segmenti e poligoni
    CheckSegmentLengths(mesh);
    CheckPolygonAreas(mesh);

    cout << "Mesh importata correttamente" << endl;
    cout << "Numero di vertici: " << mesh.Cell0DId.size() << endl;
    cout << "Numero di lati: " << mesh.Cell1DId.size() << endl;
    cout << "Numero di poligoni: " << mesh.Cell2DId.size() << endl;

    return 0;
}
