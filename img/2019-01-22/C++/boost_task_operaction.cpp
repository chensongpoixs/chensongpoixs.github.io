#include <iostream>
#include <cstdlib>

class operation
{
public:
	void complete(const int& ec, std::size_t bytes)
	{
		printf("[%s][%d]\n", __FUNCTION__, __LINE__);
		func_(this, ec, bytes);
	}
	void destroy()
	{
		printf("[%s][%d]\n", __FUNCTION__, __LINE__);
		func_(this, int(), 0);
	}
protected:
	typedef void (*func_type)(operation *, const int&, std::size_t);
	operation(func_type func) :func_(func)
	{
		printf("[%s][%d]\n", __FUNCTION__, __LINE__); 
	}
private:
	func_type	func_;
};

class reactor_op :public operation
{
public:
	bool perform()
	{
		printf("[%s][%d]\n", __FUNCTION__, __LINE__);
		return perform_func_(this);
	}
protected:
	typedef bool(*perform_func_type)(reactor_op*);
	reactor_op(perform_func_type perform_func, func_type complete_func)
		:operation(complete_func),
		perform_func_(perform_func) 
	{
		printf("[%s][%d]\n", __FUNCTION__, __LINE__);
	}
private:
	perform_func_type	perform_func_;
};


class reactive_op_base : public reactor_op
{
public:
	reactive_op_base(func_type complete_func)
		: reactor_op(&reactive_op_base::do_perform, complete_func)
	{
		printf("[%s][%d]\n", __FUNCTION__, __LINE__);
	}
	//这里处理任务 直到任务处理完成结束提出
	static bool do_perform(reactor_op * base)
	{
		reactive_op_base * r(static_cast<reactive_op_base*>(base));
		// r->perform(); // cycle
		printf("[%s][%d]function = %p\n", __FUNCTION__, __LINE__, r);
		
	    //r->complete(34, 34);
		return true;
	}
protected:
private:

};

class reactive_op : public reactive_op_base
{
public:
	reactive_op()
		:reactive_op_base(&reactive_op::do_complete)
	{
		printf("[%s][%d]\n", __FUNCTION__, __LINE__);
	}
	static void do_complete(operation* base, const int &, std::size_t)
	{
		reactor_op * o(static_cast<reactor_op*>(base));
		printf("[%s][%d]function = %p\n", __FUNCTION__, __LINE__, o);
		o->perform();
		printf("[%s][%d]\n", __FUNCTION__, __LINE__);
	}
protected:
private:
};

int main(int argc, char *argv[])
{
	reactive_op reactive;
	printf("[%s][%d]function = %p\n", __FUNCTION__, __LINE__, &reactive);
	operation * o(static_cast<operation*>(&reactive));
	o->complete(4, 5);
	return EXIT_SUCCESS;
}
