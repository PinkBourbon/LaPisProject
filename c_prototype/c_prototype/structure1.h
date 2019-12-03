#pragma once

#pragma once

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

//C++과 다른진 모르겠지만 헤더에 구현까지 들어있어서 아쉽다.
//매우 짧은 코드이기때문에 큰 문제는 없지만 조금더 파일을 분리하는 편이 확장성이 좋지 않을까?
//헤더 파일 내부에서 헤더를 막 include하게되면 나중에 꼬일 가능성이 있을수 있다.

struct character
{
	int iHP;
	int iAttack;
	int iBlock;
	int iHeal;
};

character PC = { 30, 10, 5, 10 };
character Enemy = { 0, 0, 0, 0 };

void Phase0();
void phase1();
void phase2();
void phase3(); //각 페이즈 별 설명이 없어서 어떤걸 의미하는지 바로 파악이 어려워 내가했다면 조금 길더라도 PhaseBattle 같은 식으로 지었을 듯.


void(*phase)();

void CreativeEnemy(int difficulty, character PC, character* Enemy)
{
	//PC는 포인터를 사용하지 않고 Enemy는 사용한 이유는 무엇일까?
	int BasePercent;

	switch (difficulty)//difficulty를 int가 아닌 enum형식으로 썻으면 실수가 적지 않을까?
	{
	case 1:
		BasePercent = 70;
		break;
	case 2:
		BasePercent = 80;
		break;
	case 3:

		BasePercent = 90;
		break;
	}

	//처음 배이스 퍼센트에 쓰래기값이 들어가 있기떄문에 예상 외의 값이 들어올 경우에 문제 생길 가능성이 있음.


	//각 플레이어 기반 기본 퍼센테이지 + 0~41퍼센트로 적의 스테이터스 결정.
	Enemy->iHP = (int)(PC.iHP * ((BasePercent + (rand() % 41)) / 100.0));
	Enemy->iAttack = (int)(PC.iAttack * ((BasePercent + (rand() % 41)) / 100.0));
	Enemy->iBlock = (int)(PC.iBlock * ((BasePercent + (rand() % 41)) / 100.0));
	Enemy->iHeal = (int)(PC.iHeal * ((BasePercent + (rand() % 41)) / 100.0));


	//객체지향이었다면 enemy클래스의 생성자 부분이 될듯.
}

