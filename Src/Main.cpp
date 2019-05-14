/**
* @file Main.cpp
*/
#include <iostream>


//�錾
int stack[10] = {};
int N = 0;

void push(int);
int pop();

int main() {
	push(10);
	push(30);
	std::cout << pop() << "\n";
	push(20);
	push(100);
	pop();
	std::cout << pop() << "\n";
	std::cout << pop() << "\n";

	int a;
	std::cin >> a;

	return 0;
} 

//��`
/*
* �X�^�b�N�ɒl���v�b�V������.
*/
void push(int data) {
	if (N < sizeof(stack) / sizeof(stack[0])) {
		stack[N] = data;
		N++;
	}
}

/*
* �X�^�b�N����l���|�b�v����.
*/
int pop() {
	int tmp = -1;
	if (N > 0) {
		tmp = stack[N - 1];
		// �Ȃ��Ă悢�B�|�C���^�Ȃ�K�v�@stack[N - 1] = 0; 
		N--;
	}
	return tmp;
}