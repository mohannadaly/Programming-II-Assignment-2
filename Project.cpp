/*
Mohanad Ayman Aly
20180296
Bishoy Mokhless Khela
20180072
Hassan Mahmoud Hassan
20180088
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;
const char clear[] = "cls";
const char pause[] = "PAUSE";

int* randomnumbers(int last, int numofInt); // ** generate a "numofInt" random integers in the range [0, last]
int is_numeric(string); // Check if a string is purely numeric
int char_to_int(string); // Extract numbers from a string
void SetFormat(string&); // Set the format for a string to have the first letter of each word be a capital letter

class Question {
public:
    string sentence; // main question
    static int q_id;
    int unique_id = 0; // unique ID for the question
    virtual void view() = 0; // pure virtual function for viewing question
};

int Question::q_id = 1; // initialize static variable
class TF : public Question {
public:
    bool answer = false;

    void load(string ques, string ans, vector<TF>& questions) // store the question
    {
        int found = 0;
        SetFormat(ques); // Set the format for the question
        for (unsigned int i = 0; i < questions.size(); i++)
            if (questions[i].sentence == ques) { // check for duplicates
                found = 1;
                break;
            }
        if (found == 0) { // if no duplicate was found
            sentence = ques;
            if (ans == "true") answer = true;
            unique_id = q_id++;
        }
        else questions.pop_back(); // if the question already exists

    }

    void view() {
        string ans = (answer) ? "true" : "false";
        cout << " (ID: " << unique_id << ") ";
        cout << sentence << endl;
        cout << "Answer: " << ans << endl;
    }

    char q_TF(int& score) {
        char ans = (answer) ? 'a' : 'b';
        cout << sentence << endl;
        cout << setw(20) << "[a] True" << setw(20) << "[b] False" << endl;
        char choice;
        cin >> choice;
        while (!((choice >= 'a' && choice <= 'b') || (choice >= 'A' && choice <= 'B'))) {
            cin.ignore(9999, '\n');
            cout << "Invalid choice, please try again...\n";
            cin >> choice;
        }
        cin.ignore();
        if (choice < 97) choice += 32;
        if (choice == ans) score++;
        return choice;
    }
};

class COMPLETE : public Question {
public:
    string answer;

    void load(string ques, string ans, vector<COMPLETE>& questions) // store the question
    {
        int found = 0;
        SetFormat(ques); // Set the format for the question
        for (unsigned int i = 0; i < questions.size(); i++)
            if (questions[i].sentence == ques) { // check for duplicate question
                found = 1;
                break;
            }
        if (found == 0) { // if the question wasn't found in the vector
            sentence = ques;
            answer = ans;
            SetFormat(answer);
            unique_id = q_id++;
        }
        else questions.pop_back(); // if the question is duplicated

    }

    void view() {
        cout << " (ID: " << unique_id << ") ";
        cout << sentence << endl;
        cout << "Answer: " << answer << endl;
    }

    string q_complete(int& score) {
        string input;
        cout << sentence << endl;
        getline(cin, input);
        SetFormat(input);
        if (input == answer) score += 3;
        return input;
    }
};

class MCQ : public Question {
public:
    string answers[4];

    void load(string ques, string ans, string choices[], vector<MCQ>& questions) // store the question
    {
        int found = 0;
        SetFormat(ques); // Set the format for the question
        for (unsigned int i = 0; i < questions.size(); i++)
            if (questions[i].sentence == ques) { // check for duplicate question
                found = 1;
                break;
            }
        if (found == 0) { // if question wasn't found in the vector
            sentence = ques;
            answers[0] = ans;
            loadchoices(choices);
            unique_id = q_id++;
        }
        else questions.pop_back(); // if the question was found in the vector
    }

    void loadchoices(string ans[]) { // load the choices into the answers array
        answers[1] = ans[0];
        answers[2] = ans[1];
        answers[3] = ans[2];
    }

    void view() { // print the question and its answer
        cout << " (ID: " << unique_id << ") ";
        cout << sentence << endl;
        for (unsigned int i = 0; i < 4; i++) {
            cout << '[' << (char)(97 + i) << "] " << answers[i] << "	";
        }
        cout << endl;
    }

    string q_mcq(int& score) {
        string input;
        cout << sentence << endl;
        int* m; // pointer to store random numbers for choices
        m = randomnumbers(3, 4); // point to array returned by gen_ranInt
        for (int j = 0; j < 4; j++) { // cast 97 to 'a' and increment by j to bring b c d
            cout << '[' << (char)(97 + j) << ']';
            if (answers[m[j]][0] == '*') { // exclude the '*' before printing the choice if it exists
                cout << answers[m[j]].substr(1) << ' ';
            }
            else
                cout << answers[m[j]] << ' ';
        }
        cout << endl; // print a new line after printing choices
        getline(cin, input); // get user's choice
        bool isValid = // if user's choice isn't within {a,b,c,d} (upper or lower case)
            (input[0] >= 97 && input[0] <= 100) || (input[0] >= 65 && input[0] <= 68);
        while (input.length() != 1 ||
            !isValid) { // if the user didn't input one character or the user entered an invalid character
            cout << "Invalid choice, please try again" << endl;
            getline(cin, input);
            isValid = // recheck for validity of the character
                (input[0] >= 97 && input[0] <= 100) || (input[0] >= 65 && input[0] <= 68);
        }
        int userChoice_index = (int)(input[0] -
            97); // get the index of the letter entered by the user {a:0, b:1, c:2, d:3}
        if (input[0] < 97)
            userChoice_index += 32; // get the index of the letter entered if it's an uppercase one
        if (m[userChoice_index] == 0)
            score += 2; // if the user's choice matches the first choice in the choices array, count as a correct answer
        return input;
    }
};

class Quiz {

private:

    unsigned int numofTF = 2; // number of true/false questions per quiz
    unsigned int numofComplete = 2; // number of complete questions per quiz
    unsigned int numofMCQ = 2; // number of mcq questions per quiz

public:
    vector<TF> tf_stored; // vector for tf questions in the quiz
    vector<COMPLETE> complete_stored; // vector for complete questions in the quiz
    vector<MCQ> mcq_stored; // vector for mcq questions in the quiz
    vector<string> mcq_ans; // player's answers for mcq questions
    vector<string> complete_ans; // player's answers for complete questions
    vector<char> tf_ans; // player's answers for tf questions
    int total = 0; // total score in the quiz
    int mcq_s = 0, tf_s = 0, complete_s = 0; // scores for each question type in the quiz
    void start_quiz(vector<TF> tf_v, vector<COMPLETE> complete_v,
        vector<MCQ> mcq_v) { // pass vectors storing the questions to the function
        system(clear);
        if (tf_v.size() < numofTF || complete_v.size() < numofComplete || mcq_v.size() < numofMCQ)
            cout << "Not enough questions to start quiz" << endl;
        else {
            int* x = randomnumbers(tf_v.size() - 1, numofTF); // arrays of random indices for questions
            int* y = randomnumbers(complete_v.size() - 1, numofComplete);
            int* z = randomnumbers(mcq_v.size() - 1, numofMCQ);
            cout << "**********************************" << endl;
            cout << "Quiz Started" << endl;
            cout << "**********************************" << endl;
            cout << "Choose True or False" << endl;
            cout << "**********************************" << endl;
            for (unsigned int i = 0; i < numofTF; i++) {
                tf_ans.push_back(tf_v[x[i]].q_TF(tf_s)); // store user's answer
                tf_stored.push_back(tf_v[x[i]]); // store question
            }
            cout << "**********************************" << endl;
            cout << "Choose the correct answer" << endl;
            cout << "**********************************" << endl;
            for (unsigned int i = 0; i < numofMCQ; i++) {
                mcq_ans.push_back(mcq_v[z[i]].q_mcq(mcq_s)); // store user's answer
                mcq_stored.push_back(mcq_v[z[i]]); // store question
            }
            cout << "**********************************" << endl;
            cout << "Complete the following sentences" << endl;
            cout << "**********************************" << endl;
            for (unsigned int i = 0; i < numofComplete; i++) {
                complete_ans.push_back(complete_v[y[i]].q_complete(complete_s)); // store user's answer
                complete_stored.push_back(complete_v[y[i]]); // store question
            }
            total = tf_s + mcq_s + complete_s; // total score for the quiz
            cout << "Total Score: " << total << endl;
            system(pause);
            system(clear);
        }
    }

    friend ostream& operator<<(ostream&, Quiz&);
};

ostream& operator<<(ostream& out, Quiz& a) {
    out << "**********************************" << endl; // print true or false questions
    out << "True or False Questions" << endl;
    out << "**********************************" << endl;
    for (unsigned int i = 0; i < a.tf_stored.size(); i++) {
        a.tf_stored[i].view(); // print the question and its answer
        out << "Your answer: "; // print the user's answer on the quiz
        if (a.tf_ans[i] == 'a') out << "True" << endl;
        else out << "False" << endl;
    }
    cout << "Correct True or False Questions: " << a.tf_s << endl; // total number of correct questions
    out << "**********************************" << endl; // print complete questions
    out << "Complete Questions" << endl;
    out << "**********************************" << endl;
    for (unsigned int i = 0; i < a.complete_stored.size(); i++) {
        a.complete_stored[i].view(); // print the question and its answer
        out << "Your answer: " << a.complete_ans[i]; // print the user's answer on the quiz
    }
    cout << "Correct Complete Questions: " << a.complete_s / 3 << endl; // total number of correct questions
    out << "**********************************" << endl; // print mcq questions
    out << "MCQ Questions" << endl;
    out << "**********************************" << endl;
    for (unsigned int i = 0; i < a.mcq_stored.size(); i++) {
        a.mcq_stored[i].view(); // print the question and its answer
        int x = (int)a.mcq_ans[i][0] - 97;
        out << "Your answer: " << a.mcq_stored[i].answers[x]; // print the user's answer
    }
    cout << "\nCorrect MCQ Questions: " << a.mcq_s / 2 << endl; // total number of correct questions
    cout << "Total Score: " << a.total << endl; // print the user's score on the quiz
    return out;
}

class User {
protected:
    string name; // username
    string password; // password
    bool isadmin = true; // default account type is admin
public:
    string first = "user", last = "user"; // default first and last names
    int menu_options = 0; // number of menu entries

    User(bool u_isadmin) {
        isadmin = u_isadmin;
    }

    string getName() const {
        return name;
    }

    string getPass() const {
        return password;
    }

    void setPass(string newPass) {
        password = newPass;
    }

    bool getType() const {
        return isadmin;
    }

    void update_info() {
        system(clear);
        bool loop = true;
        while (loop) {
            cout << "Update " << getName() << "'s info:" << endl;
            cout << "	[0] Back to main menu\n"
                "	[1] First Name\n"
                "	[2] Last Name\n"
                "	[3] Password\n";
            char choice;
        retry:
            cout << "My choice: ";
            cin >> choice;
            cin.ignore();
            switch (choice) {
            case ('0'): // if user chooses to go back to main menu
            {
                system(clear);
                cout << "**********************************" << endl;
                cout << getName() << "'s current info: " << endl;
                cout << "First name: " << first << endl;
                cout << "Last name: " << last << endl;
                cout << "Password: " << getPass() << endl;
                cout << "**********************************" << endl;
                system(pause);
                system(clear);
                loop = false;
                break;
            }
            case ('1'): // if user chooses to edit their first name
            {
                system(clear);
                cout << "Enter new first name: ";
                getline(cin, first);
                cout << "New First Name: " << first << endl;
                system(pause);
                system(clear);
                break;
            }
            case ('2'): // if user chooses to edit their last name
            {
                system(clear);
                cout << "Enter new last name: ";
                getline(cin, last);
                cout << "New Last Name: " << last << endl;
                system(pause);
                system(clear);
                break;
            }
            case ('3'): // if user chooses to edit their password
            {
                system(clear);
                cout << "Enter new password: ";
                string newPass;
                getline(cin, newPass);
                setPass(newPass);
                cout << "New Password: " << newPass << endl;
                system(pause);
                system(clear);
                break;
            }
            default: // if user chooses an invalid option
            {
                system(clear);
                cin.ignore(9999, '\n');
                cout << "Invalid choice, please try again...\n";
                goto retry;
            }
            }
        }
    }

    friend istream& operator>>(istream&, User*&); // overload input operator
    virtual void menu() = 0; // virtual function to print menu entries
};

class Player : public User {
public:
    int high_score = 0, low_score = 0;
    double average_score = 0.0, mcq_score = 0.0, tf_score = 0.0, complete_score = 0.0;
    vector<Quiz> quiz; // vector to store quizzes taken by player

    Player() : User(false) {
        name = "player"; // default player username
        password = "player"; // default player password
        menu_options = 7; // player account menu entries
    }

    void menu() { // print menu entires for player account
        cout << "	[3] Start a new quiz\n"
            "	[4] Display your scores statistics\n"
            "	[5] Display all your scores\n"
            "	[6] Display details of your last 2 quizzes\n";
    }

    void score_stats(int& exit) { // print score statistics for all quizzes combined
        system(clear);
        int numofQuizzes = quiz.size(); // number of quizzes taken
        if (numofQuizzes == 0) cout << "No statistics found to show yet" << endl; // if no quizzes were taken yet
        else {
            int maxScore = quiz[0].mcq_stored.size() * 2 + quiz[0].complete_stored.size() * 3 +
                quiz[0].tf_stored.size(); // maximum score to be had per quiz
            int numofTF = quiz[0].tf_stored.size() * numofQuizzes; // number of questions per question type
            int numofMCQ = quiz[0].mcq_stored.size() * numofQuizzes;
            int numofComplete = quiz[0].complete_stored.size() * numofQuizzes;
            high_score = quiz[0].total; // initialize highest score
            low_score = quiz[0].total; // initialize lowest score
            for (int i = 0; i < numofQuizzes; i++) {
                if (quiz[i].total < low_score) low_score = quiz[i].total; // find high score
                if (quiz[i].total > high_score) high_score = quiz[i].total; // find low score
                average_score += quiz[i].total; // sum of scores
                mcq_score += quiz[i].mcq_s; // sum of mcq scores
                complete_score += quiz[i].complete_s; // sum of complete scores
                tf_score += quiz[i].tf_s; // sum of tf scores
            }
            cout << "Your score statistics per quiz:" << endl;
            cout << "	-Number of quizzes taken: " << numofQuizzes << endl;
            cout << "	-Highest quiz score: " << high_score << '/' << maxScore << endl;
            cout << "	-Lowest quiz score: " << low_score << '/' << maxScore << endl;
            cout << "	-Average quiz score: " << average_score / numofQuizzes << '/' << maxScore << endl;
            cout << "Your score statistics per question type:" << endl;
            cout << "	- Number of MCQ questions: " << numofMCQ << endl;
            cout << "	- Number of Complete questions : " << numofComplete << endl;
            cout << "	- Number of T / F Questions : " << numofTF << endl;
            cout << "	- Average grade for MCQ questions: " << mcq_score / numofMCQ << endl;
            cout << "	- Average grade Complete questions : " << complete_score / numofComplete << endl;
            cout << "	- Average grade T / F Questions : " << tf_score / numofTF << endl;
            cout << "Press [b] if you want to go back to the main menu or [e] to exit" << endl;
            cout << "My choice:";
            char choice;
            cin >> choice;
            while (choice != 'b' && choice != 'e') {
                cin.ignore(9999, '\n');
                cout << "Invalid choice, please try again...\n";
                cin >> choice;
            }
            if (choice == 'e') exit = 1; // if user chooses to exit program
            else system(clear); // if user chosoes to go back to main menu
        }
    }

    void all_scores() { // print score statistics for each quiz
        system(clear);
        cout << "**********************************" << endl;
        cout << "Viewing all scores for your quizzes" << endl;
        cout << "**********************************" << endl;
        cout << "Number of quizzes taken: " << quiz.size() << endl;
        for (unsigned int i = 0; i < quiz.size(); i++) {
            cout << "**********************************" << endl;
            cout << "	Quiz " << i + 1 << ':' << endl;
            cout << "	Number of True/False correct answers: " << quiz[i].tf_s << endl;
            cout << "	Number of True/False wrong answers: " << quiz[i].tf_stored.size() - quiz[i].tf_s << endl;
            cout << "	Number of Complete correct answers: " << quiz[i].complete_s / 3 << endl;
            cout << "	Number of Complete wrong answers: " << quiz[i].complete_stored.size() - quiz[i].complete_s / 3
                << endl;
            cout << "	Number of MCQ correct answers: " << quiz[i].mcq_s / 2 << endl;
            cout << "	Number of MCQ wrong answers: " << quiz[i].mcq_stored.size() - quiz[i].mcq_s / 2 << endl;
            cout << "	Total Score: " << quiz[i].total << endl;
        }
        cout << "**********************************" << endl;
        system(pause);
        system(clear);
    }
};

class Admin : public User {
public:
    Admin() : User(true) {
        name = "admin"; // default admin username
        password = "admin"; // default admin password
        menu_options = 8; // number of menu entries for admin account
    }

    void menu() {
        cout << "	[3] View all users\n" // print admin menu entries
            "	[4] Add new user\n"
            "	[5] View all questions\n"
            "	[6] Add new question\n"
            "	[7] Load questions from file\n";
    }

    static bool Register(vector<User*>& users) {
        system(clear);
        string acc_type; // string to store new account type
    retry:
        cout << "Account type('player' or 'admin'): ";
        getline(cin, acc_type);
        if (acc_type != "admin" && acc_type != "player") { // if invalid account type is entered
            system(clear);
            cout << "Invalid account type, please try again..." << endl;
            goto retry;
        }
        User* newUser;
        if (acc_type == "admin") newUser = new Admin; // create an admin acount
        else newUser = new Player; // create a player acount
        cin >> newUser;
        for (unsigned int i = 0; i < users.size(); i++) {
            if (newUser->getName() == users[i]->getName()) return false; // return false if account already exists
        }
        users.push_back(newUser);
        return true; // return true if account is registered
    }

    static void view_users(vector<User*>& users) {
        system(clear);
    start:
        int x = 20;
        cout << "**********************************" << endl;
        cout << "Existing users in the system:" << endl;
        cout << "**********************************" << endl;
        cout << setw(x) << "First Name" << setw(x) << "Last Name " << setw(x) << "Username" << setw(x) << "Role"
            << endl; // print column titles
        for (unsigned int i = 0; i < users.size(); i++) {
            string role = (users[i]->getType()) ? "admin" : "player"; // check for user type
            cout << setw(x) << users[i]->first; // first name
            cout << setw(x) << users[i]->last; // last name
            cout << setw(x) << users[i]->getName(); // user name
            cout << setw(x) << role << endl; // acount type
        }
        cout << "Press [n] if you want to add a new user or [b] to go back to the main menu" << endl;
        char choice;
        cin >> choice;
        while (choice != 'n' && choice != 'b') {
            cin.ignore(9999, '\n');
            cout << "Invalid choice, please try again...\n";
            cin >> choice;
        }
        if (choice == 'n') { // if admin chooses to add new user
            cin.ignore();
            if (!Register(users)) {
                cout << "User already exists" << endl; // if user already exists
                system(pause);
                system(clear);
            }
            goto start;
        }
        else system(clear); // if user chooses to go back to main menu
    }

    static void load_file(vector<TF>& tf_v, vector<COMPLETE>& complete_v, vector<MCQ>& mcq_v) {
        system(clear);
        string questions_file; // name of the file to be opened
        int exit = 0; // flag to cancel loading questions from file
        cout << "Please enter the name of the file you'd like to open" << endl;
        cout << "(Please enter the full path to the file, or make sure the file is in the same folder as the program)"
            << endl;
        getline(cin, questions_file); // get full path to the file
        if (questions_file[0] == '"' &&
            questions_file[questions_file.length() - 1] == '"') // if the input contains quotation marks, remove them
            questions_file = questions_file.substr(1, questions_file.length() - 2);
        ifstream file;
        file.open(questions_file); // open the file
        while (file.fail()) { // as long as the file can't be found/opened, keep askin for another one
            cout << "Could not open file, please try again" << endl;
            cout << "(Enter b to stop)" << endl;
            getline(cin, questions_file);
            if (questions_file[0] == '"' &&
                questions_file[questions_file.length() - 1] == '"') // remove quotation marks from input
                questions_file = questions_file.substr(1, questions_file.length() - 2);
            file.open(questions_file); // try to open the file
            if (questions_file.length() == 1 &&
                questions_file[0] == 'b') { // if the user inputs 'b', set the exit flag to 1 and break the loop
                exit = 1;
                break;
            }
        }
        system(clear);
        if (exit == 0) {
            string str;
            string question, answer;
            int n = 0;
            while (!file.eof()) {
                getline(file, str);
                getline(file, question); // store main question
                getline(file, answer); // store main answer
                if (str == "TF") {
                    tf_v.push_back(TF()); // store True/False question
                    tf_v[tf_v.size() - 1].load(question, answer, tf_v);
                }
                else if (str == "COMPLETE") {
                    complete_v.push_back(COMPLETE()); // store Complete question
                    complete_v[complete_v.size() - 1].load(question, answer, complete_v);
                }
                else if (str == "MCQ") {
                    string answers[3];
                    getline(file, answers[0]);
                    getline(file, answers[1]);
                    getline(file, answers[2]);
                    mcq_v.push_back(MCQ()); // store MCQ question
                    mcq_v[mcq_v.size() - 1].load(question, answer, answers, mcq_v);
                }
            }
            cout << "File loaded successfully" << endl;
            file.close();
        }
        else {
            cout << "Cancelled file open" << endl; // if the user chooses not to open a file
        }
    }

    static void view_questions(vector<TF>& tf_v, vector<COMPLETE>& complete_v, vector<MCQ>& mcq_v) {
    retry:
        int total_questions = tf_v.size() + complete_v.size() + mcq_v.size(); // total number of questions
        int j = 1; // questions numbering
        if (total_questions == 0) {
            system(clear);
            cout << "No questions found" << endl;
        }
        else {
            system(clear);
            cout << "Number of questions available: " << total_questions << endl;
            cout << "-------------------------------------------------------------" << endl;
            cout << "	MC Questions list(Total: " << mcq_v.size() << " Questions)" << endl;
            cout << "-------------------------------------------------------------" << endl;
            for (unsigned int i = 0; i < mcq_v.size(); i++) {
                cout << '[' << j++ << ']';
                mcq_v[i].view();
            }
            cout << "-------------------------------------------------------------" << endl;
            cout << "	TF Questions list(Total: " << tf_v.size() << " Questions)" << endl;
            cout << "-------------------------------------------------------------" << endl;
            for (unsigned int i = 0; i < tf_v.size(); i++) {
                cout << '[' << j++ << ']';
                tf_v[i].view();
            }
            cout << "-------------------------------------------------------------" << endl;
            cout << "	Complete Questions list(Total: " << complete_v.size() << " Questions)" << endl;
            cout << "-------------------------------------------------------------" << endl;
            for (unsigned int i = 0; i < complete_v.size(); i++) {
                cout << '[' << j++ << ']';
                complete_v[i].view();
            }
            cout << "-------------------------------------------------------------" << endl;
            cout << '[' << Question::q_id + 1 << ']' << " All questions" << endl; // option to delete all questions
            cout << "Press [d] and the question ID if you want to delete a question (Example: d 2)" << endl;
            cout << "Press [b] if you want to go back to the main menu" << endl;
            int valid = 0;
            while (valid == 0) {
                valid = 1;
                string user_input; // variable for user's input
                getline(cin, user_input);
                if (user_input.length() == 1 && user_input == "b")
                    system(clear); // if user's input is 'b' go back to main
                else if (user_input.substr(0, 2) ==
                    "d ") { // else if user's input begins with letter 'd' followed by a space, otherwise print an error
                    if (is_numeric(user_input.substr(
                        2))) { // if the characters after the space are numeric, otherwise print an error
                        int q_num = char_to_int(user_input.substr(2)); // extract those numbers
                        if (q_num == Question::q_id + 1) {
                            tf_v.clear();
                            mcq_v.clear();
                            complete_v.clear();
                            system(clear);
                            cout << "Deleted all questions" << endl;
                            system(pause);
                        }
                        else {
                            bool found = false;
                            for (unsigned int i = 0; i < mcq_v.size(); i++) {
                                if (mcq_v[i].unique_id == q_num) {
                                    mcq_v.erase(mcq_v.begin() + i);
                                    found = true;
                                }
                            }
                            for (unsigned int i = 0; i < tf_v.size(); i++) {
                                if (tf_v[i].unique_id == q_num) {
                                    tf_v.erase(tf_v.begin() + i);
                                    found = true;
                                }
                            }
                            for (unsigned int i = 0; i < complete_v.size(); i++) {
                                if (complete_v[i].unique_id == q_num) {
                                    complete_v.erase(complete_v.begin() + i);
                                    found = true;
                                }
                            }
                            if (found) cout << "Question Deleted" << endl;
                            else cout << "Question not found" << endl;
                            system(pause);
                        }
                    }
                    else {
                        cout << "Invalid question number, please try again."
                            << endl; // if user inputs 'd ' followed by something other than a number
                        system(pause);
                    }
                    goto retry;
                }
                else { // if the user doesn't input 'b' or 'd ##'
                    cout << "Invalid input, please try again." << endl;
                    valid = 0;
                }
            }
        }
    }

    static void add_question(vector<TF>& tf_v, vector<COMPLETE>& complete_v, vector<MCQ>& mcq_v) {
        system(clear);
        int originalTF = tf_v.size(); // sizes of vectors before adding a question
        int originalComplete = complete_v.size();
        int originalMCQ = mcq_v.size();
        cout << "Enter the type of questions: " << endl;
        cout << "[1] True or False  " << endl;
        cout << "[2] Complete  " << endl;
        cout << "[3] MCQ	  " << endl;
        cout << "My choice: ";
        char choice;
        cin >> choice;
        while (!(choice >= '1' && choice <= '3')) {
            cin.ignore(9999, '\n');
            cout << "Invalid choice, please try again...\n";
            cin >> choice;
        }
        cin.ignore();
        string add_question;
        string add_answer;
    retry:
        system(clear);
        cout << "Enter the question:" << endl;
        getline(cin, add_question);
        if (add_question.length() == 0) {
            cout << "Invalid question, please try again..." << endl;
            system(pause);
            goto retry;
        }
        if (choice == '1') // add a True/False question
        {
            cout << "Enter the answer:" << endl;
            getline(cin, add_answer);
            if (add_answer != "true" && add_answer != "false") {
                cout << "Invalid answer, please try again..." << endl;
                system(pause);
                goto retry;
            }
            tf_v.push_back(TF());
            tf_v[tf_v.size() - 1].load(add_question, add_answer, tf_v); // load question into object
        }
        else if (choice == '2') //  add a complete question
        {
            cout << "Enter the answer:" << endl;
            getline(cin, add_answer);
            if (add_answer.length() == 0) {
                cout << "Invalid answer, please try again..." << endl;
                system(pause);
                goto retry;
            }
            complete_v.push_back(COMPLETE());
            complete_v[complete_v.size() - 1].load(add_question, add_answer, complete_v); // load question into object
        }
        else if (choice == '3') // add an mcq question
        {
            string add_answers[3];
            cout << "Enter the correct answer:" << endl;
            cout << "Choice 1: ";
            getline(cin, add_answer);
            cout << "Enter the other answers:" << endl;
            cout << "Choice 2: ";
            getline(cin, add_answers[0]);
            cout << "Choice 3: ";
            getline(cin, add_answers[1]);
            cout << "Choice 4: ";
            getline(cin, add_answers[2]);
            if (add_answer.length() == 0 || add_answers[0].length() == 0 || add_answers[1].length() == 0 ||
                add_answers[2].length() == 0) {
                cout << "Invalid choices, please try again" << endl;
                system(pause);
                goto retry;
            }
            mcq_v.push_back(MCQ());
            mcq_v[mcq_v.size() - 1].load(add_question, add_answer, add_answers, mcq_v); // load question into object
        }
        bool unchanged = (originalComplete == complete_v.size()) && (originalMCQ == mcq_v.size()) &&
            (originalTF == tf_v.size()); // if the question vector sizes remain unchanged
        if (!unchanged) cout << "Question added successfully" << endl; // question was added successfully
        else cout << "Question already exists" << endl; // question wasn't added
        system(pause);
        system(clear);
    }
};

istream& operator>>(istream& in, User*& a) {
retry:
    //get user input
    cout << "First name: ";
    getline(in, a->first);
    cout << "Last name: ";
    getline(in, a->last);
    cout << "Username: ";
    getline(in, a->name);
    cout << "Password: ";
    getline(in, a->password);
    bool cond = a->first.length() == 0 || a->last.length() == 0 || a->name.length() == 0 ||
        a->password.length() == 0; // check for any empty inputs
    if (cond) {
        system(clear);
        cout << "Invalid information, please try again..." << endl;
        goto retry;
    }
    return in;
}

int Login(vector<User*> users) {
    cout << "**********************************" << endl;
    cout << "Please enter your credentials:" << endl;
    cout << "**********************************" << endl;
    int acc_id = -1; // -1 : not found, -2 : wrong password, 0+ : user index
    string user_name, user_password;
    // get user's input
    cout << "Username: ";
    getline(cin, user_name);
    cout << "Password: ";
    getline(cin, user_password);
    system(clear);
    for (unsigned int i = 0; i < users.size(); i++) {
        if (user_name == users[i]->getName()) { //  check for user type using getter
            if (user_password != users[i]->getPass()) acc_id = -2; // if username is matching but password isn't
            else acc_id = i; // if username and password are matching
            break;
        }
    }
    return acc_id;
}

int main() {
    // Vectors to store questions
    vector<TF> tf;
    vector<COMPLETE> complete;
    vector<MCQ> mcq;
    // Vector of pointers to store users
    vector<User*> users;
    // Initialize default admin
    User* currentUser = new Admin;
    users.push_back(currentUser);
    // Initialize default player
    currentUser = new Player;
    users.push_back(currentUser);
    int exit = 0; // flag to exit program
retry:
    int id = Login(
        users); // index of the current user in the vector (-1 if user isn't found, -2 if entered a wrong password)
    if (id != -1 && id != -2) {
        currentUser = users[id];
        string acc_type = (currentUser->getType()) ? "ADMIN"
            : "PLAYER"; // check if user is admin or player using getter
    menu:
        cout << "Welcome " << currentUser->first << " (" << acc_type << "), please choose from the following options:"
            << endl;
        cout << "	[1] Switch accounts\n"
            "	[2] Update your info\n";
        currentUser->menu();
        cout << "	[" << currentUser->menu_options << "] Exit" << endl;
        cout << "My choice: ";
        char choice; // user's input
        cin >> choice;
        while (!(choice >= '1' && choice <= (char)(currentUser->menu_options +
            48))) { // menu_options : number of items in the menu for the current user type
            cin.ignore(9999, '\n');
            cout << "Invalid choice, please try again...\n";
            cin >> choice;
        }
        cin.ignore();
        if (choice == '1') {
            system(clear);
            goto retry; // retry : login screen
        }
        else if (choice == '2') currentUser->update_info(); // update user info (first name, last name, password)
        else {
            if (users[id]->getType()) { // if user is admin
                Admin* admin = reinterpret_cast<Admin*>(currentUser); // cast User pointer to Admin pointer
                switch (choice) {
                case ('3'):
                    admin->view_users(users);
                    break;
                case ('4'):
                    if (!admin->Register(users)) cout << "User already exists" << endl;
                    break;
                case ('5'):
                    admin->view_questions(tf, complete, mcq);
                    break;
                case ('6'):
                    admin->add_question(tf, complete, mcq);
                    break;
                case ('7'):
                    admin->load_file(tf, complete, mcq);
                    break;
                default:
                    break;
                }
            }
            else { // if user is not admin
                Player* player = reinterpret_cast<Player*>(currentUser); // cast User pointer to Player pointer
                switch (choice) {
                case ('3'): {
                    player->quiz.push_back(Quiz());
                    player->quiz[player->quiz.size() - 1].start_quiz(tf, complete, mcq);
                    break;
                }
                case ('4'):
                    player->score_stats(exit);
                    break;
                case ('5'):
                    player->all_scores();
                    break;
                case ('6'): {
                    unsigned int i = player->quiz.size();
                    system(clear);
                    if (i == 0) cout << "No quizzes taken yet" << endl;
                    else {
                        cout << "**********************************" << endl;
                        cout << "Details for last 2 quizzes:" << endl;
                        cout << "**********************************" << endl;
                        cout << "Quiz 1:" << endl;
                        cout << "**********************************" << endl;
                        cout << player->quiz[i - 1] << endl;
                        cout << "**********************************" << endl;
                        cout << "Quiz 2:" << endl;
                        cout << "**********************************" << endl;
                        if (i > 1) cout << player->quiz[i - 2] << endl;
                        system(pause);
                        system(clear);
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
        if (choice != (char)(currentUser->menu_options + 48) && exit == 0)
            goto menu; // as long as the user doesn't choose the last option, view the menu again
        system(clear);
        cout << "Exiting..." << endl; // otherwise, exit the program
    }
    else {
        system(clear);
        if (id == -1) cout << "Account not found, please try again..." << endl;
        else if (id == -2) cout << "Incorrect password, please try again..." << endl;
        goto retry;
    }
    return 0;
}

int* randomnumbers(int last, int numofInt) {
    // return an array with "numofInt" random numbers in the range [0, last]
    int* rand_nums; // create a pointer
    int i = 0; // create a counter
    rand_nums = new int[numofInt]; // create a dynamic array whose size is numofInt
    srand((unsigned)(time(nullptr) + rand())); // change the seed everytime the function is called
    // (time to change seed from run to run, rand to change the seed from call to call)
    while (i < numofInt) { // iterate for a number of numofInt
        int exists = 0; // flag to make sure the random number doesn't duplicate
        rand_nums[i] = rand() % (last + 1); // generate the random number
        // rand() % last generates a number between 0 and 'last-1'
        // rand() % last + first : shifts them by 'first' to become from 1 till 'last'
        // rand() % (last + 1) + first : (last + 1) to make it inclusive of last
        for (int j = 0; j < i; j++) { // check if the generated number exists
            if (rand_nums[i] == rand_nums[j]) {
                exists = 1;
                break;
            }
        }
        if (exists == 0) i++; // if it hasn't been found, increment i, otherwise, generate another one
    }
    return rand_nums;
}

void
SetFormat(string& variable)    // Set the first letter and each letter after a space to uppercase (string's varible)
{
    for (unsigned int i = 0; i < variable.length(); i++) {    // Remove all spaces at the begining of the string
        if (variable[i] == ' ') {
            for (unsigned int j = 0; j < variable.length(); j++) variable[j] = variable[j + 1];
            i--;
        }
        else break;
    }
    for (unsigned int i = variable.length() - 1; i > 0; i--) {    // Remove all spaces at the end of the string
        if (variable[i] == ' ') {
            variable[i] = '\0';
        }
        else break;
    }
    int start = 0;    // The start of a new word
    for (unsigned int i = 0; variable[i] != '\0'; i++) {
        if (variable[start] >= 97 && variable[start] <= 122)
            variable[start] -= 32;    // If the start of a word is lowercase, set it to uppercase
        if (variable[i] != ' ') {
            if (variable[i] >= 65 && variable[i] <= 90)
                variable[i] += 32;    // If the rest of a word is uppercase, set it to lowercase
        }
        else start = i + 1;    // If the current character is a space, skip to the next letter
    }
}

int is_numeric(string variable)    // Check if a string contains only numbers (string's variable)
{
    int numeric = 1;    // Flag for whether the current character is a number
    for (unsigned int i = 0; variable[i] != '\0'; i++) {
        if (!isdigit(variable[i])) numeric = 0;     // If the ASCII of current character doesn't correspond to a number
    }
    return numeric;    // Return the value of the flag
}

int char_to_int(string variable)    // Cast an array of numbers to an integer(Numbers' array)
{
    int num = 0;
    size_t length = variable.length();    // Converted value, Length of the array of numbers
    for (unsigned int i = 0; i < length; i++) {
        int power = 1;
        for (unsigned int j = 0; j < length - (i + 1); j++) {    // Get the multiplier of the current place
            power *= 10;
        }
        num += (variable[i] - '0') * power;    // Add current's place value
    }
    return num;    // Return the converted value
}