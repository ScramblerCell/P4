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
    prevQ = nullptr;
    nextQ = nullptr;
}

class QuestionBank {
public:
    QNode* head;
    QNode* tail;
    //constructor
    QuestionBank() {
        head = nullptr;
        tail = nullptr;
    }
    //methods
    void printQuestionInfo() {//test purposes only
        QNode* traverser = head;
        while (traverser != nullptr) {
            cout << "type is " << traverser->type << endl;
            cout << "prompt is " << traverser->questionPrompt <<endl;
            cout << "ptVal is " << traverser->ptValue <<endl;
            cout << "correctAns is " << traverser->correctAns <<endl<<endl;
            traverser = traverser -> nextQ;
        }
    }
    void appendToTail(QNode* newQuestion) {
        if (head == nullptr) {
            head = newQuestion;
            tail = newQuestion;
        } else {
            newQuestion->prevQ = tail;
            newQuestion->nextQ = nullptr;
            tail->nextQ = newQuestion;  
            tail = newQuestion;
        }
    }
    void writeQuestion(string type) {

    }
};

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

    /* Testing Purposes Below */


    //testing appendToTail
    QNode* test1 = new QNode("tf");
    test1->correctAns = "love";
    test1->ptValue = 5;
    test1->questionPrompt = "what is always the answer";
    qBank->appendToTail(test1);

    QNode* test2 = new QNode("tf");
    test2->correctAns = "hate";
    test2->ptValue = 8;
    test2->questionPrompt = "what isnt always the answer";
    qBank->appendToTail(test2);

    QNode* test3 = new QNode("mqc");
    test3->correctAns = "A";
    test3->ptValue = 6;
    test3->questionPrompt = "what isnt always the answer";
    qBank->appendToTail(test3);


    qBank->printQuestionInfo();
}