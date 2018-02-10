#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/*--Function Declarations--*/
int ex_pipe();
int ex_one_pipe();
void ex();
void parse_and_ex();
void create_cd();
void create_no_cd();

/**--Global Variables--**/
static char **givenpar;
static char *formed, *formed1;

/***-------Main Programm-------***/
int main() {
  	
  int pipenum, paramsnum, com, met, isfileopen = 0;
  int i, k, l = 0, len, j, count, lines = 0, countones, spaces;
  int *counttabs;
  char input[512], batchfile[50], choice = 'N', ch;
  com = 0;

  printf("Do you want to open a batchfile?(y/n)\n");
  while(choice != 'y' && choice != 'n'){
    scanf("%c", &choice);
  }
  if(choice == 'y'){
    printf("Give the name of the batchfile :\n");
    scanf("%s", batchfile);
    FILE *file; 
    if((file = fopen(batchfile, "r")) == NULL){
      printf("File not found, exiting...");
      exit(1);
    }
    else{
      isfileopen = 1;
      fseek(file, 0, SEEK_SET);
      while((ch = fgetc(file)) != EOF){
        if(ch != '\n') input[l] = ch;
	else{
          input[l] = ';';
          lines++;
        }
        l++;
      }
      fclose(file);
      input[l] = '\0';
    }
    goto a;
  }
  else{
													// prompt
    printf("$ chorafas_8718 ~>");
    scanf(" %511[^\n]", input);
    len = strlen(input);
    a:
		
    /****-------------My Shell------------****/
    while(strcmp(input , "quit") != 0){
												        // dispose empty characters and spaces - afairesh kenwn
      len = strlen(input);
      counttabs = (int*)malloc(len * sizeof(int));
      spaces = 0;
      for(i = 0; i < len; i++){
	if(input[i]=='\t') input[i]=' ';
      }
      for(i = 0; i < len; i++){
	if(input[i] == ' ') counttabs[i] = 1;
	  else counttabs[i] = 0;
        }

	i = len-1;
	while((i >= 0) && (counttabs[i] == 1)){ 							//countspaces after command-metraw ta telika kena meta thn entolh
	  spaces++;
	  i--;
	}

	while(counttabs[0] == 1){  									// count spaces brefore command-metraw ta arxika kena prin thn entolh
		spaces++;
		for(j = 1; j < len; j++){								// slide the command to the first index-olis8hsh ths entolhs gia na paei sthn arxh tou pinaka input
			counttabs[j-1] = counttabs[j];
			input[j-1] = input[j];
		}
	}

	countones = 0;
	k = len-spaces;  										// k = length of input - spaces, cpunt spaces in the middle
	formed = malloc(k * sizeof(char)); 								// final table with the final form of the command- telikos pinakas,h swsth entolh xwris peretairw kena
	formed1 = malloc(k * sizeof(char));
	i = 0;
	j = 0;
	while(i < k){
	  if(counttabs[i] == 0){  									// if element is not a space -otan exei gramma
	    formed[j] = input[i];  									// then add it in formed[]-bazw katey8eian to gramma ston formed
	    i++;
	    j++;
	    countones = 0;
	  }
	  else{
	    countones++;    									        // found the first space and so on...-brhka to prwto keno, k.o.k
	    if(countones == 1){
	      formed[j] = input[i]; 							                // add the one and only space i need-bazw to ena kai monadiko keno pou xreiazomai
	      j++;
	      i++;
	    }
	    else if(countones > 1){
	      while(counttabs[i] != 0){ 								// until i find next non space char-mexri na brw to epomeno gramma
	        i++;
	      }
	      formed[j] = input[i]; 									// start adding the characters-ta epommena grammata
	      i++;		
	      j++;
	      countones = 0;
	    }
          }
        }

	for(i = len-1; i >= 0; i--){  									// count length of command- krataw to mhkos ths entolhs, orio8etw ton pinaka
	  if(counttabs[i] == 0){
	    j = i + 1;
	    break;
	  }
	}

	for(i = j; i < k; i++)	formed[i] = 0; 								// rest of elements = 0-tous ypoloipous xarakthres apo de3ia tous bazw 0
	for(i = 0; i < k; i++) formed1[i] = 0; 								// formed1[] initialization-arxikopoihsh tou formed1


	for(i = 0; i < k; i++){
	  if(formed[i] == ';') com++;   								// count the number of ';'- metraw ta erwthmatika
	  //else if(formed[i] == '&' && formed[i+1] == '&') com = com + 2;  		                // in case command included "&&" instead of ';'-an eixa anti gia ; ta &&
	}

	for(met = 0; met < com + 1; met++){
	  j = 0;
	  while(formed[0] != ';' && formed[0] != '\0'){ 						// until i find a ';' or the end-auto ginetai mexri na brw to erwthmatiko=prwth entolh
	    formed1[j] = formed[0]; 									// add formed[0] to formed1[j], then slide formed[]
													// -olis8hsh tou formed kata 1 stoixeio aristera, ka8e fora bazw to prwto tou stoixeio ston formed 1
	    j++;
	    for(i = 0; i < strlen(formed); i++) formed[i] = formed[i+1];
	  }

	  if(formed[0] == ';'){ 									// slide formed again- olis8hsh tou formed gia na mhn parei to erwthmatiko h to keno o formed1
	    for(i = 0; i < strlen(formed); i++) formed[i] = formed[i+1];
	  }
	  if(formed[0] == ' '){
	    for(i = 0; i < strlen(formed); i++) formed[i] =formed[i+1];
	  }
	  if(met == com){ 										// is there an "exit"?-an exw exit meta apo ena erwthmatiko
	    if((formed1[0] == 'q') && (formed1[1] == 'u') && (formed1[2] == 'i') && (formed1[3] == 't')){
	      printf("Quit command was found, exiting...");
	      free(counttabs);
    	      free(givenpar);
    	      free(formed);
	      exit(0);
	    }
	    else if(isfileopen == 1 && formed1[0] != 'q'){
	      printf("quit command was not found, exiting...\n");
	      free(counttabs);
              free(givenpar);
              free(formed);
	      exit(0);
	    }
	  }
													// now i have the next command-mexri edw exw thn pros ektelesh entolh, px thn 1h , meta thn 2h klp

	  pipenum = 0;
	  for(i = 0; i < strlen(formed1); i++){
	    if(formed1[i] == '|') pipenum = pipenum + 1;
	  }

	  if(pipenum == 0){
	    if((formed1[0] == 'c') && (formed1[1] == 'd')){						// cd case - periptwsh cd
	      create_cd(formed1);
	      for(i = 0; i < strlen(formed1); i++) formed1[i] = ' '; 					// after cd, initialize formed1-afou ginei h cd ton kanw ek neou arxikopoihsh
	    }	    											// if not cd case - pwriptwsh oxi cd
	    else if((formed1[0] != 'c') && (formed1[1] != 'd')){
	      paramsnum = 1;       									// command parameters counter, starting from one-parametroi ths entolhs 3ekinwntas apo thn idia thn entolh, px ls
	      for(i = 0; i < k; i++){
	        if(formed1[i] == ' ') paramsnum = paramsnum + 1;
	      }
	      givenpar = (char**)malloc((paramsnum + 1) * sizeof(char*));				// +1 space for '\0'-ton +1 einai to teleutaio keli kai einai to \0
	      parse_and_ex(formed1, givenpar);
	      ex(givenpar);
	    }
          }
          else{
	    formed1[strlen(formed1)] = 0; 								// boundary-orio8ethsh tou formed1
	    ex_pipe(formed1, pipenum); 								      
	  }
	}								                                // end of for met
        if(isfileopen == 1 || fileisclosed == 1){
	  printf("\n$ chorafas_8718 ~>");
	  scanf(" %511[^\n]", input);
	  if((input[0] == 'e') && (input[1] == 'x') && (input[2] == 'i') && (input[3] == 't')){
	    printf("Did you mean quit?\n");
	  }
	}	    
    
    }													// end of while
  
  }

}													// end of main

