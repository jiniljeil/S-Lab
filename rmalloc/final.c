#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <assert.h> 
#include "rmalloc.h" 

rm_header rm_free_list = { 0x0, 0 } ;
rm_header rm_used_list = { 0x0, 0 } ;
rm_option curr_status = FirstFit; // Default
void * memory_space = NULL ;
void * first_s = NULL; 
void * rmalloc (size_t s) 
{
	// TODO 
	int pagesize = sysconf(_SC_PAGESIZE); 
	rm_header_ptr used_node = 0x0, iter, prev;
	char alloc = '0'; 
	int k = 0; 
	rm_header_ptr direct_addr; 
	size_t min_space = pagesize; 
	size_t max_space = 0; 
	// remained memory space (free)
	for (iter = rm_free_list.next; iter != 0x0; iter = iter->next) { 
		if( (iter->size == s) || (s + sizeof(rm_header) < iter->size)) {
			if (curr_status == FirstFit) {
				direct_addr = iter; 
				alloc = '1'; 
				break; 
			}else if(curr_status == BestFit) { 
				// Find the small and proper space 
				if (min_space > iter->size) { 
					direct_addr = iter ;
					min_space = iter->size; 
					alloc = '1';
				}		
			}else if(curr_status == WorstFit) { 
				// Find the large space 
				if (max_space < iter->size) { 
					direct_addr = iter; 	
					max_space = iter->size ; 	
					alloc = '1'; 
				}	
			}	
			
		}
	}
	k = 0 ; 
	for (iter = rm_free_list.next; iter != 0x0; iter = iter->next) { 
		if( direct_addr == iter ) { 
			break; 
		}
		if (k == 0) {prev = rm_free_list.next ; k++;}   
		else if ( k == 1 ) prev = prev->next ;
	}
	if (alloc == '1') { 
		rm_header_ptr remainder = (rm_header_ptr) ((void*)direct_addr + sizeof(rm_header) + s) ; 
		remainder->size = direct_addr->size - s - sizeof(rm_header); 	
		remainder->next = direct_addr->next;
		if ( k == 0 ) { 
			rm_free_list.next = remainder;  
		}else if ( k >= 1 ) { 
			prev->next = remainder;  
		}
		used_node = direct_addr; 	
		used_node->size = s ;
		used_node->next = 0x0 ;
			
		rm_header_ptr i = rm_used_list.next;
                while(i->next!= 0x0) i = i->next;
                i->next = used_node; 
	}

	// remained memory space (page) 
	if (used_node == 0x0) { 
		// allocation 
		int n_pages = 0 ;
		n_pages = (sizeof(rm_header) + s + sizeof(rm_header)) / pagesize + 1;  	
		//if (s < used_node->size) { 
			 
			if( rm_used_list.next == 0x0){ 
				used_node = (rm_header_ptr) mmap(NULL, n_pages * pagesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);  // 0x20 MAP_ANON
				first_s = used_node; 
				memory_space = used_node; 
				used_node->size = n_pages * pagesize - sizeof(rm_header); 
				used_node->next = 0x0; 
				rm_used_list.next = used_node;
				
				if ( s < used_node->size) {
				rm_header_ptr remain = (rm_header_ptr) ((void*) used_node + sizeof(rm_header) + s); 
                        	remain->size = used_node->size - s - sizeof(rm_header); 
                       		remain->next = 0x0; 
                        	rm_free_list.next = remain;
				}	
			}else{
				// new_page 
				used_node = (rm_header_ptr) mmap(memory_space + n_pages * pagesize, n_pages * pagesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0); 
				memory_space = used_node; 		
				used_node->size = n_pages * pagesize - sizeof(rm_header);
				used_node->next = 0x0; 
				rm_header_ptr i = rm_used_list.next; 
        	 	        while(i->next!= 0x0) i = i->next;
				i->next = used_node;
			
				if ( s < used_node->size) {
				rm_header_ptr remain = (rm_header_ptr) ((void*) used_node + sizeof(rm_header) + s);
				remain->size = used_node->size - s - sizeof(rm_header);
                 	        remain->next = 0x0;
				rm_header_ptr f = rm_free_list.next; 
				while(f->next != 0x0) f = f->next;
				f->next = remain;
 				}
			}
         	       	used_node->size = s;	
		//}
	}
	return ((void *) used_node + sizeof(rm_header));
}

