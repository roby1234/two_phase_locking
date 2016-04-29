/*
Onkar Singh
two phase locking
27/04/16
*/

#include <bits/stdc++.h>
#include <unistd.h>

// Definition for Read op and write op
#define	W	1
#define	R	0

using namespace std;

//structure of an instruction of a transaction
struct _instr{
	bool 	op;
	char 	var;
	int 	trnxIndex;
};

//structure of the row of resource table which consists of the resources in use
struct _resourceTable{
	bool	op;
	char	var;
	vector<int>	trnxIndexVector;
};

//list for storing transactions
vector<vector<_instr> > 	transList;

//list for storing schedule
vector<_instr>				schedule;

//resource table
vector<_resourceTable>		resourceTable;

//list for storing running transactions
vector<vector<_instr> > 	readytransList;


//function for taking input and ignoring spaces
char ignore_spaces_and_getchar(){
	char ch;
	do {ch = cin.get();} while(ch == ' ');
	return ch;
}


//function used for inserting instruction in schedule
void InsertInSchedule(_instr tempInstr){
	_instr tempSRow;
	tempSRow.op = tempInstr.op;
	tempSRow.var = tempInstr.var;
	tempSRow.trnxIndex = tempInstr.trnxIndex;
	schedule.push_back(tempSRow);
	return;
}


//function used for inserting a whole transaction in resource table
void InsertInResourceTable(int transNum){
	vector<_instr> transaction;
	int RTIndex;
	for (std::vector<_instr>::iterator i = transList[transNum].begin(); i != transList[transNum].end();i++){
		// Check if variable in the instr exists in resourceTable
		for (RTIndex = 0; RTIndex < resourceTable.size() and i->var != resourceTable[RTIndex].var; ++RTIndex);
		
		if (RTIndex == resourceTable.size()){ // variable is not present in resource table
			_resourceTable tempRRow;
			tempRRow.var = i->var;
			tempRRow.op = i->op;
			tempRRow.trnxIndexVector.push_back(i->trnxIndex);

			resourceTable.push_back(tempRRow);
		}
		else {	// if var is present in resource table
			if (resourceTable[RTIndex].op == R){// If it is Read operation
				if (i->op == R){	// If the selected instr has Read operation as well 
					resourceTable[RTIndex].trnxIndexVector.push_back(i->trnxIndex);//allow this instruction
				}
			}
		}
		_instr tempInstr;
		tempInstr.var = i->var;
		tempInstr.op = i->op;
		tempInstr.trnxIndex = i->trnxIndex;
		transaction.push_back(tempInstr);
	}
	readytransList.push_back(transaction);
	return;
}


// function to check if all resources are available
int check(vector <_instr> &v){
	bool iscorrect(false);
	int RTIndex;
	for (std::vector<_instr>::iterator i = v.begin(); i != v.end();i++){
		// Check if variable in the instr exists in resourceTable
		for (RTIndex = 0; RTIndex < resourceTable.size() and i->var != resourceTable[RTIndex].var; ++RTIndex);
		
		if (RTIndex == resourceTable.size()){ // variable is not present in resource table
			// cout <<"Resource was not found in the table "<<endl;
			iscorrect = true;
			continue;
		}
		else {	// if var is present in resource table
			// cout <<"Resource was found at "<< RTIndex<<endl;
			if (resourceTable[RTIndex].op == R){// If it is Read operation
				if (i->op == R){	// If the selected instr has Read operation as well 
					iscorrect = true;//allow this instruction
				}
				else{	// If selected instruction has Write operation
					iscorrect = false;
					break;
				}
			}
			else{	// If it is Write operation in resource table
				iscorrect = false;
				break;
			}
		}
	}
	if(iscorrect == false){
		return -1;
	}
	else{
		return 1;
	}
}

//function to remove instruction from the resource table 
void RemoveFromResourceTable(_instr tempInstr){
	int RTIndex;
	for (RTIndex = 0; RTIndex < resourceTable.size() and tempInstr.var != resourceTable[RTIndex].var; ++RTIndex);
	// Check if variable in the instr exists in resourceTable
	resourceTable[RTIndex].trnxIndexVector.erase(std::remove(resourceTable[RTIndex].trnxIndexVector.begin(), resourceTable[RTIndex].trnxIndexVector.end(), tempInstr.trnxIndex), resourceTable[RTIndex].trnxIndexVector.end());
	if(resourceTable[RTIndex].trnxIndexVector.empty()){
		resourceTable.erase(resourceTable.begin() + RTIndex);
	}
	return;
}

//function to release resource from the running transaction
void RemoveFrom_readyTransitionTable(int index){
	readytransList[index-1].erase(readytransList[index-1].begin());
	if(readytransList[index-1].empty())
		readytransList.erase(readytransList.begin() + index-1);
	return;
}


//function to print the transaction list
void print_transList(){
	cout << "Transaction List is as follows : \n";
	for (std::vector<vector<_instr> >::iterator i = transList.begin(); i != transList.end(); ++i){
		for (std::vector<_instr>::iterator j = i->begin(); j != i->end(); ++j){
			cout<<j->trnxIndex << " " << j->op << " " << j->var << " ";			
		}
		cout <<endl;
	}
}

