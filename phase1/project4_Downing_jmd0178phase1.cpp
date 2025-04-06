/*
project4_Downing_jmd0178phase1.cpp
Name: Jonah Downing
Auburn UserID: jmd0178
Instructions: cd phase1; g++ project4_Downing_jmd0178phase1.cpp -o phase1;./phase1
Additional Help
    - Youtube
        - https://www.youtube.com/watch?v=m7rrk65GiXY
        - https://www.youtube.com/watch?v=-StYr9wILqo
        - https://youtu.be/-StYr9wILqo?si=TJHz_UNhUmFMoq5a
    - chatgpt
        - how do i know when to create classes and how to divide them up for projects
        - class vs struct
        - how do pointers work in cpp
        - what is the & symbol?
        - when using pointers why use -> instead of .
        - can i declare a questionNode object outside of main so it can be accessed in any method
        - i need further clarification on pointer vs object
        - why would someone define a function/method oustide of the class it was declared in
            - so is it always better to define outside?
        - how do i know if i'm going to want questionNode to be a obect or a pointer
        - how to do use a constructor in a child class?
        - can i use == to compare striongs
        - how can i use ascii values of characters to ensure a char input is alphabetical
        - why would i be getting this? warning: 'auto' type specifier is a C++11 extension [-Wc++11-extensions] for (auto choice : traverser->choices) {
        - how to convert from char to string
        - is it bad practice to use a return to premptively exit a void method
    - W3 Schools
        - https://www.w3schools.com/cpp/cpp_pointers.asp
        - read through their tutorials on structures, pointers, classes, constructors, inheritance, maps
    - talked to aidan scott, michael algarin, and adrian bennett about general approaches; started project with derived classes, but wound up using a single parent class for simplicity’s sake
    - https://www.programiz.com/cpp-programming/library-function/cctype/toupper
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
    while (input != "MCQ" && makeUpperCase(input) != "WR" && makeUpperCase(input) != "TF") {
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
     * @brief gets "true" or "false" (case insensitive) from user for correctAns
     * 
     * @param currQ     question node being modified
     */
    void getTFcorrectAns(QNode* currQ) {
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
    /**
     * @brief gets user to enter multiple choice options and to choose the letter corresponding to the correct option
     * 
     * @param currQ     question node being modified
     * @return true     if at least one option was given; proceed to point assignment
     * @return false    if no option was given
     */
    bool getMCQchoices(QNode* currQ) {
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
            if (!getMCQchoices(currQ)) {return false;}
        }
        else {cout << "!!!type wasn't available!!!" << endl;}
        
        currQ->ptValue = getValidPt();//get point value
        appendToTail(currQ);//append to questionBank
        totalQuestions++;
        totalPoints += currQ->ptValue;
        return true;
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
        cout << "Final score: " << earnedPoints << "/" << totalPoints << endl;
    }
    //assessment
    /**
     * @brief gets "true" or "false" (case insensitive) from user and assigns it to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     * @param input         holds user's input for validation and writing to userAns
     */
    void getTFanswer(QNode* traverser, string input) {
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
    /**
     * @brief gets string from user to assign to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     * @param input         holds user's input for validation and writing to userAns
     */
    void getWRanswer(QNode* traverser, string input) {
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
    /**
     * @brief prints choices and gets string of length 1 from user and assigns it to userAns
     * 
     * @param traverser     question node that is being compared to user's answer
     */
    void getMCQanswer(QNode* traverser) {
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
    /**
     * @brief starts assessment by traversing through QuestionBank's linked list, testing user inputs against correct answers; ends with session log
     * 
     */
    void beginQuiz() {
        QNode* traverser = head;
        for (int i = 1; i <= totalQuestions; i++) {
            cout << "Question " << i << ": " << traverser->questionPrompt << endl;
            string input; 
            //get answer based on type
            if (traverser->type == "TF") {//tf
                addBreak();
                getTFanswer(traverser, input);
            } 
            else if (traverser->type == "WR") {//wr
                addBreak();
                getWRanswer(traverser, input);
            }
            else {//mcq
                getMCQanswer(traverser);
            }
            traverser = traverser->nextQ;
        }
        cout << "/!\\ Assessment Complete." << endl;
        addBreak(); addBreak(); 
        printAssessLog();
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
    qBank->printWritingLog();

    //begin assessment
    addBreak();
    if (wantToCont(assessmentMsg)) {
        addBreak();
        qBank->beginQuiz();
    };

    printGoodbye();
    //qBank->printQuestionInfo();
}