void parse_and_ex(char formed[], char * given[]){
  int i = 0;
  given[i] = strtok(formed, " "); 									// givenpar has all the parameters of the command-o givenpar exei px sto prwto keli to ls kai sto deutero to -l
  while(given[i] != NULL){
    i++;
    given[i] = strtok(NULL, " ");
  }
}

int ex_pipe(char formed[], int pipenum){
  int i, j, prog, k, z;
  int numpipes, paramsnum;
  char c_formed[100];
  char **give;
  pid_t id;
  int *pipefds;
  numpipes = 2 * pipenum;
  pipefds = malloc(numpipes * sizeof(int));
  for(i = 0; i < pipenum; i++){	
    pipe(pipefds + (i * 2));										// create the pipes
  }
  for(i = 0; i < 100; i++) c_formed[i] = 0;
  j = 0;
  while((formed[0] != '|') && (formed[0] != '\0')){
    c_formed[j] = formed[0];
    j++;
    for(i = 0; i < strlen(formed); i++) formed[i] = formed[i+1];
  }
  																																									// slide one more time-allh mia fora gia na fygei to pipe
  if(formed[0] == '|'){
    for(i = 0; i < strlen(formed); i++) formed[i] = formed[i+1];
  }
  paramsnum = 1;
  for(i = 0; i < 100; i++){
    if(c_formed[i] == ' ') paramsnum++;
  }
  give = (char**)malloc((paramsnum + 1) * sizeof(char*));
  parse_and_ex(c_formed, give);
  j = 0;
  for(k = 0; k <= pipenum; k++){									// until pipenum+1 because a single pipe requires 2 commands 
    id = fork();
    if(id == 0){
    if(k != pipenum) dup2(pipefds[j+1], 1); 								// redirection of stdout-to 1 einai to stdout
    if((k != 0) && (j != 0)) dup2(pipefds[j-2], 0); 							// redirection of stdin-to 0 einai to stdin
    for(i = 0; i < numpipes; i++) close(pipefds[i]);
      i = execvp(give[0], give);
      if(i == -1){
	printf("Command not found.\n");
	exit(0);
      }
    }
    else if(id < 0){
      exit(EXIT_FAILURE);
    }
    for(i = 0; i < 100; i++) c_formed[i] = 0;
    z = 0;
    while((formed[0] != '|') && (formed[0] != '\0')){
      c_formed[z] = formed[0];
      z++;
      for(i = 0; i < strlen(formed); i++) formed[i] = formed[i+1];
    }
      																																						
    if(formed[0] == '|'){
      for(i = 0; i < strlen(formed); i++) formed[i] = formed[i+1];
    }
    paramsnum = 1;
    for(i = 0; i < 100; i++){
      if(c_formed[i] == ' ') paramsnum++;
    }
    give = (char**)malloc((paramsnum + 1) * sizeof(char*));
    parse_and_ex(c_formed, give);
    j += 2;
  }
  for(i = 0; i < numpipes; i++){
    close(pipefds[i]);
  }

  for(i = 0; i < numpipes + 1; i++){
    wait(&prog);
  }
  return 1;
}

void create_cd(char  formed[]){
  int i, id; 
  char * paramcd;
  char * parampath;
  if(id < 0) exit(1);
  if(id == 0){
    if(formed[2] == 0 && formed[3] == 0){
      chdir(getenv("HOME"));
      printf("Transfered to Home directory.\n");
    }
    else{
      paramcd = strtok(formed, " "); 									  // "cd" to..-pairnw to cd
      parampath = strtok(NULL, " "); 									  // .."parampath"-pairnw to path
      i = chdir(parampath); 										  // change directory
      if(i == -1) printf("'%s' directory not found.\n", parampath);
      else if(parampath[0] == '.' && parampath[1] == '.') printf("Transfered to upper folder.\n");
      else printf("Transfered to directory : %s\n", parampath);
    }
  }
  else wait(NULL); 
}

void ex(char * givenpar[]){ 										  // execute command - ektelesh ths entolhs
  pid_t id;
  int i;
  id = fork();
  if(id < 0) exit(1);
  else if(id == 0){
    i = execvp(givenpar[0], givenpar);
    if(i == -1) printf("Command not found.\n");
      exit(0);
  }
  else wait(NULL);
}
