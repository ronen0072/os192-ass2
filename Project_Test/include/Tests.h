#ifndef TESTS_H_
#define TESTS_H_

#include <vector>
#include <complex>
#include <string>

using std::string;
using std::vector;
using std::complex;

// Declaring Functions
void InitializingTests();
void Initialize();
void Finialize();
bool procceseTest(string testFile,unsigned int testNumber);
unsigned int CreateTests();
void sigintHandler(int num);
void sigquitHandler(int num);
void Operating_System_Test();
string processTestOutput(string output);
string processCompareString(string str);
void createXV6_TestMakefile(string makefilePath,vector<string> userspacePrograms);
string processAppendMakefileUPROGS(vector<string> append);
string replaceInString(string str,char toReplace,string replaceWith);
int countSubStr(string str,string findSubStr);
void runSpecificTest(int testToExecute);
void eraseUserPrograms(string &data, FILE * stream);
void addUserProgramsBuildCommands(string &data, vector<string> &userspacePrograms, string folder);

#endif
