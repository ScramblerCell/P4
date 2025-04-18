/*
project4_Downing_jmd0178phase1.cpp
Name: Jonah Downing
Auburn UserID: jmd0178
Instructions: cd phase2; g++ project4_Downing_jmd0178phase2.cpp -o phase2;./phase2
*/

#define UNIT_TESTING
#ifdef UNIT_TESTING
#include <string>
#include <iostream>
#include <cctype>
#include <map>
#include <limits>
#include <regex>
#include "project4_Downing_jmd0178phase2.h"
#include <cassert>
using namespace std;
//strings for writingQuestion
string ansErrMsg = "[Answer not recognized, please try again!]";
string cmdErrMsg = "[Command not recognized, please try again!]";
string ptErrMsg = "[Not a point value, please try again!]";
string qExistErrMsg = "[That question does not exist!]";
string contMsg = "Question saved. Continue? [y/n]: ";
//strings for assessment
void printWriteOptions() {
    cout << "Do you want to?" << endl;
    cout << "\t1. Create new question" << endl;
    cout << "\t2. Edit questions" << endl;
    cout << "\t3. Delete question" << endl;
    cout << "\t4. Finish." << endl;
}

string assessmentMsg = "/!\\ Begin assessment? [y/n]: ";
string correctMsg = "[Your answer is correct!]";
void emptyCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void addBreak() {
    cout << endl;
}
int selectIntInRange(int maxNumQuestions, string promptMsg, string errMsg) {
    string buffer;
    int value;
    while (true) {
        cout << promptMsg;
        getline(cin, buffer);

        try {
            value = stoi(buffer);
        } catch (...) {
            cout << errMsg << endl;
            addBreak();
            continue;
        }

        if (value < 1 || value > maxNumQuestions) {
            cout << errMsg << endl;
            addBreak();
        } else {
            break;
        }
        }
    return value;
}
int selectIntInRangeOrQuit(int maxNumQuestions, string promptMsg) {
    
    string value;
    int valueToInt;
    while(true){
        cout << promptMsg;
        getline(cin, value);
        //check if quit()
        if (value == "quit()") {
            return -1;
        }
        //try to convert value to int
        try {   
            valueToInt = stoi(value);
        }
        catch (exception e) {
            cout << ptErrMsg << endl;
            addBreak();
            
            continue;
        }
        //verify in range
        if (valueToInt < 1 || valueToInt > maxNumQuestions) {
            
            cout << "[That question does not exist!]" << endl;
            addBreak();
            continue;
        }
        return valueToInt;
    }
    return -1;
}
//miscFunctions
/**
 * @param input     string to be uppercased
 * @return string:   uppercased string
 */
string makeUpperCase(string input) {
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return input;
}
/**
 * @brief prompts writer to choose question type
 * 
 * @return string of question type (in all caps)
 */
string chooseType() {
    string input;
    cout << "Type of question [mcq/tf/wr]: ";
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "MCQ" && input != "WR" && input != "TF") {
        emptyCin();
        cout << cmdErrMsg << endl;
        addBreak();
        cout << "Type of question [mcq/tf/wr]: ";
        getline(cin, input);
        input = makeUpperCase(input);
    }
    return input;
}
/**
 * @brief prompts user for valid double
 * 
 * @return double to be returned
 */
double getValidPt() {
    string input;
    double value;
    regex validNumberPattern("^-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?$");

    while (true) {
        cout << "Enter point value: ";
        getline(cin, input);  // Read the full input as a string
        
        // Check if the input matches a valid number format
        if (!(regex_match(input, validNumberPattern))) {
            cout << ptErrMsg << endl;
            addBreak();
            continue;  // Skip further processing and ask for input again
        }

        // Convert input to double
        value = stod(input);

        // Check if the value is negative
        if (value < 0) {
            cout << ptErrMsg << endl;
            addBreak();
        } else {
            break;  // Break the loop if everything is valid
        }
    }
    return value;
}
/**
 * @brief prompts user to continue or not
 * 
 * @param prompt    type of question asked
 * @return true     if user inputs 'y'
 * @return false    if user inputs 'n'
 */
bool wantToCont(string prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "Y" && input != "N") {
        cout <<cmdErrMsg << endl;
        addBreak();
        cout << prompt; 
        getline(cin, input);
        input = makeUpperCase(input);
        cout << endl;
    }
    if (input == "Y") {return true;} else {return false;}
}
void printWelcome() {
    addBreak();
    cout << "*** Welcome to Jonah's Testing Service ***" << endl;
}
void printGoodbye() {
    addBreak();
    cout << "*** Thank you for using the testing service, Goodbye! ***" << endl;
}

class QNode {
public:
    //properties for each question
    string type;
    string questionPrompt;
    double ptValue;
    string correctAns;
    string userAns;
    QNode* prevQ;
    QNode* nextQ;
    map<char,string> choices;
    bool isCorrect;
    //constructor
    QNode(string type);
    //methods
    /**
     * @brief compares tester's answer to correct answer
     * 
     * @return true     if same 
     * @return false    if not same
     */
    bool testUserAns() {
        return (correctAns == userAns);
    }
};
QNode::QNode(string type) {//constructor
    this->type = type;
    prevQ = nullptr;
    nextQ = nullptr;
    isCorrect = false;
    userAns = "";
}

class QuestionBank {
public:
    QNode* head;
    QNode* tail;
    int totalQuestions;
    double totalPoints;
    int earnedQuestion; //num questions answered correct
    double earnedPoints; //num questions earned by student

