#include <iostream>
#include <istream>
#include <string>
#include <wchar.h>
#include <ctime>
#include <vector>
#include <math.h>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <iomanip>

using namespace std;

int today = 20181014;

int DayOfDate(int year, int month, int day)      //제라의 공식
{
	int week;   // 일(1), 월(2), 화(3), 수(4), 목(5), 금(6), 토(7)
	int a, b, c, d;   // a: 년도의 앞 두자리, b: 년도의 뒤 두자리

	c = month;
	if (c == 1 || c == 2)
	{
		year -= 1;
		c = c + 12;
	}
	else
		c = c;

	d = day;

	a = year / 100;
	b = year % 100;
	// 제라의 공식
	week = ((21 * a) / 4 + (5 * b) / 4 + 26 * (c + 1) / 10 + d - 1) % 7;
	/*-
	a: 년도의 앞 두자리
	#NAME?
	#NAME?
	- d: 일 */

	return week + 1;
}

string wcs_to_mbs(std::wstring const str, std::locale const loc = std::locale())
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	mbstate_t state;
	vector<char> buf((str.size() + 1) * codecvt.max_length());
	wchar_t const* in_next = str.c_str();
	char* out_next = &buf[0];
	codecvt_t::result r = codecvt.out(state,
		str.c_str(), str.c_str() + str.size(), in_next,
		&buf[0], &buf[0] + buf.size(), out_next);
	return std::string(&buf[0]);
}

void ShowHelp()
{
	cout << endl;
	cout << "날짜를 입력하면 오늘로부터 몇 일이 남았는지 알려주는 d - day 계산기 입니다." << endl << endl;
	cout << "프로그램 종료를 원할 땐 esc키 를 입력하세요." << endl << endl;
	cout << "사용자 편의를 위해 여러가지 입력 양식을 제공합니다." << endl << endl;
	cout << " -숫자와 띄어쓰기 혹은 특수문자 조합의 입력 ex) 2018.10.09   181009   2018 / 010 / 09   10 - 09 등" << endl << endl;
	cout << " -한글을 포함한 입력 ex) 2018년 10월 9일   10월9일   등" << endl << endl;
	cout << " -여러가지 날짜 표현방식 " << endl;
	cout << "   1. 년 표현을(재작년, 작년, 올해, 내년, 후년)의 표현으로 사용할 수 있습니다. " << endl << endl;
	cout << "   2. 주 표현을 'X째 주' (*단 X에 사용할 수 있는 문자는{ 첫,둘,셋,넷,다섯,여섯, 1,2,3,4,5,6 }, 년, 월, 요일에 대한 입력 필수)로 표현할수 있습니다." << endl << "   ex) 2019년 2월 첫째 주 금요일, 올해 2월 첫째 주 금요일 " << endl << endl;
	cout << "   3. 또한 주 표현을 요일에 대한 표현과 함께(다음, 다다음, 다다다음, 저번, 저저번, 저저저번) 로 사용할 수 있습니다." << endl << endl;
	cout << "   4. 일 표현을(그끄저께, 그저께, 어제, 오늘, 내일, 모레, 글피, 그글피)로 사용할 수 있고 년도, 월, 일, 요일 표현 없이 단독으로 사용해야합니다. " << endl << endl;
	cout << "   5. 기념일 표현은(신정, 발렌타인데이, 삼일절, 화이트데이, 어린이날, 어버이날, 부처님 오신날, 현충일, 제헌절, 광복절, 개천절, 한글날, 성탄절)만을 " << endl << "   년도, 월, 일, 요일 표현 없이 단독으로 사용가능하고 가장 가까운 미래의 정보를 알려줍니다. " << endl << endl;
	cout << "   6. 입력날짜는 그레고리력이 적용되는 1583년 1월 1일부터 4000년 12월 31일까지만 허용합니다. " << endl << endl;
	cout << " ※이 밖의 다른 표현이나 맞춤법에 맞지 않는 표현, 띄어쓰기가 연속으로 등장하는 경우, 특수문자를 띄어쓰기나 한글과 같이 쓰는 경우에는 입력 오류로 처리합니다. " << endl;
}

