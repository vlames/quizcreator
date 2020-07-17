/************************************************************************************************************************************************\
|   Name:            Vladimir Meshcheryakov                           									         |
|   Project:         Quiz Creator                                     										 |
|   File:            general_functions.cpp                            										 |
\************************************************************************************************************************************************/


//The file uses general_structures.h file, its class and function 
//definitions to implement functionality. The class uses basic data
//types: characters, integers, and strings of characters. 
//Initialization lists are used to assign values to a newly created 
//object. No sophisticated algorithms are used. To provide usage of
//commands, linear linked list of Command objct arrays is used.


#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "general_structures.h"
using namespace std;


//Global constants
const int MAX_NUM_COMMANDS = 5; 
const int MAX_CHAR_READ = 50;
const int MAX_CHAR_IGNORE = 100;



//********************** Miscelleneous Functions *************************************************************************************************

//Initializes variable: text
//Input: prompt and text string variables
//Output: none
void get_string(char * prompt, char *& text)
{
    cout << prompt;
    cin.get(text, MAX_CHAR_READ, '\n');
    while(!cin)
    {
        cin.clear();
        cin.ignore(MAX_CHAR_IGNORE, '\n');
        cout << prompt;
        cin.get(text, MAX_CHAR_READ, '\n');
    }
    cin.ignore(MAX_CHAR_IGNORE, '\n');	
    return;
}



//Initializes the variable: integer
//Input: prompt and integer variable
//Output: none
void get_integer(char * prompt, int & integer)
{
    cout << prompt;
    cin >> integer;
    
    while (!cin)
    {
        cin.clear();
        cin.ignore(MAX_CHAR_IGNORE, '\n');
        cout << prompt;
        cin >> integer;
    }
    cin.ignore(MAX_CHAR_IGNORE, '\n');
    return;
}



//Looks for an escape character in the text input and gives their location and number back
//Input: some array of characters, empty indices array, and int container for the number of matches 
//Output: success or failure and escape character indices 1/-1 and an array of integers >= 0 
//Note: indices_out array should not have memory previously allocated outside of this function
int find_escape_character(char * text_in, int *& indices_out, int & matches_num_out)
{
	//If text is empty, do nothing
	if(!text_in)
		return -1;
	
	//If indeces_out size was previously allocated, deallocate memory 
	if(indices_out != NULL)
		delete [] indices_out;

	//Check the given text for specific escape characters and store
       	//locations of those escape characters in the array	
	char * char_ptr = text_in;
	bool found = false;
	int num = 0;
	int num_characters = strlen(text_in);
	indices_out = new int[num_characters];
	
	//Initializing the indices_out array
	for(int i = 0; i < num_characters; ++i)
	{
		indices_out[i] = 0;
	}

	for(int i = 0; i < num_characters; ++i)
	{
		if('\"' == char_ptr[i] || '\'' == char_ptr[i])
		{
			indices_out[num] = i;
			++num;
			++matches_num_out;
			found = true;
		}
	}
	
	//If after searching, no escape characters were found, return -1
	if(found == false)
	{
		matches_num_out = 0;
		return -1;
	}

	return 1;
}



//Adds specified characters to the specified locations in text using indices
//Input: text to modify, character to add, and indices with locations
//Output: formatted text or failure
int add_characters(char *& text_out, int * indices_in, char character_in, int matches_num_in)
{
	//If the text to format is empty, return with -1
	if(!text_out || !indices_in)
		return -1;
	
	//Find out the number of characters to add
	int num_indices = matches_num_in;

	//Find out the number of characters in the text and increase size
	//according to how many characters need to be incerted
	int old_size = strlen(text_out);
	
	char * old_char_ptr = text_out;
	char * new_text = new char[old_size + num_indices + 1];

	char * new_char_ptr = new_text;
	int indices_index = 0;
	
	//Copy over the old characters and the new ones character by character
	for(int i = 0; i < old_size; ++i)
	{

		//If the iteration matches with the index of the special character 
		//to be put in, then copy special character in and
	       	//advance the pointer	
		if(indices_in[indices_index] == i)
		{
			*new_char_ptr = character_in;	
			++new_char_ptr;
			++indices_index;
			*new_char_ptr = *old_char_ptr;
		}
		else
		{
			*new_char_ptr = *old_char_ptr;
		}

		if(i < old_size - 1)
		{
			++old_char_ptr;
			++new_char_ptr;
		}
		else
		{
			++old_char_ptr;
			++new_char_ptr;
			*old_char_ptr = '\0';
			*new_char_ptr = '\0';
		}
	}

	delete [] text_out;
	text_out = new_text;

	return 1;
}

