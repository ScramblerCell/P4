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
//strings for writingQuestion
string ansErrMsg = "[Answer not recognized, please try again!]";
string cmdErrMsg = "[Command not recognized, please try again!]";
string ptErrMsg = "[Not a point value, please try again!]";
string contMsg = "Question saved. Continue? [y/n]: ";
//strings for assessment
string assessmentMsg = "/!\\ Begin assessment? [y/n]: ";
string correctMsg = "[Your answer is correct!]";
void addBreak() {
    cout << endl;
}

//miscFunctions
string makeUpperCase(string input) {
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return input;
}
string chooseType() {
    string input;
    cout << "Type of question [mcq/tf/wr]: ";
    getline(cin, input);
    input = makeUpperCase(input);
    while (input != "MCQ" && makeUpperCase(input) != "WR" && makeUpperCase(input) != "TF") {
        cout << cmdErrMsg << endl;
        addBreak();
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
    //constructor
    QNode(string type);
    //methods
    bool testUserAns() {
        return (correctAns == userAns);
    }
};
QNode::QNode(string type) {//constructor
    this->type = type;
    prevQ = nullptr;
    nextQ = nullptr;
    //choices['A'] = "[[NO CHOICES WERE ADDED]]";
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
    string getValidChar(QNode* currQ) {
        string input;
        while (true) {
            addBreak();
            cout << "Select correct answer: ";
            getline(cin, input);
    
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
    void printQuestionInfo() {//test purposes only
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
        addBreak();
        cout << "Enter a question: ";
        getline(cin, input); 
        return input;
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
            string buffer;
            //initial input
            cout << endl << "Select correct Answer: "; 
            getline(cin, buffer);
            buffer = makeUpperCase(buffer);
            //input validation
            while (buffer != "TRUE" && buffer != "FALSE") {
                cout <<ansErrMsg << endl;
                addBreak();
                cout <<"Select correct Answer: "; 
                getline(cin, buffer);
                buffer = makeUpperCase(buffer);
            }
            currQ->correctAns = buffer;
        }
        else if (type == "WR") {//get correctAns for WR
            string buffer;
            //initial input
            cout << endl << "Type correct Answer: "; 
            getline(cin, buffer);
            currQ->correctAns = makeUpperCase(buffer);
        }
        else if (type == "MCQ"){//add choices & get correctAns
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

            //get correctAns for MCQ
            currQ->correctAns = getValidChar(currQ);
        }
        else {cout << "!!!type wasn't available!!!" << endl;}
        //get point value
        currQ->ptValue = getValidPt();;
        //append to questionBank
        appendToTail(currQ);
        totalQuestions++;
        totalPoints += currQ->ptValue;
        return true;
    }

    //session log 
    void printSessionLog() {
        addBreak();addBreak(); 
        cout << "=== SESSION LOG ===" <<endl;
        cout << "Total questions: " << totalQuestions << endl;
        cout << "Total point values: " << totalPoints << endl;
    }

    //assessment
    void beginQuiz() {
        QNode* traverser = head;
        for (int i = 1; i <= totalQuestions; i++) {
            cout << "Question " << i << ": " << traverser->questionPrompt << endl;
            string input; 
            //get answer based on type
            if (traverser->type == "TF") {//tf
                addBreak();
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
                    cout << correctMsg << endl;
                    addBreak();
                    earnedPoints += traverser->ptValue;
                    earnedQuestion++;   
                } 
                else {
                    cout << "[Your answer is incorrect. The correct answer is " << traverser->correctAns << ".]" << endl;
                    addBreak();
                }
            } 
            else if (traverser->type == "WR") {//wr
                addBreak();
                cout << "Your answer: ";
                getline(cin, input);
                
               //once valid, update values
               traverser->userAns = makeUpperCase(input);
               if (traverser->testUserAns()) {
                   cout << correctMsg << endl;
                   addBreak();
                   earnedPoints += traverser->ptValue;
                   earnedQuestion++;   
               } 
               else {
                   cout << "[Your answer is incorrect. The correct answer is " << traverser->correctAns << ".]" << endl;
                   addBreak();
               }
            }
            else {//mcq
                //print choices
                for (map<char, string>::iterator iterator = traverser->choices.begin(); iterator != traverser->choices.end(); ++iterator) {
                    cout << "\t" << iterator->first << ". " << iterator->second << endl;
                }
                //once valid, update values
                traverser->userAns = getValidChar(traverser);
                if (traverser->testUserAns()) {
                    cout << correctMsg << endl;
                    addBreak();
                    earnedPoints += traverser->ptValue;
                    earnedQuestion++;   
                } 
                else {
                    cout << "[Your answer is incorrect. The correct answer is " << traverser->correctAns << ".]" << endl;
                    addBreak();
                }
            }
            traverser = traverser->nextQ;
        }
        cout << "/!\\ Assessment Complete." << endl;
        addBreak(); addBreak(); 
        cout << "=== SESSION LOG ===" << endl;
        cout << "Correct answers: " << earnedQuestion << "/" << totalQuestions << endl;
        cout << "Final score: " << earnedPoints << "/" << totalPoints << endl;
        addBreak();
    }
};



int main () {
    QuestionBank* qBank = new QuestionBank();

    printWelcome();

    bool addStatus = true;
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
    }

    //session log
    qBank->printSessionLog();

    //begin assessment
    addBreak();
    if (wantToCont(assessmentMsg)) {
        addBreak();
        qBank->beginQuiz();
    };

    printGoodbye();
    //qBank->printQuestionInfo();
}