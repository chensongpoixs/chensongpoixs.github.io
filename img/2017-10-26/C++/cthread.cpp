// g++ -o cpthread cthread.cpp -lpthread -std=c++11 -g -Wall
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <cstddef>

pthread_t tid1;

void* callback()
{
	printf("[%s][%d]\n", __FUNCTION__, __LINE__);
	return NULL;
}

void* thread_function(void* arg);

class func_base
{
public:
  virtual ~func_base() {}
  virtual void run() = 0;
};

struct auto_func_base_ptr
{
  func_base* ptr;
  ~auto_func_base_ptr() { delete ptr; }
};

template <typename Function>
class func
  : public func_base
{
public:
  func(Function f)
    : f_(f)
  {
  }

  virtual void run()
  {
    f_();
  }

private:
  Function f_;
};

void* thread_function(void* arg)
{
  auto_func_base_ptr func = {
      static_cast<func_base*>(arg) };
  func.ptr->run();
  return 0;
}

void start_thread(func_base *arg)
{
	pthread_create(&tid1,NULL,
	thread_function, arg);
}

template <typename Function>
void	chen_pthread(Function f)
{
	start_thread(new func<Function>(f));
}

int main(int argc, char *argv[])
{
	
	//void start_thread(func_base* arg);

	chen_pthread(callback);
	
	printf("开启线程");
    
	void *rval;
    pthread_join(tid1,(void **)&rval);
	
    printf("线程结束%d\n", rval);

	return 0;
}
