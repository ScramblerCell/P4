/*
Name: Jonah Downing
*/
#include <string>
#include <iostream>
#include <cctype>
#include <limits>

using namespace std;

string ansErrMsg = "[Answer not recognized, please try again!]";
string cmdErrMsg = "[Command not recognized, please try again!]";
string ptErrMsg = "[Not a point value, please try again!]";

//miscFunctions
string makeUpperCase(string input) {
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return  input;
}
void emptyCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
string chooseType() {
    string input;
    cout << "Type of question [mcq/tf/wr]: ";
    cin >> input;
    input = makeUpperCase(input);
    while (input != "MCQ" && makeUpperCase(input) != "WR" && makeUpperCase(input) != "TF") {
        cout << cmdErrMsg << endl << endl;
        cout << "Type of question [mcq/tf/wr]: ";
        emptyCin();
        cin >> input;
        input = makeUpperCase(input);
    }

    return input;
}
double getValidPt(){
    double value;
    while(true){
        cout << "Enter point value: ";
        cin >> value;
        if(cin.fail() || (value < 0)){
        emptyCin();
        cout << ptErrMsg << endl << endl;
        }
        else {
            break;
        }
    
    }
    return value;
}
bool isOkToAddQ() {
    string input;
    cout << "Question saved. Continue? [y/n]: ";
    emptyCin();
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "Y" && input != "N") {
        cout << endl <<cmdErrMsg << endl << "Question saved. Continue? [y/n]: "; 
        getline(cin, input);
        input = makeUpperCase(input);
    }
    if (input == "Y") {return true;} else {return false;}
}
void printWelcome() {
    cout << "*** Welcome to Jonah's Testing Service ***" << endl << endl;
}
void printGoodbye() {
    cout << endl << "*** Thank you for using the testing service, Goodbye! ***" << endl;
}


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
            cout << endl << endl<< "type is " << traverser->type << endl;
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
    string getPrompt() {
        string input;
        cout << endl << "Enter a question: ";
        emptyCin();
        getline(cin, input); 
        cout << endl;
        return input;
    }
    void writeQuestion(string type) {
        QNode* currQ = new QNode(type);
        
        currQ->questionPrompt = getPrompt();

        
        if(type == "TF") {//get correctAns for TF
            string buffer;
            //initial input
            cout << "Select correct Answer: "; 
            getline(cin, buffer);
            buffer = makeUpperCase(buffer);
            //input validation
            while (buffer != "TRUE" && buffer != "FALSE") {
                cout << endl <<ansErrMsg << endl <<"Select correct Answer: "; 
                getline(cin, buffer);
                buffer = makeUpperCase(buffer);
            }
            currQ->correctAns = buffer;
        }
        else if (type == "WR") {//get correctAns for WR
            string buffer;
            //initial input
            cout << "Type correct Answer: "; 
            getline(cin, buffer);
            currQ->correctAns = makeUpperCase(buffer);
        }
        else {

        }
        
        currQ->ptValue = getValidPt();;

        appendToTail(currQ);
    }
};



int main () {
    
    int totalQuestions = 1;
    double totalPts = 0;
    QuestionBank* qBank = new QuestionBank();


    printWelcome();
    bool addStatus = true;
    //writeQuestion loop
    while (addStatus == true) {
        cout << "=== QUESTION " << totalQuestions << " ===" << endl;
        //write question
        qBank->writeQuestion(chooseType());
        //continue?
        totalQuestions++;
        addStatus = isOkToAddQ();

    }

    printGoodbye();

    /* Testing Purposes Below */


    /* //testing appendToTail
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
    qBank->appendToTail(test3); */


    qBank->printQuestionInfo();
}