    //constructor
    QuestionBank() {
        head = nullptr;
        tail = nullptr;
        totalQuestions = 0;
        totalPoints = 0;
        earnedPoints = 0;
        earnedQuestion = 0;
    }
    
    //writingQuestion methods
    /**
     * @brief get valid character from user
     * 
     * @param currQ     question node to write character into
     * @return string   charater (not technically char, but just a string of length 1)
     */
    string getValidChar(QNode* currQ) {
        string input;
        while (true) {
            addBreak();
            cout << "Select correct answer: ";
            getline(cin, input);
            ////???
    
            // ensure single alphabet character
            if (input.length() == 1 && isalpha(input[0])) {
                // change to uppercase
                char choice = toupper(input[0]);
    
                // Check if the choice exists in the MCQ map
                if (currQ->choices.find(choice) != currQ->choices.end()) {
                    return string(1, choice); 
                }
            }
            cout << ansErrMsg << endl;
            //addBreak();
        }
    }
    /**
     * @brief FOR TESTING: prints question info for a QuestionBank object
     * 
     */
    void printQuestionInfo() {
        QNode* traverser = head;
        while (traverser != nullptr) {
            cout << endl << endl<< "type is " << traverser->type << endl;
            cout << "prompt is " << traverser->questionPrompt <<endl;
            cout << "ptVal is " << traverser->ptValue <<endl;
            cout << "correctAns is " << traverser->correctAns <<endl<<endl;
            if (traverser->choices.empty() == 0) {
                cout << "Multiple Choice Found" << endl << "choices are:"<< endl;
                for (map<char, string>::iterator iterator = traverser->choices.begin(); iterator != traverser->choices.end(); ++iterator) {
                    cout << iterator->first << " : " << iterator->second << endl;
                }
            }
            cout << "userAns is: " <<traverser->userAns << endl;
            traverser = traverser -> nextQ;
        }
    }
    void printQNodeInfo(QNode* traverser, int QtoEdit) {
        cout << "===============================" << endl;
        cout << "=== QUESTION " << QtoEdit << " SAVED VALUES ===" << endl;
        cout << "===============================" << endl;

        cout << "\t1. Type: " << traverser->type << endl;
        cout << "\t2. Question: " << traverser->questionPrompt <<endl;
        //if mcq, print choices
        if (traverser->type == "MCQ") {
            cout << "\t3. Answer choices: " << endl;
            for (map<char, string>::iterator iterator = traverser->choices.begin(); iterator != traverser->choices.end(); ++iterator) {
                cout << "\t\t" << iterator->first << ". " << iterator->second << endl;
            }
            cout << "\t4. Correct answer: " << traverser->correctAns <<endl;
        } 
        else {
            cout << "\t3. Correct answer: " << traverser->correctAns <<endl;
        }
        cout << "===============================" << endl;
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
    void delQat(int ithQuestion) {
        QNode* traverser = head;
        //find Qnode to be deleted
        moveQNode(traverser, ithQuestion);

        //del node
        if (traverser->prevQ == nullptr) {//QNode is head 
            head = traverser->nextQ;
            if(totalQuestions > 1){
            head->prevQ = nullptr;
            }
        }
        else if (traverser->nextQ == nullptr) {//QNode is tail
            traverser->prevQ->nextQ = nullptr;
        }
        else {//QNode is body
            traverser->prevQ->nextQ = traverser->nextQ;
            traverser->nextQ->prevQ = traverser->prevQ;
        }
        totalQuestions--;
    }


    string getPrompt() {//good
        string input;
        addBreak();
        cout << "Enter a question: ";
        getline(cin, input); 
        return input;
    }
    /**
     * @brief gets "true" or "false" (case insensitive) from user for correctAns
     * 
     * @param currQ     question node being modified
     */
    void getTFcorrectAns(QNode* currQ) {
        string buffer;
        //initial input
        addBreak();
        cout << "Select correct Answer [true/false]: "; 
        getline(cin, buffer);
        buffer = makeUpperCase(buffer);
        //input validation
        while (buffer != "TRUE" && buffer != "FALSE") {
            cout <<ansErrMsg << endl;
            addBreak();
            cout <<"Select correct Answer [true/false]: "; 
            getline(cin, buffer);
            buffer = makeUpperCase(buffer);
        }
        currQ->correctAns = buffer;
    }
    /**
     * @brief gets string from user to write to correctAns
     * 
     * @param currQ     question node being modified
     */
    void getWRcorrectAns(QNode* currQ) {
        string buffer;
        //initial input
        cout << endl << "Type correct Answer: "; 
        getline(cin, buffer);
        currQ->correctAns = makeUpperCase(buffer);
    }
    void getMCQcorrectAns(QNode* currQ) {
        currQ->correctAns = getValidChar(currQ);
    }
    /**
     * @brief gets user to enter multiple choice options and to choose the letter corresponding to the correct option
     * 
     * @param currQ     question node being modified
     * @return true     if at least one option was given; proceed to point assignment
     * @return false    if no option was given
     */
    bool getMCQchoices(QNode* currQ) {//good
        //empty choices
        currQ->choices.clear();
        //add choices to map
        string choiceInput;
        char currLetter = 'A';
        cout << "[At any time, type ‘quit()’ to exit]" << endl;
        addBreak(); 
        while (true) {
            cout << "Enter choice " << currLetter << ": ";
            getline(cin, choiceInput);
            if (makeUpperCase(choiceInput) == "QUIT()") {
                break;
            }
            currQ->choices[currLetter] = choiceInput;
            currLetter++;
        }
        //in case no choices were given
        if (currQ->choices.size() == 0) {
            cout << "ERROR: No Choices were provided. Please Retry" << endl;
            return false;
        }
        return true;
    }
    /**
     * @brief writes question and adds to QuestionBank
     * 
     * @param type      determines which type of question  
     * @return true     if method was successful 
     * @return false    if method was unsuccessful
     */
    bool writeQuestion(string type) {
        QNode* currQ = new QNode(type);
        //get question
        currQ->questionPrompt = getPrompt();
        //get correctAns
        if(type == "TF") {//get correctAns for TF
            getTFcorrectAns(currQ);
        }
        else if (type == "WR") {//get correctAns for WR
            getWRcorrectAns(currQ);
        }
        else if (type == "MCQ"){//add choices & get correctAns
            if (!getMCQchoices(currQ)) return false;
            getMCQcorrectAns(currQ);
        }
        else {cout << "!!!type wasn't available!!!" << endl;}
        //get point value
        currQ->ptValue = getValidPt();
        appendToTail(currQ);//append to questionBank
        totalQuestions++;
        totalPoints += currQ->ptValue;
        return true;
    }
    void createQ() {
        cout << "=== QUESTION " << totalQuestions +1 << " ===" << endl;
        addBreak();
        //write question
        bool success = writeQuestion(chooseType());
    }
    void delQ() {
        //get int
        int QtoDel = selectIntInRange(totalQuestions, "Select a question to delete [1-" + to_string(totalQuestions) + "]: ",qExistErrMsg);
        delQat(QtoDel);
        cout << "Question " << QtoDel << " deleted." << endl;
        addBreak();
    }    
    void moveQNode(QNode* traverser, int targetQNode) {
        for ( int i = 1; i < targetQNode; i++) {
            traverser = traverser->nextQ;
        }
    }
    void editQ() {
        //choose which question to edit
        int QtoEdit = selectIntInRangeOrQuit(totalQuestions, "Select a question to edit, or type quit() [1-" + to_string(totalQuestions) + "]: ");
        //find QNode in linked list
        if (QtoEdit == -1) {
            addBreak();
            return;
        }
        QNode* traverser = head;
        moveQNode(traverser, QtoEdit);
        
        //printQuestion properties
        addBreak();
        printQNodeInfo(traverser, QtoEdit);
        addBreak();
        
        //choose which property to edit
        int maxNumQuestions = 3; 
        if (traverser->type == "MCQ") {//if MCQ, then should be 4
            maxNumQuestions = 4; 
        }
        
        int propToEdit = -2;
        while (true) {
            propToEdit = selectIntInRangeOrQuit(maxNumQuestions, "Select a number to edit, or type quit() [1-" + to_string(maxNumQuestions) + "]: ");
            if (propToEdit == -1) {//stop condition
                break;
            }
            //edit property based on propToEdit
            if (propToEdit == 1) {//edit type
                traverser->type = chooseType();
            }
            else if (propToEdit == 2) {//edit Question
                traverser->questionPrompt = getPrompt();
            }
            else if (traverser->type == "MCQ") {
                if (propToEdit == 3) {//edit choices
                    bool cont = true;
                    while (cont) {
                        cont = getMCQchoices(traverser);
                    }
                }
                else if (propToEdit == 4) {//edit correctAns
                    getMCQcorrectAns(traverser);
                }
            }
            else if (traverser->type == "TF") {//edit TF correctAns
                getTFcorrectAns(traverser);
            }
            else if (traverser->type == "WR") {//edit WR correctAns
                getWRcorrectAns(traverser);
            }
        }
        
    }


    //session log 
    void printWritingLog() {
        addBreak();addBreak(); 
        cout << "=== SESSION LOG ===" <<endl;
        cout << "Total questions: " << totalQuestions << endl;
        cout << "Total point values: " << totalPoints << endl;
    }
    void printAssessLog() {
        cout << "=== SESSION LOG ===" << endl;
        cout << "Correct answers: " << earnedQuestion << "/" << totalQuestions << endl;
            //iterate through linked list
            int i = 1;
            for (QNode* traverser = head; traverser != nullptr; traverser = traverser->nextQ) {
                cout << "\tQuestion " << i << ": " << traverser->correctAns<< endl;
                cout << "\tYour answer: " << traverser->userAns << endl;
                addBreak();
            }
        cout << "Final score: " << earnedPoints << "/" << totalPoints << endl;
    }
    void printAssessMenu(bool hasVisited) {
        int shift = 0;
        if(hasVisited){
            shift++;
        }

        cout << "Do you want to?" << endl;
        if (hasVisited) {
            cout << "\t"<< 1 << ". Edit this answer." << endl;
        }
        cout << "\t"<< 1 + shift << ". Go to next question." << endl;
        cout << "\t" << 2 + shift << ". Jump to question." << endl;
        cout << "\t" << 3 + shift << ". Submit." << endl;
    }
    //assessment
    /**
     * @brief gets "true" or "false" (case insensitive) from user and assigns it to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     * @param input         holds user's input for validation and writing to userAns
     */
    void getTFanswer(QNode* traverser) { 
        string input;
        cout << "Your answer [true/false]: ";
        getline(cin, input);
        //input validation
        input = makeUpperCase(input);
        while (input != "TRUE" && input != "FALSE") {
            cout <<ansErrMsg << endl;
            addBreak(); 
            cout << "Select correct Answer: "; 
            getline(cin, input);
            input = makeUpperCase(input);
        }
        //once valid, update values
        traverser->userAns = input;
        if (traverser->testUserAns()) {
            traverser->isCorrect = true;
            earnedPoints += traverser->ptValue;
            earnedQuestion++;   
        }

    }
    /**
     * @brief gets string from user to assign to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     * @param input         holds user's input for validation and writing to userAns
     */
    void getWRanswer(QNode* traverser) {
        string input;
        cout << "Your answer: ";
        getline(cin, input);
        
       //once valid, update values
       traverser->userAns = makeUpperCase(input);
       if (traverser->testUserAns()) {
        traverser->isCorrect = true;
           earnedPoints += traverser->ptValue;
           earnedQuestion++;   
       } 
    }

    /**
     * @brief prints choices and gets string of length 1 from user and assigns it to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     */
    void getMCQanswer(QNode* traverser) {
        //once valid, update values
        traverser->userAns = getValidChar(traverser);
        if (traverser->testUserAns()) {
            traverser->isCorrect = true;
            earnedPoints += traverser->ptValue;
            earnedQuestion++;   
        } 
    }
    /**
     * @brief starts assessment by traversing through QuestionBank's linked list, testing user inputs against correct answers; ends with session log
     * 
     */
    int getIof(QNode* targetNode) {
        int count = 1;
        QNode* traverser = head;
        while (traverser != targetNode) {
            traverser = traverser->nextQ;
            count++;
        }
        return count;
    }
    void beginQuiz() {
        QNode* currQ = head;
        bool submitNow = false;
        while (!submitNow) {

            addBreak();
            printAssessMenu(!(currQ->userAns).empty());
            addBreak();

            int shift = 0;
            if (!(currQ->userAns).empty()) {//
                shift++;
            }

            //select action
            int currAction = selectIntInRange(3+shift, "Select an action: ", "[Not an action. Try again]");
            addBreak();

            //execute chosen action
            int questionNum;

            switch (currAction - shift) {

                case 0://ask edit
                    if (currQ->isCorrect){
                        earnedPoints-=currQ->ptValue;
                        earnedQuestion--;
                        currQ->isCorrect = false;
                    }
                    //take answer
                if (currQ->type == "TF") {
                    getTFanswer(currQ);
                }
                else if (currQ->type == "WR") {
                    getWRanswer(currQ);
                }
                else if (currQ->type == "MCQ") {
                    getMCQanswer(currQ);
                }
                continue;
                break;
                case 1: //go to next question
                    
                    if (currQ->nextQ == nullptr) {//if currQ is tail
                        currQ = head;
                    }
                    else {
                        currQ = currQ->nextQ;
                    }
                break;
                case 2: //jump to question
                    questionNum = selectIntInRange(totalQuestions, "Jump to question [1-" + to_string(totalQuestions) + "]: ", qExistErrMsg);
                    moveQNode(currQ, questionNum);
                break;
                case 3://submit
                    submitNow = true;
                    continue;
                break;
                default:
                    cout << "currAction wasn't a valid int" << endl;
                break;
            }
            
            //print options
            cout << "Question " << getIof(currQ) << ": " << currQ->questionPrompt << endl;
            if (currQ->type == "MCQ") {
                for (map<char, string>::iterator iterator = currQ->choices.begin(); iterator != currQ->choices.end(); ++iterator) {
                    cout << "\t" << iterator->first << ". " << iterator->second << endl;
                }
            }
            //take answer
            if (currQ->type == "TF") {
                getTFanswer(currQ);
            }
            else if (currQ->type == "WR") {
                getWRanswer(currQ);
            }
            else if (currQ->type == "MCQ") {
                getMCQanswer(currQ);
            }
            
        }
    }
};

//Unit Testing
void TEST_emptyQBank() {
    QuestionBank* qBank = new QuestionBank();
    assert(qBank->totalQuestions == 0);
    cout << "Test Qbank size initialized to zero passed" << endl;
}
void TEST_deleteAllQ() {
    QuestionBank* qBank = new QuestionBank();
    QNode* testQ = new QNode("TF");
    qBank->appendToTail(testQ);
    qBank->delQat(1);
    cout << "Test Qbank added and removed all Question nodes" << endl;
}
void TEST_submitWithNoAnswer() {
    QNode* q1 = new QNode("TF");

    assert(q1->correctAns == "");
    cout << "Test submitting without answer passed" << endl;
}
void TEST_multipleChoicesQuestion() {
    QuestionBank* qBank = new QuestionBank();
    QNode q1("MCQ");
    assert (q1.type == "MCQ");
    cout << "Test if MCQ works passed" << endl;
}
void TEST_TF() {
    QuestionBank* qBank = new QuestionBank();
    QNode q1("TF");
    assert (q1.type == "TF");
    cout << "Test if TF works passed" << endl;
}
void TEST_WR() {
    QuestionBank* qBank = new QuestionBank();
    QNode q1("WR");
    assert (q1.type == "WR");
    cout << "Test if WR works passed" << endl;
}



int main () {
    cout << "***This is a debugging version ***" << endl;
    addBreak();

    TEST_emptyQBank();
    addBreak();
    TEST_deleteAllQ();
    addBreak();
    TEST_submitWithNoAnswer();
    addBreak();
    TEST_multipleChoicesQuestion();
    addBreak();
    TEST_TF();
    addBreak();
    TEST_WR();
    addBreak();

    cout << "***End of the Debugging Version ***" << endl;
}
#else
#include <string>
#include <iostream>
#include <cctype>
#include <map>
#include <limits>
#include <regex>
#include "project4_Downing_jmd0178phase2.h"

using namespace std;
//strings for writingQuestion
string ansErrMsg = "[Answer not recognized, please try again!]";
string cmdErrMsg = "[Command not recognized, please try again!]";
string ptErrMsg = "[Not a point value, please try again!]";
string qExistErrMsg = "[That question does not exist!]";
string contMsg = "Question saved. Continue? [y/n]: ";
//strings for assessment
void printWriteOptions() {
    cout << "Do you want to?" << endl;
    cout << "\t1. Create new question" << endl;
    cout << "\t2. Edit questions" << endl;
    cout << "\t3. Delete question" << endl;
    cout << "\t4. Finish." << endl;
}

string assessmentMsg = "/!\\ Begin assessment? [y/n]: ";
string correctMsg = "[Your answer is correct!]";
void emptyCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void addBreak() {
    cout << endl;
}
int selectIntInRange(int maxNumQuestions, string promptMsg, string errMsg) {
    string buffer;
    int value;
    while (true) {
        cout << promptMsg;
        getline(cin, buffer);

        try {
            value = stoi(buffer);
        } catch (...) {
            cout << errMsg << endl;
            addBreak();
            continue;
        }

        if (value < 1 || value > maxNumQuestions) {
            cout << errMsg << endl;
            addBreak();
        } else {
            break;
        }
        }
    return value;
}
int selectIntInRangeOrQuit(int maxNumQuestions, string promptMsg) {
    
    string value;
    int valueToInt;
    while(true){
        cout << promptMsg;
        getline(cin, value);
        //check if quit()
        if (value == "quit()") {
            return -1;
        }
        //try to convert value to int
        try {   
            valueToInt = stoi(value);
        }
        catch (exception e) {
            cout << ptErrMsg << endl;
            addBreak();
            
            continue;
        }
        //verify in range
        if (valueToInt < 1 || valueToInt > maxNumQuestions) {
            
            cout << "[That question does not exist!]" << endl;
            addBreak();
            continue;
        }
        return valueToInt;
    }
    return -1;
}
//miscFunctions
/**
 * @param input     string to be uppercased
 * @return string:   uppercased string
 */
string makeUpperCase(string input) {
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return input;
}
/**
 * @brief prompts writer to choose question type
 * 
 * @return string of question type (in all caps)
 */
string chooseType() {
    string input;
    cout << "Type of question [mcq/tf/wr]: ";
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "MCQ" && input != "WR" && input != "TF") {
        emptyCin();
        cout << cmdErrMsg << endl;
        addBreak();
        cout << "Type of question [mcq/tf/wr]: ";
        getline(cin, input);
        input = makeUpperCase(input);
    }
    return input;
}
/**
 * @brief prompts user for valid double
 * 
 * @return double to be returned
 */
