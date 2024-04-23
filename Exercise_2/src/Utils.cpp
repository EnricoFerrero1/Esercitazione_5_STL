#include "Utils.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
namespace PolygonalLibrary
{
    bool ImportMesh(const string& filepath, PolygonalMesh& mesh)
    {
        if (!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh) ||
            !ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh) ||
            !ImportCell2Ds(filepath + "/Cell2Ds.csv", mesh))
        //verifico di riuscire ad importare correttamente i dati dai 3 file
        {
            cerr << "Errore nell'importazione dei dati della mesh" << endl;
            return false;
        }
        return true;
    }

    bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh)
    {
        ifstream file(filename);
        if (!file) //controllo sull'apertura del file
        {
            cerr << "Errore nell'apertura del file " << filename << endl;
            return false;
        }
        string line;
        getline(file, line); //salto l'intestazione
        while (getline(file, line)) //leggo le righe del file
        {
            istringstream iss(line);
            unsigned int id;
            unsigned int marker;
            double x, y;
            char delim;
            //controllo di riuscire a leggere correttamente i numeri in tutte le righe
            if (!(iss >> id >> delim >> marker >> delim >> x >> delim >> y))
            {
                cerr << "Errore in " << filename << " sulla linea: " << line << endl;
                continue;
            }

            mesh.Cell0DId.push_back(id); //aggiundo ID del vertice
            mesh.Cell0DCoordinates.push_back(Vector2d(x, y)); //aggiungo coordinate
            mesh.Cell0DMarkers[marker].push_back(id); //associo il puntatore con l'ID
        }
        file.close();
        return true;
    }

    bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Errore nell'apertura del file " << filename << endl;
            return false;
        }
        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            unsigned int id, marker, origin, end;
            char delim;

            if (!(iss >> id >> delim >> marker >> delim >> origin >> delim >> end))
            {
                cerr << "Errore in " << filename << " sulla linea: " << line << endl;
                continue;
            }

            mesh.Cell1DId.push_back(id); //aggiundo ID del lato
            mesh.Cell1DVertices.push_back(Vector2i(origin, end)); //aggiungo vertice di origine e di fine
            mesh.Cell1DMarkers[marker].push_back(id); //associo il puntatore con l'ID
        }
        file.close();
        return true;
    }

    bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Errore nell'apertura del file " << filename << endl;
            return false;
        }
        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            unsigned int id, marker, numVertices, numEdges;
            char delim;
            vector<unsigned int> vertices, edges;

            iss >> id >> delim >> marker >> delim >> numVertices;
            for (unsigned int i = 0; i < numVertices; ++i)
            {
                unsigned int vertex;
                iss >> delim >> vertex;
                vertices.push_back(vertex);
            }

            iss >> delim >> numEdges;
            for (unsigned int i = 0; i < numEdges; ++i)
            {
                unsigned int edge;
                iss >> delim >> edge;
                edges.push_back(edge);
            }

            mesh.Cell2DId.push_back(id); //aggiundo ID del poligono
            mesh.Cell2DVertices.push_back(vertices); //aggiungo i suoi vertici
            mesh.Cell2DEdges.push_back(edges); //associo il puntatore con l'ID
        }
        file.close();
        return true;
    }

    //funzione che verifica che i lati abbiano lunghezza non nulla
    void CheckSegmentLengths(const PolygonalMesh& mesh)
    {
        for (const auto& vertices : mesh.Cell1DVertices)
        {
            Vector2d start = mesh.Cell0DCoordinates[vertices(0)]; //inizio lato
            Vector2d end = mesh.Cell0DCoordinates[vertices(1)]; //fine lato
            double length = (end - start).norm();  //lunghezza lato
            if (length == 0)
            {
                cerr << "Il segmento di vertici " << vertices(0) << " e " << vertices(1) << " ha lunghezza nulla" << endl;
            }
        }
    }

    //funzione che verifica che tutte le aree dei poligoni siano non nulle
    void CheckPolygonAreas(const PolygonalMesh& mesh)
    {
        for (const auto& vertices : mesh.Cell2DVertices)
        {
            double area = 0;
            int n = vertices.size();
            if (n >= 3) //se un poligono non ha almeno 3 vertici, ha sicuramente area nulla
            {
                //calcolo area poligono date le coordinate dei suoi vertici con formula di gauss
                for (int i = 0; i < n; ++i)
                {
                    Vector2d current = mesh.Cell0DCoordinates[vertices[i]];
                    Vector2d next = mesh.Cell0DCoordinates[vertices[(i + 1) % n]];
                    area += current[0] * next[1] - next[0] * current[1];
                }
                area = 0.5 * abs(area);

                if (area == 0)
                {
                    cerr << "Il poligono di vertici: ";
                    for (int i = 0; i < n; ++i) { cerr << vertices[i] << " "; }
                    cerr << "ha area nulla" << endl;
                }
            }
        }
    }

}
