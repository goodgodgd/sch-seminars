#ifndef Pose6Dof_H
#define Pose6Dof_H

#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "Modules/project_common.h"

inline QDebug operator <<(QDebug debug, const Eigen::Vector3f &vector)
{
    QString number;
    QDebugStateSaver saver(debug);
    debug.nospace() << "evec3(";
    for(int i=0; i<3; i++)
        debug.nospace() << number.sprintf("%.4f, ", vector(i)).toLocal8Bit().data();
    debug.nospace() << ")";
    return debug.space();
}


// refer to https://eigen.tuxfamily.org/dox/group__TutorialGeometry.html
class Pose6Dof
{
public:
    Eigen::Affine3f affPose;

    Pose6Dof() { affPose.setIdentity(); }

    Pose6Dof(const Pose6Dof& _pose) : affPose(_pose.affPose) {}

    Pose6Dof(const Eigen::Vector3f& translation, const Eigen::Vector3f& rotation)
        : Pose6Dof()
    {
        translate(translation);
        rotate(rotation);
    }

    void translate(const Eigen::Vector3f& trans)
    {
        Eigen::Translation3f translation(trans);
        affPose = affPose * translation;
    }

    void rotate(const float angle, const Eigen::Vector3f& axis)
    {
        Eigen::AngleAxisf anax(angle, axis);
        affPose = affPose * anax;
    }

    void rotate(const float rx, const float ry, const float rz)
    {
        Eigen::AngleAxisf anax;
        anax = Eigen::AngleAxisf(rx, Eigen::Vector3f::UnitX())
                * Eigen::AngleAxisf(ry, Eigen::Vector3f::UnitY())
                * Eigen::AngleAxisf(rz, Eigen::Vector3f::UnitZ());
        affPose = affPose * anax;
    }

    void rotate(const Eigen::Vector3f& rotAngles)
    {
        rotate(rotAngles(0), rotAngles(1), rotAngles(2));
    }


    // global^T_this * this^T_other = g^T_other
    // transform point in frame 'other' w.r.t 'this' to frame 'global'
    // usually used to get more 'global' pose
    Pose6Dof operator*(const Pose6Dof& other)
    {
        Pose6Dof dstPose;
        dstPose.affPose = this->affPose * other.affPose;
        return dstPose;
    }

    Eigen::Vector3f operator*(const Eigen::Vector3f& point)
    {
        return affPose * point;
    }

    // global^T_this.inverse * global^T_other = this^T_global * global^T_other = this^T_other
    // transform point in frame 'other' w.r.t 'global' to frame 'this'
    // usually used to get 'local' pose of 'other' w.r.t 'this'
    Pose6Dof operator/(const Pose6Dof& other)
    {
        Pose6Dof dstPose;
        dstPose.affPose = this->affPose.inverse() * other.affPose;
        return dstPose;
    }

    Eigen::Vector3f getPosition()
    {
        Eigen::Vector3f trans = affPose.translation();
        return trans;
    }

    Eigen::Matrix3f getRotation()
    {
        Eigen::Matrix3f rotat = affPose.rotation();
        return rotat;
    }

    void invert()
    {
        affPose = affPose.inverse();
    }

    Pose6Dof inverse()
    {
        Pose6Dof pose = *this;
        pose.invert();
        return pose;
    }
};

#endif // Pose6Dof_H
