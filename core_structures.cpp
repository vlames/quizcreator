/************************************************************************************************************************************************\
|   Name:            Vladimir Meshcheryakov                           										 |
|   Project:         Quiz Creator                                     										 |
|   File:            core_structures.cpp                              										 |
\************************************************************************************************************************************************/


//The quiz's name data member is managed by the array of characters.
//Now, all the questions of the Quiz class, are stored in the array of
//character arrays. The answer choices, are managed by the array of 
//arrays of arrays of characters. The memory for the data members can
//be allocated by supplying integers: num_questions and
//num_answer_choices from the client program.


#include <iostream>
#include <cstring>
#include "core_structures.h"
#include "general_structures.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
using namespace std;

//Global constants
const int MAX_CHAR_READ = 200;
const int MAX_CHAR_IGNORE = 100;
const int MAX_CHAR_QUESTIONS_READ = 15;



//************************ MC_Question Class *****************************************************************************************************

//Initializes all the members to NULL equivalent values
//Input: none
//Output: none
MC_Question::MC_Question(void)
{
	question = NULL;
	question_number = 0;
	num_answer_choices = 0;
	answer_choices = NULL;
	correct_choice_index = 0;
}



//Allocates memory to and initializes the data members given the arguments
//Input: a question and a group of answer choices
//Output: none
MC_Question::MC_Question(char * question_in, int question_number_in, char ** answer_choices_in, const int num_answer_choices_in, 
		         const int correct_choice_index_in)
{
	//Copy the num_answer_choices member
	if(num_answer_choices_in < 0)
		num_answer_choices = 0;
	else
		num_answer_choices = num_answer_choices_in; 	
	

	//Copy or set to NULL the question member
	if(question_in)
	{
		question = new char[strlen(question_in) + 1];
		strcpy(question, question_in);
	}
	else
		question = NULL;


	//Copy or set to 0 the question_number member
	if(question_number_in < 0)
		question_number = 0;
	else
		question_number = question_number_in;
	

	//Copy or set to NULL all the elements of the answer_choices
	if(answer_choices_in)
	{
		//Copies every single answer choice from the passed argument if they exist. Otherwise, initializes the member to NULL
		int i;
		answer_choices = new char*[num_answer_choices];
		
		for(i = 0; i < num_answer_choices; ++i)
		{
			if(answer_choices_in[i])
			{
				answer_choices[i] = new char[strlen(answer_choices_in[i]) + 1];
				strcpy(answer_choices[i], answer_choices_in[i]);
			}
			else
				answer_choices[i] = NULL;
		}
	}
	else
		answer_choices = NULL;
	
	//Initialize the correct_choice_index member
	if(correct_choice_index_in < 0)
		correct_choice_index = 0;
	else
		correct_choice_index = correct_choice_index_in;
}



//Initializes the data members using the passed in argument
//Input: an MC_Question reference
//Output: none
MC_Question::MC_Question(const MC_Question & question_in)
{
	num_answer_choices = question_in.num_answer_choices;
	question_number = question_in.question_number;

	//If there is a question member in the argument object
	//copy it over. Otherwise, initialize member to NULL
	if(question_in.question)
	{
		question = new char[strlen(question_in.question) + 1];
		strcpy(question, question_in.question);
	}
	else
		question = NULL;


	//If there is an answer_choices member in the argument object, allocate memory for the member
	//and copy over the information. Otherwise, member gets the value of NULL
	if(question_in.answer_choices)
	{
		char ** answer_choices_in = question_in.answer_choices;
		answer_choices = new char*[num_answer_choices];
		
		int i;
		for(i = 0; i < num_answer_choices; ++i)
		{
			if(answer_choices_in[i])
			{
				answer_choices[i] = new char[strlen(answer_choices_in[i]) + 1];
				strcpy(answer_choices[i], answer_choices_in[i]);
			}
			else
				answer_choices[i] = NULL;
		}
	}
	else
		answer_choices = NULL;

	correct_choice_index = question_in.correct_choice_index;
}



//Deallocates all the dynamic memory allocated for members
//Input: none
//Output: none
MC_Question::~MC_Question(void)
{
	//If the question array was created, deallocate memory
	//allocated for it
	if(question)
	{
		delete [] question;
		question = NULL;
	}
	
	//Deallocate memory for the answer_chocies member
	if(answer_choices)
	{
		delete_all_answer_choices();
	}
}



//Deallocates all the dynamic memory used by the answer_choices member
//Input: none
//Output: none
void MC_Question::delete_all_answer_choices(void)
{
	//If the answer_choices array was created, deallocate memory
	//allocated for it
	if(answer_choices)
	{
		int i;	
		for(i = 0; i < num_answer_choices; ++i)
		{
			//Deallocate each element of the array
			if(answer_choices[i])
				delete [] answer_choices[i];	
		}
		delete [] answer_choices;
	}
	answer_choices = NULL;

	return;
}



