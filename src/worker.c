#include "io_line/io_line.h" // read_line, write_line
#include "remove_vowels/remove_vowels.h" // remove_vowels
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <fcntl.h> // open
#include <unistd.h> // close, STDIN_FILENO


#define RESULT_FILE_NOT_SPECIFIED 51
#define FAILED_TO_OPEN_RESULT_FILE 52

#define MAX_INPUT_LINE_LEN 256


int protected_open_result_file_descriptor(int argc, char* argv[])
{
	int descriptor;

	if(argc < 2)
		exit(RESULT_FILE_NOT_SPECIFIED);

	descriptor = open(argv[1], O_WRONLY | O_TRUNC)
	if(descriptor == -1)
		exit(FAILED_TO_OPEN_RESULT_FILE);

	return descriptor;
}


void protected_get_data(int descriptor, char* buff, size_t buff_size)
{
	int result = read_line(descriptor, buff, buff_size);

	if(result != IO_LINE_OK)
		exit(result);
}


void protected_write_line(int descriptor, char* line)
{
	int result = write_line(descriptor, line);

	if(result != IO_LINE_OK)
		exit(result);
}


void protected_format_line(char* input_line, char* buff, size_t buff_size)
{
	int result = remove_vowels(input_line, buff, buff_size);

	if(result != REMOVE_VOWELS_OK)
		exit(result);
}


int main(int argc, char* argv[])
{
	int result_file_descriptor;
	char input_line[MAX_INPUT_LINE_LEN];
	char formatted_line[MAX_INPUT_LINE_LEN];

	result_file_descriptor = protected_open_result_file_descriptor(argc, argv);
	protected_get_data(STDIN_FILENO, input_line, MAX_INPUT_LINE_LEN);

	while(strlen(input_line) != 0)
	{
		protected_format_line(input_line, formatted_line, MAX_INPUT_LINE_LEN);
		protected_write_line(result_file_descriptor, formatted_line);
		protected_get_data(STDIN_FILENO, input_line, MAX_INPUT_LINE_LEN);
	}

	close(result_file_descriptor);
	return 0;
}
