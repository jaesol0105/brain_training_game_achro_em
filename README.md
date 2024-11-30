# brain_training_game_achro_em

Achro-EM Kit을 이용한 기억력 훈련 게임

## Features
* 사용자는 Achro-EM Kit을 통해 게임 중 하나를 선택하여 플레이한다.<br>
* 게임은 게임오버 될 때까지 계속되며, 스테이지를 클리어 할 때마다 스코어를 1 획득한다.<br>
* 게임 종료시 현재 스코어를 랭킹에 기록 할 수 있다.<br>
* 안드로이드 어플리케이션을 통해 게임 별 랭킹을 조회 할 수 있다.<br>

```
게임1. 숫자 순서대로 누르기 게임
게임2. 같은 카드 찾기 게임
게임3. 숫자 위치 기억하기 게임
```
## Development
> **개발 언어 및 개발 도구 :** C, java <br>
> Achro-EM, RaspberryPi, Android Studio, Apache Tomcat, MariaDB
>
> **개발 환경 :** Windows, Linux
>
> **참여 인원 :** 3명

## Devices
Push Switch : 게임 진행, 숫자 입력

Dot matrix & FND : 게임 스코어 출력

Text LCD : 안내 메세지 출력


## System Workflow
<img width="80%" src="https://user-images.githubusercontent.com/70834187/147273829-86119308-9a23-488c-9971-ac9896c65226.png"/>

## System Architecture Diagram
<img width="80%" src="https://user-images.githubusercontent.com/70834187/147273712-956b5c51-a505-4f90-8dc6-826c9d674d6d.png" />

## Developers
|이름||
|:---|:---|
|[김진원](https://github.com/wlsdnjs0707)|게임1 구현 및 fpga device 구현, 닉네임 입력 구현|
|[이재솔](https://github.com/jaesol0105)|게임2 구현 및 안드로이드 앱 제작, WAS 서버 / mariaDB 설계 및 구축|
|박석현|게임3 구현 및 PPT 작성, 발표|

