🚀 언리얼 프로젝트: 웨이브 기반 장애물 시스템

✨ 2. 주요 기능
2.1. 웨이브 관리 시스템 (C++ & 데이터 테이블)
AMapWaveManager (C++ Actor): 🌊 게임 내 웨이브 진행을 총괄 관리하는 핵심 액터입니다.

정적 장애물: 🧱 특정 위치에 고정되어 있는 장애물입니다.
움직이는 장애물: 🏃‍♀️ 지정된 속도로 이동하며 플레이어와 상호작용하는 장애물입니다.

2.2. 장애물 스폰 로직
`AMapWaveManager`의 `FindRandomSpawnLocation` 함수는 UBoxComponent 크기를 고려하여 스폰 위치를 찾습니다.
장애물이 서로 겹치지 않고 적절한 공간에 배치되도록 구현했습니다.
UProjectileMovementComponent: 🚀 장애물의 이동제어에 사용한 컨포넌트입니다.
장애물이 벽과 충돌 시 튕겨나도록 구현했습니다.
```cpp
MovementComp->bShouldBounce = true;
MovementComp->Bounciness = 1.0f;
```
`튕기는 로직`

2.3. 충돌 및 플레이어 밀어내기 로직 (블루프린트)

Launch Velocity 계산: 캐릭터의 위치에서 장애물(self)의 위치를 뺀 후 정규화하여 밀어낼 정확한 방향 벡터를 얻어 캐릭터를 밀어냈습니다.
Break Vector와 Make Vector 노드를 사용하여 Z축 값을 0.0으로 고정하여 위로 밀려나는 것을 방지합니다.
XYOverride 및 ZOverride 활성화: 밀어내는 속도를 강제로 적용합니다.

2.4. 장애물 Z축 움직임 제어
2.3과 동일하게 적용했습니다 (마스킹)

2.5. HUD (Heads-Up Display) 연동
WBP HUD (위젯 블루프린트):

Get Actor Of Class 노드를 사용하여 레벨에 스폰된 BP_MapWaveManager를 사용했고
Cast To BP_MapWaveManager를 통해 해당 인스턴스에 접근한 후, Start Wave 함수를 호출하여 웨이브를 시작하게 구현했습니다.
TextBlock 변수를 생성하고 Set Text 노드를 사용하여 📝 HUD 텍스트를 동적으로 변경할 수 있습니다.

2.6. 물약 효과 
파란 물약 : 💧 정화 효과
노란 물약 : ⏳ 슬로우 효과
보라 물약 : 👁️‍🗨️ 블라인드 효과
