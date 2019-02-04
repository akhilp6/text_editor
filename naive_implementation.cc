#include <vector>
#include <iostream>
#include <stdlib.h>

// // struct text_t{
// //     std::vector<char*> _text;
// };
typedef int key_t;
typedef char *object_t;
struct text_t{key_t      key;
                struct text_t   *left;
                struct text_t  *right;
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
  return( tmp );
}

text_t * create_text(){
    text_t *tmp_node;
    tmp_node = get_node();
    tmp_node->left = NULL;
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

void append_line( text_t *txt, char * new_line){
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

