/*Lewis Neibaur
*cs3337
*this program simulates a file tree
*created from instuctions in chapter 2.13 of 'Systems Programming in Unix/Linux' by K.C. Wang
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
typedef struct node             //node stucture for file tree
{
    struct node *parentPtr;     //pointer to parent
    struct node *childPtr;      //pointer to child
    struct node *siblingPtr;    //pointer to sibling
    char name[64];              //name of node
    char type[1];               //type of node D for directory F for File
}NODE;  
/**************************function declaration************************************************/
int initialize();               //Initialize the root node
int getInput();                 //get a line of input from user
int findCmd(char *command, char *cmd[]);//get the index for a command
NODE *newNode(char *name, char *type);  //creates a new node and returns a pointer to it
NODE *findPathnameNode(char *pathname); //returns the node specified by a path
int mkdir(char *pathname);      //make a new directory for a given pathname
int rmdir(char *pathname);      //remove the directory, if it is empty
int cd(char *pathname);         //change cwd to pathname, or to / if no pathname
int ls(char *pathname);         //list the directory contents of pathname or cwd
int pwd();                      //print the absolute pathname of cwd
void pd(NODE *d, char *path);   //recursivley gets the pathname of a node
int creat(char *pathname);      //create a File node
int rm(char *pathname);         //remove the file node
int save(char *pathname);       //save current file system tree as a real file
void writeNodes(NODE *node, char *tree);
int reload(char *pathname);     //construct a file system tree from a file
int menu();                     //show a menu of vailid commands
int quit();                     //save the file system tree, then terminate the program
/*******************************global vars***************************************************/
NODE *root, *cwd;                                       //pointers to root directory and CWD
/******************************main function****************************************************/
int main()                                              //main function
{
    initialize();                                       //initialize root directory
    char command[16], pathname[64];                     //strings to hold command and pathname
    int (* fptr[ ])( char *) ={( int (*)()) mkdir, rmdir, cd, ls, pwd, creat, rm ,save, reload, menu, quit};         //array of functions
    char *cmd[] = { "mkdir", "rmdir", "cd", "ls", "pwd", "creat", "rm", "save", "reload", "menu", "quit", NULL};    //array of funciton names
    while(1)                                            //main program loop
    {
        getInput(command, pathname);                    //get input line from user
        int index = findCmd(command, cmd);              //get index of command
        if (index >= 0)                                 //if index was successful
            fptr[index](pathname);                      // call function
        else printf("invalid command: %s\n", command);  //display error message
    }
}
/******************************functions definitions***********************************************/
int initialize(){               //Initialize the root node
    root = newNode("/", "D");   //create new root node
    root->parentPtr = root;     //roots parent is itself
    root->siblingPtr = root;    //roots sibling is itself
    root->childPtr = NULL;      //root is empty
    cwd = root;                 //root is current working directory
} 

int getInput(char *command,char *pathname){     //get a line of input from user
    char line[128];                             //string to hold user input line
    strcpy(command,"");                         //clear command
    strcpy(pathname,"");                        //clear pathname
    printf("enter command:");                   //display message to user
    fgets(line, 128, stdin);                    //get input line from user
    line[strlen(line)-1] = 0;                   //kill \n at the end of line
    sscanf(line, "%s %s", command, pathname);   //store user input in command and pathname variables
}

int findCmd(char *command, char *cmd[] ){       //get the index for a command
    int i = 0;                                  //index to iterate through commands
    while(cmd[i]){                              //itterate through commands
        if (!strcmp(command, cmd[i]))           //check if command matches
            return i;                           //found command, return index
        i++;                                    //itterate to next command
    }
    return -1;                                  //command not found, return -1
}

NODE *newNode(char *name, char *type)           //creates new node
{
    NODE *node = (NODE *)malloc(sizeof(NODE));  //allocate memory for new node
    strcpy(node->name, name);                   //set name
    strcpy(node->type, type);                   //set type
    node->parentPtr = NULL;                     //set parent to null
    node->childPtr = NULL;                      //set child to null
    node->siblingPtr = NULL;                    //set sibling to null
    return node;                                //return pointer to new node
}

