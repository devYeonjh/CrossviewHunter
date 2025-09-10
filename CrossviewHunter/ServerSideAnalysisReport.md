# CrossviewHunter 프로젝트 서버 측 코드 분석 보고서

## 개요

CrossviewHunter는 Epic Games의 Lyra 샘플을 기반으로 한 Unreal Engine 5.6 멀티플레이어 액션 게임입니다. 본 보고서는 프로젝트 내에서 서버 측 역할을 수행하는 클래스들과 네트워킹 관련 코드를 분석한 결과입니다.

## 주요 서버 측 아키텍처

### 1. 게임 모드 시스템 (Game Mode System)

#### ALyraGameMode
- **파일**: `Source/LyraGame/GameModes/LyraGameMode.h/cpp`
- **역할**: 서버에서 게임 규칙과 플레이어 관리를 담당하는 핵심 클래스
- **주요 기능**:
  - 플레이어 스폰 및 리스폰 관리 (`SpawnDefaultPawnAtTransform_Implementation`)
  - 게임 초기화 (`InitGame`, `InitGameState`)
  - 플레이어 시작 위치 선택 (`ChoosePlayerStart_Implementation`)
  - 전용 서버 로그인 처리 (`TryDedicatedServerLogin`)
  - 전용 서버 매치 호스팅 (`HostDedicatedServerMatch`)
  - Experience 시스템과의 통합

#### ALyraGameState  
- **파일**: `Source/LyraGame/GameModes/LyraGameState.h/cpp`
- **역할**: 게임 상태 정보를 관리하고 클라이언트에 복제
- **주요 기능**:
  - 서버 FPS 추적 및 복제 (`ServerFPS`)
  - 멀티캐스트 메시지 브로드캐스트 (`MulticastMessageToClients`, `MulticastReliableMessageToClients`)
  - 플레이어 상태 관리 (`AddPlayerState`, `RemovePlayerState`)
  - Ability System Component를 통한 게임 전체 이펙트 관리
  - 리플레이 시스템 지원

### 2. 플레이어 관리 시스템

#### ALyraPlayerController
- **파일**: `Source/LyraGame/Player/LyraPlayerController.h/cpp`  
- **역할**: 서버와 클라이언트 간 플레이어 입력 및 상태 관리
- **주요 서버 기능**:
  - 서버 치트 명령 실행 (`ServerCheat`, `ServerCheatAll`)
  - 팀 시스템 관리 (ILyraTeamAgentInterface 구현)
  - 리플레이 녹화 기능 (`TryToRecordClientReplay`)
  - Enhanced Input 시스템과의 통합

#### ALyraPlayerState
- **파일**: `Source/LyraGame/Player/LyraPlayerState.h/cpp`
- **역할**: 플레이어 상태 정보의 서버 측 관리 및 복제
- **주요 기능**:
  - 팀 및 스쿼드 ID 관리 (`SetSquadID`, `GetTeamId`)
  - 통계 태그 스택 관리 (`AddStatTagStack`, `RemoveStatTagStack`)
  - 클라이언트별 메시지 전송 (`ClientBroadcastMessage`)
  - 뷰 회전 복제 (`ReplicatedViewRotation`)
  - Ability System Component 통합
  - 연결 타입 관리 (Player, LiveSpectator, ReplaySpectator, InactivePlayer)

#### ALyraGameSession
- **파일**: `Source/LyraGame/System/LyraGameSession.h/cpp`
- **역할**: 게임 세션 관리 및 온라인 서비스와의 연동
- **주요 기능**:
  - 자동 로그인 처리 (`ProcessAutoLogin`)
  - 매치 시작/종료 이벤트 처리 (`HandleMatchHasStarted`, `HandleMatchHasEnded`)

### 3. 네트워킹 및 복제 시스템

#### ULyraReplicationGraph
- **파일**: `Source/LyraGame/System/LyraReplicationGraph.h/cpp`
- **역할**: 고성능 멀티플레이어를 위한 커스텀 복제 그래프 구현
- **주요 구성 요소**:
  - **GridNode**: 공간 분할 기반 복제 (`UReplicationGraphNode_GridSpatialization2D`)
  - **AlwaysRelevantNode**: 항상 복제되는 액터 관리
  - **PlayerStateFrequencyLimiter**: 대규모 플레이어 환경에서 PlayerState 복제 최적화
  - **AlwaysRelevant_ForConnection**: 연결별 항상 관련 액터 노드

#### 복제 노드 특화 클래스들
- **ULyraReplicationGraphNode_AlwaysRelevant_ForConnection**: 연결별 항상 관련 액터 처리
- **ULyraReplicationGraphNode_PlayerStateFrequencyLimiter**: 플레이어 상태 복제 빈도 제한

#### FLyraVerbMessageReplication
- **파일**: `Source/LyraGame/Messages/LyraVerbMessageReplication.h/cpp`
- **역할**: 게임플레이 메시지의 효율적인 복제
- **기능**:
  - FastArraySerializer 기반 최적화된 메시지 복제
  - 서버에서 클라이언트로 메시지 브로드캐스트 (`AddMessage`)
  - Delta 직렬화를 통한 네트워크 최적화

### 4. 장비 및 인벤토리 시스템

