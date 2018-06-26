# QT error resolution

### - Error code
```
qt.qpa.plugin: Could not find the Qt platform plugin "xcb" in ""
```

----------------------------------
### Solution 1. QT Creator 재설치
```
mkdir ~/mylib; cd ~/mylib
wget https://download.qt.io/archive/qt/5.11/5.11.0/qt-opensource-linux-x64-5.11.0.run
chmod a+x qt-opensource-linux-x64-5.11.0.run
./qt-opensource-linux-x64-5.11.0.run
```
**- 설치할 때 Installation Folder 경로를 /home/{user name}/mylib/Qt5.11.0 으로 해줘야함**
- 설치할 때 Qt 5.x 아래 Desktop gcc 64-bit 를 체크해야함 (Prebuilt Qt library)
- qtcreator 실행하면 플러그인 오류 메시지 나타남
- qtcreator - Help - About Plugins - QtQuick - QmlDesigner, QmlProfiler 두 개 체크해제

### Solution 2. 경로 설정
```
sudo gedit ~/.bashrc
```
bashrc 파일을 열고 제일 아래에
```
export QT_PLUGIN_PATH=<user qt path>/plugins
```
라고 써준뒤 저장하고 QT를 종료 및 재실행
