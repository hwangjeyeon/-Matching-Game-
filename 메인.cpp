#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

#define LEFT 75 // 키보드 왼쪽
#define RIGHT 77 // 키보드 오른쪽
#define UP 72 //키보드 위쪽
#define DOWN 80 // 키보드 아래쪽
#define SELECT 13 // 엔터


void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(){
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; 
	cursorInfo.bVisible = FALSE; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


int GameMap[4][4]; // 랜덤한 값을 맵 생성과 동시에 배치하는 맵
int FixedMap[4][4]; // 고정맵으로 값을 선택하면 이곳에 배열


class EventHandler {
private:
	int x; // 현재 x좌표값
	int y; // 현재 y좌표값
	float score; // 뒤집은 횟수
	int CheckRandomNumber[8] = { 0, }; // 1~8까지 랜덤한 숫자 값을 2개 초과해서 배치하지 않기 위해 체크용도로 설정해둔 배열
	int RandomNumber; // 게임 맵에 랜덤한 값을 배치하기 위한 배열 1~8까지 숫자 두개씩
	int LimitNumber[6] = { 0, }; // 선택된 다른 두 카드의 숫자를 비교고 그 위치값을 저장하기 위한 배열, 0에는 첫번째 카드의 숫자, 1과 2에는 각각 x값과 y값이 들어가며, 3~5도 똑같은 방식
	bool Limiter = false; // 리미트 넘버가 꽉차면 true로 바뀌면서 두 값 비교해주기 위한 용도
	bool Ending = false; // 진행 핸들러를 끝내고 엔딩을 실행시키기 위한 용도
public:
	EventHandler(int x, int y) { // 생성자
		this->x = x;
		this->y = y;
		score = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				GameMap[i][j] = 0;
				FixedMap[i][j] = 0;
			}
		}
		
		for (int i = 0; i < 4; i++) { // 랜덤 값 생성해서 게임맵 배열에 두쌍씩 1~8 숫자 설정
			for (int j = 0; j < 4; j++) {
				RandomNumber = rand() % 8 + 1;
				if (GameMap[i][j] == 0) {
					if (CheckRandomNumber[RandomNumber - 1] >= 2) {
						j--;
						continue;
					}
					GameMap[i][j] = RandomNumber;
					j--;
					CheckRandomNumber[RandomNumber - 1]+=1;
				}	
			}
		}
		DrawStartMenu(); // 시작 메뉴
		system("cls"); 
		DrawGameMap(); // 게임 맵 그리기
		DrawCursor(); // 현재 내가 있는 위치의 커서 그리기
	}

	void SetX(int x) {
		this->x = x;
	}
	
	void SetY(int y) {
		this->y = y;
	}

	int GetX() {
		return x;
	}

	int GetY() {
		return y;
	}

	bool GetEnding() {
		return Ending;
	}

	void SetScore(float s) {
		score += s;
	}

	float GetScore() {
		return score;
	}

	void DrawStartMenu() {
		cout << "\n\n\n\n";
		cout << "\t Card Number Match Game";
		Sleep(3000);

	}

	void DrawGameMap() {
		cout << "\n";
		for (int i = 0; i < 4; i++) {
			cout << "\t";
			for (int j = 0; j < 4; j++) {
				
				if (FixedMap[i][j] == 0) { // 고정 맵 값이 0이면 [?] 형태로 그려주기
					cout << "[?]   ";
				}
				else {
					cout << "[" << GameMap[i][j] << "]   "; // 고정 맵 값이 0이 아니면 [1]와 같이 게임 맵에 있는 숫자 표시
				}
			}
			
			cout << "\n\n";
			
		}

	}

	void DrawCursor() {
		gotoxy(x, y); // x값은 6칸씩 y값은 2씩 증가
		cout << "▷"; 
	}

	void DeleteCursor() {
		gotoxy(x, y);
		cout << "  "; // 이동하면 원래 있던 커서 지워주기 위한 용도
	}

	void LimitSelect() { // 선택값을 두개로 한정짓기 위한 메소드
		if (LimitNumber[0] == 0) { // 첫번째 값인지 아닌지 판단 용도
			LimitNumber[0] = GameMap[y / 2][x / 6 - 1]; // 첫번째 선택된 값 
			LimitNumber[1] = x/6-1; // 첫번째 선택된 x좌표
			LimitNumber[2] = y/2; // 첫번째 선택된 y좌표
		}
		else {
			LimitNumber[3] = GameMap[y / 2][x / 6 - 1]; // 두번째 선택된 값
			LimitNumber[4] = x/6-1; // 두번째 선택된 x좌표
			LimitNumber[5] = y/2; // 두번째 선택된 y좌표
			Limiter = true; // 선택을 두번했다고 알려주기 위한 용도
		}
	}

	void CheckLimit() {
		if (Limiter == true) { // 리미터가 true가 되면 두번 선택되었다는 것이므로 이제 선택된 두 값 비교
			if (LimitNumber[0] == LimitNumber[3]) { // 값을 비교해서 같은지 다른지 확인
				Limiter = false; // 초기화
				for (int i = 0; i < 6; i++) { //같으니까 해당 값 그대로 유지 -> 선택될때 고정 맵으로 자동으로 먼저 배열해주기때문
					LimitNumber[i] = 0; // 초기화
				}
			}
			else { // 다를때
				system("cls");
				DrawGameMap();
				DrawCursor();
				Sleep(1000);
				FixedMap[LimitNumber[2]][LimitNumber[1]] = 0; // 해당 고정맵 위치에 있는 값을 다시 0으로 초기화
				FixedMap[LimitNumber[5]][LimitNumber[4]] = 0; // 상동
				Limiter = false; // 초기화
				for (int i = 0; i < 6; i++) {
					LimitNumber[i] = 0; // 초기화
				}
			}
		}
	}

	void EndingChecker() { // 고정 맵에 있는 값들이 전부 0이 아니면 게임 종료 
		bool checker = false;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (FixedMap[i][j] == 0) {  
					checker = true;
					Ending = false;
					break;
				}
				else {
					Ending = true; 
				}
			}
			if (checker == true) {
				break;
			}
		}
	}
};


