#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

template <typename T>
class Stack
{
private:

	template<typename T>
	class Node {
	public:
		Node* pNext;
		T data;

		Node(T data = T(), Node* pNext = nullptr) {
			this->data = data;
			this->pNext = pNext;
		}
	};

	Node<T>* head;
	int Size;
public:
	Stack();
	~Stack();
	void push(T data);
	T getTop();
	T pop();
	bool isEmpty();
	int getSize();
	void clear();
};

// Main programm
bool isOperator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
		return true;
	}
	else return false;
}

bool isHighPriority(char ch) {
	if (ch == '*' || ch == '/') return true;
	else return false;
}

bool isLowPriority(char ch) {
	if (ch == '+' || ch == '-') return true;
	else return false;
}

int getRow(char ch) {
	if (ch == ')') return 1;
	else if (ch == '(') return 2;
	else if (isLowPriority(ch)) return 3;
	else if (isHighPriority(ch)) return 4;
	else return 0;
}

int getCol(char ch) {
	if (ch == '(') return 0;
	else if (isLowPriority(ch)) return 1;
	else if (isHighPriority(ch)) return 2;
	else return 3;
}

string fromInfixToPostfix(string s) {
	const int ROWS = 5;
	const int COLS = 4;
	int codeMatrix[ROWS][COLS] = {
		{1, 1, 1, 1},
		{2, 2, 2, 4},
		{3, 3, 3, 3},
		{3, 2, 2, 3},
		{3, 3, 2, 3},
	};

	Stack<char> opStack;
	string infExpression = s, postfExpression = "";
	int Error = 0; // if Error = 0, everything is ok, else something went wrong

	for (char ch : infExpression) {
		int row = getRow(ch);
		int col;
		if (opStack.isEmpty()) col = 3;
		else col = getCol(opStack.getTop());

		int code = codeMatrix[row][col];

		if (code == 1) postfExpression += ch;
		else if (code == 2) {
			postfExpression += opStack.pop();
			if (ch == ')') {
				if (opStack.isEmpty()) {
					Error = 1;
					break;
				}
				while (opStack.getTop() != '(') {
					postfExpression += opStack.pop();
				}
				opStack.pop();
			}
			else opStack.push(ch);
		}
		else if (code == 3) opStack.push(ch);
		else if (code == 4) {
			Error = 1;
			break;
		}
	}

	while (!(opStack.isEmpty())) {
		if (opStack.getTop() == '(' || opStack.getTop() == ')') {
			Error = 1;
			break;
		}
		else {
			postfExpression += opStack.pop();
		}

	}
	if (Error == 0) {
		return postfExpression;
	}
	else return "ERROR";

}

string fromInfixToPrefix(string s) {
	string infix, prefix = "";
	Stack<string> operandStack;
	Stack<char> operStack;
	infix = s;

	for (char ch : infix) {
		if (ch == '(') {
			operStack.push(ch);
		}
		else if (ch == ')') {
			while (operStack.getTop() != '(') {
				string operand2 = operandStack.pop();
				string operand1 = operandStack.pop();
				string temp = operStack.pop() + operand1 + operand2;
				operandStack.push(temp);
			}
			operStack.pop();
		}
		else if (!(isOperator(ch))) {
			string temp = "";
			temp += ch;
			operandStack.push(temp);
		}
		else {
			if (operStack.isEmpty() || operStack.getTop() == '(') {
				operStack.push(ch);
			}
			else if (isHighPriority(ch) && isLowPriority(operStack.getTop())) {
				operStack.push(ch);
			}
			else if (isHighPriority(ch) == isHighPriority(operStack.getTop()) ||
				isHighPriority(ch) != isHighPriority(operStack.getTop())) {
				string operand2 = operandStack.pop();
				string operand1 = operandStack.pop();
				string temp = operStack.pop() + operand1 + operand2;

				operandStack.push(temp);
				operStack.push(ch);
			}
		}
	}

	while (!(operStack.isEmpty())) {
		string operand2 = operandStack.pop();
		string operand1 = operandStack.pop();
		string temp = operStack.pop() + operand1 + operand2;

		operandStack.push(temp);
	}

	prefix = operandStack.pop();
	return prefix;
}

void makeOperation(Stack<int>& operandStack, Stack<char>& operStack) {
	int operand2 = operandStack.pop();
	int operand1 = operandStack.pop();
	char operation = operStack.pop();

	if (operation == '+') operandStack.push(operand1 + operand2);
	else if (operation == '-') operandStack.push(operand1 - operand2);
	else if (operation == '*') operandStack.push(operand1 * operand2);
	else if (operation == '/') operandStack.push(operand1 / operand2);
}