//Copies the contents of the passed object into the left operand
//Input: an MC_Question object
//Output: reference to the MC_Question object
MC_Question & MC_Question::operator=(const MC_Question & in)
{
	//Copy the question member
	if(in.question)
	{
		//If memory has been previously allocated - deallocate
		if(question)
		{
			delete [] question;
		}

		question = new char[strlen(in.question) + 1];
		strcpy(question, in.question);
	}


	//Copy the num_answer_choices member
	num_answer_choices = in.num_answer_choices;


	//Copy the answer_choices member
	if(in.answer_choices)
	{
		char ** choices_in = in.answer_choices;

		//If memory has been previously allocated - deallocate
		if(answer_choices)
		{
			delete_all_answer_choices();
		}

		answer_choices = new char*[num_answer_choices];
		int i;
		for(i = 0; i < num_answer_choices; ++i)
		{
			//Copy each element
			if(choices_in[i])
			{
				answer_choices[i] = new char[strlen(choices_in[i]) + 1];
				strcpy(answer_choices[i], choices_in[i]);
			}
			else
			{
				answer_choices[i] = NULL;
			}
		}
	}

	//Copy the index of the correct answer choice
	correct_choice_index = in.correct_choice_index;
	
	return *this;
}



//Supplies back the question member
//Input: an array to copy to
//Output: success or failure (1/-1) 
int MC_Question::retrieve_question(char *& question_out)
{
	if(!question)
		return -1;
	else
	{
		question_out = new char[strlen(question) + 1];
		strcpy(question_out, question);
		return 1;
	}
	return -1;
}



//Retrievs the number of answer choices
//Input: none
//Output: the num_answer_choices member
int MC_Question::retrieve_number_acs(void)
{
	if(num_answer_choices < 0)
		return 0;
	return num_answer_choices;
}



//Retrieves the answer choice requested
//Input: array reference and the number signifying which array to copy
//Output: success or failure (1/-1)
int MC_Question::retrieve_answer(char *& answer_out, int number_in)
{
	if(!answer_choices)
		return -1;

	if(number_in < 0 || number_in >= num_answer_choices)
		return -1;

	if(answer_choices[number_in])
	{
		answer_out = new char[strlen(answer_choices[number_in]) + 1];
		strcpy(answer_out, answer_choices[number_in]);
		return 1;
	}

	return -1;
}



//Retrieves the index of the correct answer choice
//Input: none 
//Output: index of the correct answer choice
int MC_Question::retrieve_correct_answer_index(void)
{
	if(!answer_choices)
		return -1;

	int index = correct_choice_index;
	return index;
}



//Allows to change a question's content
//Input: none
//Output: none
void MC_Question::edit(void)
{
	//NOT YET IMPLEMENTED
	return;
}



//Displays the data members' information
//Input: none
//Output: none
void MC_Question::display(void) const
{
	//Displays the question data member 
	if(question)
	{
		cout << question;
		cout << endl;
	}

	//Displays each answer choices 
	if(answer_choices)
	{
		int i;
		
		//Displays each individual answer choice 
		for(i = 0; i < num_answer_choices; ++i)
		{
			if(answer_choices[i])
			{
				cout << answer_choices[i];
				if(i == correct_choice_index)
				{
					cout << " (correct)";
				}
				cout << endl;
			}
		}
	}
	return;
}



//*************************** Quiz Class ******************************

//Initializes the data members to 0 equivalent values
//Input: none
//Output: none
Quiz::Quiz(void)
{
	name = NULL;
	path = NULL;
	questions = NULL;
	num_questions = 0;
	file_names = NULL;
}



//Makes a copy of a quiz
//Input: a quiz object
//Output: none
Quiz::Quiz(const Quiz & quiz_in)
{
	//Copies the name data member
	if(quiz_in.name)
	{
		name = new char[strlen(quiz_in.name) + 1];
		strcpy(name, quiz_in.name);
	}
	else
	{
		name = NULL;
	}

	//Copies the path data member
	if(quiz_in.path)
	{
		path = new char[strlen(quiz_in.path) + 1];
		strcpy(path, quiz_in.path);
	}
	else
	{
		path = NULL;
	}
	
	//Copies the num_questions data member
	num_questions = quiz_in.num_questions;

	//Copies the questions data member
	if(quiz_in.questions)
	{
		MC_Question * questions_in;
		questions_in = quiz_in.questions;

		questions = new MC_Question[num_questions];
		
		int i;
		for(i = 0; i < num_questions; ++i)
		{
			questions[i] = questions_in[i];
		}
	}
	else
	{
		questions = NULL;
	}


	//Intializes the file_names member
	if(quiz_in.file_names)
	{
		char ** file_names_in = quiz_in.file_names;
		file_names = new char*[num_questions];

		int i;
		for(i = 0; i < num_questions; ++i)
		{
			//If a file name exists at that location, then copy it
			if(file_names_in[i])
			{
				file_names[i] = new char[strlen(file_names_in[i]) + 1];
				strcpy(file_names[i], file_names_in[i]);
			}
			else
				file_names[i] = NULL;
		}
	}
	else
		file_names = NULL;
}



//Deallocates all the dynamic memory used by data members
//Input: none
//Output: none
Quiz::~Quiz(void)
{
	//If quiz name was provided, deallocate memory for it
	if(name)
		delete [] name;
	name = NULL;

	//Deallocates memory for the path member
	if(path)
	{
		delete [] path;
	}
	path = NULL;

	//Deallocate memory for the questions
	if(questions)
	{
		delete [] questions;
	}
	questions = NULL;
	
	//Deallocate memory for the file_names
	if(file_names)
	{
		int i;
		for(i = 0; i < num_questions; ++i)
		{
			if(file_names[i])
			{
				delete [] file_names[i];
			}
		}
		delete [] file_names;
	}
	file_names = NULL;
}



