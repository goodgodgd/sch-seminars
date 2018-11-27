## OpenCV 설치 가이드

### 1. 기본 준비

패키지 업데이트 및 업그레이드
```
sudo apt update && sudo apt upgrade
```

기존에 설치된게 있다면 삭제
```
sudo apt purge libopencv* python3-opencv
```

컴파일/빌드 관련 패키지
```
sudo apt install build-essential cmake cmake-qt-gui unzip pkg-config
```

### 2. qt 설치

Qt 설치
```
mkdir ~/mylib; cd ~/mylib
wget https://download.qt.io/archive/qt/5.11/5.11.0/qt-opensource-linux-x64-5.11.0.run
chmod a+x qt-opensource-linux-x64-5.11.0.run
./qt-opensource-linux-x64-5.11.0.run
```
- 설치할 때 Qt 5.x 아래 Desktop gcc 64-bit 를 체크해야함 (Prebuilt Qt library)
- qtcreator 실행시 플러그인 오류 메시지 나타날 수 있음
- qtcreator - Help - About Plugins - QtQuick - QmlDesigner, QmlProfiler 두 개 체크해제
- qtcreator에서 fcitx 한글 입력 안될때: http://wookiist.tistory.com/8

### 3. OpenCV dependency 설치

OpenGL for opencv, (필요한지 확실치 않음)
```
sudo apt install libgl1-mesa-dev mesa-utils libgtkglext1-dev
```

이미지
```
sudo apt install libjpeg-dev libtiff5-dev libpng-dev
```

비디오
```
sudo apt install libavcodec-dev libavformat-dev libswscale-dev libavresample-dev
sudo apt install libxine2-dev libv4l-dev v4l-utils
sudo apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
```

카메라
```
sudo apt install libdc1394-22-dev libgphoto2-dev libopenni2-dev
```

코덱
```
sudo apt install libxvidcore-dev libx264-dev x264
```

GUI
```
sudo apt install libgtk-3-dev
```

optimization
```
sudo apt install libatlas-base-dev libopenblas-dev libeigen3-dev gfortran libtbb2 libtbb-dev
```

python3
```
sudo apt install python3-dev python3-numpy
```

### 4. Download and unzip OpenCV

````
cd ~/mylib
wget -O opencv-3.4.1.zip https://github.com/opencv/opencv/archive/3.4.1.zip
unzip opencv-3.4.1.zip
wget -O opencv_contrib-3.4.1.zip https://github.com/opencv/opencv_contrib/archive/3.4.1.zip
unzip opencv_contrib-3.4.1.zip
````

빌드할 폴더와 설치할 폴더 생성 
````
mkdir ~/mylib/opencv-3.4.1/build
mkdir -p ~/mylib/deploy/opencv-3.4.1
````

### 5. OpenCV 옵션

CMakeCache.txt 를 보거나 cmake-gui에서 옵션을 볼 수 있음

빌드 타입과 설치할 경로를 먼저 설정 
````
CMAKE_BUILD_TYPE=Relase
CMAKE_INSTALL_PREFIX=~/mylib/deploy/opencv-3.4.1
````

WITH_xxx 와 BUILD_xxx 를 통해 방대한 opencv에서 필요한 기능만 선택하는것이 중요.
- 디폴트는 내가 원하는 것이 없고 모두 빌드하면 용량이 너무 커진다.

WITH에서는 OpenCV의 dependency 선택
````
# parallel computing
WITH_IPP=ON
BUILD_IPP_IW=ON
WITH_TBB=ON
WITH_OPENMP=ON
# video for linux
WITH_V4L=ON
WITH_LIBV4L=ON

WITH_OPENNI2=ON
WITH_OPENGL=ON
# qt가 있으면 GTK는 굳이 필요업을듯
WITH_GTK=OFF
WITH_VTK=OFF
WITH_OPENCL=OFF
WITH_MATLAB=OFF
WITH_CUDA=OFF
````

BUILD에서는 OpenCV에서 사용할 기능(모듈) 선택
````
BUILD_JAVA=OFF
BUILD_opencv_java_bindings_generator=OFF
# caffe나 tensorflow 모델을 읽어오는 기능이 있지만 안쓸것 같은..
BUILD_opencv_dnn=OFF
BUILD_opencv_ml=OFF
BUILD_opencv_stitching=OFF
BUILD_opencv_superres=OFF
# 테스트 코드
BUILD_opencv_ts=OFF
````

Qt 연결
````
WITH_QT=ON
QT_QMAKE_EXECUTABLE=~/mylib/Qt5.11.0/5.11.0/gcc_64/bin/qmake
Qt5Concurrent_DIR=~/mylib/Qt5.11.0/5.11.0/gcc_64/lib/cmake/Qt5Concurrent
Qt5Core_DIR=~/mylib/Qt5.11.0/5.11.0/gcc_64/lib/cmake/Qt5Core
Qt5Gui_DIR=~/mylib/Qt5.11.0/5.11.0/gcc_64/lib/cmake/Qt5Gui
Qt5OpenGL_DIR=~/mylib/Qt5.11.0/5.11.0/gcc_64/lib/cmake/Qt5OpenGL
Qt5Test_DIR=~/mylib/Qt5.11.0/5.11.0/gcc_64/lib/cmake/Qt5Test
Qt5Widgets_DIR=~/mylib/Qt5.11.0/5.11.0/gcc_64/lib/cmake/Qt5Widgets
````

