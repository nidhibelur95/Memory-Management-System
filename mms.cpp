//Main program: Executable

// Created by nbelur on 11/16/17.
/* This assignment is implemented in c++, compiled and executed succesfully */

#include "iostream"
#include "memory_block.h"
#include <thread>
#include <mutex>

using namespace std;
int no_of_blocks;
int fitType;

char* mmsAddress;
#define MAX_SIZE 32
#define MAX_BLOCK_SIZE 5
mms::memory_block memoryBlock [MAX_BLOCK_SIZE];

int get_pow(int x, int y){
  if (y == 0) return 1;
  if (y == 1) return x;
  return x * get_pow(x, y-1);
}

/*
 Creating unallocated memory memoryBlock with variable size
these sizes can be varied at run time, based on the request
*/
void add_memory_blocks() {
  
  cout << "\nBlockNo\tSize\tAvailable Memory\n";
  for (int i = 0; i < MAX_BLOCK_SIZE; i++){
	if (i == 0){
	  char* blockAddr = mmsAddress;
	  memoryBlock[i].set_pointer_address(blockAddr);
	}
	else{
	  char* blockAddr = memoryBlock[i-1].get_block_size() + memoryBlock[i-1].get_pointer_address();
	  memoryBlock[i].set_pointer_address(blockAddr);
	}
	
	int blockSize = get_pow(2, i);
	memoryBlock[i].set_block_size(blockSize);
	memoryBlock[i].set_requested_memory(0);
	memoryBlock[i].set_thread_id(-1);
	cout <<" "<<i<<"\t " << memoryBlock[i].get_block_size()<<"\t " << memoryBlock[i].get_block_size()-memoryBlock[i].get_requested_memory() << endl;
	
  }
}

/*** FIRST FIT ***/
int first_fit(int requested_memory, int thread_id)
{
  int block_index;
  for (int i = 0; i < MAX_BLOCK_SIZE; i++)
  {
	if ((memoryBlock[i].get_block_size() - memoryBlock[i].get_requested_memory()) >= requested_memory)
	{
	  memoryBlock[i].set_requested_memory(requested_memory);
	  memoryBlock[i].set_thread_id(thread_id);
	  block_index = i;
	  break;
	}
	
  }
  return block_index;
}

/*** BEST FIT ****/
int best_fit(int requested_memory, int thread_id){
  
  int min;
  int block_index;
  for (int i = 0; i < MAX_BLOCK_SIZE; i++) {
	int available_space = memoryBlock[i].get_block_size() - memoryBlock[i].get_requested_memory();
	if (requested_memory <= available_space){
	  int temp = available_space - requested_memory;
	  if (temp < min){
		min = temp;
		block_index = i;
	  }
	}
  }
  int curr = memoryBlock[block_index].get_requested_memory();
  memoryBlock[block_index].set_requested_memory(curr + requested_memory);
  memoryBlock[block_index].set_thread_id(thread_id);
  return block_index;
}

/*** WORST FIT ***/
int worst_fit(int requested_memory, int thread_id){
  int max = -1;
  int block_index;
  for (int i = 0; i < MAX_BLOCK_SIZE; i++) {
	int available_space = memoryBlock[i].get_block_size() - memoryBlock[i].get_requested_memory();
	if (requested_memory <= available_space){
	  int temp = available_space - requested_memory;
	  if (temp > max){
		max = temp;
		block_index = i;
	  }
	}
  }
  int curr = memoryBlock[block_index].get_requested_memory();
  memoryBlock[block_index].set_requested_memory(curr + requested_memory);
  memoryBlock[block_index].set_thread_id(thread_id);
  return block_index;
  
}

/*** deallocate memory ***/

void memory_free(int block_id) {
  
  for (int i = 0; i < MAX_BLOCK_SIZE; i++){
	if (i == block_id){
	  memoryBlock[i].set_requested_memory(0);
	  memoryBlock[i].set_thread_id(-1);
	}
  }
  
  cout << "\nBlockNo\t\tSize\t\tCurrSize\tThread\n";
  for (int i = 0; i < MAX_BLOCK_SIZE; i++) {
	cout << "   " << i << "\t\t"
		 << memoryBlock[i].get_block_size() << "\t\t"
		 << memoryBlock[i].get_block_size() - memoryBlock[i].get_requested_memory() << "\t\t"
		 << memoryBlock[i].get_thread_id()
		 <<endl;
  }
}


/*** Allocate memory **/
void memory_malloc(int thread_id) {
  int mem = rand()%8;
  
  cout << "Thread: " << thread_id<< " requesting memory: " << mem << std::endl;
  
  mutex mms_mutex;	//mutex for mms thread
  mms_mutex.lock();	//mutex lock for mms thread
  
  int block_index;
  
  if(fitType == 1)
  {
	block_index = first_fit(mem, thread_id);
  }
  else if(fitType == 2)
  {
	block_index = best_fit(mem, thread_id);
  }
  else if(fitType == 3)
  {
	block_index = worst_fit(mem, thread_id);
  }
//Set default to run first fit type.
  else
  {
	block_index = first_fit(mem, thread_id);
  }
  
  
  cout << "\nBlockNo\t\tSize\t\tcurrSize\tThread\n a";
  for (int i = 0; i < MAX_BLOCK_SIZE; i++)
  {
	cout << "   " << i << "\t\t"
		 << memoryBlock[i].get_block_size() << "\t\t"
		 << memoryBlock[i].get_block_size() - memoryBlock[i].get_requested_memory() << "\t\t"
		 << memoryBlock[i].get_thread_id()
		 <<endl;
  }
  mms_mutex.unlock();	//mutex unlock for mms thread
  this_thread::sleep_for(std::chrono::seconds(10));
  
  mms_mutex.lock();
  cout << "Thread: "<< thread_id << " waking up and de-allocating memory: " << mem <<endl;
  memory_free(block_index);
  mms_mutex.unlock();
  
}

/**MAIN FUNCTION***/

int main(int argc, char *argv[])
{
  if(argc!=3)
	cout << "Error! Command line must have 3 arguments!!" << endl;
  
  mmsAddress = (char*) malloc(MAX_SIZE);
  cout << "MMS is created with size: "<< MAX_SIZE << endl;
  
  int thread_size;
  thread_size = atoi(argv[1]);
  if(thread_size <=0 || thread_size >5)
  {
	thread_size = 5;
  }
  fitType = atoi(argv[2]);
  if(fitType == 1){
	cout << "Memory Allocation : First Fit" << endl;
  }
  else if(fitType == 2){
	cout << "Memory Allocation : Best Fit" << endl;
  }
  else if(fitType == 3){
	cout << "Memory Allocation : Worst Fit" << endl;
  }
  else{
	cout << "Memory Allocation : First Fit" << endl;
  }
  
  std::thread users[thread_size];
  add_memory_blocks();
  this_thread::sleep_for(std::chrono::seconds(1));
  
  for (int i = 0; i < thread_size; i++)
  {
	users[i] = thread(memory_malloc, i);
  }
  
  for (int i =0; i< thread_size; i++)  {
	users[i].join();
  }
  
  this_thread::sleep_for(std::chrono::seconds(thread_size*2));
  free(mmsAddress);
  cout << "Graceful close\n";
}
