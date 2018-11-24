#ifndef POSE6DOFQT_H
#define POSE6DOFQT_H

#include "pose6dof.h"

class Pose6DofQt : public Pose6Dof
{
public:
    Pose6DofQt() :Pose6Dof() {}

    Pose6DofQt(const Pose6DofQt& _pose) :Pose6Dof() { affPose = _pose.affPose; }

    Pose6DofQt(const Eigen::Vector3f& translation, const Eigen::Vector3f& rotation)
        :Pose6Dof(translation, rotation) {}

    Pose6DofQt(const Eigen::Vector3f& translation, const Eigen::Matrix3f& rotation)
        :Pose6Dof(translation, rotation) {}

    Pose6DofQt(const QVector3D& translation, const QVector3D& rotation)
        :Pose6Dof(toEigen(translation), toEigen(rotation)) {}

    inline Eigen::Vector3f toEigen(const QVector3D& qvec) const
    {
        Eigen::Vector3f evec(qvec.x(), qvec.y(), qvec.z());
        return evec;
    }

    inline QVector3D toQVec(const Eigen::Vector3f& evec) const
    {
        QVector3D qvec(evec(0), evec(1), evec(2));
        return qvec;
    }


    void translateq(const QVector3D& trans)
    {
        Eigen::Translation3f translation(toEigen(trans));
        affPose = affPose * translation;
    }

    void rotateq(const float angle, const QVector3D& axis)
    {
        Eigen::AngleAxisf anax(angle, toEigen(axis));
        affPose = affPose * anax;
    }

    void rotateq(const QVector3D& rotAngles)
    {
        rotate(rotAngles.x(), rotAngles.y(), rotAngles.z());
    }

    QVector3D getPosition() const
    {
        Eigen::Vector3f trans = affPose.translation();
        return toQVec(trans);
    }

    QVector3D operator*(const QVector3D& point) const
    {
        Eigen::Vector3f epoint = affPose * toEigen(point);
        return toQVec(epoint);
    }

    Pose6DofQt operator*(const Pose6DofQt& other) const
    {
        Pose6DofQt dstPose;
        dstPose.affPose = this->affPose * other.affPose;
        return dstPose;
    }

};

inline QDebug operator <<(QDebug debug, const Pose6DofQt &pose)
{
    QString number;
    QDebugStateSaver saver(debug);
    QVector3D posit = pose.getPosition();
    Eigen::Matrix3f rotat = pose.getRotation();
    Eigen::Quaternionf quat(rotat);
    debug.nospace() << "evec3(";
    debug.nospace() << number.sprintf("%.4f, ", posit.x()).toLocal8Bit().data();
    debug.nospace() << number.sprintf("%.4f, ", posit.y()).toLocal8Bit().data();
    debug.nospace() << number.sprintf("%.4f, ", posit.z()).toLocal8Bit().data();
    debug.nospace() << number.sprintf("%.4f, ", quat.w()).toLocal8Bit().data();
    debug.nospace() << number.sprintf("%.4f, ", quat.x()).toLocal8Bit().data();
    debug.nospace() << number.sprintf("%.4f, ", quat.y()).toLocal8Bit().data();
    debug.nospace() << number.sprintf("%.4f, ", quat.z()).toLocal8Bit().data();
    debug.nospace() << ")";
    return debug.space();
}

#endif // POSE6DOFQT_H