//Loads all the questions from an external file
//Input: external file name
//Output: none
int Quiz::load(char * file_name_in, char * path_in)
{
	if(!file_name_in)
		return -1;
	
	//Lump together the file name and the path to it	
	char * text_file_with_path = NULL;	
	if(path_in)
	{	
		text_file_with_path = new char[strlen(file_name_in) + strlen(path_in) + 2];
		strcpy(text_file_with_path, path_in);
		strcat(text_file_with_path, "/");
		strcat(text_file_with_path, file_name_in);
		
		//Creates memory for the path member and initializes it
		if(path) 
		{
			delete [] path;
		}
		else
		{
			path = new char[strlen(path_in) + 1];
			strcpy(path, path_in);
		}

	}

	read_file.open(file_name_in);
	
	//Try to open the text file to read from using
	//or not using the file path.
	//If failed, deallocate allocated memory and return failure
	if(read_file.is_open() == false)
	{
		if(!text_file_with_path)
		{
			return -1;
		}
		else
		{
			read_file.open(text_file_with_path);
			if(read_file.is_open() == false)
			{
				delete [] text_file_with_path;
				return -1;
			}
		}
	}

	char * read_text = new char[MAX_CHAR_READ + 1];
	int num_answer_choices = 0;

	read_file.ignore(MAX_CHAR_IGNORE, ':');
	while(read_file.peek() == ' ' || read_file.peek() == '\t' || read_file.peek() == '\n')
	{
		read_file.ignore();
	}
		
	//Get and set the name of a quiz
	read_file.get(read_text, MAX_CHAR_READ, ';');
	read_file.ignore(MAX_CHAR_IGNORE, '\n');

	if(name)
		delete [] name;
	else
	{
		name = new char[strlen(read_text) + 1];
		strcpy(name, read_text);
		delete [] read_text;
	}


	read_file.ignore(MAX_CHAR_IGNORE, ':');
	while(read_file.peek() == ' ' || read_file.peek() == '\t' || read_file.peek() == '\n')
	{
		read_file.ignore();
	}	
	
	//Get and set the number of questions a quiz has
	read_file >> num_questions;

	//If the number of questions for the quiz is less then
	//zero, then initialize the num_questions to zero
	if(num_questions < 0)
		num_questions = 0;

	read_file.ignore(MAX_CHAR_IGNORE, '\n');

	
	//Get the number of answer choices the questions have
	read_file.ignore(MAX_CHAR_IGNORE, ':');
	while(read_file.peek() == ' ' || read_file.peek() == '\t' || read_file.peek() == '\n')
	{
		read_file.ignore();
	}
	read_file >> num_answer_choices;	
	
	//If the number of answer choices for the quiz is less then
	//zero, then initialize the number of answer choices to zero
	if(num_answer_choices < 0)
		num_answer_choices = 0;

	read_file.ignore(MAX_CHAR_IGNORE, '\n');
	
	//Ignore the line right after the first block of
	//information to read
	read_file.ignore(MAX_CHAR_IGNORE, '\n');


	
	//Allocate memory for the answer choices, questions
	//and index of the correct answer choice
	char ** answer_choices = new char*[num_answer_choices];
	int i;
	for(i = 0; i < num_answer_choices; ++i)
	{
		answer_choices[i] = new char[MAX_CHAR_READ + 1];
	}

	if(questions)
	{
		delete [] questions;
	}
	questions = new MC_Question[num_questions];
	int correct_choice_index = 0;	
	

	//Get the question information
	read_text = new char[MAX_CHAR_READ + 1];
	read_file.get(read_text, MAX_CHAR_READ, ';');
	
	//Set questions and answer_choices in the Quiz
	int question_number = 0;
	int j = 0;
	while(j < num_questions && read_file.eof() == false)
	{
		read_file.ignore(MAX_CHAR_IGNORE, '\n');
		
		//Read the answer_choices from file
		for(i = 0; i < num_answer_choices; ++i)
		{
			read_file.get(answer_choices[i], MAX_CHAR_READ, ';');
			read_file.ignore();
			if(read_file.peek() == ' ')
			{
				correct_choice_index = i;
			}
			read_file.ignore(MAX_CHAR_IGNORE, '\n');
		}
		read_file.ignore(MAX_CHAR_IGNORE, '\n');
			
		MC_Question question(read_text, question_number, answer_choices, num_answer_choices, correct_choice_index);
		questions[j] = question;	
		delete [] read_text;
		++question_number;
		++j;
		
		//Get the question information
		read_text = new char[MAX_CHAR_READ + 1];
		read_file.get(read_text, MAX_CHAR_READ, ';');
	}

	read_file.close();

	//Deallocate memory for the answer_choices
	for(i = 0; i < num_answer_choices; ++i)
	{
		if(answer_choices[i])
		{
			delete [] answer_choices[i];
		}
	}
	delete [] answer_choices;

	if(read_text)
		delete [] read_text;
	delete [] text_file_with_path; 

	return 1;
}



