// Created by nbelur on 11/16/17.

/* 
Creating an interface to work with memory blocks. These blocks are added into the dynamically allocated mms space with their address and size defined for easier access.
*/

#ifndef MMS_MEMORY_BLOCK_H
#define MMS_MEMORY_BLOCK_H
#include <string>

namespace mms {

using namespace std;

class memory_block {
 private:
  char* pointer_address;
  int block_size;
  int allocated_memory;
  int thread_id;
 
 public:
  char *get_pointer_address() {
	return this->pointer_address;
  }
  
  int get_block_size() {
	return this->block_size;
  }
  
  int get_requested_memory() {
	return this->allocated_memory;
  }
  void set_pointer_address(char* address) {
	pointer_address = address;
  }
  
  int set_block_size(int blk_size) {
	block_size = blk_size;
  }
  
  int set_requested_memory(int requested_memory) {
	allocated_memory = requested_memory;
  }
  
  int get_thread_id(){
	return this->thread_id;
  }
  int set_thread_id(int id){
	this->thread_id = id;
  }
};
}


#endif //MMS_MEMORY_BLOCK_H