void rfree (void * p) 
{
	// TODO 
	rm_header_ptr itr, prev ;
	int start = 0;  
	char free_check = '0'; 
	for (itr = rm_used_list.next; itr != 0x0; itr = itr->next) { 
		if ( p == ((void*)itr + sizeof(rm_header))) { 
			if (rm_free_list.next == 0x0) { 
				if(start < 1) { 
                                        rm_used_list.next = itr->next;
                                }else{  
                                        prev->next = itr->next;
                                }
				itr->next = 0x0; 
				rm_free_list.next = itr; 
			}else{ 
				if(start < 1) { 
					rm_used_list.next = itr->next; 
				}else{ 
					prev->next = itr->next; 
				} 
				
				rm_header_ptr i = rm_free_list.next; 
				while(i->next != 0x0) i = i->next; 
				itr->next = 0x0; 
				i->next = itr; 
			}
			free_check = '1'; 
			break; 
		}
		if( start == 0 ) {start++; prev = rm_used_list.next;} 
		else {prev = prev->next;}
	} 
	// ASSERT FUNCTION
	assert(free_check == '1'); 
}

void * rrealloc (void * p, size_t s) 
{
	// TODO
	rm_header_ptr itr ;
	for (itr = rm_used_list.next ; itr != 0x0 ; itr = itr->next){ 
		if ((void*)itr + sizeof(rm_header) == p){ 
			if( itr->size > s) { 
				rm_header_ptr remain = (rm_header_ptr) ((void*)itr + sizeof(rm_header) + s ); 
				remain->size = itr->size - s - sizeof(rm_header); 
				remain->next = 0x0; 
			
				rm_header_ptr i = rm_free_list.next; 
				while(i->next != 0x0) i = i->next ;
				i->next = remain; 
				
				itr->size = s; 
				
			}else if( itr->size == s) { 
				return p; 
			}else { 
				rfree(p); 
				return rmalloc(s); 
			}
			break; 
		} 
	}
	return p ;  
}

void rmshrink () 
{
	// TODO
	
	if (memory_space != NULL) {  	
		int pagesize = sysconf(_SC_PAGESIZE); 
		rm_header_ptr itr; 
		void * low_address = (void *)rm_used_list.next;
		
		// Find the lowest address 
		for (itr = rm_used_list.next; itr != 0x0; itr = itr->next) { 
			if ( low_address > (void *)itr) { 
				low_address = (void *)itr; 	
			}
		}
		for (itr = rm_free_list.next; itr != 0x0; itr = itr->next) { 
			if ( low_address > (void *)itr) {
                                low_address = (void *)itr;          
                        }
		} 
		for ( void * addr = first_s ; addr > low_address; addr -= pagesize) {
			char used = '0'; 
			for ( itr = rm_used_list.next ; itr != 0x0; itr = itr->next) { 
				if ( (void *)itr <= addr && (void *)itr > addr - pagesize) { 
					used = '1'; 	
				}
			}	
			if ( used == '0') { 
				// Remove nodes in the free list
				int k = 0 ; 
				rm_header_ptr prev; 
				for ( itr = rm_free_list.next; itr != 0x0; itr = itr->next) { 
					if ( (void*)itr == addr || ((void*)itr > addr && (void *)itr <= addr + pagesize)) { 
						if( k < 1 ) { 
							rm_free_list.next = itr->next; 
						}else { 
							prev->next = itr->next;  
						}
						continue;
					}
					if( k == 0 ) {prev = rm_free_list.next; k++;}
					else  prev = prev->next; 
				}
				 
				assert(munmap(addr, pagesize) == 0);  
			}	
		}
	}
}

void rmconfig (rm_option opt) 
{
	// TODO
	curr_status = opt; 
}


void 
rmprint () 
{
	rm_header_ptr itr ;
	int i ;

	printf("==================== rm_free_list ====================\n") ;
	for (itr = rm_free_list.next, i = 0 ; itr != 0x0 ; itr = itr->next, i++) {
		printf("%3d:%p:%8d:", i, ((void *) itr) + sizeof(rm_header), (int) itr->size) ;

		int j ;
		char * s = ((char *) itr) + sizeof(rm_header) ;
		for (j = 0 ; j < (itr->size >= 8 ? 8 : itr->size) ; j++) 
			printf("%02x ", s[j]) ;
		printf("\n") ;
	}
	printf("\n") ;

	printf("==================== rm_used_list ====================\n") ;
	for (itr = rm_used_list.next, i = 0 ; itr != 0x0 ; itr = itr->next, i++) {
		printf("%3d:%p:%8d:", i, ((void *) itr) + sizeof(rm_header), (int) itr->size) ;

		int j ;
		char * s = ((char *) itr) + sizeof(rm_header) ;
		for (j = 0 ; j < (itr->size >= 8 ? 8 : itr->size) ; j++) 
			printf("%02x ", s[j]) ;
		printf("\n") ;
	}
	printf("\n") ;

}