//Creates a quiz based on the loaded information
//Input: none
//Output: success or failure (1/-1)
int Quiz::create(void)
{
	int status = 0;

	//Creates a directory for the quiz file
	status = make_directory();
	if(status == -1 && errno != EEXIST)
	{
		cout << "Error: Failed to create a directory!\n";
		return -1;
	}
	else
	{
		cout << "Directory: " << name << " is created\n";
	}

	//Creates the .html file and returns success or,
        //returns only error value
	status = create_html_file();

	//List of potential error messages
	if(status == -1)
	{
		cout << "Error: Failed to create .html file!\n";
		cout << "Reason: Quiz name is not provided!\n";
		return -1;
	}
	else if(status == -2)
	{
		cout << "Error: Failed to create .html file!\n";
		cout << "Reason: Could not open file for writing!\n";
		return -1;
	}
	else if(status == -3)
	{
		cout << "Error: Failed to initialize file names!\n";
		cout << "Reason: Number of questions is less than 0!\n";
		return -1;
	}
	else if(status == -4)
	{
		cout << "Error: Failed to create .html file!\n";
		cout << "Reason: Number of questions is less than 0!\n";
		return -1;
	}
	else
	{
	}
	
	//Creates question1.js file 
	status = create_question_one();
	if(status != 1)
	{
		return status;
	}

	//Creates questions2.js file and up
	for(int question_to_create = 2; question_to_create < num_questions + 1; ++question_to_create)
	{
		status = create_question_two_up(question_to_create);
		if(status == -1)
		{
			cout << "Error: Failed to create question ";
			cout << question_to_create << ".js file!";
			cout << endl;
			return -1;
		}
	}

	//Creates score_calculator.js file
	status = create_score_calculator();
	if(status == -1)
	{
		cout << "Error: Failed to create calculate_score.js file!";
		cout << endl;
		return -1;
	}
	return 1;
}



//Creates a directory for the quiz files
//Input: none
//Output: success or failure (1/-1)
int Quiz::make_directory(void)
{
	char * directory_path = NULL;
	int status = -1;
	
	//If the class members exist and have first characters displayable,
	//then create the variable that will hold directory path and name
	//and create the directory itself. Otherwise, create a directory using
	//only name member
	if(path && name && isprint(path[0]) != 0 && isprint(name[0]) != 0)
	{
		directory_path = new char[strlen(path) + strlen(name) + 2];
		strcpy(directory_path, path);
		strcat(directory_path, "/");
		strcat(directory_path, name);

		status = mkdir(directory_path, S_IRUSR | S_IXUSR | S_IWUSR);
	}
	else
	{
		status = mkdir(name, S_IRUSR | S_IXUSR | S_IWUSR);
	}
	
	//If the directory could not be created in either way, return failure flag
	if(status == -1)
	{
		return -1;
	}
	
	//Deallocate memory used in the function
	delete [] directory_path;

	return 1;
}



//Creates the .html file using the loaded information
//Input: none
//Output: success or failure (1/-1)
int Quiz::create_html_file(void)
{
	//If the name of quiz is not given, then return with negative flag
	if(!name)
	{
		return -1;
	}

	char * file_in_directory = NULL; 
	
	//If the class members exist and have first characters displayable,
	//then create the variable that will hold directory path, quiz name, and extension
	//Also, create the .html file itself. Otherwise, create a .html file using
	//only name member
	if(path && name && isprint(path[0]) != 0 && isprint(name[0]) != 0)
	{
		file_in_directory = new char[strlen(path) + (2 * strlen(name)) + strlen(".html") + 3];
		strcpy(file_in_directory, path);
		strcat(file_in_directory, "/");
		strcat(file_in_directory, name);
		strcat(file_in_directory, "/");
		strcat(file_in_directory, name);
		strcat(file_in_directory, ".html");
	}
	else
	{
		file_in_directory = new char[2 * strlen(name) + strlen(".html") + 2];
		strcpy(file_in_directory, name);
		strcat(file_in_directory, "/");
		strcat(file_in_directory, name);
		strcat(file_in_directory, ".html");
	}

	//Opens directory and file with extension .html for writing information
	write_file.open(file_in_directory);

	if(write_file.is_open() == false)
	{
		delete [] file_in_directory;
		return -2;
	}

	//Write line 1 - 3
	write_file << "<!-- Quiz Maker v. 1.0 -->\n";
	write_file << "\n";
	write_file << "\n";

	//Write line 4 - 6
	write_file << "<html>\n";
	write_file << "<head>\n";
	write_file << endl;

	//Write line 7 - 8
	write_file << "<title>" << "Quiz Maker";
	write_file << "</title>\n";
	write_file << endl;	

	if(num_questions < 0)
	{
		delete [] file_in_directory;
		return -3;
	}

	//Write <script src ... lines
	int i;
	for(i = 0; i < num_questions; ++i)
	{
		write_file << "<script src = \"question";
		write_file << i + 1;
		write_file << ".js\"></script>\n";
	}

	//Write "score_calculator.js"..., </head> and spaces
	write_file << "<script src = \"score_calculator.js\">";
	write_file << "</script>\n";
	write_file << "</head>\n";
	write_file << endl;
	write_file << endl;

	//Write <body> and a new line
	write_file << "<body>\n";
	write_file << endl;

	//Write <script> ... var ... </script> and a new line
	write_file << "<script>\n";
	write_file << "var score = 0;\n";
	write_file << "var right_answer_num = 0;\n";
	write_file << "var total_questions_num = 0;\n";
	write_file << "</script>\n";
	write_file << endl;

	//Write the name of the test in h3 heading and new line
	write_file << "<p><b><h3>" << name;
	write_file << "</h3></b></p>\n";
	write_file << endl;

	//Write the Display Questions block and a new line
	write_file << "<!--Display Questions-->\n";

	for(i = 0; i < num_questions; ++i)
	{
		write_file << "<p id = \"question";
		write_file << i + 1;
		write_file << "\"></p>\n";
	}
	write_file << "<p id = \"score\"></p>\n";
	write_file << "<!--End of Display Questions-->\n";
	write_file << endl;

	//Write <script> ... </script> and a new line
	write_file << "<script>\n";
	write_file << "question1();\n";
	write_file << "</script>\n";
	write_file << endl;

	//Write </body> and </html>
	write_file << "</body>\n";
	write_file << "</html>";

	write_file.close();

	cout << "File: " << name << ".html" << " is created\n";


	int initialized = initialize_file_names(file_in_directory);

	//If file names could not be initialized, then return an error
	if(initialized == -1)
	{
		delete [] file_in_directory;
		return -4;
	}

	delete [] file_in_directory;
	return 1;
}



