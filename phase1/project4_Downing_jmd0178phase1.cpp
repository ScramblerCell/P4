/*
Name: Jonah Downing
*/
#include <string>
#include <iostream>
#include <cctype>
#include <map>
#include <limits>
#include <regex>

using namespace std;

string ansErrMsg = "[Answer not recognized, please try again!]";
string cmdErrMsg = "[Command not recognized, please try again!]";
string ptErrMsg = "[Not a point value, please try again!]";

//miscFunctions
string makeUpperCase(string input) {
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return  input;
}
/* void emptyCin() {//retired
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
} */
string chooseType() {
    string input;
    cout << "Type of question [mcq/tf/wr]: ";
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "MCQ" && makeUpperCase(input) != "WR" && makeUpperCase(input) != "TF") {
        cout << cmdErrMsg << endl << endl;
        cout << "Type of question [mcq/tf/wr]: ";
        getline(cin, input);
        input = makeUpperCase(input);
    }

    return input;
}
double getValidPt() {
    string input;
    double value;
    regex validNumberPattern("^-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?$");

    while (true) {
        cout << "Enter point value: ";
        getline(cin, input);  // Read the full input as a string
        
        // Check if the input matches a valid number format
        if (!(regex_match(input, validNumberPattern))) {
            cout << ptErrMsg << endl << endl;
            continue;  // Skip further processing and ask for input again
        }

        // Convert input to double
        value = stod(input);

        // Check if the value is negative
        if (value < 0) {
            cout << ptErrMsg << endl << endl;
        } else {
            break;  // Break the loop if everything is valid
        }
    }
    return value;
}
bool isOkToAddQ() {
    string input;
    cout << "Question saved. Continue? [y/n]: ";
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "Y" && input != "N") {
        cout <<cmdErrMsg << endl << "Question saved. Continue? [y/n]: "; 
        getline(cin, input);
        input = makeUpperCase(input);
        cout << endl;
    }
    if (input == "Y") {return true;} else {return false;}
}
void printWelcome() {
    cout << "*** Welcome to Jonah's Testing Service ***" << endl;
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
    map<char,string> choices;
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
    string getValidChar(QNode* currQ) {
        string input;
        while (true) {
            cout << "Select correct answer: ";
            getline(cin, input);
    
            // Make sure it's a single alphabet character
            if (input.length() == 1 && isalpha(input[0])) {
                // Normalize to uppercase
                char choice = toupper(input[0]);
    
                // Check if the choice exists in the MCQ map
                if (currQ->choices.find(choice) != currQ->choices.end()) {
                    return string(1, choice); // Return as string
                }
            }
    
            cout << ansErrMsg << endl << endl;
        }
    }
    void printQuestionInfo() {//test purposes only
        QNode* traverser = head;
        while (traverser != nullptr) {
            cout << endl << endl<< "type is " << traverser->type << endl;
            cout << "prompt is " << traverser->questionPrompt <<endl;
            cout << "ptVal is " << traverser->ptValue <<endl;
            cout << "correctAns is " << traverser->correctAns <<endl<<endl;
            if (traverser->choices.empty() == 0) {
                cout << "Multiple Choice Found" << endl << "choices are:"<< endl;
                for (auto choice : traverser->choices) {
                    cout << choice.first << " : " << choice.second << endl;
                }
            }
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
        getline(cin, input); 
        return input;
    }
    void writeQuestion(string type) {
        QNode* currQ = new QNode(type);
        
        currQ->questionPrompt = getPrompt();

        
        if(type == "TF") {//get correctAns for TF
            string buffer;
            //initial input
            cout << endl << "Select correct Answer: "; 
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
        else if (type == "MCQ"){
            //add choices to map
            string choiceInput;
            char currLetter = 'A';
            cout << "[At any time, type ‘quit()’ to exit]" << endl;
            while (true) {
                cout << "Enter choice " << currLetter << ": ";
                getline(cin, choiceInput);
                if (makeUpperCase(choiceInput) == "QUIT()") {
                    break;
                }
                currQ->choices[currLetter] = choiceInput;
                currLetter++;
            }
            cout << endl;
            //get correctAns for MCQ
            currQ->correctAns = getValidChar(currQ);

        }
        else {cout << "!!!type wasn't available!!!";}
        
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
        cout << endl << "=== QUESTION " << totalQuestions << " ===" << endl;
        //write question
        qBank->writeQuestion(chooseType());
        totalQuestions++;
        //continue?
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