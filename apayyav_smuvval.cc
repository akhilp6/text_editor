#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stack>

typedef int key_t;
typedef char *object_t;
struct text_t{key_t      key;
                struct text_t   *left;
                struct text_t  *right;
		        int height;
               /* possibly additional information */ };

#define BLOCKSIZE 256

text_t *currentblock = NULL;
int    size_left;

text_t *get_node()
{ 
	/*
	This function creates a struct and allocates memory. Returns the pointer to memory location.
	This function creates 256 nodes at a stretch and uses them. Once finished, it repeates creating 256 nodes.
	*/
text_t *tmp;
  if( currentblock == NULL || size_left == 0)
     {  currentblock = 
                (text_t *) malloc( BLOCKSIZE * sizeof(text_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  tmp->left=NULL;
  tmp->right=NULL;
tmp->height=0;
  return( tmp );
}

text_t * create_text(){
/*
	This function obtains new struct text_t memory space from get_node function and returns a reference of it to caller.
*/
    text_t *tmp_node;
    tmp_node = get_node();
    tmp_node->left = NULL;
    tmp_node->height = 0;
    return( tmp_node );
}

int length_text(text_t *txt){
/*
	This function returns the number of lines present in the storage.
	As we are storing line count in parent, returning the line count present in the root node shall suffice.
*/
      if(txt == NULL){
        return -1;
      }
      else if(txt->left == NULL){
        return 0;
      }
      else/*{
            text_t *tmp_node = txt;
            while(tmp_node->right!=NULL){
                tmp_node = tmp_node->right;
            }*/
            return txt->key;
      }
    // if (txt == NULL) return -1;
    // return txt->_text.size();


char * get_line( text_t *txt, int index){
/*
	This function navigates through the tree to reach the correct leaf containing the required line. If found, it returns the line.
*/
    text_t *tmp_node;
    if( txt->left == NULL )
      return(NULL);
    else
    {  tmp_node = txt;
      while( tmp_node->right != NULL )
      {   if( index <= tmp_node->left->key )
               tmp_node = tmp_node->left;
          else{
			  index=index-tmp_node->left->key;
               tmp_node = tmp_node->right;
		  }
      }
      if( tmp_node->key == index )
         return( (char *) tmp_node->left );
      else
         return( NULL );
    }
   // return txt->_text[index-1];
}

void setHeight(text_t* node){
/*
	This function is used to set height of a node, using the information from its children.
	Used while insertion and appending.
*/
	node->height=node->left->height>node->right->height?node->left->height+1:node->right->height+1;
}
void left_rotation(text_t   *n){
     text_t   *tmp_node;
     key_t   tmp_key;
     tmp_node = n->left;
     tmp_key = n->key;
     n->left = n->right;
     n->right = n->left->right;   
     n->left->right = n->left->left;  
     n->left->left = tmp_node;
     n->left->key = n->left->left->key + n->left->right->key;
}
void right_rotation(text_t* n){
    text_t   *tmp_node;
    key_t   tmp_key;
    tmp_node = n->right;
    tmp_key = n->key;
    n->right = n->left;
    //n->key = n->left->key;
    n->left = n->right->left;
    n->right->left = n->right->right;
    n->right->right = tmp_node;
    //n->right->key = tmp_key;
	n->right->key = n->right->left->key + n->right->right->key;
}

void append_line( text_t *txt, char * new_line){
/*
	This function appends new line at the end.
*/
    if(txt->left == NULL){
        txt->left = (text_t*)new_line;
        txt->key = 1;
		txt->height = 0;
    }
    else{
	/*1. Create stack
	2. Push every node that is visited to stack
	3. After new node is attached, pop each element and evaluate height
	*/
	//1. Create stack
	   std::stack <text_t*> st;

        text_t* tmp_node = txt;
        while(tmp_node->right!=NULL){
	//2. Push every node that is visited to stack
	        st.push(tmp_node);
			tmp_node->key=tmp_node->key+1;
            tmp_node = tmp_node->right;
        }
        text_t* new_node = get_node();
        text_t* old_node_dup = get_node();

        old_node_dup->left=tmp_node->left;
        old_node_dup->right=NULL;
        old_node_dup->key=tmp_node->key;
        old_node_dup->height=0;

        tmp_node->left=old_node_dup;
        tmp_node->height=1;

        new_node->left = (text_t*)new_line;
        new_node->key = 1;
	    new_node->height = 0;
        tmp_node->right = new_node;
        tmp_node->key = tmp_node->key + 1;

	//3. After new node is attached, pop each element and evaluate height
	while(!st.empty()){
		text_t* current = st.top();
		st.pop();
		setHeight(current);
        int tmp_height, old_height;
//        tmp_node = pop();
        old_height = current->height;
	//The four cases below are referred from lecture notes of Dr. Kemafor.
        if( current->left->height - current->right->height == 2 ){
                if( current->left->left->height - current->right->height == 1 ){
                    right_rotation( current );
                current->right->height = current->right->left->height + 1;
                current->height  = current->right->height + 1;
            }
            else{
                left_rotation( current->left );
                right_rotation( current );
                tmp_height = current->left->left->height;
                current->left->height = tmp_height + 1;
                current->right->height = tmp_height + 1;
                current->height = tmp_height + 2;
            }
        }
        else if(current->right->height - current->left->height == 2){
            if( current->right->right->height - current->left->height == 1 ){
                left_rotation( current );
                current->left->height = current->left->right->height + 1;
                current->height  = current->left->height + 1;
            }
            else{
                right_rotation( current->right );
                left_rotation( current );
                tmp_height = current->right->right->height;
                current->right->height = tmp_height + 1;
                current->left->height = tmp_height + 1;
                current->height = tmp_height + 2;
            }
        }
     }
   }
    // txt->_text.push_back(new_line);
}


char * set_line( text_t *txt, int index, char * new_line){
/*
	Replaces content present at a line, if the line exists.
*/
    text_t *tmp_node;
    if(txt->left == NULL){
        return(NULL);
    }  
    else{
         tmp_node = txt;
        while( tmp_node->right != NULL ){ 
          if( index <= tmp_node->left->key )
               tmp_node = tmp_node->left;
          else{
			  index=index-tmp_node->left->key;
               tmp_node = tmp_node->right;
			   
		  }
        }
        if( tmp_node->key == index ){
           text_t *old_obj;
           old_obj = tmp_node->left;
           tmp_node->left = (text_t *) new_line; 
           return( (char *) old_obj ); 
        }
        else
         return( NULL );
    }
    // char *old = txt->_text[index-1];
    // txt->_text[index-1] = new_line;
    // return old;
}


void insert_line( text_t *txt, int index, char * new_line){
    if(txt->left == NULL){
        txt->left = (text_t*)new_line;
        txt->key = 1;
    }
    else{
		if(index>txt->key){
			append_line(txt, new_line);
			return;
		}
        std::stack <text_t*> st;
        text_t* tmp_node = txt;
        while(tmp_node->right!=NULL){
            if(tmp_node->left->key>=index){
                st.push(tmp_node);
				tmp_node->key=tmp_node->key+1;
                tmp_node = tmp_node->left;
            }
            else{
                st.push(tmp_node);
				tmp_node->key=tmp_node->key+1;
				index=index-tmp_node->left->key;
                tmp_node = tmp_node->right;
				
            }
        }
        text_t* new_node = get_node();
        text_t* old_node_dup = get_node();
        
        old_node_dup->left=tmp_node->left;
        old_node_dup->right=NULL;
        old_node_dup->key=tmp_node->key;
        old_node_dup->height=0;
        tmp_node->left=new_node;
        tmp_node->height=1;
        new_node->left = (text_t*)new_line;
        new_node->key = 1;
        new_node->height = 0;
        tmp_node->right = old_node_dup;
        tmp_node->key = tmp_node->key + 1;
        
        while(!st.empty()){
            text_t* current = st.top();
            st.pop();
            setHeight(current);
            int tmp_height, old_height;
            //        tmp_node = pop();
            old_height = current->height;
            if( current->left->height - current->right->height == 2 ){
                if( current->left->left->height - current->right->height == 1 ){
                    right_rotation( current );
                    current->right->height = current->right->left->height + 1;
                    current->height  = current->right->height + 1;
                }
                else{
                    left_rotation( current->left );
                    right_rotation( current );
                    tmp_height = current->left->left->height;
                    current->left->height = tmp_height + 1;
                    current->right->height = tmp_height + 1;
                    current->height = tmp_height + 2;
                }
            }
            else if(current->right->height - current->left->height == 2){
                if( current->right->right->height - current->left->height == 1 ){
                    left_rotation( current );
                    current->left->height = current->left->right->height + 1;
                    current->height  = current->left->height + 1;
                }
                else{
                    right_rotation( current->right );
                    left_rotation( current );
                    tmp_height = current->right->right->height;
                    current->right->height = tmp_height + 1;
                    current->left->height = tmp_height + 1;
                    current->height = tmp_height + 2;
                }
            }
        }
        
    }
    // txt->_text.insert(txt->_text.begin() + index - 1, new_line);
}

char * delete_line( text_t *txt, int index){
	/*
	Part of this code is referred from lecture presentations by Dr. Kemafor
	*/
    text_t *tmp_node, *parent_node, *sibling_node;
    object_t line;
    if( txt->left == NULL )
        return( NULL );
    else if( txt->right == NULL )
    {   if( txt->key == index ){
        line = (object_t) txt->left;
        txt->left = NULL;
        return( line );
       }
       else
         return( NULL );
    }
    else
    {
        tmp_node = txt;
        while( tmp_node->right != NULL ){
            parent_node = tmp_node;
            if( index <= tmp_node->left->key )
            {   tmp_node->key=tmp_node->key-1;
				tmp_node = parent_node->left;
                sibling_node = parent_node->right;
            }
            else
            {   tmp_node->key=tmp_node->key-1;
				index=index-tmp_node->left->key;
				tmp_node = parent_node->right;
                sibling_node = parent_node->left;
            }
        }
        if( tmp_node->key != index )
            return( NULL );
        else
        {   parent_node->key = sibling_node->key;
            parent_node->left = sibling_node->left;
            parent_node->right = sibling_node->right;
            line = (object_t) tmp_node->left;
            return( line );
        }
    }  
    // char *old = txt->_text[index-1];
    // txt->_text.erase(txt->_text.begin()+index-1);
    // return old;
}
