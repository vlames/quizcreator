/*************************************************************************************************************************************************\
|   Name:            Vladimir Meshcheryakov                           										  |
|   Project:         Quiz Creator                                     										  |
|   File:            main.cpp                                         										  |
\*************************************************************************************************************************************************/

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <mach-o/dyld.h>
#include "general_structures.h"
#include "core_structures.h"
using namespace std;


//Function Prototypes
int execute_command(Commands & cmds_in, char *& cmdg_in, char * cmd_in, Quiz & quiz_in, Character_Line & line_in);
int get_exe_dir(char *& path_out, int path_out_size);


//Global constants 
const int MAX_COMMAND_GROUP_CHAR = 20;
const int MAX_COMMAND_CHAR = 10;
const int MAX_CHAR_PROMPT = 40;
const int EXECUTED = 1;
const int NOT_EXECUTED = -1;
const int TURN_OFF = 0;
const int MAX_PATH_CHAR = 150;


int main(void)
{
	//Create objects to be used
	ifstream read_file;
	ofstream write_file;
	Character_Line star_line('*', 80);
	Quiz quiz;
	Commands commands;
	
	//Initializing the veriables to be used	
	int obtained_path = -1;
	int path_size = MAX_PATH_CHAR;
	bool exe_path_nobtained = false;
	bool w_path_nobtained = false;
	char * exe_path = NULL; 
	char * working_path = NULL;
	char * commands_file = NULL;
	char * questions_file = NULL;
	char * obtained_w_path = NULL;
	
	//Initializing text files to read information from
	commands_file = new char[strlen("commands.txt") + 1];
	strcpy(commands_file, "commands.txt");
	questions_file = new char[strlen("questions.txt") + 1];
	strcpy(questions_file, "questions.txt");

	//Get the directory path to the executable
	exe_path = new char[MAX_PATH_CHAR + 1];
	obtained_path = get_exe_dir(exe_path, path_size);

	
	cout << endl;


	//If the directory path for executable was not computed, set the flag
	//to true and try obtaining the current working directory path
	if(obtained_path != 0)
	{
		exe_path_nobtained = true;
		size_t size = 200;

		working_path = new char[size + 1];
		obtained_w_path = getcwd(working_path, size);
		
		//If the current working directory path could not be obtained
		//set appropriate flag to true. Otherwise, to false
		if(obtained_w_path == NULL || *obtained_w_path != '/')
		{
			w_path_nobtained = true;
		}
		else
		{
			w_path_nobtained = false;
		}

	}
	else
	{
		exe_path_nobtained = false;
	}


	//Load the available commands from a text file
	int status = commands.load(commands_file, exe_path);
	
	
	if(status == -1)
	{
		star_line.display();
		if(exe_path_nobtained == true || w_path_nobtained == true)
		{
			cout << "Error: " << strerror(errno) << endl;
			cout << "Solutions:\n";
			cout << "Solution 1: Allow this program to read and write!\n";
			cout << "Solution 2: Place: " << commands_file << " and ";
			cout << questions_file << " to current working directory\n";
			cout << "Solution 3: Run the program using terminal or command prompt\n";
		}
		else
		{
			cout << "Error: " << commands_file << " doesn't exist!\n";
		}
		
	        cout << "Exiting ...\n";
		star_line.display();
		cout << endl;
		
		//Deallocate memory
		if(working_path)
			delete [] working_path;
		delete [] exe_path;	
		delete [] commands_file;
		delete [] questions_file;

		return 0;
	}


	//Loads all the questions from a file using/not using the path
	status = quiz.load(questions_file, exe_path);


	if(status == -1)
	{
		star_line.display();
		if(exe_path_nobtained == true || w_path_nobtained == true)
		{
			cout << "Error: " << strerror(errno) << endl;
			cout << "Solutions:\n";
			cout << "Solution 1: Allow this program to read and write!\n";
			cout << "Solution 2: Place: " << commands_file << " and ";
			cout << questions_file << " to current working directory\n";
			cout << "Solution 3: Run the program using terminal or command prompt\n";
		}
		else
		{
			cout << "Error: " << questions_file << " doesn't exist!\n";
		}
		
	        cout << "Exiting ...\n";
		star_line.display();
		cout << endl;
		
		//Deallocate memory
		if(working_path)
			delete [] working_path;
		delete [] exe_path;	
		delete [] commands_file;
		delete [] questions_file;

		return 0;
	}

	//Allocate memory for prompting 
	char * prompt = new char[MAX_CHAR_PROMPT + 1];

	//Display greeting
	star_line.display();	
	cout << "Wellcome to Quiz Creator\n";
	star_line.display();	

	
	cout << endl;	

	//Display starting commands	
	char * command_group = new char[MAX_COMMAND_GROUP_CHAR + 1];
	char * command = new char[MAX_COMMAND_CHAR + 1];
	strcpy(command_group, "Quiz");
	commands.display(command_group);	


	//Keep asking for a command until user decides to turn
       	//the application off	
	int state = -1;	
	while(state != TURN_OFF)
	{
		cout << endl;	
		star_line.display();
		strcpy(prompt, "Enter a command (dc): ");
		get_string(prompt, command);	
		star_line.display();
		cout << endl;
		
		//Executes all the available commands
		state = execute_command(commands, command_group, command, quiz, star_line);
	}

	//Deallocate dynamic memory
	delete [] prompt;
	delete [] command;
	delete [] command_group;
	delete [] commands_file;
	delete [] questions_file;
	delete [] exe_path; 

	cout << endl;
	return 0;
}