//Creates the (quiz).html file
//Input: none
//Output: success or failure of the operation 1/-1
int Quiz::create_question_one(void)
{
	int question_to_create = 1;
	int actual_question_num = question_to_create;
	int array_question_num = question_to_create - 1;
	char * file_path = create_file_path(file_names[array_question_num]);	

	if(!file_path)
		return -1;

	//If the file cannot be created, return with the appropriate flag	
	write_file.open(file_path);
	if(write_file.is_open() == false)
	{
		return -1;
	}

	//Write the Question 1 ... + end line
	write_file << "//Question " << actual_question_num << " Details\n";
	write_file << endl;

	//Write function question ... + {
	write_file << "function question" << actual_question_num << "()\n";
	write_file << "{\n";

	//Write information for question1.js file
	int status =  write_question_one_up_unique_info(question_to_create, file_path);
	if(status != 1)
	{
		if(file_path)
			delete [] file_path;
		return status;
	}

	write_file.close();

	cout << "File: " << file_names[array_question_num] << " is created\n";

	//Deallocate dynamic memory used
	if(file_path)
		delete [] file_path;

	return 1;
}


//Creates the first question of the quiz
//Input: none
//Output: success or failure (1/-1)
int Quiz::create_question_two_up(int question_to_create)
{
	int actual_question_num = question_to_create;
	int array_question_num = question_to_create - 1;
	char * file_path = NULL;

	//If file names to create don't exist, return with a flag 
	if(!file_names)
		return -1;
	else if (!file_names[array_question_num])
		return -1;
	else
	{
		file_path = create_file_path(file_names[array_question_num]);	
		if(!file_path)
			return -1;
	}

	//If the file cannot be created, return with the appropriate flag	
	write_file.open(file_path);
	if(write_file.is_open() == false)
	{
		return -2;
	}

	//Write the Question number ... + end line
	write_file << "//Question " << actual_question_num << " Details\n";
	write_file << endl;

	//Write function question ... + {
	write_file << "function question" << actual_question_num << "()\n";
	write_file << "{\n";

	//Creates all unique information for questions two and up
	int status = 1;	
	status = write_question_two_up_unique_info(question_to_create, file_path);

	if(status == -1)
	{
		if(file_path)
			delete [] file_path;
		return status;
	}

	write_file << endl;	

	//Creates the rest of the information for questions two and up 
	status =  write_question_one_up_unique_info(question_to_create, file_path);

	if(status == -1)
	{
		if(file_path)
			delete [] file_path;
		return status;
	}

	write_file.close();

	cout << "File: " << file_names[array_question_num] << " is created\n"; 

	if(file_path)
		delete [] file_path;
	return 1;
}



