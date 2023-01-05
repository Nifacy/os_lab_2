#ifndef __PIPE_H__
#define __PIPE_H__


#include <stdbool.h> // bool

/* Results */

#define PIPE_OK 0
#define PIPE_ERROR_INIT_PIPE 21
#define PIPE_ERROR_MODE_ALREADY_DEFINED 22
#define PIPE_ERROR_ACCESS_DENIED 23

/* Types */

typedef enum {
	WRITE,
	READ
} PipeMode;

struct __Pipe
{
	int input_dp;
	int output_dp;

	PipeMode mode;
	bool mode_defined;
};

typedef struct __Pipe Pipe;

/* Methods */

int init_pipe(Pipe* p);
/*
	Инициализация анонимного канала

	:return:
	- PIPE_OK - pipe успешно инициализирована
	- PIPE_ERROR_INIT_PIPE - не удалось инициализировать pipe
*/


int set_mode(Pipe* p, PipeMode mode);
/*
	Устанавливает режим, в котором используется анонимный канал

	:param mode: режим работы с каналом
	- WRITE - запись в канал
	- READ - чтения из канала

	:return:
	- PIPE_OK - режим успешно установлен
	- PIPE_ERROR_MODE_ALREADY_DEFINED - режим pipe уже указан
*/


int get_input_descriptor(Pipe* p, int* out_value);
/*
	Присваивает в out_value дескриптор входного файла анонимного канала.

	:return:
	- PIPE_OK - дексриптор упсешно получен
	- PIPE_ERROR_ACCESS_DENIED - отказано в доступе
*/


int get_output_descriptor(Pipe* p, int* out_value);
/*
	Присваивает в out_value дескриптор выходного файла анонимного канала

	:return:
	PIPE_ERROR_ACCESS_DENIED - отказано в доступе
	PIPE_OK - дексриптор упсешно получен
*/


void close_pipe(Pipe* p);
/*
	Закрывает именованный канал
*/


#endif
