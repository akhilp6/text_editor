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
void left_rotation(text_t   *n){
     text_t   *tmp_node;
     key_t   tmp_key;
     tmp_node = n->left;
     tmp_key = n->key;
     n->left = n->right;
     n->key = n->right->key;
     n->right = n->left->right;   // same as saying n->right->right
     n->left->right = n->left->left;  //same as saying n->right->left
     n->left->left = tmp_node;
     n->left->key = tmp_key;
}
void right_rotation(text_t* n){
    text_t   *tmp_node;
    key_t   tmp_key;
    tmp_node = n->right;
    tmp_key = n->key;
    n->right = n->left;
    n->key = n->left->key;
    n->left = n->right->left;
    n->right->left = n->right->right;
    n->right->right = tmp_node;
    n->right->key = tmp_key;
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
	   std::stack <text_t*> st;

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
        old_node_dup->height=0;

        tmp_node->left=old_node_dup;
        tmp_node->height=1;

        new_node->left = (text_t*)new_line;
        new_node->key = tmp_node->key + 1;
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
    if(txt->left == NULL){
        txt->left = (text_t*)new_line;
        txt->key = 1;
    }
    else{
        std::stack <text_t*> st;
        text_t* tmp_node = txt;
        while(tmp_node->right!=NULL){
            if(tmp_node->key < index){
                st.push(tmp_node);
                tmp_node = tmp_node->left;
            }
            else{
                st.push(tmp_node);
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
        new_node->key = tmp_node->key + 1;
        new_node->height = 0;
        tmp_node->right = old_node_dup;
        tmp_node->key = tmp_node->key + 1;
        
        while(!st.empty()){
            text_t* current = st.top();
            st.pop();
            //setHeight(current);
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
    text_t *tmp_node, *upper_node, *other_node;
    object_t deleted_object;
    if( txt->left == NULL )
        return( NULL );
    else if( txt->right == NULL )
    {   if( txt->key == index ){
        deleted_object = (object_t) txt->left;
        txt->left = NULL;
        return( deleted_object );
       }
       else
         return( NULL );
    }
    else
    {
        tmp_node = txt;
        while( tmp_node->right != NULL ){
            upper_node = tmp_node;
            if( index < tmp_node->key )
            {   tmp_node = upper_node->left;
                other_node = upper_node->right;
            }
            else
            {    tmp_node = upper_node->right;
                other_node = upper_node->left;
            }
        }
        if( tmp_node->key != index )
            return( NULL );
        else
        {   upper_node->key = other_node->key;
            upper_node->left = other_node->left;
            upper_node->right = other_node->right;
            deleted_object = (object_t) tmp_node->left;
//            return_node( tmp_node );
//            return_node( other_node );
            return( deleted_object );
        }
    }  // end left column else
    // char *old = txt->_text[index-1];
    // txt->_text.erase(txt->_text.begin()+index-1);
    // return old;
}
