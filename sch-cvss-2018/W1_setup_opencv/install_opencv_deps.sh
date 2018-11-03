#!/usr/bin/env bash

sudo apt update && sudo apt upgrade
sudo apt purge libopencv* python3-opencv
sudo apt install -y build-essential cmake cmake-qt-gui unzip pkg-config
sudo apt install -y libgl1-mesa-dev mesa-utils libgtkglext1-dev
sudo apt install -y libjpeg-dev libtiff5-dev libpng-dev
sudo apt install -y libavcodec-dev libavformat-dev libswscale-dev libavresample-dev
sudo apt install -y libxine2-dev libv4l-dev v4l-utils
sudo apt install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt install -y libdc1394-22-dev libgphoto2-dev libopenni2-dev
sudo apt install -y libxvidcore-dev libx264-dev x264
sudo apt install -y libgtk-3-dev
sudo apt install -y libatlas-base-dev libopenblas-dev libeigen3-dev gfortran libtbb2 libtbb-dev
sudo apt install -y python3-dev python3-numpy

