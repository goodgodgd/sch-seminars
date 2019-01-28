#include "slam3dconstructor.h"

// ---------- for pose vertex ----------
void Slam3DConstructor::addPoseVertex(g2o::SE3Quat *pose, bool set_fixed)
{
    if(pose)
        std::cout << "[addPoseVertex] t=" << pose->translation().transpose()
              << ", r=" << pose->rotation().coeffs().transpose() << std::endl;
    else
        assert(!set_fixed);

    g2o::VertexSE3* v_se3 = new g2o::VertexSE3;
    v_se3->setId(getNewID());
    if(pose)
        v_se3->setEstimate(*pose);
    v_se3->setFixed(set_fixed);
    optimizer->addVertex(v_se3);
}

g2o::SE3Quat Slam3DConstructor::addNoisePoseMeasurement(const g2o::SE3Quat& srcpose)
{
    std::cout << "[addNoisePoseMeasurement] before pose: t="
              << srcpose.translation().transpose()
              << ", r=" << srcpose.rotation().coeffs().transpose() << std::endl;
    Eigen::Vector3d randv3 = Eigen::Vector3d::Random() - Eigen::Vector3d::Constant(0.5);
    Eigen::Vector3d tran_w_noise = srcpose.translation()
            + config.tran_noise.cwiseProduct(randv3);

    Eigen::Vector4d randv4 = Eigen::Vector4d::Random() - Eigen::Vector4d::Constant(0.5);
    Eigen::Vector4d rota_w_noise = srcpose.rotation().coeffs()
            + config.quat_noise.cwiseProduct(randv4);
    Eigen::Quaterniond quat_w_noise(rota_w_noise);

    g2o::SE3Quat pose_w_noise(quat_w_noise, tran_w_noise);
    pose_w_noise.normalizeRotation();
    std::cout << "      after: t="  << pose_w_noise.translation().transpose()
              << ", r=" << pose_w_noise.rotation().coeffs().transpose() << std::endl;
    return pose_w_noise;
}

void Slam3DConstructor::addEdgePosePose(int id0, int id1, const g2o::SE3Quat &relpose)
{
    std::cout << "[addEdgePosePose] id0=" << id0 << ", id1=" << id1
              << ", t=" << relpose.translation().transpose()
              << ", r=" << relpose.rotation().coeffs().transpose() << std::endl;

    g2o::EdgeSE3* edge = new g2o::EdgeSE3;
    edge->setVertex(0, optimizer->vertices().find(id0)->second);
    edge->setVertex(1, optimizer->vertices().find(id1)->second);
    edge->setMeasurement(relpose);
    Eigen::MatrixXd info_matrix = Eigen::MatrixXd::Identity(6,6);
    for(int i=0; i<3; i++)
        info_matrix(i, i) = 1. / config.tran_noise(i);
    for(int i=0; i<3; i++)
        info_matrix(3+i, 3+i) = 1. / config.quat_noise(i);
    edge->setInformation(info_matrix);
    optimizer->addEdge(edge);
}

// ---------- for point vertex ----------
void Slam3DConstructor::addPoint3DVertex(Eigen::Vector3d* pt, bool set_fixed)
{
    if(pt)
        std::cout << "[addPoint3DVertex]: " << *pt << std::endl;
    else
        assert(!set_fixed);

    g2o::VertexPointXYZ* v_pt3d = new g2o::VertexPointXYZ;
    v_pt3d->setId(getNewID());
    v_pt3d->setFixed(set_fixed);
    if(pt)
        v_pt3d->setEstimate(*pt);
    optimizer->addVertex(v_pt3d);
}

Eigen::Vector3d Slam3DConstructor::addNoisePointMeasurement(const Eigen::Vector3d& srcpt)
{
    Eigen::Vector3d randv3 = Eigen::Vector3d::Random() - Eigen::Vector3d::Constant(0.5);
    Eigen::Vector3d pt_w_noise = srcpt + config.point_noise.cwiseProduct(randv3);
    std::cout << "[addNoisePointMeasurement] before: " << srcpt
              << ", after: " << pt_w_noise << std::endl;
    return pt_w_noise;
}

void Slam3DConstructor::addEdgePosePoint(int poseid, int ptid, const Eigen::Vector3d& relpt)
{
    std::cout << "[addEdgePosePoint] poseid=" << poseid << ", ptid=" << ptid
              << ", relpt=" << relpt << std::endl;

    g2o::EdgeSE3PointXYZ* edge = new g2o::EdgeSE3PointXYZ;
    edge->setVertex(0, optimizer->vertices().find(poseid)->second);
    edge->setVertex(1, optimizer->vertices().find(ptid)->second);
    edge->setMeasurement(relpt);
    Eigen::MatrixXd info_matrix = Eigen::MatrixXd::Identity(3,3);
    for(int i=0; i<3; i++)
        info_matrix(i, i) = 1. / config.point_noise(i);
    edge->setInformation(info_matrix);
    optimizer->addEdge(edge);
}
