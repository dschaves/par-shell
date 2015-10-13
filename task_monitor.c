void task_monitor()
{
	int last_num_children = *num_children;

	for(;;)
	{
		if (*num_children != last_num_children) 
		{
		wait(NULL); 
		last_num_children++;

		}

		else sleep(1);
	}
}