void action(character* PC, character* Enemy, int iActionCode)
{
	//각 케이스문 별로 주석으로 설명하는것보단 역시 enum이 효율적으로 보임
	switch (iActionCode)
	{
		//서로 공격인 경우에는 방어력 1배수
	case 1:
	{
		{//케이스 문 내부의 변수의 지역화를 위해서 각 구문을 묶어줌.
			printf("내 행동 : 공격, 적 행동 : 공격 \n");
			int damage_PtoE = PC->iAttack - Enemy->iBlock;
			if (damage_PtoE < 0)
				damage_PtoE = 0;
			Enemy->iHP = Enemy->iHP - damage_PtoE;
		}

		{
			int damage_EtoP = Enemy->iAttack - PC->iBlock;
			if (damage_EtoP < 0)
				damage_EtoP = 0;
			PC->iHP = PC->iHP - damage_EtoP;
		}
		break;//케이스문 내무 블럭 안에 break; 가있는데 개인적으로는 밖으로 빼는게 확실하지 않을까 싶다. 일반적인 구문 모양이기 때문에.
	}

			//공격에 대한 방어는 방어력 2배수
	case 2:
	{
		printf("내 행동 : 공격, 적 행동 : 방어 \n");
		{
			int damage_PtoE = PC->iAttack - Enemy->iBlock * 2;
			if (damage_PtoE < 0)
				damage_PtoE = 0;
			Enemy->iHP = Enemy->iHP - damage_PtoE;
		}
		break;
	}

		//공격당할때 회복시에는 방어 효과를 받지 않음.
	case 3:
	{
		printf("내 행동 : 공격, 적 행동 : 회복 \n");
		{
			int damage_PtoE = PC->iAttack;
			Enemy->iHP = Enemy->iHP - damage_PtoE;
			Enemy->iHP = Enemy->iHP + Enemy->iHeal;
		}
		break;
	}

	case 4:
	{
		printf("내 행동 : 방어, 적 행동 : 공격 \n");
		{
			int damage_EtoP = Enemy->iAttack - PC->iBlock * 2;
			if (damage_EtoP < 0) {
				damage_EtoP = 0;
			}
			PC->iHP = PC->iHP - damage_EtoP;
		}
		break;
	}

	case 5:
	{
		printf("내 행동 : 방어, 적 행동 : 방어 \n");
		break;
	}

	case 6:
	{
		printf("내 행동 : 방어, 적 행동 : 회복 \n");
		Enemy->iHP = Enemy->iHP + Enemy->iHeal;
		break;
	}

	case 7:
	{
		printf("내 행동 : 회복, 적 행동 : 공격 \n");
		int damage_EtoP = Enemy->iAttack;
		PC->iHP = PC->iHP - damage_EtoP;
		PC->iHP = PC->iHP + PC->iHeal;
		break;
	}

	case 8:
	{
		printf("내 행동 : 회복, 적 행동 : 방어 \n");
		PC->iHP = PC->iHP + PC->iHeal;
		break;
	}

	case 9:
	{
		printf("내 행동 : 회복, 적 행동 : 회복 \n");
		PC->iHP = PC->iHP + PC->iHeal;
		Enemy->iHP = Enemy->iHP + Enemy->iHeal;
		break;
	}

	//각 항별로 데미지 처리 부분이 매우 공통적이므로 함수로 뺴는게 좋을것 같다.
	//혹은 각각 플레이어 , 적이 받는데미지, 힐량만 계산후에 아래에서 한번에 계산하게 하는것도 나쁘진 않을듯.
	}
}

void reward(character* PC, character* Enemy)
{
	//enemy를 받고있는 이유?
	system("cls");

	int iRewardSelect;

	for (int iCnt = 3; iCnt > 0; iCnt--) {
		printf("올리고 싶은 능력치를 선택하세요.\n");

		printf("1. 공격력 (현재 : %d) \n", PC->iAttack);
		printf("2. 방어력 (현재 : %d) \n", PC->iBlock);
		printf("3. 회복력 (현재 : %d) \n", PC->iHeal);

		printf("보상 선택 기회가 %d번 남았습니다. \n", iCnt);

		scanf_s("%d", &iRewardSelect);

		switch (iRewardSelect)
		{
		case 1: //공격력
			PC->iAttack += rand() % 3 + 1;
			break;

		case 2: //방어력
			PC->iBlock += rand() % 3 + 1;
			break;

		case 3:	//회복력
			PC->iHeal += rand() % 3 + 1;
			break;
		}
	}
}

//파일 세이브 로드.
void savedata()
{
	FILE* fp;

	fopen_s(&fp, "savedata.bin", "wb");

	fwrite(&PC, sizeof(character), 1, fp);

	fclose(fp);
}

void loaddata()//없을경우에 대한 예외처리가 필요할듯.
{
	FILE* fp;

	fopen_s(&fp, "savedata.bin", "rb");

	fread(&PC, sizeof(character), 1, fp);

	fclose(fp);
}

void phase0()//페이즈 0 은 처음 메뉴 를 가져오는 페이즈. 각 페이즈 관리는 phase 함수 포인터로 .
{
	while (true) {//phase0:
		Sleep(500);

		system("cls");

		int input_0;

		printf("메뉴를 선택하세요 : \n");
		printf("1. 게임 시작 \n");
		printf("2. 불러오기 \n");
		printf("3. 종료 \n");

		scanf_s("%d", &input_0);

		if (input_0 < 1 || input_0 > 3) {
			printf("잘못 입력하셨습니다. \n");
			_getch();
			continue;
		}

		switch (input_0) {
		case 1: //게임시작
			phase = phase1;
			return;
			break;

		case 2: //불러오기
			loaddata();
			printf("불러왔습니다.\n");
			break;

		case 3: //종료
			printf("게임을 종료합니다. \n");
			phase = phase3;
			return;
			break;
		}
	}
}