//*************************************************************************************************************************************************



//Initializes all the members of the class to their zero equivalent
//values using an initialization list
//Input: none
//Output: none
Character_Line::Character_Line(void):character('0'), num_chars(0){}



//Initiaizes all the members of the class using the passed arguments
//and the initialization list technique
//Input: a character and an integer
//Output: none
Character_Line::Character_Line(char user_char, int user_num_char):character(user_char), num_chars(user_num_char){}



//Initializes the data members using the passed argument and the 
//size of the string of characters
//Input: some character and string of characters
//Output: none
Character_Line::Character_Line(char user_char, char user_text[])
{
	character = user_char;
	num_chars = strlen(user_text);	
}



//Displays the horizontal line of characters using the data members
//Input: none
//Output: none
void Character_Line::display(void)
{
	cout << left << setfill(character) << setw(num_chars);
	cout << character;
	cout << setfill(' ');
	cout << endl;
	return;
}



//********************** Application Commands Classes ********************************************************************************************

//Initializes the members to NULL
//Input: none
//Output: none
Command::Command(void): name(NULL), symbols(NULL){}


//Allocates memory to and initializes the data members
//Input: some name and symbols fomr the client program
//Output: none
Command::Command(char * name_in, char * symbols_in)
{
	//Initialize the name data member
	if(name_in)
	{
		name = new char[strlen(name_in) + 1];
		strcpy(name, name_in);
	}
	else
		name = NULL;

	//Initialize the symbols data member
	if(symbols_in)
	{
		symbols = new char[strlen(symbols_in) + 1];
		strcpy(symbols, symbols_in);
	}
	else
		symbols = NULL;
}



//Initializes the data members using the Comand object
//Input: reference to the Command object
//Output: none
Command::Command(const Command & command_in)
{
	//Initialize the name data member
	if(command_in.name)
	{
		name = new char[strlen(command_in.name) + 1];
		strcpy(name, command_in.name);
	}
	else
		name = NULL;

	//Initialize the symbols data member
	if(command_in.symbols)
	{
		symbols = new char[strlen(command_in.symbols) + 1];
		strcpy(symbols, command_in.symbols);
	}
	else
		symbols = NULL;
}



//Overloads the assignment operator to allow an easy copy of
//an instance of the class
//Input: constant reference to an object of Command
//Output: the reference to the object that got initialized 
Command & Command::operator=(const Command & command_in)
{
	//If the referenced object and the current object
	//have the same memory address, then do not copy anything
	if(this == &command_in)
	{
		return *this;
	}

	//Deallocate memory if the object to copy to already has
	//something in it
	if(name)
	{
		delete [] name;
		name = NULL;
	}

	if(symbols)
	{
		delete [] symbols;
		symbols = NULL;
	}	
	
	//If the original name and symbols members exist,
	//copy over the information
	if(command_in.name)
	{
		name = new char[strlen(command_in.name) + 1];
		strcpy(name, command_in.name);
	}

	if(command_in.symbols)
	{
		symbols = new char[strlen(command_in.symbols) + 1];
	}
	return *this;
}



//Defines the name and the symbols members of the class
//Input: name and symbols from the client program
//Output: none
void Command::set_name(char * name_in)
{
	//If a name of the command was passed in, the do the following
	if(name_in)
	{
		//If the name of the command was previously defined
		//delete it. Otherwise, allocate memory and copy
		if(name)
			delete [] name;
		
		name = new char[strlen(name_in) + 1];
		strcpy(name, name_in);
	}

	return;
}



