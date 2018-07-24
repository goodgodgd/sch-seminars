## Build Tensorflow with CUDA 9.2 on Ubuntu 18.04

### Step 1. Install Nvidia driver

First, install basic tools
```
sudo apt update; sudo apt upgrade
sudo apt install build-essentials
sudo apt install git cmake cmake-qt-gui unzip curl
```

then install nvidia driver (currently 396)
```
sudo apt purge nvidia*
sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt update
sudo ubuntu-drivers autoinstall
```

reboot!

### Step 2. Install CUDA

1. Download CUDA from [here](https://developer.nvidia.com/cuda-downloads)
1. While downloading, install prequisites. Since the CUDA binary (downloading) was built with gcc 6.0, we have to use gcc 6.0 to build somthing with CUDA, even though Ubuntu 18.04 provides gcc 7.x.

    ```
    sudo apt install gcc-6 g++-6
    sudo apt-get install linux-headers-$(uname -r)
    ```

1. When downloading is done, install CUDA
    ```
    cd Downloads
    chmod a+x cuda_9.2.88_396.26_linux.run
    sudo ./cuda_9.2.88_396.26_linux.run --override
    
    Do you accept the previously read EULA?
    accept/decline/quit: accept
    
    You are attempting to install on an unsupported configuration. Do you wish to continue?
    (y)es/(n)o [ default is no ]: y
    
    Install NVIDIA Accelerated Graphics Driver for Linux-x86_64 396.26?
    (y)es/(n)o/(q)uit: n
    
    Install the CUDA 9.2 Toolkit?
    (y)es/(n)o/(q)uit: y
    
    Enter Toolkit Location
     [ default is /usr/local/cuda-9.2 ]: 
    
    Do you want to install a symbolic link at /usr/local/cuda?
    (y)es/(n)o/(q)uit: y
    
    Install the CUDA 9.2 Samples?
    (y)es/(n)o/(q)uit: n
    
    ...
    
    Please make sure that
     -   PATH includes /usr/local/cuda-9.2/bin
     -   LD_LIBRARY_PATH includes /usr/local/cuda-9.2/lib64, or, add /usr/local/cuda-9.2/lib64 to /etc/ld.so.conf and run ldconfig as root
    ```

1. Set gcc links and add CUDA paths to system variables.
    ```
    sudo ln -s /usr/bin/gcc-6 /usr/local/cuda-9.2/bin/gcc
    sudo ln -s /usr/bin/g++-6 /usr/local/cuda-9.2/bin/g++
    sudo apt install libcupti-dev
    
    gedit ~/.bashrc
    # insert following lines
    export PATH=/usr/local/cuda-9.2/bin${PATH:+:${PATH}}
    export LD_LIBRARY_PATH=/usr/local/cuda-9.2/lib64:/usr/local/cuda/extras/CUPTI/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
    # save and exit
    
    source ~/.bashrc
    sudo ldconfig
    ```

### Step 3. Install cuDNN

1. Method1: Use deb installer

    If you install cuDNN by installers, the library files will be located under /usr/lib/somewhere
    If you don't like it, go to Method2

    Go [there](https://developer.nvidia.com/cudnn), login and download followings.
    ```
    cuDNN <version> Runtime Library for Ubuntu16.04 (Deb)
    cuDNN <version> Developer Library for Ubuntu16.04 (Deb)
    cuDNN <version> Code Samples and User Guide for Ubuntu16.04 (Deb)
    ```
    then install them
    ```
    sudo dpkg -i libcudnn7_<version>-1+cuda9.2_amd64.deb
    sudo dpkg -i libcudnn7-dev_<version>-1+cuda9.2_amd64.deb
    sudo dpkg -i libcudnn7-doc_<version>-1+cuda9.2_amd64.deb
    ```
    Verify cuDNN installation
    ```
    cp -r /usr/src/cudnn_samples_v7/ $HOME
    cd  $HOME/cudnn_samples_v7/mnistCUDNN
    make clean && make
    ./mnistCUDNN
    
    cudnnGetVersion() : 7104 , CUDNN_VERSION from cudnn.h : 7104 (7.1.4)
    Host compiler version : GCC 6.4.0
    There are 1 CUDA capable devices on your machine :
    device 0 : sms 28  Capabilities 6.1, SmClock 1721.0 Mhz, MemSize (Mb) 11177, MemClock 5505.0 Mhz, Ecc=0, boardGroupID=0
    Using device 0
    ...
    Loading image data/five_28x28.pgm
    Performing forward propagation ...
    Resulting weights from Softmax:
    0.0000000 0.0000008 0.0000000 0.0000002 0.0000000 1.0000000 0.0000154 0.0000000 0.0000012 0.0000006 
    
    Result of classification: 1 3 5
    ```

2. Method2: Download and unzip
    
    Go [there](https://developer.nvidia.com/cudnn), login and download `cudnn-9.2-linux-x64-v7.1.tgz`  
    Unzip it and copy to /usr/local/cuda
    ```
    tar -xvzf cudnn-9.2-linux-x64-v7.1.tgz
    sudo cp -r cuda/include/* /usr/local/cuda/include
    sudo cp -r cuda/lib64/* /usr/local/cuda/lib64
    ```


### Step 4. Install NCCL (optional)

Go [there](https://developer.nvidia.com/nccl), submit a survey and download
`NCCL <version> O/S agnostic and CUDA 9.2`

```
tar -xvf nccl_<version>-1+cuda9.2_x86_64.txz
cd nccl_<version>-1+cuda9.2_x86_64
mkdir /usr/local/cuda/nccl
sudo cp -r * /usr/local/cuda/nccl
```

### Step 5. Install Bazel
```
sudo apt-get install openjdk-8-jdk
echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
curl https://bazel.build/bazel-release.pub.gpg | sudo apt-key add -
sudo apt update && sudo apt install bazel
sudo apt upgrade bazel
```

### Step 6. Build tensorflow

Install dependencies, and set default `python` as `python3` [refer here](https://skyoo2003.github.io/post/2017/03/17/what-is-alternatives-command)
```
sudo apt install python2.7-dev python-pip python-numpy python-wheel 
sudo apt install python3-dev python3-pip python3-numpy python3-wheel
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 100 --slave /usr/bin/pip pip /usr/bin/pip3
```

Download tensorflow and configure
```
git clone https://github.com/tensorflow/tensorflow
cd tensorflow
./configure 
```

Set numerous options refering the [references](https://medium.com/@asmello/how-to-install-tensorflow-cuda-9-1-into-ubuntu-18-04-b645e769f01d) and then
```
bazel build --config=opt //tensorflow/tools/pip_package:build_pip_package
```

If errors like [this](https://github.com/tensorflow/tensorflow/issues/13481) occur, try
```
sudo sh -c "echo '/usr/local/cuda-8.0/lib64' >> /etc/ld.so.conf.d/nvidia.conf"
sudo ldconfig
```

`bazel build` command builds sources in `bazel_bin` and create a script named `build_pip_package`  
Build python installer (.whl file) with `build_pip_package` and install it
```
bazel-bin/tensorflow/tools/pip_package/build_pip_package tensorflow_pkg
pip install tensorflow_pkg/tensorflow*.whl
```

### Step 7. Verify Tensorflow installation and clean build files

Run in terminal
```
python

import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')
sess = tf.Session()
print(sess.run(hello))
```

If no error occurs, tensorflow was successfully installed. 
Now remove build files which are about 8Gb in my case.
```
bazel clean
```
If you want to reinstall tensorflow with the same configuration, command again
```
pip install tensorflow_pkg/tensorflow*.whl
```


### References
1. [How to install Tensorflow + CUDA 9.1 into Ubuntu 18.04](https://medium.com/@asmello/how-to-install-tensorflow-cuda-9-1-into-ubuntu-18-04-b645e769f01d)
1. [How to Install Tensorflow GPU with CUDA 9.2 for Python on Ubuntu](https://hk.saowen.com/a/a9cc5b7c90a6f350850d8554c018f7415981fc8d470b481c90afd7573f5e12cd)
1. [텐서플로우 컴파일 설정 내용](https://hiseon.me/2018/03/17/tensorflow-build-config/)
1. To check compute capability of your GPU: https://developer.nvidia.com/cuda-gpus


