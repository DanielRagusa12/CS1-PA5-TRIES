/* COP 3502C Programming Assignment 5 
This program is written by: Daniel Ragusa */


/*
This program stores words from in.txt into a trie data structure.
Each node of the trie contains word count representing how many times the word is inserted.
Sum freq representing the max number of prefixes that this word is for words in the trie. 
And max freq which represents the max sum freq of any 1 nodes children nodes.

After the input is stored in the trie a set of queries will be answered, to check for the next most likely letter in the word. 
*/


//PPD
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "leak_detector_c.h"

#define MAXLEN 1000000

FILE *outFile;

//structure for trie node

typedef struct trie_node
{
  int word_count; 
  int sum_freq;
  int max_freq;
  struct trie_node *children[26];
  
}trie_node;

//simple init function to create a node
trie_node* init()
{
  trie_node *myTree = malloc(sizeof(trie_node));
  //setting all ints to 0
  myTree->word_count = 0;
  myTree->sum_freq = 0;
  myTree->max_freq = 0;

  for(int i=0; i<26; i++)
    myTree->children[i] = NULL; //setting all children to NULL

  return myTree; //returning new node
  
}



//function to insert a node a word in the trie 
void insert(trie_node *root, char word[], int k,int count) 
{
  
  if (k == strlen(word)) //if we reach here we are at the end of the word 
  {
    root->word_count+=count; //increasing the words count 
    root->sum_freq+=count; //increasing sum count
    return;
  }


  root->sum_freq+=count; //adding count to sum freq

    // See if the next place to go exists, if not, create it.
  int currIndex = word[k] - 'a';
  if(root->children[currIndex] == NULL)
  {
    root->children[currIndex] = init(); //creating new node 
    
  }


  if(root->children[currIndex]->sum_freq+count>root->max_freq) //here we calculate ahead and if the next node will have a sum freq higher than the max then we need to update it 
    root->max_freq=root->children[currIndex]->sum_freq+count; //updating max 

    

  insert(root->children[currIndex], word, k+1,count); //recursive call with k+1
}


//search function that will answer queries 
int search(trie_node *root, char word[], int k) 
{
  int flag=0; //flag to see if we found the word 
  
  if (k == strlen(word)) //running until end of the search word
  {
    for(int i=0;i<26;i++) //for all children pointers
      {
        if(root->children[i]!=NULL) //if the child is not NULL
        {
          if(root->max_freq==root->children[i]->sum_freq) //we check if the max freq matches the nodes sum freq
          {
            printf("%c",'a'+i); //if so we print the character
            fprintf(outFile,"%c",'a'+i);
            flag=1; //set flag to 1 to indicate success
          }
            
        }
      }

    if(flag==1) //return 1 if we did the job
      return 1;
    

    return 0; //return 0 if not 
    
  }
    

  int idx = word[k] - 'a'; //calculating next index
  
  if(root->children[idx] == NULL) //if the next node is NULL
    return 0; //the word does not exist 

  return search(root->children[idx], word, k+1);
}

//function to free memory
void free_mem(trie_node *root)
{
  for(int i=0;i<26;i++)
    {
      if(root->children[i] != NULL)
        free_mem(root->children[i]); 
    }
    
  free(root);
  
}





int main(void) 
{
  atexit(report_mem_leak);
  FILE *inFile;
  inFile=fopen("in.txt","r");
  outFile=fopen("out.txt","w");

  int n;
  fscanf(inFile,"%d",&n);

  trie_node *root=init(); //create root node 

  


  for(int i=0;i<n;i++) 
  {
    int command;
    fscanf(inFile,"%d",&command);

    if(command==1) //if we are inserting 
    {
      char tempName[MAXLEN];
      int tempCount;
      fscanf(inFile,"%s %d",tempName,&tempCount);
      insert(root,tempName,0,tempCount); //inserting word
         
    }
    if(command==2) //if we are answering querie
    {
      char tempName[MAXLEN];
      fscanf(inFile,"%s",tempName);
      if(search(root,tempName,0)==0) //if we didnt find the word 
      {
        printf("unknown word");
        fprintf(outFile,"unknown word");
        
      }
        

      printf("\n");
      fprintf(outFile,"\n");
      
    }
    
      
  }


  free_mem(root);
 
  
  return 0;
}