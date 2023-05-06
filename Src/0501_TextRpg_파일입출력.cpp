#include <iostream>
#include <crtdbg.h>
#include <ctime>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

using namespace std;

typedef struct INFO
{
	char cName[32];
	char cJob[32];
	int iHp;
	int iAtk;
	int iLevel;
	int iExp;

}INFO, * PINFO;

int Create_Rand(void);
void Shuffle(int _iArray[]);
void Input_Array(int _iArray[], int _iSize); // �迭�� ���� �ִ´�.
void Print_Origin(int _iArray[]); // 5 * 5 ���
void Move(INFO* _pPlayer, int _iInput);

INFO Select_Job();
void Render(INFO* _pinfo);
void Field(INFO* _pPlayer);
void Create_Object(INFO _pMonster[]);
void Battle(INFO* _pPlayer, INFO* _pMonster);

void Write_Object(INFO* _tObject);
void Read_Object(INFO* _tObject);

void main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	srand(unsigned(time(NULL)));

	INFO tPlayer = {};
	int iInput = 0;

	FILE* pReadFile = NULL;		// ���� ��Ʈ��
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");

	if (0 == err)
	{
		// fread(�Է��� �޸��� �ּ�, �Է��� �޸� ������, �Է��� �޸� ����, ���� ��Ʈ��);
		fread(&tPlayer, sizeof(tPlayer), 1, pReadFile);
		fclose(pReadFile);
		cout << "���� �ҷ����� ���� : ĳ���� ������ �о�ɴϴ�..." << endl;
		Render(&tPlayer);
		system("pause");

	}
	else
	{
		cout << "���� �ҷ����� ���� : ĳ���͸� �����մϴ�." << endl;
		system("pause");
		tPlayer = Select_Job();
	}

	//Read_Object(&tPlayer);

	while (true)
	{
		system("cls");
		Render(&tPlayer);
		cout << "1. ����� 2. ���� : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Field(&tPlayer);
			break;
		case 2:
			Write_Object(&tPlayer);
			return;
		case 3: 

			return;

		default:
			break;
		}
	}
}

void Render(INFO* _pStat)
{
	cout << "===========================================" << endl
		<< "�̸� : " << _pStat->cName << endl
		<< "���� : " << _pStat->cJob << "\t���� : " << _pStat->iLevel << "\t����ġ: " << _pStat->iExp << endl
		<< "ü�� : " << _pStat->iHp << "\t���ݷ� : " << _pStat->iAtk << endl;
}

INFO Select_Job()
{
	INFO tTemp = {};
	char cName[32];

	int iInput = 0;

	cout << "�г����� �Է����ּ��� : ";
	cin >> cName;
	strcpy_s(tTemp.cName, sizeof(tTemp.cName), cName);

	cout << "������ �����ϼ���(1. ���� 2. ������ 3. ����) : ";
	cin >> iInput;

	switch (iInput)
	{
	case 1:
		//tTemp = { "����", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "����");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	case 2:
		//tTemp = { "������", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "������");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	case 3:
		//tTemp = { "����", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "����");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	}

	tTemp.iLevel = 1;
	tTemp.iExp = 0;

	Write_Object(&tTemp);

	return tTemp;
}


void Field(INFO* _pPlayer)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		cout << "1. �ʱ� ����� 2. �߱� ����� 3. ��� ����� 4. �� �ܰ� : ";
		cin >> iInput;

		if ((0 >= iInput) || (4 < iInput))
			continue;

		else if (4 == iInput)
			return;

		else if (4 > iInput)
		{
			Move(_pPlayer, iInput);
			/*		
			PINFO pMonster = new INFO[3]{};
			Create_Monster(pMonster);
			Battle(_pPlayer, &pMonster[iInput - 1]);

			delete[] pMonster;		// ���� �迭�� �ش��ϴ� heap������ ��� ����
			pMonster = nullptr;
			*/
		}
	}
}

void Create_Object(INFO _pMonster[])
{
	strcpy_s(_pMonster[0].cName, sizeof(_pMonster[0].cName), "�ʱ� ����");
	strcpy_s(_pMonster[1].cName, sizeof(_pMonster[1].cName), "�߱� ����");
	strcpy_s(_pMonster[2].cName, sizeof(_pMonster[2].cName), "��� ����");

	strcpy_s(_pMonster[0].cJob, sizeof(_pMonster[0].cJob), "�ʱ�");
	strcpy_s(_pMonster[1].cJob, sizeof(_pMonster[1].cJob), "�߱�");
	strcpy_s(_pMonster[2].cJob, sizeof(_pMonster[2].cJob), "���");

	for (int i = 0; i < 3; ++i)
	{
		_pMonster[i].iHp = (1 + i) * 30;
		_pMonster[i].iAtk = (1 + i) * 3;
		_pMonster[i].iLevel = (1 + i) * 3;
		_pMonster[i].iExp = (1 + i) * 30;
	}
}

