#include "objloader.h"
#include <QIODevice>
#include <QDebug>
#include <QStringList>
#include <QFile>

ObjLoader::ObjLoader()
{
    //sinitializeOpenGLFunctions();

    m_vbo.create();
    m_nbo.create();
}

bool ObjLoader::Load(QString objFile, QString mtlFile)
{
    if(loadMtlFile(mtlFile)<0)
        return false;

    if(loadObjFile(objFile)<0)
        return false;

    return true;
}


int ObjLoader::loadMtlFile(QString pathToFile)
{
    QFile f(pathToFile);

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Error: Mtl File cannot be opened!";
        return -2;
    }

    QTextStream ts(&f);

    int index=-1;

    while(!ts.atEnd())
    {
        QString line = ts.readLine();

        QStringList strList = line.split(QRegExp("(\\s+)"));
        strList.removeAll("");

        if(strList.size() <= 0)
            continue;

        if(strList[0] == "newmtl")
        {
            MaterialInfo* tMtl = new MaterialInfo();

            tMtl->Name = strList[1];
            m_mtls.push_back(tMtl);

            index = m_mtls.size()-1;
        }

        if(strList[0] == "Ns")
        {
            m_mtls[index]->Shininess = strList[1].toFloat();
        }

        if(strList[0] == "Ka")
        {
            QVector3D temp;
            temp.setX(strList[1].toFloat());
            temp.setY(strList[2].toFloat());
            temp.setZ(strList[3].toFloat());
            m_mtls[index]->Ambient = temp;
        }

        if(strList[0] == "Kd")
        {
            QVector3D temp;
            temp.setX(strList[1].toFloat());
            temp.setY(strList[2].toFloat());
            temp.setZ(strList[3].toFloat());
            m_mtls[index]->Diffuse = temp;
        }

        if(strList[0] == "Ks")
        {
            QVector3D temp;
            temp.setX(strList[1].toFloat());
            temp.setY(strList[2].toFloat());
            temp.setZ(strList[3].toFloat());
            m_mtls[index]->Specular = temp;
        }
    }

    for(int i=0;i<m_mtls.size();i++)
    {
        qDebug()<<"Ka"<<m_mtls[i]->Ambient;
        qDebug()<<"Kd"<<m_mtls[i]->Diffuse;
        qDebug()<<"Ks"<<m_mtls[i]->Specular;
        qDebug()<<"Ns"<<m_mtls[i]->Shininess;
    }

    qDebug()<<"Size of materials"<<m_mtls.size();

    return 0;
}

int ObjLoader::loadObjFile(QString pathToFile)
{
    QFile f(pathToFile);

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Error: Obj file cannot be opened!";
        return -2;
    }

    QTextStream ts(&f);

    int offset=0;
    int tCount=0;
    Mesh* tMesh=NULL;

    while(!ts.atEnd())
    {
        QString line = ts.readLine();

        QStringList strList = line.split(QRegExp("(\\s+)|/"));
        strList.removeAll("");

        if(strList.size() <= 0)
            continue;

        if(strList[0] == "v")
        {
            Point3 tmp(strList[1].toFloat(),strList[2].toFloat(),strList[3].toFloat());
            m_vertices.push_back(tmp);
        }

        if(strList[0] == "vt")
        {

        }

        if(strList[0] == "vn")
        {
            Point3 tmp(strList[1].toFloat(),strList[2].toFloat(),strList[3].toFloat());
            m_normals.push_back(tmp);
        }

        if(strList[0] == "usemtl")
        {
            if(tMesh!=NULL)
            {
                tMesh->dataCount=tCount;
                tCount=0;
                m_meshes.push_back(tMesh);
                //delete tMesh;
            }
            tMesh=new Mesh;
            tMesh->dataOffset=offset;
            for(int i=0;i<m_mtls.size();i++)
            {
                if(strList[1] == m_mtls[i]->Name)
                {
                    tMesh->material=m_mtls[i];
                }
            }
        }

        if(strList[0] == "f")
        {
            for(int i=0;i<3;i++)
            {
                m_dataVertices.push_back(m_vertices[strList[3*i+1].toInt()-1].x);
                m_dataVertices.push_back(m_vertices[strList[3*i+1].toInt()-1].y);
                m_dataVertices.push_back(m_vertices[strList[3*i+1].toInt()-1].z);
                m_dataNormals.push_back(m_normals[strList[3*i+3].toInt()-1].x);
                m_dataNormals.push_back(m_normals[strList[3*i+3].toInt()-1].y);
                m_dataNormals.push_back(m_normals[strList[3*i+3].toInt()-1].z);
                offset+=1;
                tCount+=1;
            }
        }
    }

    if(tMesh!=NULL)
    {
        tMesh->dataCount=tCount;
        m_meshes.push_back(tMesh);
    }

    initializeOpenGLFunctions();

    qDebug()<<"Size of Meshes:"<<m_meshes.size();

    return 0;
}


void ObjLoader::allocateVertexBuffer(QOpenGLShaderProgram *program)
{
    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(&(m_dataVertices[0]),m_dataVertices.size()*sizeof(float));

    program->enableAttributeArray(0);
    program->setAttributeBuffer(0,GL_FLOAT,0,3);

    m_nbo.create();
    m_nbo.bind();
    m_nbo.allocate(&(m_dataNormals[0]),m_dataNormals.size()*sizeof(float));

    program->enableAttributeArray(1);
    program->setAttributeBuffer(1,GL_FLOAT,0,3);
}

void ObjLoader::drawObjEntity(QOpenGLShaderProgram *program, QMatrix4x4 model, QMatrix4x4 view, QMatrix4x4 proj)
{
    QMatrix4x4 mv=view*model;
    program->setUniformValue("MV",mv);
    program->setUniformValue("N",mv.normalMatrix());
    program->setUniformValue("MVP",proj*mv);
    m_vao.bind();

    for(int i=0;i<m_meshes.size();i++)
    {
        Mesh* m=m_meshes[i];

        program->setUniformValue("Ka",m->material->Ambient);
        program->setUniformValue("Kd",m->material->Diffuse);
        //program->setUniformValue("Kd",QVector3D(1.0,0.0,0.0));
        program->setUniformValue("Ks",m->material->Specular);
        program->setUniformValue("shininess",m->material->Shininess);

       glDrawArrays(GL_TRIANGLES,m->dataOffset,m->dataCount);
    }
    m_vao.release();
}
