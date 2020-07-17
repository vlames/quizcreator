/*************************************************************************************************************************************************\
|   Name:            Vladimir Meshcheryakov                           										  |
|   Project:         Quiz Creator                                     										  |
|   File:            core_structures.h                                										  |
\*************************************************************************************************************************************************/


#ifndef CORE_STRUCTURES_H
#define CORE_STRUCTURES_H

#include <fstream>
using namespace std;


//The Quiz class can be used to store questions and answer choices for
//a quiz. It can add, remove, change, and display the whole data base
//of questions and answer choices.


//Manages memory for a multiple choice question and answer choices
class MC_Question
{
	public:
		//Initializes all the members to zero equivalent
		//values
		//Input: none
		//Output: none
		MC_Question(void);


		//Allocates memory to and initializes the data members
		//Input: none
		//Output: none
		MC_Question(const int num_answer_choices_in);


		//Allocates memory to and initializes the data members
		//Input: a question and its answer choices
	        //Output: none
		MC_Question(char * question_in, int question_number, char ** answer_choices_in, const int num_answer_choices_in,
			    const int correct_choice_index);


		//Allocates memory to and initializes the data members
		//Input: a question reference
		//Output: none
		MC_Question(const MC_Question & question_in);
		

		//Deallocates the dynamically allocated memory for
		//the data members
		//Input: none
		//Output: none
		~MC_Question(void);


		//Copies the contents of the passed object
		//into the left operand
		//Input: an MC_Question object
		//Output: reference to the MC_Question object
		MC_Question & operator=(const MC_Question & in);
		

		//Supplies back the question member
		//Input: an array to copy to
		//Output: success or failure (1/-1) 
		int retrieve_question(char *& question_out);

		
		//Retrievs the number of answer choices
		//Input: none
		//Output: the num_answer_choices member
		int retrieve_number_acs(void);


		//Retrieves the answer choice requested
		//Input: array reference and the number signifying which array to copy
		//Output: success or failure (1/-1)
		int retrieve_answer(char *& answer_out, int number_in);
		
		
		//Retrieves the index of the correct answer choice
		//Input: none
		//Output: the index of the correct answer choice
		int retrieve_correct_answer_index(void);


		//Edits the contents of the question
		//Input: none 
		//Output: none
		void edit(void);


		//Displays the contents of a question
		//Input: none
		//Output: none
		void display(void) const;


	private:
		ofstream write_file;	

		char * question;
		int question_number;
		int num_answer_choices;
		char ** answer_choices;
		int correct_choice_index;

		void delete_all_answer_choices(void);
};



//Provides storage for all the questions and answer choices supplied
//from the client program. It allows multiple operations to manage
//the data base
class Quiz
{
	public:
		//Initializes all the members to zero equivalent
	       	//values
		//Input: none
		//Output: none
		Quiz(void);

		
		//Makes a copy of a quiz object initializing the
		//data members
		//Input: a Quiz object
		//Output: none
		Quiz(const Quiz & quiz_in);


		//Allocates memory to and initializes the data members
		//Input: none
		//Output: none
		Quiz(int num_questions_in, char * name_in);


		//Deallocates all the dynamic memory used
		//Input: none
		//Output: none
		~Quiz(void);
		

		//Loads questions from an external file
		//Input: external file name
		//Output: none
		int load(char * file_name_in, char * path_in);

		
		//Creates a quiz
		//Input: none
		//Output: success or failure (1/-1)
		int create(void);


		//Displays all the questions and answer choices
		//contained in the data base
		//Input: none
		//Output: none
		void display(void);


	private:

		ifstream read_file;
		ofstream write_file;	
	
		char * name;
		char * path;
		int num_questions;
		char ** file_names;
		MC_Question * questions;

		int make_directory(void);
		int create_html_file(void);
		int create_question_one(void);
		int create_question_two_up(int question_to_create);
		char * create_file_path(char * file_name);
		int create_score_calculator(void);
		int initialize_file_names(char * file_in);
		int write_question_two_up_unique_info(int question_to_create, char * file_path);
		int write_question_one_up_unique_info(int question_to_create, char * file_path);
};



#endif
