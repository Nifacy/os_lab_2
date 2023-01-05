#include "io_line/io_line.h" // read_line
#include "pipe/pipe.h" // Pipe, init_pipe, set_mode, get_input_descriptor, get_output_descriptor

#include <stdlib.h> // exit
#include <string.h> // strlen
#include <unistd.h> // close, write, access, dup2, execv, fork
#include <fcntl.h> // open
#include <sys/wait.h> // waitpid, pid_t
#include <sys/types.h> // pid_t
#include <sys/stat.h> // O_CREAT, S_IWRITE, S_IREAD


#define MAX_FILEPATH_LEN 256
#define MAX_INPUT_STRING_LEN 256

#define CREATE_PROCESS_ERROR 41
#define CREATE_PIPE_ERROR 42

#define WORKER_ERROR_EXECUTE_FILE 43 

#define MANAGER_ERROR_CREATE_RESULT_FILE 44
#define MANAGER_ERROR_RESULT_FILE_NOT_SPECIFIED 45
#define MANAGER_ERROR_OPEN_RESULT_FILE 46


#define WORKER_PATH "worker"


void protected_create_file(char* filename)
{
	int descriptor;

	descriptor = open(filename, O_CREAT, S_IWRITE | S_IREAD);

	if(descriptor == -1)
		exit(MANAGER_ERROR_CREATE_RESULT_FILE);

	close(descriptor);
}


char* protected_get_result_file(int argc, char* argv[], int index)
{
	int descriptor;

	if(index >= argc)
		exit(MANAGER_ERROR_RESULT_FILE_NOT_SPECIFIED);

	// file doesn't exists
	if(access(argv[index], F_OK) != 0)
		protected_create_file(argv[index]);
	
	descriptor = open(argv[index], O_WRONLY);

	if(descriptor == -1)
		exit(MANAGER_ERROR_OPEN_RESULT_FILE);

	close(descriptor);
	return argv[index];
}


void init_worker(int input_file_descriptor, char* result_filepath)
{
	char* args[] = {WORKER_PATH, result_filepath, NULL};
	
	dup2(input_file_descriptor, STDIN_FILENO);
	execv(WORKER_PATH, args);
	exit(WORKER_ERROR_EXECUTE_FILE);
}


void protected_init_pipe(Pipe* p)
{
	if(init_pipe(p) != PIPE_OK)
		exit(res);
}


int protected_get_input_descriptor(Pipe* p)
{
	int pipe_dp;
	int result;

	result = get_input_descriptor(p, &pipe_dp);

	if(result != PIPE_OK)
		exit(result);

	return pipe_dp;
}


int protected_get_output_descriptor(Pipe* p)
{
	int pipe_dp;
	int result;

	result = get_output_descriptor(p, &pipe_dp);

	if(result != PIPE_OK)
		exit(result);

	return pipe_dp;
}


void send_data(int pipe_descriptor, char* data)
{
	size_t string_len;

	string_len = strlen(data);
	data[string_len] = '\n';
	write(pipe_descriptor, data, string_len + 1);
}


pid_t create_worker(Pipe* p, char* result_filepath)
{
	pid_t pid = fork();

	if(pid < 0)
		exit(CREATE_PROCESS_ERROR);

	if(pid == 0)
	{
		set_mode(p, READ);
		init_worker(protected_get_input_descriptor(p), result_filepath);
	}

	set_mode(p, WRITE);
	return pid;
}


void close_worker(pid_t worker_id, int pipe_descriptor)
{	
	write(pipe_descriptor, "\n", 1);
	waitpid(worker_id, NULL, 0);
}


int main(int argc, char* argv[])
{
	char* first_filepath;
	char* second_filepath;
	char input_string[MAX_INPUT_STRING_LEN];

	pid_t first_worker_id, second_worker_id;

	Pipe pipe1, pipe2;

	// reading output files' paths
	first_filepath = protected_get_result_file(argc, argv, 1);
	second_filepath = protected_get_result_file(argc, argv, 2);

	// init pipe
	protected_init_pipe(&pipe1);
	first_worker_id = create_worker(&pipe1, first_filepath);

	protected_init_pipe(&pipe2);
	second_worker_id = create_worker(&pipe2, second_filepath);

	// read input string
	while(1)
	{
		int res = read_line(STDIN_FILENO, input_string, MAX_INPUT_STRING_LEN);

		if(res != IO_LINE_OK)
			exit(res);

		if(strlen(input_string) == 0)
			break;

		if(strlen(input_string) > 10)
			send_data(protected_get_output_descriptor(&pipe2), input_string);

		else
			send_data(protected_get_output_descriptor(&pipe1), input_string);
	}

	close_worker(first_worker_id, protected_get_output_descriptor(&pipe1));
	close_worker(second_worker_id, protected_get_output_descriptor(&pipe2));
	
	exit(0);
}