class PlayHandler {
private:
	EventHandler* ev = new EventHandler(6, 1); // 이벤트 핸들러 
public:
	int keyEvent() { // 키보드 입력 값 받는 용도
		int temp = _getwch();
		if (temp == 224) {
			switch (temp) {
			case 75:
				return LEFT;
				break;
			case 77:
				return RIGHT;
				break;
			case 72:
				return UP;
				break;
			case 80:
				return DOWN;
				break;
			};
		}
	}

	void Progress() { // 게임 진행을 위한 메소드
		while (true) { // 엔딩 조건이 될때까지 키보드 입력 무한으로 받음
			int n = keyEvent();
			switch (n) {
			case UP: // 키보드 위쪽
				if (ev->GetY() == 1) { // 한계선 넘어서 가려고 하면 값 무시
					continue;
				}else{
					ev->DeleteCursor();
					ev->SetY(ev->GetY() - 2);
					ev->DrawCursor();
					
				}
				break;
			case LEFT: // 키보드 왼쪽
				if (ev->GetX() == 6) { // 한계선 넘어서 가려고 하면 값 무시// 한계선
					continue;
				}
				else {
					ev->DeleteCursor();
					ev->SetX(ev->GetX() - 6);
					ev->DrawCursor();
					
				}
				break;
			case RIGHT: // 키보드 오른쪽 키
				if (ev->GetX() == 24) { // 한계선 넘어서 가려고 하면 값 무시
					continue;
				}
				else {
					ev->DeleteCursor();
					ev->SetX(ev->GetX() + 6);
					ev->DrawCursor();
					
				}
				break;
			case DOWN: // 키보드 아래 키
				if (ev->GetY() == 7) { // 한계선 넘어서 가려고 하면 값 무시
					continue;
				}
				else {
					ev->DeleteCursor();
					ev->SetY(ev->GetY() + 2);
					ev->DrawCursor();
					
				}
				break;
			case SELECT: // 키보드 엔터 키
				if (FixedMap[ev->GetY() / 2][ev->GetX() / 6 - 1] != 0) { // 내가 선택한 위치의 값이 0이 아니면 무시 -> 이미 선택되었거나 짝을 찾아서 끝난 경우
					continue;
				}
				ev->SetScore(0.5); // 한장 뒤집을 때 0.5 두개 뒤집으면 1
				FixedMap[ev->GetY()/2][ev->GetX()/6-1] = GameMap[ev->GetY()/2][ev->GetX() / 6-1]; // 선택과 동시에 일단 고정 맵에 넣어준다
				ev->LimitSelect(); 
				ev->CheckLimit();   
				system("cls");
				ev->DrawGameMap();
				ev->DrawCursor();
				
			}
			ev->EndingChecker(); // 진행 핸들러 루프 해제 조건
			if (ev->GetEnding()) {
				
				break;
			}
		}
	}
	
	void End() { // 엔딩
		system("cls"); // 게임 맵 제거 용
		cout << "\n\n\n\n";
		cout << "\t\tYOU WIN";  // 승리 문구
		cout << "\n\n\t   Number of Flips: " << ev->GetScore(); // 카드 뒤집은 횟수 표시 한번 뒤집으면 0.5점씩 올라오서 두개 선택할때 1번 뒤집은 것으로 됨
		Sleep(5000); // 화면 5초간 보여주는 용도
		delete ev;
		exit(0);
	}
};


int main() {
	system("mode con cols=40 lines=10 | title 카드 짝 맞추기 게임"); // 화면 크기 조절
	CursorView(); // 깜빡이는 커서 숨기기
	srand((unsigned)time(NULL)); // 랜덤성 부여 목적
	PlayHandler *ph = new PlayHandler(); // 게임 진행 핸들러
	ph->Progress(); // 키보드 입력 무한 루프 도는 진행 핸들러, 엔딩체커를 통해서 해당 루프 벗어남
	ph->End(); // 엔딩 창 띄어주는 용도
	delete ph; // 메모리 제거
}
