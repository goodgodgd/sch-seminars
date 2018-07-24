## ROS Install

### Remote PC setup

Turtlebot3 e-manual 에 있는
[PC Setup](http://emanual.robotis.com/docs/en/platform/turtlebot3/pc_setup/#install-ubuntu-on-remote-pc)
대로 따라하면 됨.  

#### 변경점

1. 매뉴얼에 있는 그대로 따라하면 약 2GB에 달하는 수백개의 패키지를 설치하게 됨 (opencv 등 포함)
1. 파이썬처럼 가상환경에 따로 설치할 수가 없으므로 아예 우분투 자체를 virtualbox에 새로 설치하는 것을 권함
1. 네트워크에 직접 접속하기 위해 virtualbox의 네트워크 설정을 bridged adapter로 설정

### Raspberry Pi 설정

Turtlebot3 e-manual 에 있는 Raspberry Pi 3 setup이 두가지가 있는데  
하나는 ros가 설치된 raspbian 을 설치하는 것이고  
하나는 기본 ubuntu MATE에 ros를 설치하는 방법이다.

아무거나 상관 없으나 MATE보다는 raspbian이 더 가벼운 것 같고  
ros 설치도 미리 되어 있으니 ROBOTIS에서 제공하는 raspbian을 설치한다.  
[ROBOTIS raspbian](http://emanual.robotis.com/docs/en/platform/turtlebot3/raspberry_pi_3_setup/#install-linux-based-on-raspbian)
설치

#### 변경점
1. 그냥 설치하면 메인 드라이브의 용량이 7.xGB 만 할당되어 있다.
1. `sudo apt install gparted` 를 설치하여 SD카드 용량을 모두 활용하게 해주자.
1. ext4 root 파티션으로 꽉 채우지 말고 2GB 정도 남기고 늘린다.
1. 나머지 2GB 정도는 swap partition 으로 사용한다. 램이 1GB로 매우 부족하다. 