//Defines the symbols member of the class
//Input: symbols array
//Output: none
void Command::set_symbols(char * symbols_in)
{
	//If symbols were passed in, then do the following
	if(symbols_in)
	{
		//If there was previous definition of symbols,
		//delete memory. Then, copy the information
		if(symbols)
			delete [] symbols;
		
		symbols = new char[strlen(symbols_in) + 1];
		strcpy(symbols, symbols_in);
	}

	return;
}



//Deallocates all the dynamic memory used for the data members
//Input: none
//Output: none
Command::~Command(void)
{
	//Deallocate memory for the first member
	if(name)
		delete [] name;

	//Deallocate memory for the second member
	if(symbols)
		delete [] symbols;
}



//Displays the name and symbols for a command
//Input: none
//Output: none
void Command::display(void)
{
	//If the member "name" exists, then display it
	if(name)
	{
		cout << "-" << " ";
		cout << name << " ";
	}
	
	//If the member "symbols" exists, then display it
	if(symbols)
	{
		cout << "("; 
		cout << symbols;
		cout << ")";
	}

	cout << endl;

	return;
}



//Compares the passed symbols to the member's symbols
//Input: some symbols array
//Output: true (if there is a match) or false otherwise
bool Command::compare_symbols(char * symbols_in)
{
	if(symbols)
	{
		if(symbols_in)
		{
			if(strcmp(symbols, symbols_in) == 0)
				return true;
		}
	}
	return false;
}



//************************** Command_Node Class ***************************************************************************************************

//Initializes the data members to NULL
//Input: none
//Output: none
Command_Node::Command_Node(void)
{
	group_name = NULL;
	commands = NULL;
	max_num_commands = 0;
	current_command = 0;
	next = NULL;
}



//Allocates memory to and initializes the data members
//Input: a name for the group of commands
//Output: none
Command_Node::Command_Node(char * group_name_in, int num_commands_in)
{
	current_command = 0;

	//Initializes the maximum number of commands allowed member
	if(num_commands_in <= 0)
		max_num_commands = MAX_NUM_COMMANDS;
	else
		max_num_commands = num_commands_in;


	//If the commands group name was passed, then allocate
	//copy over the name
	if(group_name_in)
	{
		group_name = new char[strlen(group_name_in) + 1];
		strcpy(group_name, group_name_in);
	}
	else
		group_name = NULL;


	//Allocate memory for the commands array	
	commands = new Command*[max_num_commands];
	for(int i = 0; i < max_num_commands; ++i)
	{
		commands[i] = NULL;
	}

	next = NULL;
}



//Initializes data members using the reference to Command_Node object
//Input: reference to the Command_Node object
//Output: none
Command_Node::Command_Node(const Command_Node & node_in)
{
	current_command = 0;
	max_num_commands = node_in.max_num_commands;

	//If there is a name to copy, copy it
	if(node_in.group_name)
	{
		group_name = new char[strlen(node_in.group_name) + 1];
		strcpy(group_name, node_in.group_name);
	}
	else
		group_name = NULL;
	
	//If there are commands to copy, copy them
	if(node_in.commands)
	{
		commands = new Command*[max_num_commands];
		Command ** temp = node_in.commands;
		int i;	
		for(i = 0; i < max_num_commands; ++i)
		{
			if(temp[i])
			{
				commands[i] = new Command(*temp[i]);
				++current_command;
			}
			else
				commands[i] = NULL;
		}
	}
	else
		commands = NULL;
	
	next = NULL;
}



//Adds a command to the node
//Input: some command reference
//Output: none
void Command_Node::add_command(const Command & command_in)
{
	//If the commands array doesn't exist, create one
	//and add the element
	if(!commands)
	{
		int i;
		max_num_commands = MAX_NUM_COMMANDS;	
		commands = new Command*[max_num_commands];
		for(i = 0; i < max_num_commands; ++i)
		{
			commands[i] = NULL;
		}
		commands[0] = new Command(command_in);
		++current_command;
		return;
	}
	
	//If the list of commands is not full, add the
	//current command to the list
	if(current_command < max_num_commands)
	{
		int i = current_command;
		commands[i] = new Command(command_in);
		++current_command;
		return;
	}
	
	//If the list is full, allocate a bigger array
	//copy over the information, and add the 
	//current command
	if(current_command == max_num_commands)
	{
		increase_size();	
		int i = current_command;
		commands[i] = new Command(command_in);
		++current_command;
	}	
	return;
}