int calculateInfix(string s) {
	string infix = s;
	string strNum = "";
	int num;
	int Error = 0; // if Error = 0, everything if OK
	Stack<int> operandStack; // Stack with numbers
	Stack<char> operStack; // Stack with operations
	for (char ch : infix) {
		if (ch == '(') operStack.push(ch);
		else if (ch == ')') {
			if (strNum != "") {
				operandStack.push(stoi(strNum));
				strNum = "";
			}
			while (operStack.getTop() != '(') {
				makeOperation(operandStack, operStack);
			}
			operStack.pop();
		}
		else if (!(isOperator(ch))) {
			strNum += ch;
		}
		else {
			if (strNum != "") {
				operandStack.push(stoi(strNum));
				strNum = "";
			}
			if (operStack.isEmpty() || operStack.getTop() == '(') {
				operStack.push(ch);
			}
			else if (isHighPriority(ch) && isLowPriority(operStack.getTop())) {
				operStack.push(ch);
			}
			else {
				makeOperation(operandStack, operStack);

				operStack.push(ch);
			}

		}
	}
	if (strNum != "") operandStack.push(stoi(strNum));
	
	while (!(operStack.isEmpty())) {
		makeOperation(operandStack, operStack);
	}

	return operandStack.pop();
}

int calculatePostfix(string s) {
	string infix = s;
	Stack<int> operandStack;
	Stack<char> operStack;
	string strNum;

	for (char ch : infix) {
		if (ch == ' ') {
			operandStack.push(stoi(strNum));
			strNum = "";
		}
		else if (isOperator(ch)) {
			if (strNum != "") {
				operandStack.push(stoi(strNum));
				strNum = "";
			}
			operStack.push(ch);
			makeOperation(operandStack, operStack);
		}
		else {
			strNum += ch;
		}
	}

	return operandStack.pop();
}

int main() {
	setlocale(LC_ALL, "Russian");

	string infix_1a = "a+b*c-d/f*m";
	string infix_1b = "a+b-f*d+c/m-(n+k*x-z)/w";

	string postfix_1a = fromInfixToPostfix(infix_1a);
	string postfix_1b = fromInfixToPostfix(infix_1b);

	string prefix_1a = fromInfixToPrefix(infix_1a);
	string prefix_1b = fromInfixToPrefix(infix_1b);

	string infix_2a = "12+2*3-16/8*4";
	string infix_2b = "12+(2*3-16/8)*4";

	int result_2a = calculateInfix(infix_2a);
	int result_2b = calculateInfix(infix_2b);

	string postfix_4 = "12 2 3 *16 8 -*+";

	cout << "\tTask 1:" << endl;
	cout << "a) Infix form: " << infix_1a << endl;
	cout << "Postfix form: " << postfix_1a << endl;
	cout << "Prefix form: " << prefix_1a << endl;
	cout << "b) Infix form: " << infix_1b << endl;
	cout << "Postfix form: " << postfix_1b << endl;
	cout << "Prefix form: " << prefix_1b << endl;
	cout << endl;

	cout << "\tTask 2:" << endl;
	cout << "a) Infix form: " << infix_2a << endl;
	cout << "Result: " << result_2a << endl;
	cout << "b) Infix form: " << infix_2b << endl;
	cout << "Result: " << result_2b << endl;
	cout << endl;

	cout << "\tTask4:" << endl;
	cout << "Postfix form: " << postfix_4 << endl;
	cout << calculatePostfix(postfix_4) << endl;
	cout << endl;
}


// Methods of class Stack
template<typename T>
inline Stack<T>::Stack()
{
	Size = 0;
	head = nullptr;
}

template<typename T>
Stack<T>::~Stack()
{
	clear();
}

template<typename T>
void Stack<T>::push(T data)
{
	if (head == nullptr) {
		head = new Node<T>(data);
	}
	else {
		head = new Node<T>(data, head);
	}
	Size++;
}

template<typename T>
T Stack<T>::getTop()
{
	if (head == nullptr) return NULL;
	else return head->data;
}

template<typename T>
T Stack<T>::pop()
{
	Node<T>* temp = head;
	head = head->pNext;
	T tempData = temp->data;

	delete temp;
	Size--;

	return tempData;
}

template<typename T>
bool Stack<T>::isEmpty()
{
	if (Size <= 0) return true;
	else return false;
}

template<typename T>
int Stack<T>::getSize()
{
	return Size;
}

template<typename T>
void Stack<T>::clear()
{
	while (Size) {
		pop();
	}
}