//Writes all the questions one and up unique information into a file specified by file_path
//Input: file_path and question number to create
//Output: failure or success of the operation 1/-1
//Note: file to write information to should be opened and closed outside of the function
int Quiz::write_question_one_up_unique_info(int question_to_create, char * file_path)
{
	//Check for possible invalid argument inputs
	if(!file_path)
		return -1;
	if(question_to_create < 1)
		return -1;

	int actual_question_num = question_to_create;
	int array_question_num = question_to_create - 1;

	if(write_file.is_open() == false)
		return -2;

	//Write tab + document.get ...
	if(actual_question_num > 1)
		write_file << "\t";
	write_file << "\t" << "document.getElementById(\"question";
	write_file << actual_question_num << "\").innerHTML = \"\\\n";

	//Write <form ...
	if(actual_question_num > 1)
		write_file << "\t";
	write_file << "\t" << "<form name = \'question" << actual_question_num; 
	write_file << "\'>\\\n";

	//Write <p> + question ...
	if(actual_question_num > 1)
		write_file << "\t";
	write_file << "\t" << "<p>";

	//Write the question based on its number
	char * question = NULL; 
	int retrieved = questions[array_question_num].retrieve_question(question);
	if(retrieved == -1)
	{
		return retrieved;
	}

	//Checks the question for escape characters
	int * indices = NULL;
	char character = '\\';
	int matches = 0;
	int found = find_escape_character(question, indices, matches);

	//If one or more escape characters were found, then fix the question
	if(found == 1)
		add_characters(question, indices, character, matches);

	if(indices)
	{
		delete [] indices;
		indices = NULL;
	}
	write_file << question << "</p>\\\n";

	char * answer = NULL; 
	int id_num = (actual_question_num - 1) + ((actual_question_num - 2) * 3) + 4;
	int correct_answer_index = questions[array_question_num].retrieve_correct_answer_index();
	int number_answer_choices = questions[array_question_num].retrieve_number_acs();

	//If the number of anwer choices or correct answer choice index is negative then return from 
	//the function deallocating memory	
	if(number_answer_choices <= 0 || correct_answer_index < 0)
	{
		if(question)
			delete [] question;
		return number_answer_choices;
	}

	//Write the answer choices' information formatted according to
	//the position of the correct answer choice.	
	for(int i = 0; i < number_answer_choices; ++i)
	{
		//If the correct answer choice is the first one to write,
		//then write <br> at the third iteration of the "for" loop
		if(correct_answer_index == 0 && i == 2)	
		{
			if(actual_question_num > 1)
				write_file << "\t";
			write_file << "\t" << "<br>\\\n";	
		}
		
		//If the correct answer choice is the second to be written,
		//then write <br> at the second iteration of the "for" loop
		if(correct_answer_index == 1 && i == 1)
		{
			if(actual_question_num > 1)
				write_file << "\t";
			write_file << "\t" << "<br>\\\n";	
		}

		//If the correct answer choice is the last to be written,
		//then write <br> at the second and third iteration of the "for" loop
		if((correct_answer_index == 2 && i == 1) || (correct_answer_index == 2 && i == 2))
		{
			if(actual_question_num > 1)
				write_file << "\t";
			write_file << "\t" << "<br>\\\n";	
		}

		//Adds right_answer information
		if(i == correct_answer_index)
		{
			if(actual_question_num > 1)
				write_file << "\t";
			write_file << "\t" << "<div id = ";
			write_file << "'right_answer";
			write_file << actual_question_num;
			write_file << "'>";

		}
		else
		{
			if(actual_question_num > 1)
				write_file << "\t";
			write_file << "\t";
		}
		
		//Writes <input type> ...
		write_file << "<input type = 'radio'";
		write_file << " id = '" << id_num << "'";
		write_file << " name = 'answer'";
		write_file << " value = '";
		
		//Writes 1 if the iteration matches the correct_answer index
		if(i == correct_answer_index)
			write_file << 1;
		else
			write_file << 0;
		
		int retrieved_answer_status = questions[array_question_num].retrieve_answer(answer, i);

		//Return from the function if the answer choices could not be retrieved
		if(retrieved_answer_status == -1)
		{
			return retrieved_answer_status;
		}
		
		//Looks for the escape caracters in the answer choice	
		matches = 0;
		indices = NULL;
		found = 0;
		found = find_escape_character(answer, indices, matches);

		//If one or more escape characters were found, then fix the question
		if(found == 1)
			add_characters(answer, indices, character, matches);
		
		if(indices)
		{
			delete [] indices;
			indices = NULL;
		}
		//Writes the correct answer choice ...
		write_file << "'>" << answer; 
		if(i == correct_answer_index)
			write_file << "</div>\\\n"; 
		else
			write_file << "\\\n";

		++id_num;

		if(answer)
			delete [] answer;
	}

	//Displays <p><input ...
	if(actual_question_num > 1)
		write_file << "\t";
	write_file << "\t" << "<p><input type = 'button'";
	write_file << " id = '" << id_num << "'";
	write_file << " value = ";

	//If the question number to create is less then the total
	//number of questions, then write "Next". Otherwise, ...
	if(actual_question_num < num_questions)
		write_file << "'Next'";
	else
		write_file << "'Calculate Score'";

	write_file << " " << "onclick = '";
	
	//If the question number to create is less then the total
	//number of questions, then write "question". Otherwise, ...
	if(actual_question_num < num_questions)
		write_file << "question" << actual_question_num + 1;
	else
		write_file << "calculate_score";
       	write_file << "()'></p>\\\n";


	//Displays form ...
	if(actual_question_num > 1)
		write_file << "\t";
	write_file << "\t" << "</form>\";\n";

	if(actual_question_num > 1)
	{
		write_file << "\t\t";	
		write_file << "++total_questions_num;";
		write_file << endl;
		write_file << "\t}";
		write_file << endl;
	}
	write_file << "}";
	
	//Deallocate dynamic memory
	if(question)
		delete [] question;

	return 1;
}



