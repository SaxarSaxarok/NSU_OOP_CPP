#pragma once
#include <string>
#include <unordered_map>
#include<array>
#include"Utilities .h"
#include"Container.h"
#include <format>

class Object{
public:
	inline static int numberInstances = 0;
	virtual std::string toString() const = 0;

	Object(){ ++numberInstances; }
	virtual ~Object(){ --numberInstances; }
};

class Task: public virtual Object{
public:
	virtual void run() = 0;
	bool isCompleted = false;
	virtual bool hasResult() const = 0;
};

class Named: public virtual Object{
public:
	std::string name;
	Named( std::string name ): name( name ){}
	std::string toString() const{ return name; }
};

class BinaryOperatorTask: public Task, public Named{
	double left_;
	double right_;
	char operation_;
	double result_ = 0;

public:
	BinaryOperatorTask( std::string name, double left, double right, char  operation ):
		Named( name ),
		left_( left ),
		right_( right ),
		operation_( operation ){
	}

	bool hasResult() const{ return true; }

	void run(){
		if ( isCompleted ) throw "The task has already been completed";
		switch ( operation_ )
		{
		case '+':
			result_ = left_ + right_;
			break;
		case '-':
			result_ = left_ - right_;
			break;
		case '/':
			result_ = left_ / right_;
			break;
		case '*':
			result_ = left_ * right_;
			break;
		default:
			throw "Bad operation";
			break;
		}
		isCompleted = true;
	}

	std::string toString() const{
		return name + " : " + std::format( "{} {} {} = {} ({})",
										   left_, operation_, right_, isCompleted ? std::to_string( result_ ) : "?", isCompleted ? "DONE" : "IN PROCESS" );
	}
};

class AddTask: public Task, public Named{
	Container<Task*>* container_;
	Task* task_;

public:
	AddTask( std::string name, Container<Task*>* container, Task* task ): Named( name ), container_( container ), task_( task ){}

	bool hasResult() const{ return false; }

	void run(){
		if ( isCompleted ) throw "The task has already been completed";
		container_->push_back( task_ );
		isCompleted = true;
	}

	std::string toString() const{
		return name + " : " + std::format( "Add \"{}\" to container ({})",
										   task_->toString(), isCompleted ? "DONE" : "IN PROCESS" );
	}
};

class CountObjectTask: public Task, public Named{
	int result_ = 0;
	Container<Object*>* container_;
public:
	CountObjectTask( std::string name, Container<Object*>* container ): Named( name ), container_( container ){}

	bool hasResult()const{ return true; }

	void run(){
		if ( isCompleted ) throw "The task has already been completed";
		result_ = container_->size();
	}

	std::string toString() const{
		return name + " : " + std::format( "Container сontain {} objects ({})",
										   isCompleted ? std::to_string( result_ ) : "?", isCompleted ? "DONE" : "IN PROCESS" );
	}
};

class CountTaskTask: public Task, public Named{
	int result_ = 0;
	Container<Task*>* container_;
public:
	CountTaskTask( std::string name, Container<Task*>* container ): Named( name ), container_( container ){}
	bool hasResult() const{ return true; }

	void run(){
		for ( auto& task : *container_ ) { if ( task->hasResult() ) result_++; }
		isCompleted = true;
	}

	std::string toString() const{
		return name + " : " + std::format( "Container сontain {} result task ({})",
										   isCompleted ? std::to_string( result_ ) : "?", isCompleted ? "DONE" : "IN PROCESS" );
	}
};

class ClearTask: public Task{
	bool hasResult() const{ return false; }
	Container<Object*>* container;

	ClearTask( Container<Object*>* container ): container( container ){}

	void run(){ container->clear(); }
};

class GetNumberInstancesTask: public Task, public Named{

	int result_ = 0;
public:

	GetNumberInstancesTask( std::string name ): Named( name ){}

	void run(){
		result_ = Object::numberInstances;
		isCompleted = true;
	}

	bool hasResult() const{ return true; }

	std::string toString() const{
		return name + " : " + std::format( "Now there are {} objects ({})",
										   isCompleted ? std::to_string( result_ ) : "?", isCompleted ? "DONE" : "IN PROCESS" );
	}
};