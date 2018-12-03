import os
from git import Repo

if not os.path.isdir("../Open3D"):
    print("clone Open3D repo to get sample data...")
    Repo.clone_from("https://github.com/IntelVCL/Open3D.git", "../Open3D")

TEST_DATA_PATH = "../Open3D/examples/TestData"
