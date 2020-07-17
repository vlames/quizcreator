/*************************************************************************************************************************************************\
|   Name:            Vladimir Meshcheryakov                           										  |
|   Project:         Quiz Creator                                     										  |
|   File:            general_structures.cpp                           										  |
\*************************************************************************************************************************************************/

#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

#include <fstream>
using namespace std;


//This file describes how to use the class Character_Line and 
//the Command_List classes as stand alone classes. There are currently
//two different ways to create a horizontal line of characters: 
//by physically specifying the number of characters to display or by 
//making one of the constructors figure it out. 


//Funtion prototypes
void get_string(char * prompt_in, char *& text_in);
void get_integer(char * prompt_in, int & integer_in);
int find_escape_character(char * text_in, int *& indices_out, int & matches_num_out);
int add_characters(char *& text_out, int * indices_in, char character_in, int matches_num_in);



//Provides the interface feature to display the horizontal line of
//characters
class Character_Line
{
	public:
		//Initializes all of the class members to their zero
		//equivalent values
		//Input: none
		//Output: none
		Character_Line(void);


		//Initializes all members based on the user's input
		//Input: character symbol and integer value
		//Output: none
		Character_Line(char user_char, int user_num_char);

		
		//Initializes all the data members to the argument values
		//Input: a character and some character string
		//Output: none
		Character_Line(char user_char, char user_text[]);
	

		//Displays the horizontal line of characters
		//Input: none
		//Output: none
		void display(void);
	
	private:
		char character;
		int num_chars;
};



//Provides storage for a command
class Command
{
	public:
		//Initializes the data members to NULL
		//Input: none
		//Output: none
		Command(void);


		//Initializes and allocates memory to the data members
		//Input: name and symbol characters for a command
		//Output: none
		Command(char * name_in, char * symbols_in);


		//Allocates memory to and initializes the data members
		//Input: an object reference of Command
		//Output: none
		Command(const Command & command_in);


		//Deallocates all the dynamic memory used for members
		//Input: none
		//Output: none
		~Command(void);
		
		
		//Overloads the assignment operator so that the members of the class were initialized the same
		//Input: an object reference to get information from
		//Output: reference to the initialized object
		Command & operator=(const Command & command_in);
		
		
		//Sets the data members to the passed values
		//Input: a command name and its symbols
		//Output: none
		void set_name(char * name_in);


		//Sets the symbols for a command
		//Input: symbols from the client program
		//Output: none
		void set_symbols(char * symbols_in);

		
		//Compares symbols member to the passed argument
		//Input: symbols array
		//Output: true (if equal) or false otherwise
		bool compare_symbols(char * symbols_in);


		//Displays the contents of the members
		//Input: none
		//Output: none
		void display(void);


	private:

		char * name;
		char * symbols;
};



//Provides storage and use of a group of application commands
class Command_Node
{
	public:
		//Initializes the data members to NULL
		//Input: none
		//Output: none
		Command_Node(void);


		//Allocates memory to and initializes the data members of the class
		//Input: name of the comand group
		//Ouptput: none
		Command_Node(char * group_name_in, int num_commands_in);


		//Allocates memory to and initialize the data members
		//Input: an object of the Command_Node class
		//Output: none
		Command_Node(const Command_Node & node_in);


		//Deallocates all the dynamic memory used for the data members
		//Input: none
		//Output: none
		~Command_Node(void);
		

		//Sets the group_name data member
		//Input: a group name
		//Output: none
		void set_name(char * group_name_in);


		//Initializes the pointer it was called on to the address of the next member
		//Input: none
		//Output: address of the next pointer
		Command_Node *& go_next(void);


		//Connects two LLL links
		//Input: pointer to the object to connect
		//Output: none
		void connect(Command_Node * to_connect);
		
		
		//Adds a command to the group of commands
		//Input: a command
	       	//Output: none	
		void add_command(const Command & command_in);

		
		//Compares the group name passed to the member one
		//Input: some group name
		//Output: true or false
		bool compare_group_name(char * group_name_in);


		//Looks for a command specified by symbols from client
		//Input: command symbols from client program
		//Output: true (if found) or false otherwise
		bool find_command(char * symbols_in);


		//Displays the contents of the members
		//Input: none
		//Output: none
		void display(void);
	
	private:
	
		char * group_name;
		int max_num_commands;
		int current_command;
		
		Command ** commands;
		Command_Node * next;
		
		void increase_size(void);	
};



//Provides storage and use for all user-defined groups 
//of application commands
class Commands
{
	public:
		//Initializes the data member to NULL
		//Input: none
		//Output: none
		Commands(void);


		//Copies over the list of commands if it exists
		//Input: a list of commands
		//Output: none
		Commands(const Commands & commands_in);


		//Deallocates all the dynamic memory used for the member of the class
		//Input: none
		//Output: none
		~Commands(void);


		//Adds a command to the list of commands
		//Input: command and the command group name
		//Output: none
		void add_command_group(const Command_Node & node_in);


		//Removes a command from the list of commands
		//Input: command and the command group name
		//Output: none
		void remove_command_group(char * group_name);
		
		
		//Checks if there is a command specified by symbols
		//Input: command group name and symbols for a command
		//Output: true (if such command is found) or false
		bool verify_command(char * command_group_in, char * symbols_in);


		//Loads commands from an external file
		//Input: external file name
		//Output: success or failure (1/-1)
		int load(char * text_file, char * path_in);


		//Displays the commands for the particular group of commands
		//Input: command group name
		//Output: none
		void display(char * group_name_in);


		//Displays all the commands found in the list
		//Input: none
		//Output: none
		void display_all(void);	



	private:
		ifstream read_file;
		Command_Node * head; 		
		Command_Node * tail;
};


#endif
