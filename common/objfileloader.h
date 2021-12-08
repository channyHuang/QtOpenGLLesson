#ifndef OBJFILELOADER_H
#define OBJFILELOADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <QVector3D>
#include <QDebug>
#include <QOpenGLFunctions>

#include "vector3i.h"

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector3D texcoord;
};

struct FileObject {
    std::vector<Vertex> vertex;
    std::vector<Vector3i> faces;
};

class ObjFileLoader
{
public:
    ObjFileLoader();
    ~ObjFileLoader();

    const GLfloat *constData() const { return m_data.constData(); }
    FileObject getDataStruct() { return m_stFileObject; }
    int getDataCount() const { return m_nDataCount; }
    int getVertexCount() const { return m_nVertexCount; }
    int getFaceCount() const { return m_nFaceCount; }

    bool loadObjFile(std::string sObjFile);
    bool loadObjFile(std::string sObjFile, FileObject &stFileObject);
    bool pushToData(FileObject &stFileObject);

private:
    QVector<GLfloat> m_data;
    int m_nDataCount = 0, m_nVertexCount = 0, m_nFaceCount = 0;

    FileObject m_stFileObject;
};

#endif // OBJFILELOADER_H