void phase1()//페이즈 1은 난이도 선택.
{
	while (true) {
		Sleep(500);

		system("cls");

		printf("PC's HP : %d \n", PC.iHP);//애초에 PC가 전역번수이기 떄문에 위에서도 포인터로 받아들일 필요가 없지 않을까?
		printf("PC's AttackPoint : %d \n", PC.iAttack);
		printf("PC's BlockPoint : %d \n", PC.iBlock);
		printf("PC's HealPoint : %d \n", PC.iHeal);


		int input_1;
		printf("\n난이도를 선택하세요 : \n");
		printf("1. 쉬움 2. 보통 3. 어려움\n");
		printf("4. 저장하기 5. 불러오기 6. 뒤로가기\n");
		scanf_s("%d", &input_1);

		if (input_1 < 1 || input_1 > 6) {
			printf("잘못 입력하셨습니다. \n");
			_getch();
			continue;
		}
		switch (input_1) {
		case 4:
			savedata();//아래 저장같은경우 세이브 데이터 자체에 들어가도 될듯.
			printf("저장했습니다. \n");
			continue;
		case 5:
			loaddata();//위와 마찬가지.
			printf("불러왔습니다. \n");
			continue;
		case 6:
			phase = phase0;
			return;
			break;
		}

		CreativeEnemy(input_1, PC, &Enemy);
		printf("적을 생성했습니다. \n");

		printf("Enemy's HP : %d \n", Enemy.iHP);
		printf("Enemy's AttackPoint : %d \n", Enemy.iAttack);
		printf("Enemy's BlockPoint : %d \n", Enemy.iBlock);
		printf("Enemy's HealPoint : %d \n", Enemy.iHeal);
		_getch();

		phase = phase2;
		return;

	}
}

void phase2()//실제 전투에 들어가는부분.
{
	while (true) {
		system("cls");

		printf("PC's HP : %d \n", PC.iHP);
		printf("PC's AttackPoint : %d \n", PC.iAttack);
		printf("PC's BlockPoint : %d \n", PC.iBlock);
		printf("PC's HealPoint : %d \n", PC.iHeal);

		printf("\n");

		printf("Enemy's HP : %d \n", Enemy.iHP);
		printf("Enemy's AttackPoint : %d \n", Enemy.iAttack);
		printf("Enemy's BlockPoint : %d \n", Enemy.iBlock);
		printf("Enemy's HealPoint : %d \n", Enemy.iHeal);

		int input_2;
		printf("\n행동을 선택하세요 : \n");
		printf("1. 공격, 2. 방어, 3. 회복  4.도망가기\n");
		scanf_s("%d", &input_2);

		if (input_2 < 1 || input_2 > 4) {
			printf("잘못 입력하셨습니다. \n");
			_getch();
			continue;
		}

		switch (input_2) {//여기에서도 한번어 액션에 들어갈걸 분류해주고있다. 내부에서 하게 하거나 아님 함수자체를 나누는건 어떨까?
		case 1:
			action(&PC, &Enemy, 1 + rand() % 3);
			break;

		case 2:
			action(&PC, &Enemy, 4 + rand() % 3);
			break;

		case 3:
			action(&PC, &Enemy, 7 + rand() % 3);
			break;

		case 4:
			phase = phase1;
			return;
			break;
		}

		if (PC.iHP <= 0) {
			printf("졌습니다.\n");
			phase = phase0;
		}

		if (Enemy.iHP <= 0) {
			printf("이겼습니다. \n");
			Sleep(500);
			reward(&PC, &Enemy);
			phase = phase1;
			return;
		}

		Sleep(500);
	}
}

void phase3()//게임 종료하는 페이즈.
{
	Sleep(500);

	system("cls");

	return;
}