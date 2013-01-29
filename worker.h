#ifndef __WORKER_H_
#define __WORKER_H_

int run_worker_threads();

int commit_task(render_params_t* task);

int stop_worker_threads();

#endif /* __WORKER_H_ */
