#include "inf_int.h"

/*
Originally written by
컴퓨터공학부
정진경
*/

inf_int::inf_int()
{
	this->digits = new char[2];	// 동적할당

	this->digits[0] = '0';		// default 값 0 설정
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// 음수 처리
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// 숫자를 문자열로 변환하는 과정
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}
	if (i == 0) {	// 숫자의 절댓값이 0일 경우
		new (this) inf_int();	// 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨. 
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	// to be filled 
	// 부호 처리 
	// "100"이 들어왔다면 내부 표현에 맞게 "001"로 변환
	// ex) "-1053" -> thesign=false, digits="3501", len=4
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// 이미 문자열이 있을 경우 제거.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// 부호가 같고, 절댓값이 일치해야함.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	inf_int tmp_a(a);
	inf_int tmp_b(b);

	if (a.length > b.length)
		return true;
	else if (a.length == b.length) {
		std::reverse(tmp_a.digits, &tmp_a.digits[strlen(tmp_a.digits)]);
		std::reverse(tmp_b.digits, &tmp_b.digits[strlen(tmp_b.digits)]);
		for (unsigned int i = 0; i < tmp_a.length; i++) {
			if (tmp_a.digits[i] > tmp_b.digits[i]) {
				return true;
			}
			else if (tmp_a.digits[i] < tmp_b.digits[i]) {
				return false;
			}
		}
	}
	return false;
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// 이항의 부호가 같을 경우 + 연산자로 연산
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// 이항의 부호가 다를 경우 - 연산자로 연산
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c, d;

	c = a;
	d = b;
	if ((c.thesign == false && d.thesign == false)) {
		c = c + d;
		return c;
	}
	else if (c < d) {
		for (unsigned i = 0; i < d.length; i++) {
			d.Sub(c, i);
		}
		d.thesign = false;
		return d;
	}
	else {
		for (unsigned i = 0; i < c.length; i++) {
			c.Sub(d, i);
		}
		return c;
	}
}

/*inf_int operator*(const inf_int& a, const inf_int& b)
{
	// to be filled
*/


std::ostream& operator<<(std::ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, Add(a, 2, 2)의 결과는 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// 할당 실패 예외처리
			std::cout << "Memory reallocation failed, the program will terminate." << std::endl;

			exit(0);
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// 값 연산


	if (this->digits[index - 1] > '9') {	// 자리올림이 발생할 경우
		this->digits[index - 1] -= 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Add('1', index + 1);			// 윗자리에 1을 더한다
	}
}

void inf_int::Sub(inf_int& other, const unsigned int index)
{
	int a, b = 0;

	if (other.length < this->length) {
		other.digits = (char*)realloc(other.digits, (this->length - other.length) + 1);
		if (other.digits == NULL) {
			std::cerr << "Memory reallocation failed, the program will terminate." << std::endl;
			exit(0);
		}
		for (unsigned int i = other.length; i < this->length; i++)
			other.digits[i] = '0';
		other.digits[this->length] = '\0';
		other.length = this->length;
	}

	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {
			std::cout << "Memory reallocation failed, the program will terminate." << std::endl;

			exit(0);
		}

		this->length = index;
		this->digits[this->length] = '\0';
	}

	if (this->digits[index - 1] < '0') {
		this->digits[index - 1] = '0';
	}

	a = (int)this->digits[index] - 48;
	b = (int)other.digits[index] - 48;

	if (other.digits[index] > '9') {
		b = 10;
	}

	if (a < b ) {
		a += 10;
		other.digits[index + 1] += 1;
	}
	a = a - b;
	this->digits[index] = a + 48;
}