#include <iostream>
#include "Container.h"
#include"Object.h"
int main(){
	Container<std::string> messages;
	Container<Task*> tasks {
		new BinaryOperatorTask( "Addition",1.0,2.0,'+' ),
		new BinaryOperatorTask( "Substraction",1.0,2.0,'-' ),
		new BinaryOperatorTask( "Multiplication",1.0,2.0,'*' ),
		new BinaryOperatorTask( "Division",1.0,2.0,'/' ),
		new BinaryOperatorTask( "Addition 2",0.5,3.1,'+' ),
		new BinaryOperatorTask( "Substraction 2",0.5,3.1,'-' ),
		new BinaryOperatorTask( "Multiplication 2",0.5,3.1,'*' ),
		new BinaryOperatorTask( "Division 2",0.5,3.1,'/' ),
	};

	tasks.push_front( new CountTaskTask( "Firts cout task task", &tasks ) );
	tasks.push_back( new CountTaskTask( "Second cout task task", &tasks ) );
	tasks.push_front( new GetNumberInstancesTask( "First get Number Instances Task" ) );
	tasks.push_back( new GetNumberInstancesTask( "First get Number Instances Task" ) );


	std::cout << "Number instances of Object before run all: " << Object::numberInstances << '\n';
	while ( !tasks.isEmpty() )
	{
		Task* task = tasks.pop_back();
		task->run();
		messages.push_back( task->toString() );
		delete task;
	}

	for ( auto& message : messages )
	{
		std::cout << message << '\n';
	}

	messages.clear();
	tasks.clear();
	std::cout << "Number instances of Object after run all: " << Object::numberInstances << '\n';
}