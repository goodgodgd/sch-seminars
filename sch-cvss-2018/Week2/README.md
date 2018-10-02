### 객체지향의 개념

비슷한 내용이지만 약간씩 다른 설명이 있습니다. 그런 부분은 경험하면서 해석해보세요.
- http://tenlie10.tistory.com/1
- http://88240.tistory.com/228
- http://cjmyun.tripod.com/Knowledgebase/ConceptsOfObjectOriented.htm

아래 블로그는 좀 심화된 내용인데 지금은 그런가보다 하고 나중에 한번씩 다시 보세요.
- http://vandbt.tistory.com/10
- http://vandbt.tistory.com/39

### 리눅스 한글 세팅
우분투 18.04에서는 기본 입력기로 ibus를 지원한다.  
영어만 쓰면 별 문제는 없겠으나 한글을 쓰기 시작하면 뭔가 잘 안될 때가 많다.  
그래서 우분투 16.04에서 쓰던 fcitx (파이틱스)를 쓴다.
1. 일단 설치  
    `sudo apt install fcitx-hangul`
2. Settings -> Region & Language -> Manage Installed Languages ->  
한글 있나 확인. 없음 추가 -> Input method 'fcitx'로 변경
3. ibus 삭제 후 재시작
    ```
    sudo apt remove ibus ibus-hangul
    sudo apt autoremove
    ```
4. Settings -> Devices -> Keyboard -> Typing -> Switch to next input source 클릭  
    -> backspace (<-) 키 눌러 기존 키 없앰 
5. 오른쪽 위 입력기 버튼 -> configure ->  Input method ->  
    (+) 누르고 Hangul 추가 (없으면 sudo apt upgrade 후 재시작)
6. Global config -> Trigger Input Method 에서 한글키 설정
7. Qt Creator에서 한글 입력이 안될 때  
    http://wookiist.tistory.com/8
    