NODE *findPathnameNode(char *pathname){                     //returns the node specified by a path
    char path[128];                                         //string to hold tokenized path
    NODE *next, *current;                                   //pointers to the current node in the path and the next node to be checked
    char *token;                                            //variable to store the current token from the path in
    strcpy(path, pathname);                                 //use path to preserve pathname. strtok destroys original string
    token = strtok(path,"/");                               //get first token
    if(pathname[0] == '/')                                  //check if pathname is absolute or relative
        current = root;                                     //if absolute start at root
    else current = cwd;                                     //if relative start at cwd
    while(token){                                           //iterate through tokens to find pathname node
        if (strcmp(token, "..") == 0){                      //check for ..
            current = current->parentPtr;                   //go to parrentnode
        }
        else if (current->childPtr == NULL){                //check if current node has children
            printf("could not find directory: %s\n", token);//print error message
            return NULL;                                    //cannot find directory
        }
        else{                                               //if current has children
            next = current->childPtr;                       //get ready to check childnode
            int i = 1;
            while(i){                                       //check all children
                if (strcmp(next->name,token) == 0           //if next matches token
                && strcmp(next->type,"D") == 0){            //and next is a directory
                    current = next;                         //update current
                    break;                                  //break out of inner loop to move to next token
                }
                else if (next->siblingPtr == NULL){         //if there is no sibling to check
                    printf("could not find directory: %s\n", token);  //print error message
                    return NULL;                            //cannot find directory
                }
                else{                                       //if there is a sibling to check
                    next = next->siblingPtr;                //advance to sibling and repeat loop
                }
            }
        }                                                   //only when directory has been found and entered
        token = strtok(0,"/");                              //step to next token
    }
    return current;                                         //return node specified by pathname
}

int mkdir(char *pathname){                                  //creates the directory listed
    char path[128];                                         //string to hold tokenized path
    NODE *next, *current;                                   //pointers to the current node in the path and the next node to be checked
    char *token;                                            //variable to store the current token from the path in
    strcpy(path,pathname);                                  //make temporary copy of pathname. strtok destroys string
    token = strtok(path,"/");                               //get first token
    if(pathname[0] == '/')                                  //check if pathname is absolute or relative
        current = root;                                     //if absolute start at root
    else current = cwd;                                     //if relative start at cwd
    while(token){                                           //iterate through tokens
        if (current->childPtr == NULL){                     //check if current node has children
            NODE *newN = newNode(token, "D");               //create new node
            newN->parentPtr = current;                      //newn points to current node
            current->childPtr = newN;                       //current node points to newn
            current = current->childPtr;                    //advance to next node in the path
        }
        else{                                               //if current has children
            next = current->childPtr;                       //get ready to check childnode
            int i = 1;
            while(i){                                       //check all children
                if (strcmp(next->name,token) == 0           //if next matches token
                && strcmp(next->type,"D") == 0){            //and next is a directory
                    current = next;                         //update current
                    break;                                  //break out of inner loop to move to next token
                }
                else if (next->siblingPtr == NULL){         //if there is no sibling to check
                    NODE *newN = newNode(token, "D");       //create new node
                    newN->parentPtr = current;              //newn points to current node
                    next->siblingPtr = newN;                //next node points to newn
                    current = next->siblingPtr;             //advance to next node in the path
                    break;                                  //break inner loop to move to next token
                }
                else{                                       //if there is a sibling to check
                    next = next->siblingPtr;                //advance to sibling and repeat loop
                }
            }
        }                                                   //only when directory has been found and entered
        token = strtok(0,"/");                              //step to next token
    } 
    return 0;                                               //return 0 for success
} 

int rmdir(char *pathname){
    NODE *delete, *relative;                                //placeholders for node to be deleted and closest relative node
    delete = findPathnameNode(pathname);                    //get node specified by path name
    if (!delete) return -1;                                 //if delete is null stop function
    if (delete->childPtr != NULL){                          //if directory has children
        printf("error: cannot delete directory '%s' because it is not empty.\n", delete->name);   //display error message
        return -1;                                          //return -1 for failure
    }
    else if (strcmp(delete->type,"D") != 0){                //if path is not a directory
        printf("error: cannot delete '%s' because it is not a directory.\n", delete->name);   //display error message
        return -1;                                          //return -1 for failure
    }
    else{                                                   //else find closest relative node to delete
        relative = delete->parentPtr;                       //start with the parent
        if (relative->childPtr == delete){                  //if delete is the first child
            relative->childPtr = delete->siblingPtr;        //closest relative points to next relative
            free(delete);                                   //free memory allocated to delete
        }
        else{                                               //if delete is not first child
            relative = relative->childPtr;                  //move to first child
            while (relative->siblingPtr != delete){         //if delete is not next sibling
                relative = relative->siblingPtr;            //go to next sibling
            }
            relative->siblingPtr = delete->siblingPtr;      //closest relative points to next relative
            free(delete);                                   //free memory allocated to delete
        }
    }
    return 0;                                               //return 0 for success
} 