//function to print the running transaction list
void print_readytransList(){
	cout << "Ready Transaction List is as follows : \n";
	for (std::vector<vector<_instr> >::iterator i = readytransList.begin(); i != readytransList.end(); ++i){
		for (std::vector<_instr>::iterator j = i->begin(); j != i->end(); ++j){
			cout<<j->trnxIndex << " " << j->op << " " << j->var << " ";			
		}
		cout <<endl;
	}
}


//function to print the resource table
void print_resourceTable(){
	cout << "Resource Table is as follows : \n";
	for (vector<_resourceTable>::iterator i = resourceTable.begin(); i != resourceTable.end(); ++i){
		for (vector<int>::iterator j = i->trnxIndexVector.begin(); j != i->trnxIndexVector.end(); ++j){
			cout << *j << " ";
		}
		cout << i->op << " " << i->var << '\n';
	}
}


//function to print the shedule
void print_schedule(){
	cout << "Schedule is as follows : \n";
	for (vector<_instr>::iterator i = schedule.begin(); i != schedule.end(); ++i){
		cout << i->trnxIndex << " " << i->op << " " << i->var << '\n';
	}
}


// the main function
int main(){
	char ch;
	//taking input
	while(!cin.eof()){
		vector<_instr> transaction;
		ch = ignore_spaces_and_getchar();
		while(!cin.eof() and ch != '\n'){
			_instr tempInstr;

			tempInstr.op = (ch == 'W'?1:0 );
			tempInstr.var = ignore_spaces_and_getchar();
			tempInstr.trnxIndex = transList.size() + 1;

			// Pushing the new intruction
			transaction.push_back(tempInstr);

			// Taking input for next operation
			ch = ignore_spaces_and_getchar();
		}
		// Pushing the new transaction in transaction list
		transList.push_back(transaction);
	}
	print_transList();
	srand(time(0));
	int count = 0;
	// int transcount = 0;
	int qwe = 0;
	// bool visited[transList.size() + 1];
	// for (int l = 0; l < transList.size(); ++l)visited[l]=false;		// l = 0, 1, 2, . . . . . size-1
	while (!transList.empty() || !readytransList.empty()){
		qwe++;
		cout << "\n\n\n\n\nIteration " << qwe << endl; 
		

		//checking if all the transactions has been brought to ready transaction list
		if(transList.size() != 0){


			//choosing random transaction to put it in running transactions 
			int randTransIndex = (rand()%(transList.size())) + 1;
			cout << "randTransIndex = "<<randTransIndex << endl;
			//printing the selected vector
			cout << "Selected vector is as follows : "<<endl;
			for (std::vector<_instr>::iterator j = transList[randTransIndex-1].begin(); j != transList[randTransIndex-1].end(); ++j){
				cout<<j->trnxIndex << " " << j->op << " " << j->var << endl;			
			}

			//check if all the locks can be acquired by transaction
			if(check(transList[randTransIndex-1]) == 1){	//transaction can acquire all the locks
				

				//if yes insert transaction in resource table and erase it from initial transaction list
				InsertInResourceTable(randTransIndex-1);
				transList.erase(transList.begin()+randTransIndex - 1);
				print_resourceTable();
				print_readytransList();

				
				//taking random transaction from the running transactions
				int randomInstructionIndex = (rand()%(readytransList.size())) + 1;
				cout << "randomInstructionIndex " << randomInstructionIndex << endl;
				std::vector<_instr>::iterator i = readytransList[randomInstructionIndex-1].begin();


				// Insert this instruction in schedule
				InsertInSchedule(*i);
				print_schedule();

				// Free the resources of this instruction
				RemoveFromResourceTable(*i);
				print_resourceTable();

				// remove the instruction from running transaction
				RemoveFrom_readyTransitionTable(randomInstructionIndex);
				print_readytransList();
			}

			//if all resources cannot be provided
			else if (check(transList[randTransIndex-1]) == -1){
				cout << "Sorry, resources are not available\n";
				count++;
				int randomInstructionIndex = (rand()%(readytransList.size())) + 1;
				cout << "randomInstructionIndex " << randomInstructionIndex << endl;
				std::vector<_instr>::iterator i = readytransList[randomInstructionIndex-1].begin();
				InsertInSchedule(*i);
				print_schedule();
				RemoveFromResourceTable(*i);
				print_resourceTable();
				RemoveFrom_readyTransitionTable(randomInstructionIndex);
				print_readytransList();
			}
		}
		else{// if transaction list becomes empty, i.e. all the transactions have been brought to running transactions 
			print_resourceTable();
			print_readytransList();
			int randomInstructionIndex = (rand()%(readytransList.size())) + 1;
			cout << "randomInstructionIndex " << randomInstructionIndex << endl;
			std::vector<_instr>::iterator i = readytransList[randomInstructionIndex-1].begin();
			InsertInSchedule(*i);
			print_schedule();
			RemoveFromResourceTable(*i);
			print_resourceTable();
			RemoveFrom_readyTransitionTable(randomInstructionIndex);
			print_readytransList();
		}
		if(count > 1000000){
			cout << "Deadlock and Aborted\n";
			break;
		}
	}

	//print the final schedule
	print_schedule();
	return 0;
}

