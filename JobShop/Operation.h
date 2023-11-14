#pragma once

struct Operation
{
	int machine;
	int duration;
	int job_no;
	int start_time = 0;
	int end_time = 0;
	bool is_empty = true;
};