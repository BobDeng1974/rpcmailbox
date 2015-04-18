#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char * str;
typedef str * messages;

const int msglength = 80;
const int messageslength = 100;

struct message  {
        str user;
        int msgid;
        str msg;
};
typedef struct message message;

struct usermsgid  {
        str user;
        int msgid;
};
typedef struct usermsgid usermsgid;

/* Stores users and length of users array */
static str * users;
static int usersl = 0;

/* Stores messages and length of messages array */
static message * mailbox;
static int mailboxl = 0;



int index_of_user(str * s)
{
	int i;
	for (i = 0; i < usersl; i++)
	{
		//printf("is %s == %s\n", *s, users[i]);

		if (strcmp(*s, users[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* Adds `*s` to the end of the user list and increments the usersl (length)*/
int add_to_users(str * s)
{
	printf("Trying to add a user now");	
	if (index_of_user(s) == -1)
	{
		users[usersl] = calloc(1, sizeof(char*));
		users[usersl++] = *s;
		
		printf(". Successfully adding user '%s'\n", *s);
		return 1;	
	}
	else 
	{
		printf(" but user '%s' already exists .. didn't add user\n", *s);
		return 0;
	}	
}


/* Ends all services for the user - removes it from users array, deletes its messages */
int remove_user_messages(str * s)
{
	/* Remove user from users array and decrement array length */
	remove_user(s);
	remove_messages(s);
}

/* Remove user from users array and decrement array length. 
	Leaves users array in contigous state. */
int remove_user(str * s) 
{
	printf("Trying to delete user '%s'", *s);

	int index = index_of_user(s);
	
	if (index == -1)
	{
		printf("but user '%s' doesn't exist\n", *s);
		return 0;
	}
 	
	// Copy i+1th element into i until end of user array (userl)
	int i;
	for (i = index; i < usersl-1; i++)
	{
		users[i] = users[i+1];
	}
	
	// set this pointer value to null and deallocate it, decrement usersl to be correct
	users[--usersl] = (void *) 0;
	free(users[usersl]);	
	
	printf(" ... deleted user '%s'\n", *s);
	return 1;	
}


/* Remove all messages associated with user from messages array. 
	Leaves messages array in contigous state.  */
int remove_messages(str * s)
{

}


int print_users() 
{
	printf("Printing %d users...\n", usersl);
	printf("\tuserid : username\n\t-------:---------\n");

	int i;
	for (i = 0; i < usersl; i++)
	{
		printf("\t%6d : '%s'\n", i,  users[i]);
	}
	return 1;
}



int main(int argc, char * argv[]) 
{
	//printf("%d\n", is_in("tim", users, &usersl));	
	//add_to_users("tim", users, &usersl);
	//printf("%d\n", is_in("tim", users, &usersl));	
	int i;
	
	
	
//`	printf("%d\n", is_in("tim", users, &usersl));

//	char * s[3] = {"123456", "1094821", "190"};
	users = calloc(1, sizeof(str *));
	

	
	
	str name = calloc(8, sizeof(char));
	strcpy(name, "ishaan");
	

	str name2 = calloc(8, sizeof(char));
	strcpy(name2, "arya");

	str name3 = calloc(8, sizeof(char));
	strcpy(name3, "samir");
	
	str name4 = calloc(8, sizeof(char));
	strcpy(name4, "parul");


	printf("%s - %p\n", name, &name);
	
	index_of_user(&name);	
		//addUser(&name);
	add_to_users(&name);
	//	printf("%s\n", (*s)++);	
		//printf("%s\n\n", s[i]);
	print_users();
	
	remove_user(&name);

	print_users();		

	add_to_users(&name2);
	add_to_users(&name3);
	add_to_users(&name4);
	
	print_users();
	
	remove_user(&name2);
	print_users();
	remove_user(&name4);
	print_users();
	
	add_to_users(&name);
	add_to_users(&name2);
	print_users();
		

} 