int cd(char *pathname){                                     //changes cwd to path listed
    NODE *current;                                          //temporay node
    current = findPathnameNode(pathname);                   //get node from pathname
    if (!current) return -1;                                //if current is null stop function
    if (strcmp(current->type,"D") == 0)                     //make sure node is a directory
        cwd = current;                                      //set cwd before exiting
    else printf("path specified is a file");                //print error message
    return 0;                                               //return 0 for sucess;
} 

int ls(char *pathname){                                     //list children of cwd or specified path
    NODE *next;                                             //nodes to step through children without changing cwd
    next = findPathnameNode(pathname);                      //get node specified by pathname
    if (!next) return -1;                                   //if next is null stop function
    if (next->childPtr != NULL){                            //if next node has children
        next = next->childPtr;                              //go to childe
        while(next != NULL){                                //loop through all children of current
            printf("%s-%s    ",next->type, next->name);     //print name of directory
            next = next->siblingPtr;                        //advance to sibling and repeat loop 
        }
    }
    else printf("empty directory: %s", next->name);         //print error message
    printf("\n");                                           //add new line
    return 0;                                               //return 0 for sucess;
} 

int pwd(){                                                  //print directory path for cwd
    char path[128];                                         //string to store pathname
    strcpy(path,"");                                        //empty path
    pd(cwd, path);                                          //get path of cwd
    printf("%s\n",path);                                    //print cwd path
}

void pd(NODE *d, char *path){                               //recursivley adds pathname to string in argument path
    if(d != NULL){                                          //if d is a valid node
        if(d->parentPtr == d){                              //if at root node
            strcat(path, root->name);                       //add root name
            return;                                         // go to next
        }
        pd(d->parentPtr, path);                             //go to parents first
        
        strcat(path, d->name);                              //add next node name
        strcat(path, "/");                                  //add '/'
        return;                                             //go to next node
    }
    else printf("error: invalid directory");                //print error message
} 

int creat(char *pathname){                                  //create file in pathname
    NODE *dnode, *relative;                                 // node for parent directory of file to be made, and most recent relative
    char dname[64], bname[64];                              //strings to hold directory name and base name
    if (strchr(pathname,'/') == NULL){                      //if file is in cwd
        dnode = cwd;                                        //if file is in cwd use cwd
        strcpy(bname, pathname);                            //bname is pathname
    } 
    else {                                                  //else find directory to use
        char temp[ 128];                                    // dirname(), basename() destroy original pathname 
        strcpy( temp, pathname);                            // make copy of pathname
        strcpy( dname, dirname( temp));                     // get dname    
        strcpy( temp, pathname);                            // make copy of pathname
        strcpy( bname, basename( temp));                    // get bname
        dnode = findPathnameNode(dname);                    //get node from dirname
        if (!dnode) return -1;                              //if dnode is null stop function
    }
    if(dnode->childPtr){                                    //if dnode has children
        relative = dnode->childPtr;                         //go to child node
        while (relative->siblingPtr){                       //until next sibling is null
            relative = relative->siblingPtr;                //got to next sibiling
        }
        NODE *newN = newNode(bname, "F");                   //create new node
        newN->parentPtr = dnode ;                           //newn points to dnode
        relative->siblingPtr = newN;                        //relative node points to newn
    }
    else {                                                  //if dnode had no children
        NODE *newN = newNode(bname, "F");                   //create new node
        newN->parentPtr = dnode ;                           //newn points to dnode
        dnode->childPtr = newN;                             //relative node points to newn
    }
    return 0;                                               //return 0 for success
}