void Battle(INFO* _pPlayer, INFO* _pMonster)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		Render(_pMonster);
		cout << "1. ���� 2. ���� : ";
		cin >> iInput;

		if (1 == iInput) // 1. ���� �̸�
		{
			// �÷��̾�� ���� ü�� ���
			_pPlayer->iHp -= (_pMonster->iAtk);
			_pMonster->iHp -= (_pPlayer->iAtk);

			// �÷��̾��� ü���� 0���ϸ� �÷��̾� ���
			if (0 >= _pPlayer->iHp)
			{
				//(_pPlayer->iHP) = 100;
				cout << endl << "�÷��̾� ���" << endl;
				system("pause");
				break;
			}

			// �÷��̾��� ü���� 0�̻��̰�, ������ ü���� 0���ϸ� �¸�
			if ((0 <= _pPlayer->iHp) && (0 >= _pMonster->iHp))
			{
				_pPlayer->iExp += _pMonster->iExp; // ����ġ ȹ��
				cout << endl << "�¸�" << endl;

				// ����ġ�� 100�̻��̸� ������
				if (100 <= _pPlayer->iExp)
				{
					(_pPlayer->iLevel)++;
					_pPlayer->iExp -= 100;
					cout << endl << "������!" << endl;
				}
				system("pause");
				break;
			}
		}
		else if (2 == iInput)
			return;
	}
}


void Move(INFO* _pPlayer, int _iInput)
{
	int iMyArray[25] = {};
	int iSize = (sizeof(iMyArray)) / (sizeof(int));
	int iLocation = 0;

	char iInput = 'z';

	Input_Array(iMyArray, iSize);

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		cout << endl << "\t================= ����� =================" << endl << endl;

		// 5 * 5 ���
		Print_Origin(iMyArray);
	
		cout << "q �� �ܰ�" << endl << endl
			<< "\tw ��" << endl
			<< "a ����\t\td ������" << endl
			<< "\ts �Ʒ�" << endl
			<< " : ";
		cin >> iInput;
	
		// 1 �ڸ� ã��.
		for (int i = 0; i < iSize; ++i)
		{
			if (8 == iMyArray[i])
				iLocation = i;
		}

		//2�� �Ʒ� �̵�
		if ('s' == iInput)
		{
			if (iLocation < (iSize - 5))
			{
				int iTemp = 0;

				iTemp = iMyArray[iLocation];
				iMyArray[iLocation] = iMyArray[iLocation + 5];
				iMyArray[iLocation + 5] = iTemp;

				//	��ȯ �ڸ��� ���� 15 �̻��̸� �� �ڸ��� ���� ����
				//	1���� �ڸ��� 15�̻��� �ڸ��� ��ȯ�Ǹ� ���� �߻�
				if (15 <= (iMyArray[iLocation]))
				{
					cout << "\t==============���� ����!==============" << endl;
					system("pause");

					PINFO pMonster = new INFO[3]{};
					Create_Object(pMonster);
					Battle(_pPlayer, &pMonster[_iInput - 1]);

					delete[] pMonster;		// ���� �迭�� �ش��ϴ� heap������ ��� ����
					pMonster = nullptr;

					iMyArray[iLocation] = 0;

					if (0 >= _pPlayer->iHp)
					{
						(_pPlayer->iHp) = 100;
						return;
					}
				}

			}
		}


		//4�� ���� �̵�
		if ('a' == iInput)
		{
			int iTemp = 0;
			if (0 < iLocation)
			{
				// (iLocation - 1)�� (4, 9, 14, 19, 24)�� �Ǹ� �ȵ�.
				if (4 != ((iLocation - 1) % 5))
				{
					iTemp = iMyArray[iLocation];
					iMyArray[iLocation] = iMyArray[iLocation - 1];
					iMyArray[iLocation - 1] = iTemp;

					if (15 <= (iMyArray[iLocation]))
					{
						cout << "\t==============���� ����!==============" << endl;
						system("pause");
						PINFO pMonster = new INFO[3]{};
						Create_Object(pMonster);
						Battle(_pPlayer, &pMonster[_iInput - 1]);

						delete[] pMonster;
						pMonster = nullptr;

						iMyArray[iLocation] = 0;

						if (0 >= _pPlayer->iHp)
						{
							(_pPlayer->iHp) = 100;
							return;
						}
					}
				}
			}
		}

		//6�� ������ �̵�
		if ('d' == iInput)
		{
			int iTemp = 0;
			if (iLocation < (iSize - 1))
			{
				// (iLocation + 1)�� 5�� ����� �Ǹ� �ȵ�.(0, 5, 10, 15, 20)
				if (0 != ((iLocation + 1) % 5))
				{
	
					iTemp = iMyArray[iLocation];
					iMyArray[iLocation] = iMyArray[iLocation + 1];
					iMyArray[iLocation + 1] = iTemp;

					if (15 <= (iMyArray[iLocation]))
					{
						cout << "\t==============���� ����!==============" << endl;
						system("pause");
						PINFO pMonster = new INFO[3]{};
						Create_Object(pMonster);
						Battle(_pPlayer, &pMonster[_iInput - 1]);

						delete[] pMonster;
						pMonster = nullptr;

						iMyArray[iLocation] = 0;

						if (0 >= _pPlayer->iHp)
						{
							(_pPlayer->iHp) = 100;
							return;
						}
					}
				}
			}
		}

		//8�� ���� �̵�
		if ('w' == iInput)
		{
			int iTemp = 0;
			if (0 < iLocation)
			{

				iTemp = iMyArray[iLocation];
				iMyArray[iLocation] = iMyArray[iLocation - 5];
				iMyArray[iLocation - 5] = iTemp;

				if (15 <= (iMyArray[iLocation]))
				{
					cout << "\t==============���� ����!==============" << endl;
					system("pause");
					PINFO pMonster = new INFO[3]{};
					Create_Object(pMonster);
					Battle(_pPlayer, &pMonster[_iInput - 1]);

					delete[] pMonster;
					pMonster = nullptr;

					iMyArray[iLocation] = 0;

					if (0 >= _pPlayer->iHp)
					{
						(_pPlayer->iHp) = 100;
						return;
					}
				}
			}
		}

		if ('q' == iInput)
			return;
	}
}