//Executes the available commands
//Input: available commands
//Output: a number to signify the operations performed 
int execute_command(Commands & cmds_in, char *& cmdg_in, char * cmd_in, Quiz & quiz_in, Character_Line & line_in)
{
	bool found;

	//Verify if the passed in command exists in the passed
	//group of commands. If it doesn't, signal that the
	//command was not executed.
	found = cmds_in.verify_command(cmdg_in, cmd_in);
	if(!found)
	{
		cout << "Command is not Found!";
		cout << endl;
		return NOT_EXECUTED;
	}

	
	//If command group was chosen to be a Quiz 
	//then display and perform Quiz operations
	if(strcmp(cmdg_in, "Quiz") == 0)
	{

		//Creates the entire quiz
		if(strcmp(cmd_in, "cq") == 0)
		{
			int status = quiz_in.create();
			
			if(status == -1)
			{
				return NOT_EXECUTED;
			}
			return EXECUTED;
		}

		//Recreates the question specified by user
		if(strcmp(cmd_in, "rq") == 0)
		{
			cout << "NOT IMPLEMENTED YET!\n";
			return EXECUTED;
		}

		//Displays the quiz information 
		if(strcmp(cmd_in, "dq") == 0)
		{
			quiz_in.display();
			return EXECUTED;
		}

		//Displays the available commands
		if(strcmp(cmd_in, "dc") == 0)
		{
			//Displays all the quiz information
			cmds_in.display(cmdg_in);
			return EXECUTED;
		}
	}



	//If the user decides to turn off the phone
	//then set the flag to 0
	if(strcmp(cmd_in, "off") == 0)
	{
		cout << "Shutting Quiz Creator Down ...\n";
		cout << endl;
		line_in.display();
		return TURN_OFF;
	}

	return NOT_EXECUTED;
}



//Supplies back the current executable's directory path (Mac OSX 10.2+)
//Input: container for the path and its size
//Output: success or failure flags 0/-1
int get_exe_dir(char *& path_out, int path_out_size)
{
	int status = 0;
	int path_length = 0;
	uint32_t path_size = path_out_size;
	char * real_path = NULL;

	//Return appropriate flag if any of no memory for path is given
	//or the size is not valid	
	if(!path_out || path_out_size < 0)
	{
		return -1;
	}
	
	status = _NSGetExecutablePath(path_out, &path_size);

	//On failure of getting the path, try again.
	//Otherwise, get real path through real_path
	if(status == -1)
	{
		if(errno != 0)
		{
			return errno;
		}
		else
		{
			delete [] path_out;
			path_out_size = path_size;
			path_out = new char[path_out_size + 1];
		}
	}
	else
	{
		real_path = new char[strlen(path_out) + 1];
		realpath(path_out, real_path);

		delete [] path_out;
		path_length = strlen(real_path);
		path_out = new char[path_length + 1];
		strcpy(path_out, real_path);
	}
	
	//Initializing elements after the last '/' in the path to '\0'
	int i = path_length - 1;	
	char * ptr = &path_out[i];
	while(ptr && *ptr != '/' && i != 0)
	{
		*ptr = '\0';
		--ptr;
		--i;
	}

	//Remove the '/' symbol from the path
	if(ptr && *ptr == '/')
	{
		*ptr = '\0';
	}

	delete [] real_path;
	
	return status;
}
