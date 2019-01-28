#include "slam3dconstructor.h"

void Slam3DConstructor::addPoseVertex(Eigen::Quaterniond quat,
                                       Eigen::Vector3d tran, bool set_fixed)
{
    g2o::SE3Quat pose(quat, tran);
    addPoseVertex(pose, set_fixed);
}

void Slam3DConstructor::addPoseVertex(g2o::SE3Quat& pose, bool set_fixed)
{
    std::cout << "add pose: t=" << pose.translation().transpose()
              << " r=" << pose.rotation().coeffs().transpose() << std::endl;
    g2o::VertexSE3* v_se3 = new g2o::VertexSE3;
    v_se3->setId(getNewID());
    if(set_fixed || config.init_vtx)
        v_se3->setEstimate(pose);
    v_se3->setFixed(set_fixed);
    optimizer->addVertex(v_se3);
    gt_poses.push_back(pose);
}

g2o::SE3Quat Slam3DConstructor::addNoisePoseMeasurement(const g2o::SE3Quat& srcpose)
{
    std::cout << "[addNoise] before pose: " << srcpose.translation()
              << " " << srcpose.rotation().coeffs();
    Eigen::Vector3d randv3 = Eigen::Vector3d::Random() - Eigen::Vector3d::Constant(0.5);
    Eigen::Vector3d tran_w_noise = srcpose.translation()
            + config.tran_noise.cwiseProduct(randv3);

    Eigen::Vector4d randv4 = Eigen::Vector4d::Random() - Eigen::Vector4d::Constant(0.5);
    Eigen::Vector4d rota_w_noise = srcpose.rotation().coeffs()
            + config.quat_noise.cwiseProduct(randv4);
    Eigen::Quaterniond quat_w_noise(rota_w_noise);

    g2o::SE3Quat pose_w_noise(quat_w_noise, tran_w_noise);
    pose_w_noise.normalizeRotation();
    std::cout << "[addNoise] after pose: " << pose_w_noise.translation()
              << " " << pose_w_noise.rotation().coeffs();
    return pose_w_noise;
}

void Slam3DConstructor::addEdgePosePose(int id0, int id1, g2o::SE3Quat& relpose)
{
    std::cout << "add edge: id0=" << id0 << ", id1" << id1
              << ", t=" << relpose.translation().transpose()
              << ", r=" << relpose.rotation().coeffs().transpose() << std::endl;

    g2o::EdgeSE3* edge = new g2o::EdgeSE3;
    edge->setVertex(0, optimizer->vertices().find(id0)->second);
    edge->setVertex(1, optimizer->vertices().find(id1)->second);
    edge->setMeasurement(relpose);
    Eigen::MatrixXd info_matrix = Eigen::MatrixXd::Identity(6,6) * 10.;
    edge->setInformation(info_matrix);
    optimizer->addEdge(edge);
}