// �迭�� ���� �ִ´�.
void Input_Array(int _iArray[], int _iSize)
{
	for (int i = 0; i < _iSize; ++i)
	{
		_iArray[i] = i + 1;
	}
	Shuffle(_iArray); // �ڸ� ����
}

// 5 * 5 ������ ��� : 1(�÷��̾�)�� �ƴϸ� *�� ����Ѵ�.
void Print_Origin(int _iArray[])
{
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int iIndex = (i * 5) + j;

			if (0 == (_iArray[iIndex]))
			{
				cout << "\t" << "    " << "0";
			}
			else if (8 != (_iArray[iIndex]))
			{
				cout << "\t" << "    " << "*";
				//cout << "\t" << "    " << (_iArray[iIndex]);
			}
			else
			{
				cout << "\t" << "    " << (_iArray[iIndex]);
			}
		}
		cout << endl << endl;
	}
}

// ������ ����
int Create_Rand(void)
{
	return rand() % 25 + 1;
}

// �ڸ� ����
void Shuffle(int _iArray[])
{
	for (int i = 0; i < 50; ++i)
	{
		int iDst = Create_Rand();
		int iSrc = Create_Rand();

		int iTemp = _iArray[iDst - 1];
		_iArray[iDst- 1] = _iArray[iSrc-1];
		_iArray[iSrc-1] = iTemp;
	}
}

void Write_Object(INFO* _Object)
{
	// ���� ��Ʈ��
	FILE* pFile = NULL;
	// 1. ���� ��Ʈ�� ����(���� ���� �ؽ�Ʈ ���)
	errno_t err = fopen_s(&pFile, "./Data/PlayerData.txt", "wb");

	if (0 == err)
	{
		//	// 2. ���Ͽ� ���ų� �б� �Լ��� ȣ��
		fwrite(_Object->cName, sizeof(_Object->cName), 1, pFile);
		fwrite(_Object->cJob, sizeof(_Object->cJob), 1, pFile);
		fwrite(&_Object->iHp, sizeof(int), 1, pFile);
		fwrite(&_Object->iAtk, sizeof(int), 1, pFile);
		fwrite(&_Object->iLevel, sizeof(int), 1, pFile);
		fwrite(&_Object->iExp, sizeof(int), 1, pFile);
		cout << "ĳ���� ���� ���� ����" << endl;
		system("pause");
		fclose(pFile);	//3. ������ �Ҹ�
	}
	else
	{
		cout << "ĳ���� ���� ���� ����" << endl;
		system("pause");
	}
}

void Read_Object(INFO* _tObject)
{
	FILE* pReadFile = NULL;		// ���� ��Ʈ��
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");
	// fread(�Է��� �޸��� �ּ�, �Է��� �޸� ������, �Է��� �޸� ����, ���� ��Ʈ��);

	if (0 == err)
	{
		fread(&_tObject, sizeof(_tObject), 1, pReadFile);
		fclose(pReadFile);
		cout << "���� �ҷ����� ����" << endl;
		Render(_tObject);
		system("pause");

	}
	else
	{
		cout << "���� �ҷ����� ����" << endl;
		system("pause");
	}
}
