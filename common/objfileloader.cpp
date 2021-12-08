#include "objfileloader.h"

ObjFileLoader::ObjFileLoader()
{

}

ObjFileLoader::~ObjFileLoader() {}

bool ObjFileLoader::loadObjFile(std::string sObjFile) {
    //qDebug() << "ObjFileLoader::loadObjFile " << sObjFile.c_str();
    bool res = loadObjFile(sObjFile, m_stFileObject);

    //pushToData(m_stFileObject);
    return res;
}

bool ObjFileLoader::loadObjFile(std::string sObjFile, FileObject &stFileObject) {
    std::ifstream ifs(sObjFile);
    if (!ifs.is_open()) {
        return false;
    }
    m_nVertexCount = stFileObject.vertex.size();
    m_nFaceCount = 0;
    QVector3D vec;
    Vector3i veci[3];
    std::string value;
    char tmp[1024];
    int nVertexIdx = 0, nNormalIdx = 0;
    while (std::getline(ifs, value)) {
        if (value.length() <= 5) continue;
        switch (value[0]) {
        case 'f':
            /*sscanf(value.c_str(), "%s %lld/%lld/%lld %lld/%lld/%lld %lld/%lld/%lld", tmp,
                     &veci[0][0], &veci[0][1], &veci[0][2],
                    &veci[1][0], &veci[1][1], &veci[1][2],
                    &veci[2][0], &veci[2][1], &veci[2][2]);*/
            sscanf(value.c_str(), "%s %lld//%lld %lld//%lld %lld//%lld", tmp,
                                 &veci[0][0], &veci[0][2],
                                &veci[1][0], &veci[1][2],
                                &veci[2][0], &veci[2][2]);

            stFileObject.faces.push_back(Vector3i(veci[0][0] - 1, veci[1][0] - 1, veci[2][0] - 1));
            m_nFaceCount++;
            break;
        case 'v':
            sscanf(value.c_str(), "%s %f %f %f\n", &tmp, &vec[0], &vec[1], &vec[2]);
            if (value[1] == 'n') {
                if (nNormalIdx >= m_nVertexCount) {
                    stFileObject.vertex.push_back({QVector3D(0, 0, 0), vec});
                    m_nVertexCount++;
                } else {
                    stFileObject.vertex[nNormalIdx].normal = vec;
                }
                nNormalIdx++;
            } else if (value[1] == ' ') {
                if (nVertexIdx >= m_nVertexCount) {
                    stFileObject.vertex.push_back({vec, QVector3D(0, 0, 0)});
                    m_nVertexCount++;
                } else {
                    stFileObject.vertex[nVertexIdx].position = vec;
                }
                nVertexIdx++;
            }
            break;
        default:
            break;
        }
    }
    ifs.close();
    qDebug() << "load obj " << sObjFile.c_str() << " vertex/normal count = " << nVertexIdx << " " << nNormalIdx << " " << m_nFaceCount;
    return (nVertexIdx == nNormalIdx);
}

bool ObjFileLoader::pushToData(FileObject &stFileObject) {
    m_data.resize(m_nVertexCount * 6 + m_nFaceCount * 3);
    m_nDataCount = 0;
    for (size_t i = 0; i < m_nVertexCount; ++i) {
        GLfloat *p = m_data.data() + m_nDataCount;
        *p++ = stFileObject.vertex[i].position.x();
        *p++ = stFileObject.vertex[i].position.y();
        *p++ = stFileObject.vertex[i].position.z();
        *p++ = stFileObject.vertex[i].normal.x();
        *p++ = stFileObject.vertex[i].normal.x();
        *p++ = stFileObject.vertex[i].normal.x();
        m_nDataCount += 6;
    }
    for (size_t i = 0; i < m_nFaceCount; ++i) {
        GLfloat *p = m_data.data() + m_nDataCount;
        *p++ = stFileObject.faces[i].x;
        *p++ = stFileObject.faces[i].y;
        *p++ = stFileObject.faces[i].z;
        m_nDataCount += 3;
    }
    return true;
}