//Writes all the questions two and up unique information into a file specified by file_path
//Input: file_path and question number to create
//Output: failure or success of the operation 1/-1
//Note: file to write information to should be opened and closed outside of the function
int Quiz::write_question_two_up_unique_info(int question_to_create, char * file_path)
{
	//Check for possible invalid argument inputs
	if(!file_path)
		return -1;
	if(question_to_create < 2)
		return -1;

	int actual_question_num = question_to_create;
	int array_question_num = question_to_create - 1;

	if(write_file.is_open() == false)
		return -2;
	
	int id_num = (actual_question_num - 1) + (actual_question_num - 2)*3;
	int answer_choices_num = 0;
	if(actual_question_num  == num_questions + 1)
		answer_choices_num = questions[array_question_num - 1].retrieve_number_acs();
	else
		answer_choices_num = questions[array_question_num].retrieve_number_acs();

	//Return from the function if the number of answer choices is less than 0
	if(answer_choices_num < 0)
	{
		return answer_choices_num;
	}
	
	//Write the tab and the if	
	write_file << "\t" << "if(";

	//Write the document ... statements based on the
	//number of answer choices available
	for(int i = 0; i < answer_choices_num; ++i)
	{
		write_file << "document.";
		write_file << "getElementById";
		write_file << "(\"" << id_num;
		write_file << "\").checked";
		write_file << " == false";

		if(i < answer_choices_num - 1)
			write_file << " && ";

		if(i == answer_choices_num - 1)
			write_file << ")\n";
		++id_num;
	}

	//Write tab, braket, alert ..., and braket
	write_file << "\t{\n";
	write_file << "\t\t" << "alert(\"PLEASE, SELECT ONE";
	write_file << " OF THE CHOICES!\");\n";
	write_file << "\t}\n";

	//Write else and braket
	write_file << "\t" << "else\n";
	write_file << "\t{\n";


	char * answer = NULL;

	//Obtain the correct answer index of the previous question and its id
	int correct_answer_index = questions[array_question_num - 1].retrieve_correct_answer_index();
	int correct_answer_id = (actual_question_num - 1) + (actual_question_num - 2) * 3 + correct_answer_index;	

	if(correct_answer_index < 0)
	{
		return correct_answer_index;
	}

	//Write the if block and braket
	write_file << "\t\t" << "if(document.getElementById";
	write_file << "(\"" << correct_answer_id;
	write_file << "\").checked == true)\n";
	write_file << "\t\t{\n";

	//Write the document.get ... 
	write_file << "\t\t\t" << "document.getElementById";
	write_file << "(\"right_answer";
	write_file << actual_question_num - 1;
	write_file << "\").innerHTML = \"\\";
	write_file << endl;

	//Write the <input type ...
	write_file << "\t\t\t" << "<input type = \'radio\'";
	write_file << " id = '";
	write_file << correct_answer_id;
	write_file << "' name = \'answer\' value ";
	write_file << "= \'1\' checked><mark style = ";
	write_file << "\'background-color:53D800\'>";

	//Insert correct answer text here
	int retrieved_status = questions[array_question_num - 1].retrieve_answer(answer, correct_answer_index);

	//If an answer choice could not be retrieved, then return from function
	if(retrieved_status == -1)
	{
		return retrieved_status;
	}
	
	//Checks the question for escape characters
	int * indices = NULL;
	int matches = 0;
	char character = '\\';
	int found = find_escape_character(answer, indices, matches);

	//If one or more escape characters were found, then fix the question
	if(found == 1)
		add_characters(answer, indices, character, matches);

	if(indices)
	{
		delete [] indices;
		indices = NULL;
	}
	
	write_file << answer;
	write_file << "</mark>\";";
	write_file << endl;

	//Write ++right_answer_num
	write_file << "\t\t\t";
	write_file << "++right_answer_num;";
	write_file << endl;

	//Write the closing bracket
	write_file << "\t\t";
	write_file << "}";
	write_file << endl;

	//Write else
	write_file << "\t\t";
	write_file << "else";
	write_file << endl;

	//Write the opening bracket
	write_file << "\t\t";
	write_file << "{";
	write_file << endl;

	//Write document.getElementById ...
	write_file << "\t\t\t";
	write_file << "document.getElementById(\"";
	write_file << "right_answer";
	write_file << actual_question_num - 1;
	write_file << "\").innerHTML = \"\\\n";

	//Write <input ...
	write_file << "\t\t\t";
	write_file << "<input type = 'radio' id = \'";
	write_file << correct_answer_id;
	write_file << "\' ";
	write_file << "name = \'answer\' value = \'";
	write_file << "1";
	write_file << "\'>";
	write_file << "<mark style = \'background-color:";
	write_file << "53D800\'>";
	write_file << answer;
	write_file << "</mark>\";";
	write_file << endl;

	//Write the backward bracket
	write_file << "\t\t}";
	write_file << endl;

	write_file << endl;

	//Write the document ... statements based on the
	//number + 1 of answer choices available
	id_num = (actual_question_num - 1) + (actual_question_num - 2) * 3;
	int button_num_to_disable = 4;	
	for(int i = 0; i < button_num_to_disable;  ++i)
	{
		write_file << "\t\t";
		write_file << "document.";
		write_file << "getElementById";
		write_file << "(\"" << id_num;
		write_file << "\").disabled";
		write_file << " = true;";
		write_file << endl;

		++id_num;
	}

	if(answer)
	{
		delete [] answer;
	}

	return 0;
}



