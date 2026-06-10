# BC_Ch3_Assignment_4

<br>

<p align="center">
<img src="https://github.com/devcol-main/BC_Ch3_Assignment_4/blob/9d98ea917fa3e76ebd70f5aab49d1a02df3339d9/Recorded/Complex.gif" width="100%">
</p>

---

<p align="center">
<img src="https://github.com/devcol-main/BC_Ch3_Assignment_4/blob/9d98ea917fa3e76ebd70f5aab49d1a02df3339d9/Recorded/Basic.gif" width="100%">
</p>

<br>

## Related Links

- Google Drive (Share as Commenter) : https://drive.google.com/drive/folders/1_oS7Ye7cIGjDeKh576aLIErvQf6auTvm?usp=sharing
- YouTube: https://youtu.be/r0LRk7ACkAY

<br>

## 1️⃣ 필수 기능 가이드

<aside>

### 필수 구현 기능

---

1. C++ Pawn 클래스 및 컴포넌트 구성
    - [x]  **Pawn 클래스 생성**: `Pawn`을 상속받는 새로운 C++ 클래스를 생성합니다.
    - [x]  **컴포넌트 추가**: 아래 컴포넌트들을 Pawn 클래스에 추가합니다.
        - `CapsuleComponent` (또는 Box/Sphere 중 택 1)
        - `SkeletalMeshComponent`
        - `SpringArmComponent`
        - `CameraComponent`
    - [x]  **계층 구조 설정**: 충돌 컴포넌트를 **RootComponent**로 설정하고, 나머지 컴포넌트들을 부착합니다.
    - [x]  **DefaultPawn 설정**: `GameMode` 클래스에서 `DefaultPawnClass`를 본인이 만든 Pawn으로 지정합니다.
    - [x]  **Physics 설정**: 루트 충돌체 및 Mesh의 `Simulate Physics`를 **false**로 설정합니다. (물리 대신 코드로 직접 제어)

---

1. Enhanced Input 매핑 & 바인딩
    - [x]  **입력 액션(IA) 생성**: 아래 두 가지 액션을 생성합니다. (타입: **Vector2D**)
        - `IA_Move` (WASD 이동용)
        - `IA_Look` (마우스 회전용)
    - [x]  **IMC 매핑**: 생성한 액션들을 `Input Mapping Context`에 등록하고 키를 할당합니다.
    - [x]  **액션 바인딩**: `SetupPlayerInputComponent()`에서 입력 처리 함수와 액션들을 바인딩합니다.

---

1. 이동 및 회전 로직 구현
    - [x]  **프레임 독립성**: `DeltaTime`을 사용하여 프레임 속도와 관계없이 일정한 속도로 움직이도록 구현합니다.
    - [x]  **이동 구현**: `AddActorLocalOffset()` 등을 활용해 WASD 입력에 따라 Pawn이 움직이도록 작성합니다.
        - 이동 방향은 Pawn의 Forward/Right 벡터를 기준으로 결정됩니다.
    - [x]  **회전 구현**: `AddActorLocalRotation()` 등을 활용해 마우스 입력에 따라 회전하도록 작성합니다.
        - 마우스 입력값으로 **Yaw와 Pitch를 직접 계산**하여 구현합니다.
        - ⚠️ **주의**: `AddControllerYawInput()`, `AddControllerPitchInput()` 등 엔진 기본 제공 함수는 사용하지 않습니다.
    - [x]  **제한 사항**: 평면 상에서의 이동과 회전만 처리하며, 중력이나 낙하 효과는 고려하지 않습니다.
</aside>

<br>

## 2️⃣ 도전 기능 가이드


<aside>
💡

- 한 단계 더, 점프업을 위해서 도전기능을 구현해보세요!
- 아래 항목을 개인 노션으로 복사해서, 
각 항목을 완료할 때마다 체크리스트를 표시하며 진행해보세요.
</aside>

<aside>

### 도전 구현 기능

---

1. 6자유도(6 DOF) 비행체 구현
    - [x]  **6축 액션 구현**: 아래 방향에 대한 이동 및 회전을 모두 구현합니다.
        - [x]  **이동**: 전/후(W, S), 좌/우(A, D), 상/하(Space, Shift)
        - [x]  **회전**: Yaw(마우스 X), Pitch(마우스 Y), Roll(마우스 휠 또는 별도 키)
    - [x]  **Local 기반 이동**: 단순 월드 좌표 이동이 아닌, Pawn의 현재 회전 상태(로컬 좌표계)를 기준으로 이동 방향이 결정되도록 구현합니다.

---

1. 중력 및 낙하 시스템 구현
    - [x]  **인공 중력**: `Tick()` 함수에서 매 프레임 중력 가속도를 직접 계산하여 적용합니다. (예: -980 cm/s²)
    - [x]  **지면 충돌 감지**: `LineTrace` 또는 `SweepTrace`를 사용하여 지면과의 충돌을 체크합니다.
    - [x]  **상태 초기화**: 지면에 착지하는 순간 Z축 낙하 속도를 0으로 초기화합니다.

---

1. 에어 컨트롤(Air Control) 구현
    - [x]  **이동 속도 제한**: 공중에 떠 있는 상태에서는 지상 이동 속도의 **30~50%** 수준으로 제한합니다.
    - [x]  **상태별 로직 분기**: 지상 상태와 공중 상태를 구분하여 각각 자연스러운 움직임이 나타나도록 구현합니다.

<br>

## 3️⃣ 평가 **기준**

### 과제 평가 기준

| 항목 | 점수 | 평가 내용 |
| --- | --- | --- |
| **완성도(필수)** | 50점 | - C++로 Pawn 클래스를 올바르게 생성했는가?
- 생선된 Pawn이 물리가 아닌 조작으로만 움직이는가?
- WASD와 마우스 입력에 따라 Pawn이 정상적으로 움직이고 회전하는가? |
| **이해도(필수)** | 30점 | - Pawn과 PlayerController의 관계를 정확히 이해했는가?
- Enhanced Input의 액션, 매핑 컨텍스트, 바인딩 관계를 이해했는가?
- 물리 계산이 아닌 직접 연산에 의한 이동, 회전을 이해했는가? |
| **우수성(도전)** | 20점 | - 6자유도 비행체로 구현하여 모든 축의 이동과 회전이 가능한가?
- 중력과 낙하 로직이 자연스럽게 구현되었는가?
- 지면 충돌 판정과 착지 처리가 올바르게 작동하는가? |

## Assets License terms: 
### [Creative Commons Attribution (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)

* [DROPSHIP R35](https://fab.com/s/ef49e1f1932b) by [GrafxBOX](https://www.fab.com/sellers/GrafxBOX)

---