double getValidPt() {
    string input;
    double value;
    regex validNumberPattern("^-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?$");

    while (true) {
        cout << "Enter point value: ";
        getline(cin, input);  // Read the full input as a string
        
        // Check if the input matches a valid number format
        if (!(regex_match(input, validNumberPattern))) {
            cout << ptErrMsg << endl;
            addBreak();
            continue;  // Skip further processing and ask for input again
        }

        // Convert input to double
        value = stod(input);

        // Check if the value is negative
        if (value < 0) {
            cout << ptErrMsg << endl;
            addBreak();
        } else {
            break;  // Break the loop if everything is valid
        }
    }
    return value;
}
/**
 * @brief prompts user to continue or not
 * 
 * @param prompt    type of question asked
 * @return true     if user inputs 'y'
 * @return false    if user inputs 'n'
 */
bool wantToCont(string prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "Y" && input != "N") {
        cout <<cmdErrMsg << endl;
        addBreak();
        cout << prompt; 
        getline(cin, input);
        input = makeUpperCase(input);
        cout << endl;
    }
    if (input == "Y") {return true;} else {return false;}
}
void printWelcome() {
    addBreak();
    cout << "*** Welcome to Jonah's Testing Service ***" << endl;
}
void printGoodbye() {
    addBreak();
    cout << "*** Thank you for using the testing service, Goodbye! ***" << endl;
}