//Increase the size of the commands array
//Input: none
//Output: none
void Command_Node::increase_size(void)
{
	//Increase the size of the array
	max_num_commands += MAX_NUM_COMMANDS;

	//Create a bigger array and initialize its members
	//to NULL	
	int i;
	Command ** temp = new Command*[max_num_commands];
	for(i = 0; i < max_num_commands; ++i)
	{
		temp[i] = NULL;
	}
	
	//Copy over the information
	for(i = 0; i < max_num_commands - MAX_NUM_COMMANDS; ++i)
	{
		temp[i] = new Command(*commands[i]);
	}
	
	//Delete old array
	for(i = 0; i < max_num_commands - MAX_NUM_COMMANDS; ++i)
	{
		delete commands[i];
	}
	delete [] commands;

	commands = temp;

	return;
}



//Deallocates memory for all the members if they exist
//Input: none
//Output: none
Command_Node::~Command_Node(void)
{
	if(group_name)
		delete [] group_name;

	if(commands)
	{
		for(int i = 0; i < current_command; ++i)
		{
			if(commands[i])
				delete commands[i];
		}
		delete [] commands;
	}
}



//Set the name for a group of commands
//Input: command group name
//Output: none
void Command_Node::set_name(char * group_name_in)
{
	//Initialize the group name member if the argument
	//value was other than NULL
	if(group_name_in)
	{
		if(group_name)
			delete [] group_name;

		group_name = new char[strlen(group_name_in) + 1];
		strcpy(group_name, group_name_in);
	}
	return;
}



//Assignes the pointer that the function is called on to the
//address pointed to by next
//Input: none
//Output: address of the next member of the class
Command_Node *& Command_Node::go_next(void)
{
	return next;
}


//Connects two LLL links
//Input: a pointer to the object to connect to
//Output: none
void Command_Node::connect(Command_Node * to_connect)
{
	next = to_connect;
	return;
}



//Displays the contents of the members
//Input: none
//Ouptut: none
void Command_Node::display(void)
{
	if(group_name)
	{
		cout << group_name;
       		cout << " Commands";	
		cout << endl;
	}

	if(commands)
	{
		int i;
		for(i = 0; i < current_command; ++i)
		{
			commands[i]->display();
		}
	}
	return;
}



//Compares the group name passed to the one of the class
//Input: command name
//Output: true/false 
bool Command_Node::compare_group_name(char * group_name_in)
{
	if(strcmp(group_name_in, group_name) == 0)
	{
		return true;
	}	
	return false;
}



//Looks for a command specified by symbols from client
//Input: command symbols from client program
//Output: true (if found) or false otherwise
bool Command_Node::find_command(char * symbols_in)
{
	bool match = false;
	if(commands)
	{
		int i;
		for(i = 0; i < current_command; ++i)
		{
			if(commands[i])
			{
				match = commands[i]->compare_symbols(symbols_in);
				if(match)
				{
					return match;
				}
			}
		}

	}
	return match;
}


//************************** Commands Class *******************************************************************************************************

//Initializes members to NULL
//Input: none
//Output: none
Commands::Commands(void): head(NULL), tail(NULL){}



//Allocates memory to and initializes all the data members
//Input: a reference to the Commands object
//Output: none
Commands::Commands(const Commands & commands_in)
{
	if(commands_in.head)
	{
		Command_Node * current = commands_in.head;
		
		//Copy the first node's information
		head = new Command_Node(*current);
		current = current->go_next();
		tail = head;

		//Copy the rest of the nodes' information
		while(current)
		{
			tail->go_next() = new Command_Node(*current);
			tail = tail->go_next();
			current = current->go_next();
		}
	}
	else
	{
		head = NULL;
		tail = NULL;
	}
}



//Deallocates all the dynamically allocated memory for the members
//Input: none
//Output: none
Commands::~Commands(void)
{
	if(head)
	{
		Command_Node * temp = head;
		while(head)
		{
			temp = temp->go_next();
			delete head;
			head = temp;
		}
	}
	head = tail = NULL;
}



