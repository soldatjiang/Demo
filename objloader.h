#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

struct MaterialInfo
{
    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;
};

struct LightInfo
{
    QVector4D position;
    QVector3D Intensity;
};


/*struct Entity
{
    QString name;
    QVector<Mesh*> meshes;
};*/

struct Mesh
{
    int dataCount;
    int dataOffset;
    MaterialInfo* material;
};

class Point3
{
public:
    float x,y,z;
    Point3():x(0),y(0),z(0){}
    Point3(float x,float y,float z):x(x),y(y),z(z){}
};


class ObjLoader : protected QOpenGLFunctions
{
public:
    ObjLoader();
    bool Load(QString objFile,QString mtlFile);
    void drawObjEntity(QOpenGLShaderProgram* program,QMatrix4x4 model,QMatrix4x4 view,QMatrix4x4 proj);
    void allocateVertexBuffer(QOpenGLShaderProgram* program);
private:
    int loadMtlFile(QString pathToFile);
    int loadObjFile(QString pathToFile);

    QVector<Point3> m_vertices;
    QVector<Point3> m_normals;

    QVector<float> m_dataVertices;
    QVector<float> m_dataNormals;

    QVector<MaterialInfo*> m_mtls;
    QVector<Mesh*> m_meshes;
    QOpenGLBuffer m_vbo,m_nbo;
    QOpenGLVertexArrayObject m_vao;
};

#endif // OBJLOADER_H
