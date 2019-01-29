#include "posepoint3dconstructor.h"

PosePoint3DConstructor::PosePoint3DConstructor()
    : SE3LoopConstructor()
{
}

void PosePoint3DConstructor::construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config)
{
    optimizer = _optimizer;
    config = _config;
    setParameter();

    // add pose vertices at (0,0,0) and (1,0,0)
    createInitPoseVerts();
    // add pose vertices around circle
    createCirclePoseVerts();
    // add edges between pose edges
    setEdgesBtwPoses();

    // add point vertices above the poses
    createPointVerts();
    // add edges between poses and points
    setEdgesBtwPosePoint();
}

void PosePoint3DConstructor::createPointVerts()
{
    Eigen::Vector3d pt;
    srand(100);

    // generate random point within (0, 0, 5) ~ (4, 4, 10)
    for(int y=0; y<5; y++)
        for(int x=0; x<5; x++)
        {
            pt(0) = double(x);
            pt(1) = double(y);
            pt(2) = rand() % 5 + 5;
            addPoint3DVertex(&pt);
            gt_points.push_back(pt);
        }
}

void PosePoint3DConstructor::setEdgesBtwPosePoint()
{
    const int firstPointIndex = int(gt_poses.size());
    Eigen::Vector3d local_pt;

    for(size_t i=0; i<gt_poses.size(); i++)
    {
        for(size_t k=0; k<gt_points.size(); k++)
        {
            local_pt = gt_poses[i].inverse().map(gt_points[k]);
            print_se3(gt_poses[i], "[setEdgesBtwPosePoint] pose ");
            print_vec3(gt_points[k], "    global:", false);
            print_vec3(gt_points[k], "    local:", true);
            // connect edge only if local x y coordinates are < 2
            if(fabs(local_pt.x()) < 2.1 && fabs(local_pt.y()) < 2.1)
            {
                if(config.edge_noise)
                    local_pt = addNoisePointMeasurement(local_pt);
                addEdgePosePoint(i, firstPointIndex + k, local_pt);
            }
        }
    }
}