#### ULyraQuickBarComponent
- **파일**: `Source/LyraGame/Equipment/LyraQuickBarComponent.h/cpp`
- **역할**: 플레이어 퀵바 시스템의 서버 측 관리
- **서버 기능**:
  - 액티브 슬롯 인덱스 설정 (`SetActiveSlotIndex` - Server RPC)
  - 아이템 슬롯 관리 (`AddItemToSlot`, `RemoveItemFromSlot`)
  - 복제된 슬롯 배열 및 액티브 인덱스 관리

### 5. 온라인 서브시스템 통합

프로젝트는 다음 온라인 서비스들과 통합되어 있습니다:

#### Epic Online Services (EOS)
- OnlineSubsystemEOS 플러그인을 통한 Epic Games 서비스 연동
- 크로스플랫폼 매치메이킹 및 친구 시스템 지원

#### Steam 통합
- OnlineSubsystemSteam을 통한 Steam 서비스 연동
- Steam 친구, 업적, 클라우드 세이브 기능

#### CommonUser 서브시스템
- **파일**: `Plugins/CommonUser/Source/CommonUser/Public/CommonUserSubsystem.h`
- 플랫폼 독립적인 사용자 관리 시스템 제공

## 네트워크 복제 패턴

### 1. UFUNCTION 복제 패턴

#### Server RPCs
```cpp
// 서버에서 실행되는 함수들
UFUNCTION(Reliable, Server, WithValidation)
void ServerCheat(const FString& Msg);

UFUNCTION(Server, Reliable, BlueprintCallable)
void SetActiveSlotIndex(int32 NewIndex);
```

#### Client RPCs  
```cpp
// 클라이언트에서 실행되는 함수들
UFUNCTION(Client, Unreliable, BlueprintCallable)
void ClientBroadcastMessage(const FLyraVerbMessage Message);
```

#### Multicast RPCs
```cpp
// 모든 클라이언트에 브로드캐스트
UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
void MulticastMessageToClients(const FLyraVerbMessage Message);

UFUNCTION(NetMulticast, Reliable, BlueprintCallable)  
void MulticastReliableMessageToClients(const FLyraVerbMessage Message);
```

### 2. 복제되는 속성들

#### 게임 상태 복제
- `ServerFPS`: 서버 FPS 정보
- `RecorderPlayerState`: 리플레이 녹화 플레이어 상태

#### 플레이어 상태 복제
- `PawnData`: 폰 데이터 정의
- `MyPlayerConnectionType`: 플레이어 연결 타입
- `MyTeamID`, `MySquadID`: 팀/스쿼드 정보
- `StatTags`: 통계 태그 스택
- `ReplicatedViewRotation`: 뷰 회전 정보

#### 장비 시스템 복제
- `Slots`: 퀵바 슬롯 배열
- `ActiveSlotIndex`: 현재 활성 슬롯 인덱스

## 성능 최적화 기술

### 1. 복제 그래프 최적화
- **공간 분할**: GridNode를 통한 거리 기반 복제 관련성
- **빈도 제한**: PlayerState 복제 빈도 조절로 대역폭 최적화
- **Always Relevant**: 중요한 액터들의 항상 복제 보장

### 2. Fast Array Serialization
- `FLyraVerbMessageReplication`에서 Delta 직렬화 사용
- 변경된 부분만 네트워크로 전송하여 대역폭 절약

### 3. 조건부 복제
- `ReplicatedUsing` 함수를 통한 변경 시점 감지 및 처리
- 필요한 경우에만 클라이언트 업데이트 수행

## 보안 고려사항

### 1. 서버 권한 검증
- `WithValidation`을 통한 RPC 입력 검증
- `BlueprintAuthorityOnly`를 통한 서버 전용 함수 제한

### 2. 치트 방지
- 중요한 게임플레이 로직은 서버에서만 실행
- 클라이언트 입력에 대한 서버 측 검증 구현

### 3. 접근 제어
- 권한이 필요한 기능들에 대한 적절한 접근 제어
- 서버 전용 데이터의 클라이언트 노출 방지

## 결론

CrossviewHunter 프로젝트는 Epic Games의 Lyra 프레임워크를 기반으로 한 견고한 서버 측 아키텍처를 구축하고 있습니다. 주요 강점은 다음과 같습니다:

### 강점
1. **모듈화된 아키텍처**: ModularGameplayActors를 기반으로 한 확장 가능한 구조
2. **고성능 네트워킹**: 커스텀 복제 그래프를 통한 대역폭 최적화
3. **크로스플랫폼 지원**: EOS, Steam 등 다양한 온라인 서비스 통합
4. **확장성**: Experience 시스템을 통한 동적 콘텐츠 로딩
5. **보안성**: 서버 권한 검증 및 치트 방지 메커니즘

### 개선 권장사항
1. **로드 밸런싱**: 대규모 동시 접속 사용자를 위한 서버 클러스터링 고려
2. **모니터링**: 서버 성능 및 네트워크 상태 실시간 모니터링 시스템 구축
3. **데이터 보호**: 플레이어 데이터 암호화 및 백업 시스템 강화
4. **스케일링**: 자동 스케일링 메커니즘 도입 검토

이러한 서버 측 인프라는 안정적이고 확장 가능한 멀티플레이어 게임 서비스를 제공할 수 있는 견고한 기반을 제공합니다.