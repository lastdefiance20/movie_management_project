# 영화 관리 프로그램_20201809_정재윤

2020-2학기 프로그래밍및실습(가) 프로젝트

## 구현된 기능

1. 영화 목록 출력
2. 영화 추가
3. 영화 삭제
4. 영화 검색 (영화 이름, 장르, 배급사)
5. 영화 목록 불러오기 
6. 현재 영화 목록 저장 
7. 랜덤 영화 추천
8. 영화 시간별 정렬
9. 나가기

## 실행방법
git clone https://github.com/lastdefiance20/movie_management_project.git
cd movie_management_project
make
./main

## 오류사항
* 해결됨

```
-- visual studio 2019에서는 이름 띄어쓰기 작동 확인
-- putty에서는 이름 띄어쓰기 작동안됨

해결함
왜 작동을 안했는가 : 윈도우 기준 리눅스 기준 \n의 아스키코드가 다르다?
dat 파일을 윈도우에서 따로 만들면 리눅스에서는 작동안함.

구동 os가 달라서 생겼던 문제

만약 영화 검색 같은 부분이 제대로 작동하지 않는다면 rm movie.dat으로 저장된 파일을 지우고 해보자.
```
