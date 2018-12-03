# Iterative Closest Point (ICP)

ICP는 두 개의 point cloud가 비슷한 모양을 가질 때 이 둘을 정합하는 (registration) 상대적인 pose (rotation + translation = rigid transformation)을 찾는 알고리즘이다. 단순히 거리 기반으로 point-to-point correspondence를 찾아 그 거리를 최소화하도록 최적화 하는 과정을 반복하기 때문에 이러한 이름이 붙었다.

## 1. Method

**문제 정의**
​	- Input argument: scene point cloud P (p_i, i=1,...,N_p), model point cloud M (m_k, k=1,...,N_m)
​	- Output: relative pose (rigid transformation)

ICP는 6단계의 알고리즘으로 쓸 수 있다.  
원문: https://docs.opencv.org/3.0-beta/modules/surface_matching/doc/surface_matching.html  
문서에 붙어 있는 reference paper도 봐두자. 논문을 보고 공부하는 것도 좋고 논문쓸 때 ICP에 대한 reference가 필요할 수 있다.

1. Sampling
	- scene point p_i 에 대해 model point cloud 와 점대점 매칭을 하기 때문에 점의 수에 비례해서 속도가 느려진다.
	- 점의 수가 많을 때는 다운샘플링을 해도 그다지 성능 저하가 일어나지 않는다. 
	- Naive한 Uniform Sampling을 해도 되지만 좀 더 스마트한 방법은 normal vector의 분포가 균일해지도록 맞추는 것이다.
	- 상대적인 3차원 pose를 구하기 위해서는 최소 3가지 방향으로 끌어당기는 힘이 필요하다.
	- 어느 한쪽의 인력이 과도하고 다른 방향의 인력이 약하다면 인력이 강한쪽만 정확히 정합이 되고 약한 쪽은 정합이 안된채로 끝나버린다.
	- 그래서 세 방향의 인력이 균일하도록 샘플링을 하면 효율적이고 효과적인 샘플링을 할 수 있다.
		- 효율적: 넓은 평면의 점들은 듬성듬성하게 뽑아도 된다.
		- 효과적: 모든 점을 활용할 때보다 더 정확한 정합을 찾을 수 있다.
		- 참고: https://ieeexplore.ieee.org/document/1240258
2. Correspondence Search
	- (I)CP 이름 그대로 가장 가까운 점들을 매칭한다.
	- 모든 scene point p_i 에 대해 공간상에서 가장 가까운 model point m_k 를 찾는다.
	- return [(i, k)]
	- 모든 scene point p_i 에 대해 correspondence를 찾는 것보다 서로 1:1 매칭만을 찾는 것이 더 효율적이다.
	- 입력 데이터가 unordered point cloud로 주어진 경우 3차원 공간에서 점 사이의 거리를 측정하여 correpondence를 구하지만 depth map 처럼 ordered point cloud로 주어진 경우에는 reprojection을 이용한 방법이 효율적이다.
	- depth map으로 주어지고 카메라의 이동이 크지 않은 경우 현재까지 측정한 상대 pose를 기반으로 
3. Weighting of Pairs
4. Rejection of Pairs
5. Error Metric
6. Minimization