//Loads commands from an external file
//Input: external file name
//Output: success or failure (1/-1)
int Commands::load(char * text_file, char * path_in)
{
	Command command;

	if(!text_file)
		return -1;
	
	char * text_file_with_path = NULL;	
	if(path_in)
	{	
		text_file_with_path = new char[strlen(text_file) + strlen(path_in) + 2];
		strcpy(text_file_with_path, path_in);
		strcat(text_file_with_path, "/");
		strcat(text_file_with_path, text_file);
	}

	//Try to open the text file to read from using
	//or not using the file path.
	//If failed, return failure
	read_file.open(text_file);
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
		
	char * name = new char[MAX_CHAR_READ];
	char * symbols = new char[MAX_CHAR_READ];
	char * group_name = new char[MAX_CHAR_READ];

	read_file.ignore(MAX_CHAR_IGNORE, '\n');
	read_file.get(group_name, MAX_CHAR_READ, '\t');
	while(read_file.eof() != true)
	{

		Command_Node command_group;
	
		//Read in the group name for commands
		command_group.set_name(group_name);
		
		//Ignore the following tabs	
		while(read_file.peek() == '\t')
		{
			read_file.ignore(MAX_CHAR_IGNORE, '\t');
		}
		
		//Read until the end of line
		while(read_file.peek() != '\n')
		{
			read_file.get(name, MAX_CHAR_READ, '(');
			read_file.ignore();
			read_file.get(symbols, MAX_CHAR_READ, ')');
			read_file.ignore(MAX_CHAR_IGNORE, ',');
			command.set_name(name);
			command.set_symbols(symbols);
			command_group.add_command(command);
		}
		read_file.ignore(MAX_CHAR_IGNORE, '\n');
		add_command_group(command_group);
		read_file.get(group_name, MAX_CHAR_READ, '\t');
	}		

	read_file.close();

	//Deallocate dynamic memory used locally
	delete [] group_name;
	delete [] name;
	delete [] symbols;
	delete [] text_file_with_path; 
	return 1;
}



//Adds a command to the list of commands
//Input: the command's group name and the command name
//Output: none
void Commands::add_command_group(const Command_Node & node_in)
{
	if(!head)
	{
		head = new Command_Node(node_in);
		tail = head;
	}
	else
	{
		tail->go_next() = new Command_Node(node_in);
		tail = tail->go_next();
	}
	return;
}



//Removes a command from the list of commands
//Input: the coommand's group name and the command name
//Output: none
void Commands::remove_command_group(char * group_name_in)
{
	//NOT YET IMPLEMENTED
	return;
}



//Checks if there is a command specified by symbols passed
//Input: symbols for a command to find and command group name
//Output: true or false depending if the command was found
bool Commands::verify_command(char * command_group_in,
		              char * symbols_in)
{
	bool found_command = false;
	if(head)
	{
		bool found_group = false;
		Command_Node * current = head;

		//Look for a group name in every Command_Node
		while(current)
		{
			found_group = current->compare_group_name(command_group_in);

			//If the group name was found, then look
			//for a command specified by symbols
			if(found_group)
			{
				found_command = current->find_command(symbols_in);
			
				//If there is a command specified by
				//the passed symbols, then return
				//a positive responce
				if(found_command)
				{
					return found_command;
				}
			}

			current = current->go_next();
		}
	}
	return found_command;
}



//Displays commands for the certain group of commands
//Input: commands group name
//Output: none
void Commands::display(char * group_name_in)
{
	if(group_name_in)
	{
		Command_Node * current = head;
		bool found = false;
		int count = 0; 
		while(current)
		{
			found = current->compare_group_name(group_name_in);
			if(found && count == 0)
			{
				count = 1;
				current->display();
			}
			current = current->go_next();
		}
	}
	return;
}



//Displays all the commands that exist in the application
//Input: none
//Output: none
void Commands::display_all(void)
{
	if(head)
	{
		Command_Node * temp = head;
		while(temp)
		{
			temp->display();
			if(temp->go_next())
				cout << endl;	
			temp = temp->go_next();
		}
	}
	return;
}