//윤년인지 아닌지 확인하는 함수
bool IsLeapYear(int yearToTest)
{
	//4로 나눠 떨어지면 윤년이지만
	if (yearToTest % 4 == 0)
	{
		//4로 나눠 떨어지는 연도 중 100으로도 나누어 떨어지는 연도는 윤년이 아니다.
		if (yearToTest % 100 == 0)
		{
			//그리고 4로 나눠 떨어지는 연도 중 100으로도 나누어 떨어지는 연도 중에서도 400으로 나누어 떨어지는 연도는 다시 윤년이다.
			if (yearToTest % 400 == 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return true;
	}
	else
		return false;
}


int Num_K(const wchar_t *str)//공백과 특수문자가 없는 한글을 처리하는 함수
{/*
 cout << "Num_K" << endl;*/
	const wchar_t *Ann[12];//기념일 한글이름 배열
	const wchar_t *Yestom[8];//오늘을 기준으로 한 날짜 한글배열
	int a = 12;//기념일 배열과 확인 후 초기화시켜줄 변수
	int b = 10, i;//Yestom 배열과 확인 후 초기화시켜줄 변수, 제어문 작동용 i
	Ann[0] = L"신정";
	Ann[1] = L"발렌타인데이";
	Ann[2] = L"삼일절";
	Ann[3] = L"화이트데이";
	Ann[4] = L"어린이날";
	Ann[5] = L"어버이날";
	Ann[6] = L"현충일";
	Ann[7] = L"제헌절";
	Ann[8] = L"광복절";
	Ann[9] = L"개천절";
	Ann[10] = L"한글날";
	Ann[11] = L"성탄절";
	int Anndate[] = { 101,214,301,314,505,508,606,717,815,1003,1009,1225 };//기념일 숫자날짜
	Yestom[0] = L"그끄저께";
	Yestom[1] = L"그저께";
	Yestom[2] = L"어제";
	Yestom[3] = L"오늘";
	Yestom[4] = L"내일";
	Yestom[5] = L"모레";
	Yestom[6] = L"글피";
	Yestom[7] = L"그글피";
	//변수 선언, 초기화
	for (i = 0; i < 12; i++)
	{
		if (wcscmp(Ann[i], str) == 0)//입력구문과 기념일배열을 비교, 같다면 a에 몇번째 기념일배열 변수와 같은지 저장
		{
			a = i;
			break;
		}
	}
	if (a == 12)
	{
		for (i = 0; i < 8; i++)
		{
			if (wcscmp(Yestom[i], str) == 0)//Yestom배열과 비요, 같다면 오늘을 기준으로 며칠 차이나는지 b에 저장
			{
				b = i - 3;
				break;
			}
		}
		if (-4 < b&&b < 5)//b는 10으로 초기화되있어서 Yestom배열에서 걸러지지 않는다면 작동하지 않는 제어문이며, 걸러졌다면 오늘을 기준으로 b를 더하여 return
		{
			return today + b;
		}
		else
			return 0;
	}
	else
	{
		if (today % 10000 == Anndate[a])//찾은 날짜가 가장 가까운 미래의 년도의 날짜를 출력하도록 설정
			return today;
		else if (today % 10000 > Anndate[a])
			return ((today / 10000 + 1) * 10000 + Anndate[a]);
		else
			return ((today / 10000) * 10000) + Anndate[a];
	}
}

int Num_KB(string str)
{
	/*cout << "Num_KB" << endl;*/   //어떤 Num 함수가 동작했는지 확인하기 위해 넣은 테스트 코드
	string temp;
	vector<string> token;
	for (stringstream sts(str); (sts >> temp);)      // 문자열 str로 만들어진 문자열 스트림 sts생성 후, 문자열 스트림 sts를 이용하여 한 단어씩 temp에 입력
	{
		token.push_back(temp);      // sts에 남은 단어가 없다면 NULL을 반환하고 아니라면 단어 temp를 token에 추가
	}

	/*for (vector<string>::iterator it = token.begin(); it != token.end(); ++it)
	cout << *it << "\n";*/   //token 배열에 들어간 요소들을 보기 위한 테스트 코드

	//공백을 기준으로 나눈 요소들의 개수가 3이 아니라면 오류이므로 오류를 뜻하는 -1값을 반환한다.
	//XX 주 ㅁ요일 형태가 올바른 형태
	if (token.size() != 3)
	{
		//cout << token[0] << endl;         //배열의 크기가 3이 아닐 경우 분리가 안된 것인지 아니면 다른 문제인지 확인하기 위해 첫번째 요소가 무엇으로 저장되어있는지 확인하기 위함.
		//cout << token.size() << endl;      //배열의 크기에 따라 잘 오류를 걸러냈는지 확인하기 위함.

		return -1;
	}
	//두번 째 토큰이 "주"라는 단어가 아니거나, 세번 째 토큰의 두번째와 세번째 글자가 "요일"이 아니면 오류를 뜻하는 -1를 반환한다.
	else if (token[1].compare("주") != 0 || token[2].find("요일") != 2)
	{
		/*   cout << token[1].compare("주") << endl;         //위 if문에 걸린 것이 첫번째 조건 때문인지
		cout << token[2].find("요일") << endl;*/            //두번째 조건 때문인지 확인하는 테스트 코드

		return -1;
	}
	//위 오류 2가지 if문에 걸리지 않았다면
	else
	{
		//세번째 요소에서 3글자 이상인지 확인하여, "요일"이라는 문자열 뒤에 필요치 않은 문자열이 포함되어 있는지 확인한다.
		//한글은 2칸씩으로 계산 되기 때문에 token[2].length() > 3 이 아닌 token[2].length() > 6 으로 검사해주어야 한다.
		if (token[2].length() > 6)
		{
			//cout << "요일 형식에 이상있음" << endl;      //어떤 조건문에서 걸러진 것인지 확인하기 위한 테스트 출력문
			//cout << token[2].length() << endl;         //오류로 걸러졌을 경우 token[2].length()가 어떤 값을 가졌었는지 확인하기 위함.
			return -1;
		}
	}

	int week;

	//XX 주 ㅁ요일 형태 중  XX에 대한 분석을 위한 조건문들이다.
	//"다음"이라는 문자열을 발견했을 경우
	if (token[0].find("다음") != -1)
	{
		//해당 문자열의 길이에 따라 나눠진다.
		switch (token[0].length())
		{
		case 4:      //한글은 역시나 2칸씩으로 계산되기 때문에 4일 경우는 두 글자일 경우이다.
			week = 1;
			break;
		case 6:      //세 글자 일 경우
		   //"다다"라는 문자열이 발견된다면 다다음주라고 인식하고 값을 저장
			if (token[0].find("다다") != -1)
			{
				week = 2;
			}
			else
			{
				//cout << "다다 오류" << endl;      //주 표현 중 어떤 오류처리 구문에서 걸러졌는지 확인하기 위함.
				return -1;
			}
			break;
		case 8:      //네 글자 일 경우
		   //"다다다"라는 문자열이 발견된다면 다다다음주라고 인식하고 값을 저장
			if (token[0].find("다다다") != -1)
			{
				week = 3;
			}
			else
			{
				//cout << "다다다 오류" << endl;      //주 표현 중 어떤 오류처리 구문에서 걸러졌는지 확인하기 위함.
				return -1;
			}
			break;
		default:   //그 이외의 길이의 문자열인 경우는 오류이다
		   //cout << token[0].length() << endl;      //주 표현을 하는 문자열이 얼마만큼의 길이를 가지는지 확인하기 위한 테스트 코드.
			return -1;
			break;
		}
	}
	//"다음"이 발견되지 않으면 "저번"을 찾아본다. 발견된 경우
	else if (token[0].find("저번") != -1)
	{
		//해당 문자열의 길이에 따라 나눠진다. 아래는 "다음"이 포함된 윗 구문과 동일한 방식으로 처리한다.
		switch (token[0].length())
		{
		case 4:
			week = -1;
			break;
		case 6:
			if (token[0].find("저저") != -1)
			{
				week = -2;
			}
			else
			{
				return -1;
			}
			break;
		case 8:
			if (token[0].find("저저저") != -1)
			{
				week = -3;
			}
			else
			{
				return -1;
			}
			break;
		default:
			return -1;
			break;
		}
	}
	//"저번"조차 발견되지 않았을 경우 "이번"을 찾아본다. 발견된 경우
	else if (token[0].find("이번") != -1)
	{
		//해당 문자열이 2글자 짜리인지 검사한다. 맞다면 "이번 주"로 인식하고 값을 저장한다.
		if (token[0].length() == 4)
			week = 0;
		else
			return -1;
	}
	//위 경우에 전부 포함되지 않는 문자열을 오류이다.
	else
	{
		return -1;
	}


	int day;      //요일을 저장하기 위한 변수
	//3번째 요소인 ㅁ요일 형태에서 ㅁ을 검사하기 위한 구문이다.
	//각각 일요일은 1 월요일은 2 형태로 토요일 6까지 값을 저장한다.
	if (token[2].find("일") == 0)
	{
		day = 1;
	}
	else if (token[2].find("월") == 0)
	{
		day = 2;
	}
	else if (token[2].find("화") == 0)
	{
		day = 3;
	}
	else if (token[2].find("수") == 0)
	{
		day = 4;
	}
	else if (token[2].find("목") == 0)
	{
		day = 5;
	}
	else if (token[2].find("금") == 0)
	{
		day = 6;
	}
	else if (token[2].find("토") == 0)
	{
		day = 7;
	}
	//만약 일,월,화,수,목,금,토 중에 아무것도 포함되지 않았다면 틀린 문자열이다.
	else
	{
		return -1;
	}

	int curTime = today;      //메인에서 받아놓은 현재 날짜를 저장하는 변수
	int curYear, curMonth, curDate, curDay, curTemp;      //각각 현재 연도, 달, 일, 요일 그리고 임시적으로 값을 잠시 담아 둘 변수

	//curTime이라는 8자리 숫자를 각각 연,월,일 변수에 찢어서 저장한다.
	curYear = curTime / 10000;
	curTemp = curTime % 10000;
	curMonth = curTemp / 100;
	curDate = curTemp % 100;
	//날짜를 바탕으로 DayOfDate함수를 사용하여 요일을 저장한다.
	curDay = DayOfDate(curYear, curMonth, curDate);

	//목표하는 날을 저장하는 변수
	//현재날짜 + 목표 주의 현재날짜와 같은 요일까지의 날 + 목표하는 요일의 값 - 현재날짜의 요일 값
	int date = curDate + week * 7 + day - curDay;

	//date 값이 0이하가 나왔을 경우
	if (date <= 0)
	{
		//달에 해당하는 값을 1 줄이고 그 값이 0이 되었는지 확인
		if (--curMonth == 0)
		{
			//그랬을 경우 연도 값을 하나 줄이고 월을 12월로 설정
			curYear--;
			curMonth = 12;
		}
		//31일의 날짜를 가지는 달인지 확인 후
		if (curMonth == 1 || curMonth == 3 || curMonth == 5 || curMonth == 7 || curMonth == 8 || curMonth == 10 || curMonth == 12)
		{
			//맞다면 현재 날짜에 31일을 더한다.
			date = date + 31;
		}
		//30일의 날짜를 가지는 달도 위의 31일의 날짜를 가지는 달과 같은 방식으로 해준다.
		else if (curMonth == 4 || curMonth == 6 || curMonth == 9 || curMonth == 11)
		{
			date = date + 30;
		}
		//위 값이 둘다 아닐 경우 2월이다.
		// 0이하의 달과 13이상의 달은 다른 함수에서 오류처리 해주기 때문에 무시한다.
		else
		{
			//윤년인지 확인하고 맞다면 29일을
			if (IsLeapYear(curYear))
			{
				date = date + 29;
			}
			//아니라면 28일을 더해준다.
			else
			{
				date = date + 28;
			}
		}

	}
	//date값이 0을 초과하는 값일 경우
	else
	{
		//31일을 가지는 달인지 확인한다.
		if (curMonth == 1 || curMonth == 3 || curMonth == 5 || curMonth == 7 || curMonth == 8 || curMonth == 10 || curMonth == 12)
		{
			//그 후 date값이 32이 이상인지 검사한다.
			if (date >= 32)
			{
				//해당된다면 해당 달의 값이 12인지 검사한다.
				if (curMonth == 12)
				{
					//해당된다면 달의 값을 1로 변경하고, 연도 값을 1증가 시키고 date에서 31일을 빼준다.
					curMonth = 1;
					curYear++;
					date = date - 31;
				}
				else
				{
					//아니라면 달의 값을 1증가 시키고 date의 값에서 31일을 빼준다.
					curMonth++;
					date = date - 31;
				}
			}
		}
		//30일을 가지는 달인지 확인해 준다
		else if (curMonth == 4 || curMonth == 6 || curMonth == 9 || curMonth == 11)
		{
			//해당된다면 date값이 31일 이상인지 확인한다.
			if (date >= 31)
			{
				//해당된다면 달의 값을 1 증가시키고 date 값에서 30을 빼준다.
				curMonth++;
				date = date - 30;
			}
		}
		//31일을 가지는 달, 30일을 가지는 달이 둘다 아닐 경우 2월이다
		//0이하의 값 13이상의 값을 가지는 달은 다른 함수에서 오류처리 해줄것이기 때문에 무시한다.
		else
		{
			//윤년인지 확인하고 윤년 여부에 따라 날짜 계산을 해준다.
			if (IsLeapYear(curYear))
			{
				if (date >= 30)
				{
					curMonth++;
					date = date - 29;
				}
			}
			else
			{
				if (date >= 29)
				{
					curMonth++;
					date = date - 28;
				}
			}
		}
	}
	//연도 값, 달의 값, 일의  값을 8자리 숫자형태로 변환하며 리턴한다.
	return curYear * 10000 + curMonth * 100 + date;
}

int Num_KNB(const wchar_t* str)
{
	/*cout << "Num_KNB" << endl;*/
	int result = 0;

	//입력된 문자열이 가르키는 날짜의 정보(년,월,일)을 저장할 변수
	int tyear = 0;
	int tmonth = 0;
	int tday = 0;

	int getDay = 0;//요일 저장
	int yearNext = 0;   //년 표현 바로 다음 위치(월에 해당하는 값을 조사하기 위한 첫번째 위치를 가르키는 변수)

	//와이드문자열 형태의 년,월,일 값을 저장할 변수
	wchar_t year[4] = { 0 };
	wchar_t month[2] = { 0 };
	wchar_t day[2] = { 0 };
	int jjae = 0;   //몇 번째인지 저장 할 변수

	wchar_t checkDay[] = { 0xC77C,0xC6D4, 0xD654,0xC218, 0xBAA9, 0xAE08,0xD1A0 }; //일월화수목금토

	int Check[5] = { 0,0,0,0,0 };
	//년, 월, 일, 번째, 요일의 갯수

	for (int i = 0; i < wcslen(str); i++)
	{
		//"올"이라는 글자 검사
		if ((str[i] == 0xC62C) && (i < wcslen(str) - 1))
		{
			//"올해"라는 단어가 2회 이상 사용 된 문자열 - 오류처리
			if (Check[0] == 1)
			{
				/*cout << "올해가 여러개 들어간 입력";*/
				return -1;
			}
			//"올"이 검사된 위치의 뒷 글자가 "해"라는 글자인지 검사
			if (str[i + 1] == 0xD574)
			{
				Check[0] = 1;   //연도 값이 설정되었다는 플래그
				tyear = today / 10000;
				yearNext = i + 2;
				/*cout << "\"올해\"가 들어간 입력" << endl;*/
			}
			else
			{
				/*cout << "올 다음에 해가 안나옴" << endl;*/
				return -1;
			}
		}
		//올해라는 단어 없으면 그냥 나옴
	}
	for (int i = 0; i < wcslen(str); i++)
	{
		if (str[i] == 0xB144)//"년"이라는 단어가 있고 "올해"라는 단어가 없는 것
		{
			if (Check[0] == 0)
			{
				/*cout << "\"년\"이 하나 들어간 입력" << endl;*/
				Check[0] = 1;
				yearNext = i + 1;

				for (int j = 1; i - j >= 0; j++)
				{
					//년 앞에 숫자(아스키코드 숫자 범위 조사)
					if (0x30 <= str[i - j] && str[i - j] <= 0x39)
					{
						if (4 - j >= 0)
						{
							year[4 - j] = str[i - j] - 48;

						}
						else
						{
							/*cout << "숫자가 4자리 이상" << endl;*/
							return -1;
						}
					}//년 앞에 한글(아스키코드 한글 범위 조사)
					else if ((0xAC00 <= str[i - j]) && (str[i - j] <= 0xD7A3))
					{
						if (str[i - j] == 0xC791)//"작"이라는 단어인지 검사
						{
							tyear = (today / 10000) - 1;

							if (i - (j + 1) >= 0 && str[i - (j + 1)] == 0xC7AC)//작년 앞에 '재'가 있는지 검사
							{
								tyear = today / 10000 - 2;
								if (i - (j + 2) >= 0)
								{
									/*cout << "재 앞에 뭔가가 있음" << endl;*/
									return -1;
								}
							}
							else if (i - (j + 1) >= 0 && str[i - (j + 1)] == 0xC7AC)
							{
								/*cout << "작 앞에 뭔가가 있음" << endl;*/
								return -1;
							}
							break;
						}
						else if (str[i - j] == 0xB0B4)//'년' 앞에 '내'가 있는지 검사
						{
							tyear = (today / 10000) + 1;
							if (i - (j + 1) >= 0)
							{
								/*cout << "내년 앞에 뭔가가 있음" << endl;*/
								return -1;
							}
							break;
						}
						else if (str[i - j] == 0xD6C4)//'년'앞에 '후'가 있는지 검사
						{
							tyear = (today / 10000) + 2;
							if (i - (j + 1) >= 0)
							{
								/*cout << "후년 앞에 뭔가가 있음" << endl;*/
								return -1;
							}
							break;
						}
						else
						{
							/*cout << "년 앞에 한글도 숫자도 아님" << endl;*/
							return -1;
						}
					}
					else
					{
						/*cout << "년 앞에 숫자, 한글 이외의 값이 나옴" << endl;*/
						return -1;
					}
				}
			}
			else
			{
				/*cout << "\"년\"이 여러개 들어간 입력" << endl;*/
				return -1;
			}
		}
	}

	if (!Check[0]) // 년도에 해당하는 문자열이 없을 시
	{
		tyear = -1;
		yearNext = 0;
	}

	//공백 문자이면 검사 시작 위치를 한칸 넘긴다.
	if (str[yearNext] == 0x20)
	{
		yearNext++;
	}

	int monthNext = 0;   //일 정보를 검사하기 위한 첫 위치를 정해주기 위한 변수

	//
	for (int i = yearNext; i < wcslen(str); i++)
	{
		//"월"이라는 단어 검사
		if (str[i] == 0xC6D4)
		{
			//"월" 후에 "요"라는 단어 검사
			if (i < wcslen(str) - 1 && str[i + 1] == 0xC694)
			{
				continue;
			}
			if (Check[1] == 0)//월의 정보가 아직 저장이 안된 경우
			{
				Check[1] = 1; //월의 정보가 저장되었다고 플래그
				monthNext = i;//월의 위치반환
			}
			else
			{
				/*cout << "월이 여러개" << endl;*/
				return -1;
			}
		}
	}

	if (Check[1] == 0)
	{/*
	 cout << "월이 없음" << endl;*/
		return -1;
	}
	/*else
	cout << "월이 하나 있음" << endl;*/

	if (monthNext - (yearNext + 2) == 0) //월 앞에 두자리
	{
		if (str[monthNext - 1] >= 0x30 && 0x39 >= str[monthNext - 1])//월앞이 숫자
		{
			if (str[monthNext - 2] >= 0x30 && 0x39 >= str[monthNext - 2])//월 앞앞이 숫자
			{
				month[0] = str[monthNext - 2] - 48;
				month[1] = str[monthNext - 1] - 48;
			}
			else
			{/*
			 cout << "월 앞 숫자 앞 숫자 이외 값" << endl;*/
				return -1;
			}
		}
		else
		{/*
		 cout << "월 앞 숫자 이외 값" << endl;*/
			return -1;
		}
	}
	else if (monthNext - (yearNext + 1) == 0) //월 앞에 한자리
	{
		if (str[monthNext - 1] >= 0x30 && 0x39 >= str[monthNext - 1])
		{
			month[0] = 0x30 - 48;
			month[1] = str[monthNext - 1] - 48;
		}
		else
		{/*
		 cout << "월 앞 숫자 이외 값" << endl;*/
			return -1;
		}
	}
	else
	{/*
	 cout << "월 앞에 두자리 이상 또는 월 앞에 숫자 없음" << endl;*/
		return -1;
	}

	monthNext++;

	//공백이면 다음 칸으로
	if (str[monthNext] == 0x20)
	{
		monthNext++;
	}

	int dayNext = 0;

	for (int i = monthNext; i < wcslen(str); i++)
	{
		if ((str[i] == 0xC77C && str[i - 1] != 0xC694))//일, 일 앞이 요일때 제외
		{
			//"일" 뒤가 "요" 일때
			if (i < wcslen(str) - 1 && str[i + 1] == 0xC694)
			{
				continue;
			}

			if (Check[2] == 0)
			{
				Check[2] = 1;
				dayNext = i;//일의 위치반환
			}
			else
			{/*
			 cout << "일이 여러개" << endl;*/
				return -1;
			}
		}
	}


	if (Check[2])
	{/*
	 cout << "일이 하나 있음" << endl;*/

		if (dayNext - (monthNext + 2) == 0) //일 앞에 두자리
		{
			if (str[dayNext - 1] >= 0x30 && 0x39 >= str[dayNext - 1])//일앞이 숫자
			{
				if (str[dayNext - 2] >= 0x30 && 0x39 >= str[dayNext - 2])
				{
					day[0] = str[dayNext - 2] - 48;
					day[1] = str[dayNext - 1] - 48;
				}
				else
				{/*
				 cout << "일 앞 숫자 앞 숫자 이외 값" << endl;*/
					return -1;
				}
			}
			else
			{/*
			 cout << "일 앞 숫자 이외 값" << endl;*/
				return -1;
			}
		}
		else if (dayNext - (monthNext + 1) == 0) //일 앞에 한자리
		{
			if (str[dayNext - 1] >= 0x30 && 0x39 >= str[dayNext - 1])
			{
				day[0] = 0x30 - 48;
				day[1] = str[dayNext - 1] - 48;
			}
			else
			{/*
			 cout << "일 앞 숫자 이외 값" << endl;*/
				return -1;
			}
		}
		else
		{/*
		 cout << "일 앞에 두자리 이상 또는 일 앞에 숫자 없음" << endl;*/
			return -1;
		}

		if (dayNext != wcslen(str) - 1)//일요일이랑 일 다음 입력 거르기
		{/*
		 cout << "일 다음에 입력이 있음" << endl;*/
			return -1;
		}

		if (tyear == 0) {
			for (int i = 0; i < sizeof(year) / sizeof(year[0]); i++)
			{
				tyear = tyear * 10 + (int)year[i];
			}
		}
		for (int i = 0; i < sizeof(month) / sizeof(month[0]); i++)
		{
			tmonth = tmonth * 10 + (int)month[i];
		}
		for (int i = 0; i < sizeof(day) / sizeof(day[0]); i++)
		{
			tday = tday * 10 + (int)day[i];
		}
		if (tyear == -1)
		{
			if (tmonth * 100 + tday >= today % 10000)
				tyear = today / 10000;
			else
				tyear = today / 10000 + 1;
		}
		result = tyear * 10000 + tmonth * 100 + tday;

		return result;
	}
	else //숫자+일이 없을때
	{
		if (tyear == 0) {
			for (int i = 0; i < sizeof(year) / sizeof(year[0]); i++)
			{
				tyear = tyear * 10 + (int)year[i];
			}
		}
		for (int i = 0; i < sizeof(month) / sizeof(month[0]); i++)
		{
			tmonth = tmonth * 10 + (int)month[i];
		}
		/*
		cout << "숫자+일 이 없음" << endl;*/
		for (int i = monthNext; i < wcslen(str); i++)
		{
			if (str[i] == 0xC9F8)//째
			{
				if (i != wcslen(str) - 7)
				{/*
				 cout << "째 입력 다음의 공간이 넉넉하지 않거나 너무 많음" << endl;*/
					return -1;
				}
				else
				{
					if (str[i + 1] == 0x20)//공백
					{
						if (str[i + 2] == 0xC8FC)//주
						{
							if (str[i + 3] == 0x20)
							{
								for (int j = 0; j < sizeof(checkDay) / sizeof(checkDay[0]); j++)
								{
									if (str[i + 4] == checkDay[j])
									{
										getDay = j;
										break;
									}

									if (j == sizeof(checkDay) / sizeof(checkDay[0]) - 1)
									{/*
									 cout << "요일 설정이 잘못됨" << endl;*/
										return -1;
									}
								}
								if (str[i + 5] == 0xC694)//요
								{
									if (str[i + 6] == 0xC77C)//일, 배열의 마지막 방 접근
									{
										if ((0xAC00 <= str[i - 1]) && (str[i - 1] <= 0xD7A3))//째 앞 검사, i는 째의 위치
										{
											for (int k = i - 1; k >= monthNext; k--)
											{
												if (str[k] == 0xC12F)//섯
												{
													if (k - 1 >= monthNext)
													{
														if (str[k - 1] == 0xB2E4)//다
														{
															if (!(k - 1 == monthNext))
															{/*
															 cout << "다섯 앞에 문자가 있음" << endl;*/
																return -1;
															}
															jjae = 5;
															break;
														}
														else if (str[k - 1] == 0xC5EC)//여
														{
															if (!(k - 1 == monthNext))
															{/*
															 cout << "여섯 앞에 문자가 있음" << endl;*/
																return -1;
															}
															jjae = 6;
															break;
														}

														else
														{/*
														 cout << "섯 앞에 다른 글자" << endl;*/
															return -1;
														}
													}

													else
													{/*
													 cout << "섯 앞에 공간 없음" << endl;*/
														return -1;
													}
												}
												else if (str[k] == 0xCCAB)//첫
												{
													if (k - 1 >= monthNext)
													{/*
													 cout << "첫 앞에 다른 문자가 옴" << endl;*/
														return -1;
													}

													jjae = 1;
												}
												else if (str[k] == 0xB458)//둘
												{
													if (k - 1 >= monthNext)
													{/*
													 cout << "둘 앞에 다른 문자가 옴" << endl;*/
														return -1;
													}
													jjae = 2;
												}
												else if (str[k] == 0xC14B)//셋
												{
													if (k - 1 >= monthNext)
													{/*
													 cout << "셋 앞에 다른 문자가 옴" << endl;*/
														return -1;
													}
													jjae = 3;
												}
												else if (str[k] == 0xB137)//넷
												{
													if (k - 1 >= monthNext)
													{/*
													 cout << "넷 앞에 다른 문자가 옴" << endl;*/
														return -1;
													}
													jjae = 4;
												}
												else
												{/*
												 cout << "째 앞에 한글이지만 지정된 단어가 아님" << endl;*/
													return -1;
												}
											}
										}
										else if ((0x30 <= str[i - 1] && str[i - 1] <= 0x39))
										{
											int countFrontjjae = 1;//째 앞의 문자열 갯수
											for (int k = i - 1; k >= monthNext; k--)
											{
												if (countFrontjjae == 1)
												{
													jjae = (int)str[k] - 48;
												}

												countFrontjjae++;
											}
											if (countFrontjjae != 2)
											{/*
											 cout << "째 앞에 한자리 숫자 이외 값" << endl;*/
												return -1;
											}
											/*else
											{
											cout << "째 앞의 입력값이 숫자" << endl;
											}*/

										}
										else
										{/*
										 cout << "째 앞이 숫자,한글이 아님" << endl;*/
											return -1;
										}
										if (!(1 <= jjae && jjae <= 6))
										{/*
										 cout << "째 앞의 입력값이 1~6에 해당하지 않음" << endl;*/
											return -1;
										}
										/*
										cout << "올바른 입력" << endl;*/
										if (tyear == -1)
										{
											if (tmonth * 100 + tday >= today % 10000)
												tyear = today / 10000;
											else
												tyear = today / 10000 + 1;
										}
										int dayCount[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
										int getDayCheck = 0;
										int firstDay = 0;
										int lastDay = 0;
										if (IsLeapYear(tyear))
											dayCount[1] = 29;

										//일 : 0 월 : 1, 화 : 2, 수 : 3,목 : 4,금 : 5,토 : 6
										if (tmonth < 1 || tmonth >12)
										{/*
										 cout << "월이 1보다 작거나 12보다 큼" << endl;*/
											return -1;
										}
										getDayCheck = getDay + dayCount[tmonth - 1];
										firstDay = DayOfDate(tyear, tmonth, 1) - 1;
										//                              cout<<"firstday : "<<firstDay<<endl;

										//                       cout<<"Getday : "<<getDay<<endl;



										if ((jjae - 1) * 7 + getDay < firstDay)

										{/*

										 cout << "첫번째날보다 전의 날.." << endl;*/

											return -1;

										}

										if ((jjae - 1) * 7 + getDay >= firstDay + dayCount[tmonth - 1])

											//2020년 1월일때) 4*7+6>3+31

										{/*

										 cout << "마지막 날보다 이후 날" << endl;*/

											return -1;

										}

										tday = (jjae - 1) * 7 + getDay + 1 - firstDay;

										result = tyear * 10000 + tmonth * 100 + tday;
										return result;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return -1;
}

//숫자로만 이루어진 문자열
int Num_N(const wchar_t* str) {

	//현재 날짜
	int year = today / 10000;

	//함수의 반환값
	int result = 0;

	//wcstol에 쓰일 변수
	wchar_t* error;
	int base = 10;

	//입력 값의 길이에 따라
	switch (wcslen(str)) {

	case 4:
	{
		//cout<<"4자리 수입니다"<<endl;
		int result = (int)wcstol(str, &error, base);

		//입력 날짜가 현재 날짜보다 이전일때 내년 날짜로 계산
		if (result < today % 10000)
			result = result + (year + 1) * 10000;
		else
			result = result + year * 10000;

		//cout<<result<<endl;
		return result;
	}

	case 6:
	{
		//cout<<"6자리 수입니다"<<endl;
		int result = (int)wcstol(str, &error, base);

		//6자리 입력값의 앞 2자리(년도) 50을 기준으로 분류
		if (result / 10000 >= 50)
			result = (year / 100 - 1) * 100 * 10000 + result;
		else
			result = year / 100 * 100 * 10000 + result;

		//cout<<result<<endl;
		return result;
	}
	case 8:
	{
		//cout<<"8자리 수입니다"<<endl;
		result = (int)wcstol(str, &error, base);

		//cout<<result<<endl;
		return result;
	}
	default:
	{
		//cout<<"숫자로만 입력 시 4,6,8자리 수를 입력하십시오.(ex 19950114)"<<endl;
		result = -1;
	}
	}
	return result;
}


//숫자와 특수문자로 이루어진 문자열 (숫자 + 띄어쓰기 포함)
int Num_NS(const wchar_t* str)
{
	wchar_t t[30];
	wchar_t *ptr = nullptr;
	wcscpy(t, str);
	int result = 0;
	int delNum = 0;
	wchar_t delType = ' ';

	int year = 0, month = 0, day = 0;

	wchar_t *del;
	wchar_t *error;
	int base = 10;

	for (int i = 0; i < wcslen(str); i++)
	{
		//Classifying 함수 상단에서 같은 특수문자만 사용할 수 있게 정의

		//어떤 특수문자가 입력되었는지, 특수문자가 몇개인지(1 또는 2)
		if (((str[i] == 0x2e || str[i] == 0x2f) || str[i] == 0x2d) || str[i] == 0x20)
		{
			delNum++;
			delType = str[i];
		}
	}

	//특수문자 하나 쓰인 숫자 입력
	if (delNum == 1)
	{
		del = wcstok_s(t, L".-/ ", &ptr);
		month = (double)wcstol(del, &error, base);
		if (month < 0)
		{
			//cout<<"오버플로우 발생"<<endl;
			return -1;
		}

		del = wcstok_s(NULL, L".-/ ", &ptr);

		if (del != NULL)
		{
			day = (double)wcstol(del, &error, base);
			if (day < 0)
			{
				//cout<<"오버플로우 발생"<<endl;
				return -1;
			}
		}
		else
		{
			//cout<<"특수문자 이후에 숫자를 입력하십시오"<<endl;
			return -1;
		}

		if (month >= 100 || day >= 100)
		{
			//cout<<"월,일의 입력이 잘못되었습니다."<<endl;
			return -1;
		}
		else
		{
			if ((month * 100 + day) >= today % 10000)
				year = today / 10000;
			else
				year = today / 10000 + 1;
			result = year * 10000 + month * 100 + day;
			return result;
		}
	}
	//특수문자 두개 쓰인 숫자 입력
	else if (delNum == 2)
	{
		wchar_t* del[30];
		int n = 1;

		del[0] = wcstok_s(t, L"./- ", &ptr);

		do
		{
			del[n++] = wcstok_s(NULL, L"./- ", &ptr);

		} while (del[n - 1] != NULL);
		year = (double)wcstol(del[0], &error, base);

		if (del[2] == NULL)
		{
			//cout<<"특수문자 이후에 숫자 값을 입력하십시오."<<endl;
			return -1;
		}

		month = (double)wcstol(del[1], &error, base);

		day = (double)wcstol(del[2], &error, base);

		if ((month < 0 || day < 0) || (year < 0 || year >= 9999))
		{
			//cout<<"오버플로우 발생"<<endl;
			return -1;
		}

		if ((month >= 100 || day >= 100))
		{
			//cout<<"월,일의 입력이 잘못되었습니다."<<endl;
			return -1;
		}
		else
		{

			result = year * 10000 + month * 100 + day;
			return result;
		}
	}

	else {
		//cout<<"\"";
		//wcout<<delType;
		//cout<<"\"은(는) 1개 또는 2개만 사용할 수 있습니다. (ex 01.14 , 2018.09.03)"<<endl;
		return -1;
	}

	return result;

}

int Num_KN(const wchar_t* str)
{
	int tyear = 0;
	int tmonth = 0;
	int tday = 0;
	int D[3] = { 0,0,0 }; //년,월,일이 몇번 등장했는지 저장하는 배열
	int sp = 0;
	int i = 1;
	if (!(0x30 <= str[0] && str[0] <= 0x39))
		return -1; //맨 앞에 숫자가 아니면 안됨
	for (; i != wcslen(str); i++)
	{
		if (0x30 <= str[i] && str[i] <= 0x39) //숫자면 점프
			continue;
		else if (str[i] == 0xB144) //"년" 일경우
		{
			if (D[1] || D[2]) // 월, 일 먼저 입력될경우
				return -1;
			D[0]++; //년이 등장하면 +1
			for (; sp < i; sp++) //앞에 숫자들을 취합
				tyear = tyear * 10 + ((int)str[sp] - 48); //유니코드에서 숫자는 n+48임 
			sp = i + 1;
		}
		else if (str[i] == 0xC6D4) //"월" 일경우
		{
			if (D[2]) //일 먼저 입력될경우
				return -1;
			D[1]++; //월이 등장하면 +1
			for (; sp < i; sp++)
				tmonth = tmonth * 10 + ((int)str[sp] - 48);
			sp = i + 1;
		}
		else if (str[i] == 0xC77C) //"일" 일경우
		{
			D[2]++; //일이 등장하면 +1
			for (; sp < i; sp++)
				tday = tday * 10 + ((int)str[sp] - 48);
			sp = i + 1;
		}
		else
			return -1; //오류
	}
	//cout << tyear << endl << tmonth << endl << tday << endl << D[0] << endl << D[1] << endl << D[2] << endl; 맞는것인가 테스트
	if (tmonth < 0 || tmonth>99 || tday < 0 || tmonth>99)
		return -4; //입력 범위 초과 월,일이 100을 넘으면 앞에 단위에 영향을 줄 수 있음
	if (D[0] == 1)
	{
		if (D[1] == 1)
		{
			if (D[2] == 1) //년 월 일 모두 있을경우
			{
				if (tyear > 9 && tyear < 100 && str[0] != 48) //년 두자리만 쓰고 선행 0 없을때
					return tyear * 10000 + tmonth * 100 + tday;
				else //아니면 정상적으로 출력
					return tyear * 10000 + tmonth * 100 + tday;
			}
			else
				return -2; //일 입력 한번이 아님
		}
		else
			return -3;// 월 입력 한번이 아님
	}
	else if (D[0] == 0)
	{
		if (D[1] == 1)
		{
			if (D[2] == 1) //월,일만 입력했을경우
			   //return (today / 10000) * 10000 + tmonth * 100 + tday;
			{ //가까운 미래를 출력해주기위한 부분
				int strnum = tmonth * 100 + tday;
				if (today % 10000 == strnum)
					return today;
				else if (today % 10000 > strnum)
					return ((today / 10000 + 1) * 10000 + strnum);
				else
					return ((today / 10000) * 10000) + strnum;
			}
			else
				return -5; //일 입력 없을때
		}
		else if (D[1] == 0)
			return -8;//년 월 없음
		else
			return -7; //월을 여러번 입력했을때
	}
	else
		return -8; //년 오류
}

//Purpose : Classifying type of array & Check error
int Classifying(const wchar_t* str, string strM) {
	/*
	cout << "Clsfy" << endl;*/
	int checkType[5] = { 0 };
	int result = 0;

	wchar_t checkDel[30] = { 0 };
	int del = 0;        //특수문자 2개 입력 중 같은 특수문자인지 검사.

	int Sflag = 1, Bflag = 1;//연속 2번 특수문자,띄어쓰기 검출 플래그

	for (int i = 0; i < (unsigned)wcslen(str); i++)
	{
		//Korean
		if ((0xAC00 <= str[i]) && (str[i] <= 0xD7A3))
			//||(0x3131 <= str[i]))&& str[i] <= 0x318E)
		{
			checkType[0]++;
			Sflag = 0;
			Bflag = 0;
		}
		//Numeric
		else if ((0x30 <= str[i] && str[i] <= 0x39))
		{
			checkType[1]++;
			Sflag = 0;
			Bflag = 0;
		}
		// . (0x2e) / (0x2f) - (0x2d)
		else if (((str[i] == 0x2e || str[i] == 0x2f) || str[i] == 0x2d) && (Sflag == 0))
		{
			checkType[2]++;
			Sflag = 1;
			Bflag = 0;

			checkDel[del++] = str[i];


			if (checkType[2] == 2)
			{
				if (checkDel[0] != checkDel[1])
				{
					return -1;
				}
			}

			if (checkType[2] > 2)
			{
				checkType[4]++;

			}
		}
		// Blank (0x20)
		else if (str[i] == 0x20 && Bflag == 0)
		{
			checkType[3]++;
			Sflag = 0;
			Bflag = 1;
		}
		// Error
		else
		{/*
		 cout << "error spot is [" << i << "]" << endl;*/
			checkType[4]++;
			Sflag = 0;
			Bflag = 0;
		}
	}

	// Insert values to classify type of string
	//—————————————————————————————————————
	// Certain type detail
	// K - Korean, N - Numeric, S - speacial chr, B - Blank

	// Return value is
	// -1 : error , 1 : K , 2 : K+N , 3 : K+N+B , 4: K+B , 5 : N , 6 : N+B , 7 : N+S

	// Error (arr[4] != 0)
	if (checkType[4])
	{
		return -1;
	}

	//Including Korean
	else if (checkType[0])
	{
		//Including Speacial char K+S
		if (checkType[2])
		{
			return -1;
		}
		//Including Numeric
		if (checkType[1]) {
			//Including Blank
			if (checkType[3])
			{
				result = Num_KNB(str);
				return result;
			}
			result = Num_KN(str);
			return result;
		}
		//Excluding Numeric, Including Blank
		else if (checkType[3]) {
			result = Num_KB(strM);
			return result;
		}
		result = Num_K(str);
		return result;
	}

	// Numeric , Excluding Korean
	else if (checkType[1])
	{
		//Including Speacial char
		if (checkType[2])
		{
			//Incuding S B simultaneously
			if (checkType[3])
			{
				return -1;
			}
			result = Num_NS(str);
			return result;
		}
		//Including Blank
		else if (checkType[3])
		{
			result = Num_NS(str);
			return result;
		}
		//Just Numeric                                             
		result = Num_N(str);
		return result;

	}
	else {
		return -1;
	}
	return 0;
}

void PrintDate(int dates, int duedate)//duedate 까지 dates일 남았다는 정보를 출력하기 위한 출력함수
{
 //cout << "PrintD" << endl;//함수가 작동하는지 확인해보기 위한 출력구문
	//입력받은 날짜를 년도, 월, 일로 쪼개는 작업
	int FinalYear = duedate / 10000;
	int FinalYearTemp = FinalYear;//FinalYear변수를 보존하기 위한 임시변수(변경하기 위한 변수)
	int FinalMonth = (duedate % 10000) / 100;
	int FinalDay = duedate % 100;


	string DayOfTheWeek;
	int week;   // 일(0), 월(1), 화(2), 수(3), 목(4), 금(5), 토(6)                                                
	int a, b, c, d;   // a: 년도의 앞 두자리, b: 년도의 뒤 두자리                                                

	c = FinalMonth;
	if (c == 1 || c == 2)
	{
		FinalYearTemp -= 1;
		c = c + 12;
	}
	else
		c = c;

	d = FinalDay;

	a = FinalYearTemp / 100;
	b = FinalYearTemp % 100;
	// 입력받은 날짜의 요일을 구하기 위한 제라의 공식                                                 
	week = ((21 * a) / 4 + (5 * b) / 4 + 26 * (c + 1) / 10 + d - 1) % 7;
	/*-
	a: 년도의 앞 두자리
	#NAME?
	#NAME?
	- d: 일      */

	switch (week)
	{
	case 0:
		DayOfTheWeek = "일";
		break;
	case 1:
		DayOfTheWeek = "월";
		break;
	case 2:
		DayOfTheWeek = "화";
		break;
	case 3:
		DayOfTheWeek = "수";
		break;
	case 4:
		DayOfTheWeek = "목";
		break;
	case 5:
		DayOfTheWeek = "금";
		break;
	case 6:
		DayOfTheWeek = "토";
		break;
	}

	string Ann[12];//인자로 받은 날짜가 기념일인지 아닌지 확인하기 위한 기념일 배열
	int Anncheck = 12;
	Ann[0] = "신정";
	Ann[1] = "발렌타인데이";
	Ann[2] = "삼일절";
	Ann[3] = "화이트데이";
	Ann[4] = "어린이날";
	Ann[5] = "어버이날";
	Ann[6] = "현충일";
	Ann[7] = "제헌절";
	Ann[8] = "광복절";
	Ann[9] = "개천절";
	Ann[10] = "한글날";
	Ann[11] = "성탄절";
	int Anndate[] = { 101,214,301,314,505,508,606,717,815,1003,1009,1225 };//기념일의 월,일정보
	for (int i = 0; i < 12; i++)//기념일을 입력받았다면 Anncheck변수에 몇번째 기념일인지 확인
	{
		if (duedate % 10000 == Anndate[i])//입력받은 날짜의 월일정보와 기념일 월일정보를 비교, 맞다면 저장 후 반복문 탈출, 아니라면 모두 비교 후 저장하지 않고 탈출
		{
			Anncheck = i;
			break;
		}
	}
	if (duedate == today)//오늘
	{
		if (Anncheck < 12)//입력받은 날짜가 기념일일시 출력구문
		{
			cout << FinalYear << "년 " << FinalMonth << "월 " << FinalDay << "일 " << DayOfTheWeek << "요일 " << Ann[Anncheck] << "까지 D" << -dates << " 일 남았습니다" << endl;
			cout << endl;
		}
		else//입력받은 날짜가 기념일이 아닐시 출력구문
		{
			cout << FinalYear << "년 " << FinalMonth << "월 " << FinalDay << "일 " << DayOfTheWeek << "요일 까지 D-0 일 남았습니다" << endl;
			cout << endl;
		}
	}
	else if (duedate < today)//과거의 날
	{
		if (Anncheck < 12)//입력받은 날짜가 기념일일시 출력구문
		{
			cout << FinalYear << "년 " << FinalMonth << "월 " << FinalDay << "일 " << DayOfTheWeek << "요일 " << Ann[Anncheck] << "까지 D+" << -dates << " 일 남았습니다" << endl;
			cout << endl;
		}
		else//입력받은 날짜가 기념일이 아닐시 출력구문
		{
			cout << FinalYear << "년 " << FinalMonth << "월 " << FinalDay << "일 " << DayOfTheWeek << "요일 까지 D+" << abs(dates) << "일 남았습니다" << endl;
			cout << endl;
		}
	}
	else//미래의 날
	{
		if (Anncheck < 12)//입력받은 날짜가 기념일일시 출력구문
		{
			cout << FinalYear << "년 " << FinalMonth << "월 " << FinalDay << "일 " << DayOfTheWeek << "요일 " << Ann[Anncheck] << "까지 D" << -dates << " 일 남았습니다" << endl;
			cout << endl;
		}
		else//입력받은 날짜가 기념일이 아닐시 출력구문
		{
			cout << FinalYear << "년 " << FinalMonth << "월 " << FinalDay << "일 " << DayOfTheWeek << "요일 까지 D" << -dates << "일 남았습니다" << endl;
			cout << endl;
		}
	}
}

int CalcDate(int num)
{
	/*cout << "CalcD" << endl;*/
	int today_year = today / 10000; // 8자리 날짜에서 년도 4자리 추출 (현재 날짜)
	int today_month = (today % 10000) / 100; // 8자리 날짜에서 월 2자리 추출
	int today_day = today % 100; //8자리 날짜에서 일 2자리 추출

	int today_year2 = num / 10000; // d-day가 궁금한 날짜
	int today_month2 = (num % 10000) / 100;
	int today_day2 = num % 100;
	int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//각 달이 며칠까지 있는지를 배열에 저장
	int i;
	int total = 0;
	int total2 = 0;
	total = (today_year - 1) * 365 + (today_year - 1) / 4 - (today_year - 1) / 100 + (today_year - 1) / 400; //날짜 계산식
	if (!(today_year % 4) && today_year % 100 || !(today_year % 400)) //윤년 판단
		months[1]++; //윤년일 시 2월달 일수 조정
	for (i = 0; i < today_month - 1; i++)
		total += months[i];
	total += today_day;

	total2 = (today_year2 - 1) * 365 + (today_year2 - 1) / 4 - (today_year2 - 1) / 100 + (today_year2 - 1) / 400;
	if (!(today_year2 % 4) && today_year2 % 100 || !(today_year2 % 400))
		months[1]++;
	for (i = 0; i < today_month2 - 1; i++)
		total2 += months[i];
	total2 += today_day2;

	return total2 - total; //원하는 날짜의 계산값-현재 날짜의 계산값
}

bool NumTest(int chdate)
{/*
 cout << "Numtest" << endl;*/
 //날짜를 뜻하는 숫자를 각각 연도와 달 일로 구분하여 저장                                                   
	int year, temp, month, date;
	year = chdate / 10000;
	temp = chdate % 10000;
	month = temp / 100;
	date = temp % 100;

	//연도, 달 범위 체크                                                
	if (year < 1583 || year>4000)
	{/*
	 cout << "연도의 값은 1583과 4000사이의 값이여야 합니다." << endl;*/
		return false;
	}
	if (month < 1 || month >12)
	{/*
	 cout << "달의 값은 1과 12사이의 값이여야 합니다." << endl;*/
		return false;
	}

	//마지막 날이 31일인 달과 30일인 달 그리고 2월을 따로 구분하여 일수 범위 체크                                                
	//마지막날이 31일인 경우                                                
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		if (date < 1 || date>31)
		{/*
		 cout << "해당 달의 일수는 1과 31 사이의 값이여야 합니다." << endl;*/
			return false;
		}
		else
			return true;
	}
	//마지막날이 30일인 경우                                                
	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		if (date < 1 || date>30)
		{/*
		 cout << "해당 달의 일수는 1과 30 사이의 값이여야 합니다." << endl;*/
			return false;
		}
		else
			return true;
	}
	//2월인 경우                                                
	else
	{
		//윤년인 경우                                             
		if (IsLeapYear(year))
		{
			if (date < 1 || date>29)
			{/*
			 cout << "윤년일 경우 2월의 일수는 1과 29 사이의 값이여야 합니다." << endl;*/
				return false;
			}
			else
				return true;
		}
		//평년인 경우                                             
		else
		{
			if (date < 1 || date>28)
			{/*
			 cout << "평년일 경우 2월의 일수는 1과 28 사이의 값이여야 합니다." << endl;*/
				return false;
			}
			else
				return true;
		}
	}
}
int main(int argc, const char * argv[])
{
	wstring wstr;//입력을 저장하기 위한 wstring 변수
	const wchar_t* wcstr;//입력을 저장하기 위한 wchar_t* 변수
	string strM;//입력을 저장하기 위한 string 변수
	int csf, dates;//함수의 반환된 자료를 다음 함수로 넘겨주기 위한 변수
	while (true)
	{
		struct tm date;//현재 날짜를 받아온 뒤 today변수에 저장하기 위한 변수
		time_t curTime;
		time(&curTime);
		date = *localtime(&curTime);

		int year = date.tm_year + 1900;
		int month = date.tm_mon + 1;
		int day = date.tm_mday;

		today = year * 10000 + month * 100 + day;//today에 현재 날짜를 저장

		cout << "날짜를 입력해주세요." << endl << "(도움말: H  프로그램 종료: Q)" << endl;
		locale::global(locale(""));
		getline(wcin, wstr);//입력을 wstring 변수에 저장
		wcstr = wstr.c_str();//wchar_t* 형식으로 변환 후 저장
		strM = wcs_to_mbs(wstr);//string 형식으로 변환 후 저장

		if (wstr.length() > 30)//입력문자열이 30자를 초과할 경우 오류메세지 출력 후 반복문 처음부터 출력
		{
			cout << "오류입니다." << endl;
			cout << endl;
			continue;
		}
		else//Q를 입력받으면 프로그램 종료, H를 입력받으면 도움말창 출력
		{
			if (strM.compare("Q") == 0)
				exit(0);
			else if (strM.compare("H") == 0)
			{
				ShowHelp();
				continue;
			}
		}
		csf = Classifying(wcstr, strM);//입력받은 문자열을 Classifying함수로 넘겨준뒤, 반환값을 int형 변수 csf에 저장
		//cout << csf << endl;//csf값이 정상적으로 입력되었는지 확인하기 위한 출력구문
		if (csf == -1)//csf에서 오류를 반환하였을때 오류구문 출력
		{
			cout << "오류입니다." << endl;
			cout << endl;
			continue;
		}
		if (NumTest(csf) == 1)//csf에서 오류가 아닌 값을 반환하였을때 년/월/일이 유효한 값인지 검사
		{
			dates = CalcDate(csf);//유효한 값이라면 CalcDate함수에서 D-Day를 계산
			PrintDate(dates, csf);//계산된 D-Day값과 입력받은 날짜를 PrintDate함수에서 출력
		}
		else//유효한 년/월/일이 아닌 경우 오류 메세지 출력
		{
			cout << "오류입니다." << endl;
			cout << endl;
		}
	}

	////Num_K테스트
	//cout << "입력인자" << endl;
	//const wchar_t *TK[20];//testcase 입력인자 배열
	//int AS[20] = { 20190101,20190214,20190301,20190314,20190505,20190508,20190606,20190717,20190815,20191003,20191009,20181225,0,20181014,20181012,20181010,0,0,0};//assumption배열
	//int RS[20];//result배열
	//char TF[20];//truefalse배열
	//TK[0] = L"신정";
	//TK[1] = L"발렌타인데이";
	//TK[2] = L"삼일절";
	//TK[3] = L"화이트데이";
	//TK[4] = L"어린이날";
	//TK[5] = L"어버이날";
	//TK[6] = L"현충일";
	//TK[7] = L"제헌절";
	//TK[8] = L"광복절";
	//TK[9] = L"개천절";
	//TK[10] = L"한글날";
	//TK[11] = L"성탄절";
	//TK[12] = L"크리스마스";
	//TK[13] = L"내일";
	//TK[14] = L"어제";
	//TK[15] = L"그끄저께";
	//TK[16] = L"빼빼로데이";
	//TK[17] = L"한글";
	//TK[18] = L"신 정";
	//TK[19] = L"발렌타인 데이";
	//locale::global(locale(""));
	//for (int j = 0; j < 20; j++)
	//{
	//	wcout << TK[j] << " ";
	//}
	//cout << endl;
	//cout << "예상결과,결과값,T/F 순 출력" << endl;
	//for (int i = 0; i < 20; i++)
	//{
	//	RS[i] = Num_K(TK[i]);
	//	if (AS[i] == RS[i])
	//	{
	//		TF[i] = 'T';
	//	}
	//	else {
	//		TF[i] = 'F';
	//	}
	//	cout << AS[i] << "  " << RS[i] << "  " << TF[i] << endl;
	//}

	//Num_KN 테스트용 코드
	/*const wchar_t* str[20]; //입력 배열
	str[0] = L"10000년0월0일";
	str[1] = L"0년0월0일";
	str[2] = L"0년100월0일";
	str[3] = L"0년0월100일";
	str[4] = L"1000년1월1일";
	str[5] = L"100년1월1일";
	str[6] = L"1년1월1일";
	str[7] = L"1000년100월1일";
	str[8] = L"1000년1월100일";
	str[9] = L"1000년1일";
	str[10] = L"1000년1월";
	str[11] = L"1월1일";
	str[12] = L"1일";
	str[13] = L"1월";
	str[14] = L"1000년";
	str[15] = L"1월1000년1일";
	str[16] = L"1일1월";
	str[17] = L"12월1일";
	str[18] = L"2018년12월1일";
	str[19] = L"10월10일";
	int test[20]; //출력 예상 배열
	test[0] = 100000000;
	test[1] = 00000000;
	test[2] = -4;
	test[3] = -4;
	test[4] = 10000101;
	test[5] = 1000101;
	test[6] = 10101;
	test[7] = -4;
	test[8] = -4;
	test[9] = -3;
	test[10] = -2;
	test[11] = 20190101;
	test[12] = -8;
	test[13] = -5;
	test[14] = -3;
	test[15] = -1;
	test[16] = -1;
	test[17] = 20181201;
	test[18] = 20181201;
	test[19] = 20191010;
	locale::global(locale(""));
	cout << "Num_KN 테스트!" << endl;
	cout << "예상 결과     입력값   예상   실제리턴값" << endl << endl;
	for (int i = 0; i < 20; i++)
	{
		int csf = Num_KN(str[i]);
		if (csf == test[i])
			cout << "예상적중!";
		else
			cout << "실패ㅠㅠ";
		wcout << "    " << str[i];
		cout << "   " << test[i] << "   " << csf << "   " << endl; //입력 예상 실제리턴 출력

	}
	return 0;*/

	////Num_KNB테스트
	//cout << "입력인자" << endl;
	//const wchar_t *TK[142];//testcase 입력인자 배열
	//int AS[142] = { 20190101,20190214,20190301,20190314,20190505,20190508,20190606,20190717,20190815,20191003,20191009,20181225,-1,20181014,20181012,20181010,-1,-1,20181010,-1,-1,10000101,15900101,101,780101,-1,-1,-1,-1,-1,20181026,-1,-1,-1,-1,-1,-1,20181026,-1,20181040,-1,20180229,20190340,10101,-1,19950114,20140114,20181214,20189924,-1,-1,-1,-1,-1,20181032,-1,-1,-1,20171312,-1,15830101,-1,-1,20191030,20190505,20190508,20181212,20190717,20191225,20191003,20191009,  20181201,-1,-1,20181012,-1,-1,20181208,-1,-1,20190101,190101,-1,-1,-1,-1,-1,-1,190229,90329,-1,-1,-1,-1,-1,-1,-1,-1,1181003,0/*100개*/,20181020, 20171020, 20161020, -1, -1, 20191020, -1, 20201020, -1, -1, 20181020, 20191020, -1, 20181002, -1, -1, -1, -1, -1, -1,-1,-1,-1, 201020, -1, -1, 20181020, -1, -1, -1, -1, -1, -1, -1, 20181006, 20181005, -1, -1, -1, -1, -1, -1 };//assumption배열
	//int RS[142];//result배열
	//char TF[142];//truefalse배열
	//TK[0] = L"2019년 1월 1일";
	//TK[1] = L"2019년 2월 셋째 주 목요일";
	//TK[2] = L"2019년 3월 1째 주 금요일";
	//TK[3] = L"2019년 3월 14일";
	//TK[4] = L"2019년 5월 05일";
	//TK[5] = L"2019년 05월 8일";
	//TK[6] = L"2019년 6월 6일";
	//TK[7] = L"7월 17일";
	//TK[8] = L"2019년 8월 15일";
	//TK[9] = L"2019년 10월 3일";
	//TK[10] = L"10월 09일";
	//TK[11] = L"2018년 12월 25일";
	//TK[12] = L"2018년 1225";
	//TK[13] = L"2018년 10월 14일";
	//TK[14] = L"2018년10월 12일";
	//TK[15] = L"2018년 10월10일";
	//TK[16] = L"2018년 10일 10월";
	//TK[17] = L"2018월 10년 10일";
	//TK[18] = L"올해 10월 10일";
	//TK[19] = L"2018년 1월";
	//TK[20] = L"1590년 1월 1월";
	//TK[21] = L"1000년 1월 1일";
	//TK[22] = L"1590년 1월 1일";
	//TK[23] = L"0000년 1월 1일";
	//TK[24] = L"0078년 1월 1일";
	//TK[25] = L"2018년 10월 셋째 주";
	//TK[26] = L"2018년 10월 8째 주 수요일";
	//TK[27] = L"2018년 10월 여덟째 주 수요일";
	//TK[28] = L"2018년 10월 다섯째 주 목요일";
	//TK[29] = L"2018년 10월 넷째 주 삼요일";
	//TK[30] = L"2018년 10월 넷째 주 금요일";
	//TK[31] = L"2018년 10월 3째 금요일";
	//TK[32] = L"올해 10월 셋째 주";
	//TK[33] = L"올해 10월 8째 주 수요일";
	//TK[34] = L"올해 10월 여덟째 주 수요일";
	//TK[35] = L"올해 10월 다섯째 주 목요일";
	//TK[36] = L"올해 10월 넷째 주 삼요일";
	//TK[37] = L"올해 10월 넷째 주 금요일";
	//TK[38] = L"올해 10월 3째 금요일";
	//TK[39] = L"올해 10월 40일";
	//TK[40] = L"올 해 3월 첫째 주 토요일";
	//TK[41] = L"올해 2월 29일";
	//TK[42] = L"2019년 3월 40일";
	//TK[43] = L"1년 1월 1일";
	//TK[44] = L"1995 년 1월 14일";
	//TK[45] = L"1995년 1월14일";
	//TK[46] = L"2014년1월 14일";
	//TK[47] = L"12월 14일";
	//TK[48] = L"99월 24일";
	//TK[49] = L"19 15년 1월 14일";
	//TK[50] = L"2000년 0001월 14일";
	//TK[51] = L"99999월 14일";
	//TK[52] = L"1월 1일일";
	//TK[53] = L"1월 1일 2015년";
	//TK[54] = L"2018년 10월 32일";
	//TK[55] = L"00001년 1월 1일";
	//TK[56] = L"내년 1월 7째 주 월요일";
	//TK[57] = L"후년 3월 1째 토요일";
	//TK[58] = L"작년 13월 12일";
	//TK[59] = L"2018년 1월 첫째 주 일요일";
	//TK[60] = L"1583년 1월 1일";
	//TK[61] = L"1583년 1월 1째 주 일요일";
	//TK[62] = L"2018년 3월";
	//TK[63] = L"내년 10월 30일";
	//TK[64] = L"2019년 5월 05일";
	//TK[65] = L"2019년 05월 8일";
	//TK[66] = L"올해 12월 셋째 주 수요일";
	//TK[67] = L"7월 17일";
	//TK[68] = L"내년 12월 25일";
	//TK[69] = L"2019년 10월 3일";
	//TK[70] = L"10월 09일";
	//TK[71] = L"12월 첫째 주 토요일";
	//TK[72] = L"2018년 1225";
	//TK[73] = L"2018 12월 3일";
	//TK[74] = L"2018년10월 12일";
	//TK[75] = L"2018년 12일 10월";
	//TK[76] = L"2018년 10일 10월";
	//TK[77] = L"올해 12월 8일";
	//TK[78] = L"올해 9월";
	//TK[79] = L"2018년 1월";
	//TK[80] = L"2019년 01월 1일";
	//TK[81] = L"019년 1월 1일";
	//TK[82] = L"작년 3째 주 수요일";
	//TK[83] = L"11월 02째 주 목요일";
	//TK[84] = L"11월 첫째 주 월요일";
	//TK[85] = L"2019년3월셋째 주 수요일 ";
	//TK[86] = L"11월 셋째 주수요일";
	//TK[87] = L"11월 0020일";
	//TK[88] = L"19년 2월 29일";
	//TK[89] = L"9년 3월 29일";
	//TK[90] = L"10월 2018년 20일";
	//TK[91] = L"2018년 셋째 주 수요일";
	//TK[92] = L"내년 성탄절";
	//TK[93] = L"3월 0첫째 주 목요일";
	//TK[94] = L"12월 0째 주 일요일";
	//TK[95] = L"2째 주 수요일";
	//TK[96] = L"다음 주 화요일";
	//TK[97] = L"다다다음 주 일요일";
	//TK[98] = L"118년 10월 3일";
	//TK[99] = L"0년 0월 0일";
	//TK[100] = L"올해 10월 20일";
	//TK[101] = L"작년 10월 20일";
	//TK[102] = L"재작년 10월 20일";
	//TK[103] = L"제작년 10월 20일";
	//TK[104] = L"재재작년 10월 20일";
	//TK[105] = L"내년 10월 20일";
	//TK[106] = L"올내년 10월 20일";
	//TK[107] = L"후년 10월 20일";
	//TK[108] = L"내후년 10월 20일";
	//TK[109] = L"내년년 10월 20일";
	//TK[110] = L"10월 20일";
	//TK[111] = L"내년10월 20일";
	//TK[112] = L"올해 10월월요일 20일";
	//TK[113] = L"올해 10월 02일";
	//TK[114] = L"올해 10월월 20일";
	//TK[115] = L"올해 10월 20일입니다";
	//TK[116] = L"2018년 10월 100일";
	//TK[117] = L"2018년 10월 십일";
	//TK[118] = L"2018년 10월 이0일";
	//TK[119] = L"올해 10월 이십일";
	//TK[120] = L"올해 영2월 20일";
	//TK[121] = L"올해 시월 20일";
	//TK[122] = L"20000년 10월 20일";
	//TK[123] = L"20년 10월 20일";
	//TK[124] = L"올해 열하나월 20일";
	//TK[125] = L"올해 100월 20일";
	//TK[126] = L"올해 10월20일";
	//TK[127] = L"올해 10월 월요일";
	//TK[128] = L"올해 10일 일요일";
	//TK[129] = L"올해 10월 20일일";
	//TK[130] = L"올해 10월월 20일";
	//TK[131] = L"올해 20일";
	//TK[132] = L"올해 10월 첫째 금요오일";
	//TK[133] = L"올해 10월 첫째 주 일요일";
	//TK[134] = L"올해 10월 첫째 주 토요일";
	//TK[135] = L"올해 10월 첫째 주 금요일";
	//TK[136] = L"올해 10월 다섯째 주 금요일";
	//TK[137] = L"올해 10월 열다섯째 주 금요일";
	//TK[138] = L"올해 10월 여섯째 주 금요일";
	//TK[139] = L"올해 10월 버섯째 주 금요일";
	//TK[140] = L"올해 10월 섯째 주 금요일";
	//TK[141] = L"올해 10월 첫 째 주 금요일";
	//locale::global(locale(""));
	//for (int j = 0; j < 142; j++)
	//{
	//	wcout << TK[j] << " / ";
	//}
	//cout << endl;
	//cout << "예상결과,결과값,T/F 순 출력" << endl;
	//for (int i = 0; i < 142; i++)
	//{
	//	RS[i] = Num_KNB(TK[i]);
	//	if (AS[i] == RS[i])
	//	{
	//		TF[i] = 'T';
	//	}
	//	else {
	//		TF[i] = 'F';
	//	}
	//	cout << AS[i] << "  " << RS[i] << "  " << TF[i] << endl;
	//}

	//	//PrintDate테스트
	//	int TK1[] = { 0,1,10,100,365,500,1000,-1,-10,-100,-365,-500,-1000 };//Testcase 입력인자1
	//	int TK2[] = { 20181013,20181014,20181023,20190120,20191014,20200224,20210708,20181012,20181003,20180705,20171013,20180531,20160117 };//Testcase 입력인자2
	//	char TF[13];
	//	string AS1[] = { "2018년 10월 13일 토요일 까지 D-0일 남았습니다","2018년 10월 14일 일요일 까지 D-1일 남았습니다","2018년 10월 23일 화요일 까지 D-10일 남았습니다","2019년 1월 20일 일요일 까지 D-100일 남았습니다","2019년 10월 14일 월요일 까지 D-365일 남았습니다",
	//"2020년 2월 24일 월요일 까지 D-500일 남았습니다","2021년 7월 8일 목요일 까지 D-1000일 남았습니다","2018년 10월 12일 금요일 까지 D+1일 남았습니다","2018년 10월 3일 수요일 개천절까지 D+10 일 남았습니다","2018년 7월 5일 목요일 까지 D+100일 남았습니다","2017년 10월 13일 금요일 까지 D+365일 남았습니다","2018년 5월 31일 목요일 까지 D+500일 남았습니다","2016년 1월 17일 일요일 까지 D+1000일 남았습니다" };
	//	for (int i = 0; i < 13; i++)
	//	{
	//		cout << TK1[i] <<"<<입력인자1  "<< TK2[i] <<"<<입력인자2"<< endl;
	//		cout << AS1[i] << "    <<예상결과값,아래 결과값" << endl;
	//		PrintDate(TK1[i], TK2[i]);
	//	}
	//	cout << AS1[0]<<AS1[1];//string배열 출력확인


	////CalcDate테스트
	//int arr[10][2] = { {15830101,-159167},{19991231,-6882},{20000101,-6881},{20181014,0}
	//	  ,{39991231,723623},{19961206,-7982},{20181225,72},{20200101,444},{30000101,358382},{20000106,-6856} };
	//cout << "날짜" << setw(17) << " d-day" << setw(19) << "예상결과" << setw(19) << "요일" << endl << endl;
	//for (int i = 0; i < 10; i++)
	//{
	//	cout << arr[i][0] << setw(15);
	//	cout << CalcDate(arr[i][0]) << setw(15);
	//	cout << arr[i][1] << setw(15);
	//	PrintDate(CalcDate(arr[i][0]), arr[i][0]);
	//}


	//NumTest, IsLeapYear, Num_KB 의 테스트를 위한 구문이다.
   ////int str[54][2] =
   ////{
   ////   { 15820101, 0 },
   ////   { 15821231, 0 },
   ////   { 15830101, 1 },
   ////   { 40000101, 1 },
   ////   { 40001231, 1 },
   ////   { 40010101, 0 },
   ////   { 20180001, 0 },
   ////   { 20180101, 1 },
   ////   { 20181201, 1 },
   ////   { 20181231, 1 },
   ////   { 20181301, 0 },

   ////   { 20180100, 0 },
   ////   { 20180101, 1 },
   ////   { 20180131, 1 },
   ////   { 20180132, 0 },
   ////   { 20180700, 0 },
   ////   { 20180701, 1 },
   ////   { 20180731, 1 },
   ////   { 20180732, 0 },
   ////   { 20180800, 0 },
   ////   { 20180801, 1 },
   ////   { 20180831, 1 },
   ////   { 20180832, 0 },
   ////   { 20181200, 0 },
   ////   { 20181201, 1 },
   ////   { 20181231, 1 },
   ////   { 20181232, 0 },

   ////   { 20180400, 0 },
   ////   { 20180401, 1 },
   ////   { 20180430, 1 },
   ////   { 20180431, 0 },
   ////   { 20180600, 0 },
   ////   { 20180601, 1 },
   ////   { 20180630, 1 },
   ////   { 20180631, 0 },
   ////   { 20180900, 0 },
   ////   { 20180901, 1 },
   ////   { 20180930, 1 },
   ////   { 20180931, 0 },
   ////   { 20181100, 0 },
   ////   { 20181101, 1 },
   ////   { 20181130, 1 },
   ////   { 20181131, 0 },
   ////   { 20160200, 0 },
   ////   { 20160201, 1 },
   ////   { 20160229, 1 },
   ////   { 20160230, 0 },
   ////   { 20180200, 0 },
   ////   { 20180201, 1 },
   ////   { 20180228, 1 },
   ////   { 20180229, 0 },
   ////   { 201800101, 0 },
   ////   { 201801001, 0 },
   ////   { 2180229, 0 }
   ////};

   ////int str[14][2] = 
   ////{ 
   ////   {0, 1},
   ////   {1, 0},
   ////   {2, 0},
   ////   {4, 1},
   ////   {8, 1},
   ////   {100, 0},
   ////   {200, 0},
   ////   {400, 1},
   ////   {800, 1},
   ////   {1583, 0},
   ////   {2016, 1},
   ////   {2018, 0},
   ////   {2020, 1},
   ////   {4000, 1}
   ////};

   //string str[45][3] =
   //{
   //   { " ", "-1", "20181013" },
   //   { "이번주금요일", "-1", "20181013" },
   //   { "이번주 금요일", "-1", "20181013" },
   //   { "이번 주 금 요일", "-1", "20181013" },
   //   { "이번 주 그음요일", "-1", "20181013" },
   //   { "이번 주 요일", "-1", "20181013" },
   //   { "이번 추 금요일", "-1", "20181013" },
   //   { "이번 주 금효일", "-1", "20181013" },
   //   { "이번 쥬 금요힐", "-1", "20181013" },
   //   { "이번 주 금요일입니다", "-1", "20181013" },

   //   { "따다음 주 금요일", "-1", "20181013" },
   //   { "아다다음 주 금요일", "-1", "20181013" },
   //   { "다음다다 주 금요일", "-1", "20181013" },
   //   { "다다다다음 주 금요일", "-1", "20181013" },
   //   { "다음 주 금요일", "20181019", "20181013" },
   //   { "다다음 주 금요일", "20181026", "20181013" },
   //   { "다다다음 주 금요일", "20181102", "20181013" },

   //   { "쩌저번 주 금요일", "-1", "20181013" },
   //   { "져저저번 주 금요일", "-1", "20181013" },
   //   { "저번저저 주 금요일", "-1", "20181013" },
   //   { "저저저저번 주 금요일", "-1", "20181013" },
   //   { "저번 주 금요일", "20181005", "20181013" },
   //   { "저저번 주 금요일", "20180928", "20181013" },
   //   { "저저저번 주 금요일", "20180921", "20181013" },

   //   { "이번이번 주 금요일", "-1", "20181013" },
   //   { "지난 주 금요일", "-1", "20181013" },
   //   { "이펀 주 금요일", "-1", "20181013" },
   //   { "이번 주 일요일", "20181007", "20181013" },
   //   { "이번 주 월요일", "20181008", "20181013" },
   //   { "이번 주 화요일", "20181009", "20181013" },
   //   { "이번 주 수요일", "20181010", "20181013" },
   //   { "이번 주 목요일", "20181011", "20181013" },
   //   { "이번 주 금요일", "20181012", "20181013" },
   //   { "이번 주 토요일", "20181013", "20181013" },
   //   { "이번 주 삐요일", "-1", "20181013" },

   //   { "저번 주 화요일", "20180925", "20181003" },
   //   { "저번 주 화요일", "20171226", "20180103" },
   //   { "저번 주 화요일", "20181023", "20181103" },
   //   { "저번 주 화요일", "20180220", "20180303" },
   //   { "저번 주 화요일", "20160223", "20160303" },

   //   { "다음 주 금요일", "20180202", "20180127" },
   //   { "다음 주 금요일", "20190104", "20181227" },
   //   { "다음 주 금요일", "20180706", "20180627" },
   //   { "다음 주 금요일", "20180309", "20180227" },
   //   { "다음 주 금요일", "20160304", "20160227" }
   //};

   //cout.width(20);
   //cout << "입력인자";

   //cout.width(10);
   //cout << "현재날짜";

   //cout.width(10);
   //cout << "예상결과";
   //cout.width(10);
   //cout << "실제결과";
   //cout.width(10);
   //cout << "정상여부" << endl << endl;
   //for (int i = 0; i < 45; i++)
   //{
   //   today = stoi(str[i][2]);

   //   cout.width(20);
   //   cout << str[i][0];

   //   cout.width(10);
   //   cout << today;

   //   cout.width(10);
   //   cout << str[i][1];
   //   cout.width(10);
   //   //cout << NumTest(str[i][0]);
   //   //cout << IsLeapYear(str[i][0]);
   //   cout << Num_KB(str[i][0]);
   //   cout.width(10);
   //   //if (str[i][1] == NumTest(str[i][0]))
   //   //if (str[i][1] == IsLeapYear(str[i][0]))
   //   if (stoi(str[i][1]) == Num_KB(str[i][0]))
   //      cout << "정상작동" << endl;
   //   else
   //      cout << "오류" << endl;
   //}
   //_getch();
}