class QNode {
public:
    //properties for each question
    string type;
    string questionPrompt;
    double ptValue;
    string correctAns;
    string userAns;
    QNode* prevQ;
    QNode* nextQ;
    map<char,string> choices;
    bool isCorrect;
    //constructor
    QNode(string type);
    //methods
    /**
     * @brief compares tester's answer to correct answer
     * 
     * @return true     if same 
     * @return false    if not same
     */
    bool testUserAns() {
        return (correctAns == userAns);
    }
};
QNode::QNode(string type) {//constructor
    this->type = type;
    prevQ = nullptr;
    nextQ = nullptr;
    isCorrect = false;
    userAns = "";
}

class QuestionBank {
public:
    QNode* head;
    QNode* tail;
    int totalQuestions;
    double totalPoints;
    int earnedQuestion; //num questions answered correct
    double earnedPoints; //num questions earned by student

    //constructor
    QuestionBank() {
        head = nullptr;
        tail = nullptr;
        totalQuestions = 0;
        totalPoints = 0;
        earnedPoints = 0;
        earnedQuestion = 0;
    }
    
    //writingQuestion methods
    /**
     * @brief get valid character from user
     * 
     * @param currQ     question node to write character into
     * @return string   charater (not technically char, but just a string of length 1)
     */
    string getValidChar(QNode* currQ) {
        string input;
        while (true) {
            addBreak();
            cout << "Select correct answer: ";
            getline(cin, input);
            ////???
    
            // ensure single alphabet character
            if (input.length() == 1 && isalpha(input[0])) {
                // change to uppercase
                char choice = toupper(input[0]);
    
                // Check if the choice exists in the MCQ map
                if (currQ->choices.find(choice) != currQ->choices.end()) {
                    return string(1, choice); 
                }
            }
            cout << ansErrMsg << endl;
            //addBreak();
        }
    }
    /**
     * @brief FOR TESTING: prints question info for a QuestionBank object
     * 
     */
    void printQuestionInfo() {
        QNode* traverser = head;
        while (traverser != nullptr) {
            cout << endl << endl<< "type is " << traverser->type << endl;
            cout << "prompt is " << traverser->questionPrompt <<endl;
            cout << "ptVal is " << traverser->ptValue <<endl;
            cout << "correctAns is " << traverser->correctAns <<endl<<endl;
            if (traverser->choices.empty() == 0) {
                cout << "Multiple Choice Found" << endl << "choices are:"<< endl;
                for (map<char, string>::iterator iterator = traverser->choices.begin(); iterator != traverser->choices.end(); ++iterator) {
                    cout << iterator->first << " : " << iterator->second << endl;
                }
            }
            cout << "userAns is: " <<traverser->userAns << endl;
            traverser = traverser -> nextQ;
        }
    }
    void printQNodeInfo(QNode* traverser, int QtoEdit) {
        cout << "===============================" << endl;
        cout << "=== QUESTION " << QtoEdit << " SAVED VALUES ===" << endl;
        cout << "===============================" << endl;

        cout << "\t1. Type: " << traverser->type << endl;
        cout << "\t2. Question: " << traverser->questionPrompt <<endl;
        //if mcq, print choices
        if (traverser->type == "MCQ") {
            cout << "\t3. Answer choices: " << endl;
            for (map<char, string>::iterator iterator = traverser->choices.begin(); iterator != traverser->choices.end(); ++iterator) {
                cout << "\t\t" << iterator->first << ". " << iterator->second << endl;
            }
            cout << "\t4. Correct answer: " << traverser->correctAns <<endl;
        } 
        else {
            cout << "\t3. Correct answer: " << traverser->correctAns <<endl;
        }
        cout << "===============================" << endl;
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
    void delQat(int ithQuestion) {
        QNode* traverser = head;
        //find Qnode to be deleted
        moveQNode(traverser, ithQuestion);

        //del node
        if (traverser->prevQ == nullptr) {//QNode is head 
            head = traverser->nextQ;
            if(totalQuestions > 1){
            head->prevQ = nullptr;
            }
        }
        else if (traverser->nextQ == nullptr) {//QNode is tail
            traverser->prevQ->nextQ = nullptr;
        }
        else {//QNode is body
            traverser->prevQ->nextQ = traverser->nextQ;
            traverser->nextQ->prevQ = traverser->prevQ;
        }
        totalQuestions--;
    }


    string getPrompt() {//good
        string input;
        addBreak();
        cout << "Enter a question: ";
        getline(cin, input); 
        return input;
    }
    /**
     * @brief gets "true" or "false" (case insensitive) from user for correctAns
     * 
     * @param currQ     question node being modified
     */
    void getTFcorrectAns(QNode* currQ) {
        string buffer;
        //initial input
        addBreak();
        cout << "Select correct Answer [true/false]: "; 
        getline(cin, buffer);
        buffer = makeUpperCase(buffer);
        //input validation
        while (buffer != "TRUE" && buffer != "FALSE") {
            cout <<ansErrMsg << endl;
            addBreak();
            cout <<"Select correct Answer [true/false]: "; 
            getline(cin, buffer);
            buffer = makeUpperCase(buffer);
        }
        currQ->correctAns = buffer;
    }
    /**
     * @brief gets string from user to write to correctAns
     * 
     * @param currQ     question node being modified
     */
    void getWRcorrectAns(QNode* currQ) {
        string buffer;
        //initial input
        cout << endl << "Type correct Answer: "; 
        getline(cin, buffer);
        currQ->correctAns = makeUpperCase(buffer);
    }
    void getMCQcorrectAns(QNode* currQ) {
        currQ->correctAns = getValidChar(currQ);
    }
    /**
     * @brief gets user to enter multiple choice options and to choose the letter corresponding to the correct option
     * 
     * @param currQ     question node being modified
     * @return true     if at least one option was given; proceed to point assignment
     * @return false    if no option was given
     */
    bool getMCQchoices(QNode* currQ) {//good
        //empty choices
        currQ->choices.clear();
        //add choices to map
        string choiceInput;
        char currLetter = 'A';
        cout << "[At any time, type ‘quit()’ to exit]" << endl;
        addBreak(); 
        while (true) {
            cout << "Enter choice " << currLetter << ": ";
            getline(cin, choiceInput);
            if (makeUpperCase(choiceInput) == "QUIT()") {
                break;
            }
            currQ->choices[currLetter] = choiceInput;
            currLetter++;
        }
        //in case no choices were given
        if (currQ->choices.size() == 0) {
            cout << "ERROR: No Choices were provided. Please Retry" << endl;
            return false;
        }
        return true;
    }
    /**
     * @brief writes question and adds to QuestionBank
     * 
     * @param type      determines which type of question  
     * @return true     if method was successful 
     * @return false    if method was unsuccessful
     */
    bool writeQuestion(string type) {
        QNode* currQ = new QNode(type);
        //get question
        currQ->questionPrompt = getPrompt();
        //get correctAns
        if(type == "TF") {//get correctAns for TF
            getTFcorrectAns(currQ);
        }
        else if (type == "WR") {//get correctAns for WR
            getWRcorrectAns(currQ);
        }
        else if (type == "MCQ"){//add choices & get correctAns
            if (!getMCQchoices(currQ)) return false;
            getMCQcorrectAns(currQ);
        }
        else {cout << "!!!type wasn't available!!!" << endl;}
        //get point value
        currQ->ptValue = getValidPt();
        appendToTail(currQ);//append to questionBank
        totalQuestions++;
        totalPoints += currQ->ptValue;
        return true;
    }
    void createQ() {
        cout << "=== QUESTION " << totalQuestions +1 << " ===" << endl;
        addBreak();
        //write question
        bool success = writeQuestion(chooseType());
    }
    void delQ() {
        //get int
        int QtoDel = selectIntInRange(totalQuestions, "Select a question to delete [1-" + to_string(totalQuestions) + "]: ",qExistErrMsg);
        delQat(QtoDel);
        cout << "Question " << QtoDel << " deleted." << endl;
        addBreak();
    }    
    void moveQNode(QNode* traverser, int targetQNode) {
        for ( int i = 1; i < targetQNode; i++) {
            traverser = traverser->nextQ;
        }
    }
    void editQ() {
        //choose which question to edit
        int QtoEdit = selectIntInRangeOrQuit(totalQuestions, "Select a question to edit, or type quit() [1-" + to_string(totalQuestions) + "]: ");
        //find QNode in linked list
        if (QtoEdit == -1) {
            addBreak();
            return;
        }
        QNode* traverser = head;
        moveQNode(traverser, QtoEdit);
        
        //printQuestion properties
        addBreak();
        printQNodeInfo(traverser, QtoEdit);
        addBreak();
        
        //choose which property to edit
        int maxNumQuestions = 3; 
        if (traverser->type == "MCQ") {//if MCQ, then should be 4
            maxNumQuestions = 4; 
        }
        
        int propToEdit = -2;
        while (true) {
            propToEdit = selectIntInRangeOrQuit(maxNumQuestions, "Select a number to edit, or type quit() [1-" + to_string(maxNumQuestions) + "]: ");
            if (propToEdit == -1) {//stop condition
                break;
            }
            //edit property based on propToEdit
            if (propToEdit == 1) {//edit type
                traverser->type = chooseType();
            }
            else if (propToEdit == 2) {//edit Question
                traverser->questionPrompt = getPrompt();
            }
            else if (traverser->type == "MCQ") {
                if (propToEdit == 3) {//edit choices
                    bool cont = true;
                    while (cont) {
                        cont = getMCQchoices(traverser);
                    }
                }
                else if (propToEdit == 4) {//edit correctAns
                    getMCQcorrectAns(traverser);
                }
            }
            else if (traverser->type == "TF") {//edit TF correctAns
                getTFcorrectAns(traverser);
            }
            else if (traverser->type == "WR") {//edit WR correctAns
                getWRcorrectAns(traverser);
            }
        }
        
    }


    //session log 
    void printWritingLog() {
        addBreak();addBreak(); 
        cout << "=== SESSION LOG ===" <<endl;
        cout << "Total questions: " << totalQuestions << endl;
        cout << "Total point values: " << totalPoints << endl;
    }
    void printAssessLog() {
        cout << "=== SESSION LOG ===" << endl;
        cout << "Correct answers: " << earnedQuestion << "/" << totalQuestions << endl;
            //iterate through linked list
            int i = 1;
            for (QNode* traverser = head; traverser != nullptr; traverser = traverser->nextQ) {
                cout << "\tQuestion " << i << ": " << traverser->correctAns<< endl;
                cout << "\tYour answer: " << traverser->userAns << endl;
                addBreak();
            }
        cout << "Final score: " << earnedPoints << "/" << totalPoints << endl;
    }
    void printAssessMenu(bool hasVisited) {
        int shift = 0;
        if(hasVisited){
            shift++;
        }

        cout << "Do you want to?" << endl;
        if (hasVisited) {
            cout << "\t"<< 1 << ". Edit this answer." << endl;
        }
        cout << "\t"<< 1 + shift << ". Go to next question." << endl;
        cout << "\t" << 2 + shift << ". Jump to question." << endl;
        cout << "\t" << 3 + shift << ". Submit." << endl;
    }
    //assessment
    /**
     * @brief gets "true" or "false" (case insensitive) from user and assigns it to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     * @param input         holds user's input for validation and writing to userAns
     */
    void getTFanswer(QNode* traverser) { 
        string input;
        cout << "Your answer [true/false]: ";
        getline(cin, input);
        //input validation
        input = makeUpperCase(input);
        while (input != "TRUE" && input != "FALSE") {
            cout <<ansErrMsg << endl;
            addBreak(); 
            cout << "Select correct Answer: "; 
            getline(cin, input);
            input = makeUpperCase(input);
        }
        //once valid, update values
        traverser->userAns = input;
        if (traverser->testUserAns()) {
            traverser->isCorrect = true;
            earnedPoints += traverser->ptValue;
            earnedQuestion++;   
        }

    }
    /**
     * @brief gets string from user to assign to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     * @param input         holds user's input for validation and writing to userAns
     */
    void getWRanswer(QNode* traverser) {
        string input;
        cout << "Your answer: ";
        getline(cin, input);
        
       //once valid, update values
       traverser->userAns = makeUpperCase(input);
       if (traverser->testUserAns()) {
        traverser->isCorrect = true;
           earnedPoints += traverser->ptValue;
           earnedQuestion++;   
       } 
    }

    /**
     * @brief prints choices and gets string of length 1 from user and assigns it to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     */
    void getMCQanswer(QNode* traverser) {
        //once valid, update values
        traverser->userAns = getValidChar(traverser);
        if (traverser->testUserAns()) {
            traverser->isCorrect = true;
            earnedPoints += traverser->ptValue;
            earnedQuestion++;   
        } 
    }
    /**
     * @brief starts assessment by traversing through QuestionBank's linked list, testing user inputs against correct answers; ends with session log
     * 
     */
    int getIof(QNode* targetNode) {
        int count = 1;
        QNode* traverser = head;
        while (traverser != targetNode) {
            traverser = traverser->nextQ;
            count++;
        }
        return count;
    }
    void beginQuiz() {
        QNode* currQ = head;
        bool submitNow = false;
        while (!submitNow) {

            addBreak();
            printAssessMenu(!(currQ->userAns).empty());
            addBreak();

            int shift = 0;
            if (!(currQ->userAns).empty()) {//
                shift++;
            }

            //select action
            int currAction = selectIntInRange(3+shift, "Select an action: ", "[Not an action. Try again]");
            addBreak();

            //execute chosen action
            int questionNum;

            switch (currAction - shift) {

                case 0://ask edit
                    if (currQ->isCorrect){
                        earnedPoints-=currQ->ptValue;
                        earnedQuestion--;
                        currQ->isCorrect = false;
                    }
                    //take answer
                if (currQ->type == "TF") {
                    getTFanswer(currQ);
                }
                else if (currQ->type == "WR") {
                    getWRanswer(currQ);
                }
                else if (currQ->type == "MCQ") {
                    getMCQanswer(currQ);
                }
                continue;
                break;
                case 1: //go to next question
                    
                    if (currQ->nextQ == nullptr) {//if currQ is tail
                        currQ = head;
                    }
                    else {
                        currQ = currQ->nextQ;
                    }
                break;
                case 2: //jump to question
                    questionNum = selectIntInRange(totalQuestions, "Jump to question [1-" + to_string(totalQuestions) + "]: ", qExistErrMsg);
                    moveQNode(currQ, questionNum);
                break;
                case 3://submit
                    submitNow = true;
                    continue;
                break;
                default:
                    cout << "currAction wasn't a valid int" << endl;
                break;
            }
            
            //print options
            cout << "Question " << getIof(currQ) << ": " << currQ->questionPrompt << endl;
            if (currQ->type == "MCQ") {
                for (map<char, string>::iterator iterator = currQ->choices.begin(); iterator != currQ->choices.end(); ++iterator) {
                    cout << "\t" << iterator->first << ". " << iterator->second << endl;
                }
            }
            //take answer
            if (currQ->type == "TF") {
                getTFanswer(currQ);
            }
            else if (currQ->type == "WR") {
                getWRanswer(currQ);
            }
            else if (currQ->type == "MCQ") {
                getMCQanswer(currQ);
            }
            
        }
    }
};

int main () {
    QuestionBank* qBank = new QuestionBank();

    printWelcome();

    /* bool addStatus = true;
    //writeQuestion loop
    while (addStatus == true) {
        addBreak();
        addBreak();
        cout << "=== QUESTION " << qBank->totalQuestions +1 << " ===" << endl;
        addBreak();
        //write question
        if (qBank->writeQuestion(chooseType())) {
            addBreak();
            addStatus = wantToCont(contMsg);
        }
    } */

    bool modifyQuiz = true;
    while (modifyQuiz) {
        //print menu
        addBreak();
        printWriteOptions();
        addBreak();

        //select action
        int action = selectIntInRange(4, "Select answer: ",qExistErrMsg);
        if (action == 1) {//create Q
            addBreak();
            addBreak();
            qBank->createQ();
        }
        else if (action == 2) {//edit Q
            if (qBank->totalQuestions > 0) {
                addBreak();
                qBank->editQ();
            }
            else {
                cout << "There are no questions to edit" << endl;
            }
        }
        else if (action == 3) {//delQ
            addBreak();
            if (qBank->totalQuestions > 0) {
                qBank->delQ();
            }
            else {
                cout << "There are no questions to delete" << endl;
            }
        }
        else if (action == 4) {//finish
            modifyQuiz = false;
        }
    }

    //session log
    qBank->printWritingLog();
    addBreak();

    //begin assessment
    if (wantToCont(assessmentMsg)) {
        addBreak();
        
        qBank->beginQuiz();
        
        cout << "/!\\ Assessment Complete." << endl;
        addBreak(); addBreak(); 
        
        qBank->printAssessLog();
        addBreak();
    };

    printGoodbye();
    //qBank->printQuestionInfo();
}
#endif