//Creates the score_calculator.js file
int Quiz::create_score_calculator(void)
{
	int actual_question_num = num_questions + 1;
	char * file_path = NULL;
	
	char * file_name = new char[strlen("score_calculator") + strlen(".js") + 1];
	strcpy(file_name, "score_calculator");
	strcat(file_name, ".js");

	file_path = create_file_path(file_name);	

	if(!file_path)
		return -1;
	
	//If the file cannot be created, return with the appropriate flag	
	write_file.open(file_path);
	if(write_file.is_open() == false)
	{
		return -2;
	}

	//Write the Calculate a score ... + end line
	write_file << "//Calculate a score based on the answers to the ";
	write_file << "quiz questions\n";
	write_file << endl;

	//Write function calculate_score ... + {
	write_file << "function calculate_score()\n";
	write_file << "{\n";
	
	//Creates all unique information for questions two and up
	int status = 1;	
	status = write_question_two_up_unique_info(actual_question_num, file_path);
	
	if(status == -1)
	{
		if(file_path)
			delete [] file_path;
		if(file_name)
			delete [] file_name;
		return status;
	}

	//Write ++total...
	write_file << "\t\t";
	write_file << "++total_questions_num;\n";

	write_file << endl;

	//Write score = ...
	write_file << "\t\t";
	write_file << "score = right_answer_num / total_questions_num;\n";
	
	//Write score *= ...	
	write_file << "\t\t";
	write_file << "score *= 100;\n";

	//Write score = Math ...
	write_file << "\t\t";
	write_file << "score = Math.round(score);\n";

	write_file << endl;

	//Write document.get ...
	write_file << "\t\t";
	write_file << "document.getElementById(\"score\")";
	write_file << ".innerHTML = \"<b><h3>Score: \"";
	write_file << " + score + \"%</h3></b>\";\n";

	//Write curly bracket
	write_file << "\t}\n";
	
	//Write curly bracket
	write_file << "}\n";

	write_file.close(); 
	
	cout << "File: " << file_name << " is created\n";
	
	//Deallocate dynamic memory
	if(file_path)
		delete [] file_path;
	delete [] file_name;

	return 0;
}


//Provides a file path using quiz name and passed file name argument or
//a file path using path, name, and file_name members
//Input: name of the file to be included in the path 
//Output: path, in the form Quiz_Name/File_Name
char * Quiz::create_file_path(char * file_name)
{
	//If file name was not provided, then return failure signal
	if(!file_name)
	{
		return NULL;
	}

	char * file_in_directory = NULL; 

	if(path && isprint(path[0]) != 0)
	{
		//Creates a variable that holds full file path
		file_in_directory = new char[strlen(path) + strlen(name) + strlen(file_name) + 3];
		strcpy(file_in_directory, path);
		strcat(file_in_directory, "/");
		strcat(file_in_directory, name);
		strcat(file_in_directory, "/");
		strcat(file_in_directory, file_name);
	}
	else
	{
		//Concatenates Directory Name, "/", and File Name 
		file_in_directory = new char[strlen(name) + strlen(file_name) + 2];
		strcpy(file_in_directory, name);
		strcat(file_in_directory, "/");
		strcat(file_in_directory, file_name);
	}

	return file_in_directory;
}



//Initializes all the question file names to be created
//Input: none
//Output: success or failure (1/-1)
int Quiz::initialize_file_names(char * file_in)
{
	if(!file_in)
		return -1;

	read_file.open(file_in);

	if(read_file.is_open() == false)
	{
		cout << "Error: Failed to initialize file names\n";
		return -1;
	}

	int i;
	int num_lines_to_ignore = 8;
	for(i = 0; i < num_lines_to_ignore; ++i)
	{
		read_file.ignore(MAX_CHAR_IGNORE, '\n');
	}

	//If there is previously allocated memory for the file names
	//deallocate it
	if(file_names)
	{
		for(i = 0; i < num_questions; ++i)
		{
			if(file_names[i])
			{
				delete [] file_names[i];
			}
		}
		delete [] file_names;
	}

	file_names = new char*[num_questions];

	//Get all the question file names from .html file
	int max = MAX_CHAR_QUESTIONS_READ;
	for(i = 0; i < num_questions; ++i)
	{
		file_names[i] = new char[max + 1];
		read_file.ignore(MAX_CHAR_IGNORE, '\"');
		read_file.get(file_names[i], max, '\"');
		read_file.ignore(MAX_CHAR_IGNORE, '\n');
	}

	read_file.close();
	return 1;
}

//Displays all the data members of the class
//Input: none
//Output: none
void Quiz::display()
{
	//Displays the quiz name data member
	if(name && isprint(name[0]) == 1)
	{
		cout << "Quiz Name: " << name;
		cout << endl;
	}
	else
	{
		cout << "Quiz Name: None";
		cout << endl;
	}


	//Displays the number of questions loaded
	cout << "Number of questions: ";
	cout << num_questions;
	cout << endl;

	if(questions && num_questions > 0)
	{
		cout << endl;
		
		int i;
		for(i = 0; i < num_questions; ++i)
		{
			questions[i].display();
			if(i < num_questions - 1)
				cout << endl;
		}
	}	
	return;
}
