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
text_t *free_list = NULL;
int nodes_taken = 0;
int nodes_returned = 0;

text_t *get_node()
{ text_t *tmp;
  nodes_taken += 1;
  if( free_list != NULL )
  {  tmp = free_list;
     free_list = free_list -> right;
  }
  else
  {  if( currentblock == NULL || size_left == 0)
     {  currentblock = 
                (text_t *) malloc( BLOCKSIZE * sizeof(text_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  }
tmp->height=0;
  return( tmp );
}

text_t * create_text(){
    text_t *tmp_node;
    tmp_node = get_node();
    tmp_node->left = NULL;
    tmp_node->height = 1;
    return( tmp_node );
}

int length_text(text_t *txt){
      if(txt == NULL){
        return -1;
      }
      else if(txt->left == NULL){
        return 0;
      }
      else{
            text_t *tmp_node = txt;
            while(tmp_node->right!=NULL){
                tmp_node = tmp_node->right;
            }
            return tmp_node->key;
      }
    // if (txt == NULL) return -1;
    // return txt->_text.size();
}

char * get_line( text_t *txt, int index){
    text_t *tmp_node;
    if( txt->left == NULL )
      return(NULL);
    else
    {  tmp_node = txt;
      while( tmp_node->right != NULL )
      {   if( index < tmp_node->key )
               tmp_node = tmp_node->left;
          else
               tmp_node = tmp_node->right;
      }
      if( tmp_node->key == index )
         return( (char *) tmp_node->left );
      else
         return( NULL );
    }
   // return txt->_text[index-1];
}

void setHeight(text_t* node){
	node->height=node->left->height>node->right->height?node->left->height+1:node->right->height+1;
}

void append_line( text_t *txt, char * new_line){
    if(txt->left == NULL){
        txt->left = (text_t*)new_line;
        txt->key = 1;
    }
    else{
	/*1. Create stack
	2. Push every node that is visited to stack
	3. After new node is attached, pop each element and evaluate height
	*/
	//1. Create stack
	std::stack <text_t> st;

        text_t* tmp_node = txt;
        while(tmp_node->right!=NULL){
	//2. Push every node that is visited to stack
	    st.push(tmp_node);
            tmp_node = tmp_node->right;
        }
        text_t* new_node = get_node();
	text_t* old_node_dup = get_node();

	old_node_dup->left=tmp_node->left;
	old_node_dup->right=NULL;
	old_node_dup->key=tmp_node->key;
	old_node_dup->height=1;	

	tmp_node->left=old_node_dup;	
	tmp_node->height=2;

        new_node->left = (text_t*)new_line;
        new_node->key = tmp_node->key + 1;
	new_node->height = 1;
        tmp_node->right = new_node;

	//3. After new node is attached, pop each element and evaluate height
	while(!st.empty()){
		text_t* current = st.top();
		st.pop();
		setHeight(current);
	}
    }
    // txt->_text.push_back(new_line);
}


char * set_line( text_t *txt, int index, char * new_line){
    text_t *tmp_node;
    if(txt->left == NULL){
        return(NULL);
    }  
    else{
         tmp_node = txt;
        while( tmp_node->right != NULL ){ 
          if( index < tmp_node->key )
               tmp_node = tmp_node->left;
          else
               tmp_node = tmp_node->right;
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
    // txt->_text.insert(txt->_text.begin() + index - 1, new_line);
}

char * delete_line( text_t *txt, int index){
    // char *old = txt->_text[index-1];
    // txt->_text.erase(txt->_text.begin()+index-1);
    // return old;
}