외부에서 추가된 모듈인 contrib과 함께 빌드,
contrib을 추가하면 수십개의 추가 모듈이 들어오는데 그중에 필요한 것만 골라서 빌드한다.

````
# contrib 경로 설정
OPENCV_EXTRA_MODULES_PATH=~/mylib/opencv_contrib-3.4.1/modules
# 꼭 필요
BUILD_opencv_flann=ON
BUILD_opencv_xfeatures2d=ON
BUILD_opencv_ximgproc=ON
# 혹시 몰라서
BUILD_opencv_line_descriptor=ON
BUILD_opencv_calib3d=ON
BUILD_opencv_ccalib=ON
BUILD_opencv_optflow=ON
BUILD_opencv_rgbd=ON
BUILD_opencv_saliency=ON
BUILD_opencv_stereo=ON
BUILD_opencv_tracking=ON
# 안써
BUILD_opencv_aruco=OFF
BUILD_opencv_bgsegm=OFF
BUILD_opencv_bioinspired=OFF
BUILD_opencv_cvv=OFF
BUILD_opencv_datasets=OFF
BUILD_opencv_dnn=OFF
BUILD_opencv_dnn_objdetect=OFF
BUILD_opencv_dpm=OFF
BUILD_opencv_face=ON
BUILD_opencv_freetype=OFF
BUILD_opencv_fuzzy=OFF
BUILD_opencv_hfs=OFF
BUILD_opencv_img_hash=OFF
BUILD_opencv_phase_unwrapping=ON
BUILD_opencv_plot=OFF
BUILD_opencv_reg=OFF
BUILD_opencv_surface_matching=OFF
BUILD_opencv_structured_light=OFF
BUILD_opencv_text=OFF
BUILD_opencv_xobjdetect=OFF
BUILD_opencv_xphoto=OFF
````

터미널에서 아래와 같이 옵션을 써도 되고 cmake-gui에서 체크하고 configure 해도 된다.
````
$ cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python2=OFF \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-3.4.0/modules \
-D WITH_TBB=ON \
-D WITH_V4L=ON
````

### 6. 빌드 및 설치

````
cd ~/mylib/opencv-3.4.1/build
# 4개의 thread로 동시 빌드 - 속도 빨라짐
make -j4
# CMAKE_INSTALL_PREFIX 에서 설정한 경로로 opencv를 사용하는데 필요한 파일들만 설치해준다.
make install
# 빌드를 하면 중간 단계의 파일들이 많이 생겨서 용량을 많이 차지한다.
# 이미 필요한 파일들은 설치했으므로 빌드하면서 생긴 모든 파일을 삭제한다.
make clean
````

### 7. Python-opencv 설치

1. 가상환경 생성  
    python package 들을 시스템에 설치하기 보다는 가상환경을 만들어 설치하는 것이 좋다.  
    시스템에는 python pip numpy 정도만 설치한다.
    python3을 쓸때는 python3, pip3 라고 3만 붙여서 똑같이 설치하면 된다.  
    일단 pipenv를 설치하고 프로젝트를 수행할 위치에서 가상환경을 만든다.
    pipenv의 자세한 사용법은 구글링이나 아래 블로그를 참조
    
    [pipenv 사용법1](https://cjh5414.github.io/how-to-manage-python-project-with-pipenv/)  
    [pipenv 사용법2](https://graspthegist.com/post/python-pipenv/)
    
    ```bash
    pip install pipenv
    pipenv --python=python2
    ```
    하고나면 $HOME/.virtualenvs/envname 폴더가 생겼을 것이다.  
    envname은 "환경을 생성한 폴더명-hash"로 만들어진다. 예를 들면 `project-XACzLyqR` 와 같은  

2. cv2.so 복사
    위에서 `make install` 하면 `~/mylib/deploy/opencv-3.4.1` 경로에 라이브러리가 설치된다.
    ~/mylib/deploy/opencv-3.4.1/lib/python2.x 아래 보면 cv2.so가 있다.  
    ~/mylib/deploy/opencv-3.4.1/lib/python3.x 아래에는 cv2~~~.so 라는 좀 더 복잡한 이름의 라이브러리 파일이 있다.  
    
    이제 so 파일을 `$HOME/.virtualenvs/envname/lib/python2.7/site-packages` 폴더에 복사하면 설치 끝이다.

3. 설치 확인
    ```bash
    cd 프로젝트 폴더
    pipenv shell
    python
    import cv2
    print cv2.__version__
    ```
    에러가 안나면 설치가 잘 된것이고 에러가 나면 구글링이다^^;;


### (Optional) 네이버 D2 폰트 설치

코딩할 때나 gedit 쓸 때 한글을 쓸때가 많은데 영어랑 폰트가 다르면 어색해 보입니다.
특히 gedit에서 한글을 쓰면 칸이 늘어나죠;;
그래서 한글이 포함된 폰트를 쓰면 좋고 게다가 코딩용으로 커스터마이징 된 폰트면 더 좋겠죠.
찾아보니 네이버에서 만든게 있네요. 개인적으로 별로 예쁜것 같진 않지만 익숙해지면 되겠죠.

````
cd ~/mylib
wget -O D2Coding.zip https://github.com/naver/d2codingfont/raw/master/D2Coding-Ver1.3.2-20180524.zip
unzip D2Coding.zip -d D2Coding
sudo cp -r ./D2Coding/* /usr/share/fonts/truetype
sudo fc-cache -fv
````
