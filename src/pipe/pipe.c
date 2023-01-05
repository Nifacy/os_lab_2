#include "pipe.h"
#include <unistd.h> // pipe, close


int init_pipe(Pipe* p)
{
	int dp[2];

	if(pipe(dp) != 0)
		return PIPE_ERROR_INIT_PIPE;

	p->input_dp = dp[0];
	p->output_dp = dp[1];
	p->mode_defined = false;

	return PIPE_OK;
}


int set_mode(Pipe* p, PipeMode mode)
{
	if(p->mode_defined)
		return PIPE_ERROR_MODE_ALREADY_DEFINED;

	if(mode == WRITE)
		close(p->input_dp);

	if(mode == READ)
		close(p->output_dp);

	p->mode = mode;
	p->mode_defined = true;

	return PIPE_OK;
}


int get_input_descriptor(Pipe* p, int* out_value)
{
	if(!p->mode_defined || p->mode != READ)
		return PIPE_ERROR_ACCESS_DENIED;

	*out_value = p->input_dp;
	return PIPE_OK;
}


int get_output_descriptor(Pipe* p, int* out_value)
{
	if(!p->mode_defined || p->mode != WRITE)
		return PIPE_ERROR_ACCESS_DENIED;

	*out_value = p->output_dp;
	return PIPE_OK;
}


void close_pipe(Pipe* p)
{
	if(!p->mode_defined || p->mode == WRITE)
		close(p->output_dp);

	if(!p->mode_defined || p->mode == READ)
		close(p->input_dp);

	p->mode_defined = false;
}
