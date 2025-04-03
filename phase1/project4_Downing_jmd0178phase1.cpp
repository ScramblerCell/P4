/*
Name: Jonah Downing
*/
#include <string>
#include <iostream>
#include <cctype>

using namespace std;
class QNode {
public:
    //properties for each question
    string type;
    string questionPrompt;
    double ptValue;
    string correctAns;
    QNode* prevQ;
    QNode* nextQ;
    //constructor
    QNode(string type);
    //methods

};
QNode::QNode(string type) {//constructor
    this->type = type;
}

class QuestionBank {
public:
    QNode* head;
    QNode* tail;
    //constructor
    QuestionBank();
    //methods
    void printQuestionInfo() {//test purposes only
        QNode* traverser = head;
        while (traverser != nullptr) {
            cout << "type is " << traverser->type << endl;
            cout << "prompt is " << traverser->questionPrompt <<endl;
            cout << "ptVal is " << traverser->ptValue <<endl;
            cout << "correctAns is " << traverser->correctAns <<endl;
            traverser = traverser -> nextQ;
        }
    }
    void appendToTail(QNode* newQuestion) {
        newQuestion->prevQ = tail;
        newQuestion->nextQ = nullptr;
        tail->nextQ = newQuestion;  
        tail = newQuestion;
    }

};
QuestionBank::QuestionBank() {//constructor
    QNode* tempHead = new QNode("");
    tail = tempHead->nextQ;
}

//printFunctions
void printWelcome() {
    cout << "*** Welcome to Jonah's Testing Service ***" << endl << endl;
}
void printGoodbye() {
    cout << "*** Thank you for using the testing service, Goodbye! ***" << endl;
}

int main () {
    int totalQuestions = 0;
    double totalPts = 0;
    string currType;
    QuestionBank* qBank = new QuestionBank();


    printWelcome();




    printGoodbye();

    qBank->printQuestionInfo();
}