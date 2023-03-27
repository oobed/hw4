#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int helper(bool& flag, Node * root){
    
    if(root == nullptr){
      return 0;
    }

   int height_left = helper(flag, root->left);
   int height_right = helper(flag, root->right);

   //2 child
   //left child
   //right child
   if(root->left != nullptr && root->right != nullptr){
     if(height_left != height_right){
       flag = false;
     }
     else{
       return height_left+1;
     }
   }
   else if(root->left != nullptr && root->right == nullptr){
     return height_left +1;
   }
   else if(root->left == nullptr && root->right != nullptr){
     return height_right+1;
   }

}


bool equalPaths(Node * root)
{
    bool flag = true;
    helper(flag, root);
    return flag;


    //reach the end off the bottom
   
}