int rm(char *pathname){
    NODE *relative, *delete;                                // node to be deleted, and most recent relative
    char dname[64], bname[64];                              //strings to hold directory name and base name
    if (strchr(pathname,'/') == NULL){                      //if file is in cwd
        relative = cwd;                                     //if file is in cwd use cwd
        strcpy(bname, pathname);                            //bname is pathname
    } 
    else {                                                  //else find directory to use
        char temp[ 128];                                    // dirname(), basename() destroy original pathname 
        strcpy( temp, pathname);                            // make copy of pathname
        strcpy( dname, dirname( temp));                     // get dname    
        strcpy( temp, pathname);                            // make copy of pathname
        strcpy( bname, basename( temp));                    // get bname
        relative = findPathnameNode(dname);                 //get node from dirname
        if (!relative) return -1;                           //if relative is null stop function
    }
    if (strcmp(relative->childPtr->name, bname) == 0){      //if delete is the first child
        delete = relative->childPtr;                        //save pointer to node to be deleted
        if (strcmp(delete->type,"F") != 0){                 //if path is not a file
            printf("error: cannot delete '%s' because it is not a File.\n", delete->name);   //display error message
            return -1;                                      //return -1 for failure
        }
        relative->childPtr = delete->siblingPtr;            //closest relative points to next relative
        free(delete);                                       //free memory allocated to delete
    }
    else{                                                   //if delete is not first child
        relative = relative->childPtr;                      //move to first child
        while (strcmp(relative->siblingPtr->name, bname) != 0){ //if delete is not next sibling
            relative = relative->siblingPtr;                //go to next sibling
            if (relative->siblingPtr == NULL){              //if there is no next sibling
                printf("error: could not locate file: %s\n", bname);    //print error message
                return -1;                                  //exit function
            }
        }
        delete = relative->siblingPtr;                      //save pointer to node to be deleted
        if (strcmp(delete->type,"F") != 0){                 //if path is not a file
            printf("error: cannot delete '%s' because it is not a File.\n", delete->name);   //display error message
            return -1;                                      //return -1 for failure
        }
        relative->siblingPtr = delete->siblingPtr;          //closest relative points to next relative
        free(delete);                                       //free memory allocated to delete
    }
    return 0;                                               //return 0 for success
} 

int save(char *pathname){
    
    if (strcmp(pathname, "") == 0) strcpy(pathname, "autosave.txt");    //default save file
    if (root->childPtr != NULL){                            //if fileTree is not empty
        char tree[2048];                                    //variable to store tree string
        strcpy(tree,"");                                    //tree is empty string
        writeNodes(root->childPtr, tree);                   //fill in tree
        FILE *fp = fopen(pathname, "w +");                  // fopen a FILE stream for WRITE 
        fprintf(fp, "%s",tree);                             // print tree to file 
        fclose( fp);                                        // close FILE stream when done
        return 0;
    }
    printf("there is nothing to save. Tree is empty.\n");   //else print error message
    return -1;                                              //and exit function
} 

void writeNodes(NODE *node, char *tree){
    if(node == NULL) return;                                //if node is null return to previous node
    char path[128];                                         //variable to store path to node
    strcpy(path,"");                                        //empty path
    pd(node, path);                                         //get path to node and store in path
    sprintf(tree, "%s%s %s\n",tree, node->type, path);      //add path to tree
    writeNodes(node->childPtr, tree);                       //go to child node
    writeNodes(node->siblingPtr, tree);                     //go to sibling node
    return;
}

int reload(char *pathname){                                 //creates a file tree from a file
    initialize();                                           //empty file tree
    char type[1];                                           //variable to store type
    char path[128];                                         //variable to store path
    FILE *fp = fopen(pathname, "r +");                      // fopen a FILE stream for read
    if (fp){                                                //if file open was successful
        while(fscanf(fp,"%s %s\n",type, pathname) != EOF){  //iterate through lines of file
            if (strcmp(type,"D") == 0) mkdir(pathname);     //make a directory
            if (strcmp(type,"F") == 0) creat(pathname);     //make a file
        }
        fclose( fp);                                        //close file
        return 0;                                           //return 0 for success
    }
    else{
        printf("error, coul]
         not open file: %s\n",pathname);// print error message
        return -1;                                          //return -1 for failure
    }   
    
} 

int menu(char *pathname){           //dislplay a menu of valid commands
    printf("mkdir [pathname]      make a new directory for a given pathname\n");
    printf("rmdir [pathname]      remove the directory, if it is empty\n");
    printf("cd [pathname]         change cwd to pathname, or to / if no pathname\n");
    printf("ls [pathname]         list the directory contents of pathname or cwd\n");
    printf("pwd                   the absolute pathname of cwd\n");
    printf("creat [pathname]      create a File node\n");
    printf("rm [pathname]         remove the file node\n");
    printf("save [pathname]       save current file system tree as a real file\n");
    printf("reload [pathname]     construct a file system tree from a file\n");
    printf("menu [pathname]       show a menu of vailid commands\n");
    printf("quit [pathname]       save the file system tree, then terminate the program\n");
    return 0;
} 

int quit(){                 //quit the program
    save("autosave.txt");   //save to default file
    exit(0);                //exit program 
} 
