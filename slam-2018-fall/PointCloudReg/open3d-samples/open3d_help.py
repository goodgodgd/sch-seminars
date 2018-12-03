from prepare_testdata import TEST_DATA_PATH


def example_import_function():
    from open3d import read_point_cloud
    pcd = read_point_cloud(TEST_DATA_PATH + "/ICP/cloud_bin_0.pcd")
    print(pcd)


def example_help_function():
    import open3d
    help(open3d)
    help(open3d.PointCloud)
    help(open3d.read_point_cloud)


if __name__ == "__main__":
    example_import_function()
    example_help_function()
