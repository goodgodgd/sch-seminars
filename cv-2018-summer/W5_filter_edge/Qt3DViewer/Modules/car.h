#ifndef CAR_H
#define CAR_H

#include "project_common.h"
#include "glvertexmanager.h"
#include "pose6dofqt.h"


class Car
{
public:
    Car() { createShape(); }

    Car(Pose6DofQt& _pose) : Car() { setPose(_pose); }

    Car(Eigen::Vector3f translation, Eigen::Vector3f rotation)
        : Car()
    {
        pose.translate(translation);
        pose.rotate(rotation);
    }


    void setPose(Pose6DofQt& _pose) { pose = _pose; }
    Pose6DofQt getPose() { return pose; }

    void draw()
    {
        QVector3D normal;
        std::vector<QVector3D> colors;
        colors.emplace_back(1,1,1);
        colors.emplace_back(1,0,0);
        colors.emplace_back(0,1,0);
        colors.emplace_back(0,0,1);

        std::vector<QVector3D> triplet(3);

        for(size_t ti=0; ti<vertices.size()/3; ti++)
        {
            normal = QVector3D::crossProduct(vertices[ti*3 + 2] - vertices[ti*3 + 0],
                                             vertices[ti*3 + 1] - vertices[ti*3 + 0]);
            for(int k=0; k<3; k++)
                triplet[k] = pose * vertices[ti*3 + k];

            gvm::AddVertex(eVertexType::triangle, triplet[0], colors[ti], normal, 1);
            gvm::AddVertex(eVertexType::triangle, triplet[1], colors[ti], normal, 1);
            gvm::AddVertex(eVertexType::triangle, triplet[2], colors[ti], normal, 1, true);

            gvm::AddVertex(eVertexType::triangle, triplet[2], colors[ti], normal, 1);
            gvm::AddVertex(eVertexType::triangle, triplet[1], colors[ti], normal, 1);
            gvm::AddVertex(eVertexType::triangle, triplet[0], colors[ti], normal, 1, true);

            qDebug() << "draw" << triplet[0] << triplet[1] << triplet[2];
        }
    }

private:
    void createShape()
    {
        // four faces of tetrahedron -> 12 vertices of 4 triangles
        vertices.emplace_back(0.3f, 0, 0);
        vertices.emplace_back(0, 0.1f, 0);
        vertices.emplace_back(0, -0.1f, 0);

        vertices.emplace_back(0.3f, 0, 0);
        vertices.emplace_back(0, 0.1f, 0);
        vertices.emplace_back(0, 0, 0.2f);

        vertices.emplace_back(0.3f, 0, 0);
        vertices.emplace_back(0, -0.1f, 0);
        vertices.emplace_back(0, 0, 0.2f);

        vertices.emplace_back(0, 0.1f, 0);
        vertices.emplace_back(0, -0.1f, 0);
        vertices.emplace_back(0, 0, 0.2f);
    }

    std::vector<QVector3D> vertices;
    Pose6DofQt pose;
};

#endif // CAR_H
