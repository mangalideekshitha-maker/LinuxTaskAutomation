#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H

void job_control_init(void);
int job_control_handle(char **args);
int job_control_execute(char **args);

#endif
