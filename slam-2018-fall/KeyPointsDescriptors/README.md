### Dark pgmr

- key points  
    http://darkpgmr.tistory.com/131  
- scale space  
    http://darkpgmr.tistory.com/137
- descriptors
    http://darkpgmr.tistory.com/116?category=460965

### Opencv doc

- everything about key points and descriptors (python example)  
    https://docs.opencv.org/3.1.0/db/d27/tutorial_py_table_of_contents_feature2d.html
- align image using ORB  
    https://www.learnopencv.com/image-alignment-feature-based-using-opencv-c-python/
- cv::Feature2D class inheritance diagram
    https://docs.opencv.org/3.4/d0/d13/classcv_1_1Feature2D.html
- cv::DescriptorMatcher inheritance diagram
    https://docs.opencv.org/3.1.0/db/d39/classcv_1_1DescriptorMatcher.html


### Features

1. SIFT (Scale Invariant Feature Transform)  
    - Lowe, D. G., “Distinctive Image Features from Scale-Invariant Keypoints”, IJCV 2004.
    - 한글 설명: https://m.blog.naver.com/laonple/220879911249  
    - Key point detection: Gaussian scale space 를 만들고 (u, v, scale)에서 
    LoG (Laplacian of Gaussian)의 극대점을 찾아 그곳을 key point 로 사용
    - Orientation: Gradient direction 에 대한 histogram 을 만들어 orientation 찾음 -> rotation invariant
    - Descriptor
        - 우선 key point 주변 영상을 key point orientation 에 따라 회전 시킨 후 (혹은 윈도우를 회전) 
        - 주변 16x16 픽셀 영역에서 gradient 계산
        - 16x16 픽셀 -> 4x4 픽셀의 4x4 영역으로 분할
        - 각 영역에서 gradient histogram 계산 (8 bins)
        - 16개의 histogram을 합쳐 128 차원 descriptor 완성

2. SURF (Speeded-Up Robust Features)
    - Bay, H., et al, "SURF: Speeded Up Robust Features", ECCV, 2006
    - 한글 설명: https://m.blog.naver.com/laonple/220913997108
    - SIFT의 경량화 버전
    - LoG를 box filter로 근사하여 계산 (integral image로 빠르게 계산 가능)
    - Key point detection: LoG의 Hessian을 반응값으로 삼아 (u, v, scale)에서 반응의 극대점 찾음
    - Orientation:  wavelet으로 주변의 x, y gradient 구해서 gx, gy 평면에 점을 뿌리고 회전하면서 60도 범위 안에 가장 많은 gradient가 가장 큰 각도 찾음
    - Descriptor: 
        - 우선 key point 주변 영상을 key point orientation 에 따라 회전 시킨 후 (혹은 윈도우를 회전) 
        - 주변 16x16 픽셀 영역에서 gradient 계산
        - 16x16 픽셀 -> 4x4 픽셀의 4x4 영역으로 분할
        - 각 영역에서 sum(dx), sum(dy), sum(|dx|), sum(|dy|) 계산
        - 16개의 4차원 벡터를 합쳐 64 차원 descriptor 완성

3. BRIEF
    - Calonder, Michael, et al. "Brief: Binary robust independent elementary features." ECCV, 2010.
    - 한글설명: http://www.navisphere.net/2356/brief-binary-robust-independent-elementary-features/
    - key point 는 다른 방법 (Hessian 등)으로 찾고 descriptor 를 만드는 방법만 제안
    - Descriptor:
        - 주변 영역에서 n개의 point pair (pa, pb) 를 정한다.
        - point pair 에서 I(pa) > I(pb) 면 1 아니면 0의 값을 descriptor 에 추가한다.
        - 결과적으로 n-dim binary string 이 나온다.
        - 고정된 point pair 패턴을 정하는 정하는 방법에 따라 성능이 좀 다른데 가우시안 노이즈로 패턴을 만들때 가장 잘 된다고 함.
        - 끝

### Matchers

- https://docs.opencv.org/3.1.0/dc/dc3/tutorial_py_matcher.html
1. Brute-Force matcher
2. FLANN: Fast Library for Approximate Nearest Neighbors
