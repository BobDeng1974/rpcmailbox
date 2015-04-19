#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef char * str;
typedef str * messages;


struct message {
        str user;
        int msgid;
        str msg;
};
typedef struct message message;

struct usermsgid {
        str user;
        int msgid;
};
typedef struct usermsgid usermsgid;

struct listmessages {
	messages list;
	int length;
};
typedef struct listmessages listmessages;

/* Limits and keywords */

const int CHARLIMIT = 80;
const int BOXMSGLIMIT = 100;
const int USERMSGLIMIT = 20;

/* Stores users and length of users array */
static str * users;
int usersl = 0;
int usersinit = 0;

/* Stores messages and length of messages array */
static message ** mailbox;
int mailboxl = 0;
int mailboxinit = 0;


int index_of_user(str * username)
{
	int i;
	for (i = 0; i < usersl; i++)
	{
		//printf("is %s == %s\n", *s, users[i]);

		if (strcmp(*username, users[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* Creates a message structure and returns it (returns 0 if theres a problem) 
message make_message(str * s, int msgid, str * user)
{
	

}*/


/* Adds *s to the end of the message list and increments the mailboxl (length) */
int add_to_mailbox(message * m)
{
	pthread_mutex_lock(&mutex);

	printf("Trying to add message '%.7s... to mailbox ", m->msg);

	// Make space for new message	
	if (mailboxinit == 0)
	{
		mailbox = calloc(1, sizeof(message *));
		mailboxinit = 1;
	}
	else if (mailboxl + 1 > BOXMSGLIMIT)
	{
		mailboxpop(); 	// decremenets length of mailbox (mailboxl)
	}
	else
	{
		mailbox[mailboxl] = (message*) calloc(1, sizeof(message*));
	}

	// Add new message	
	mailbox[mailboxl++] = m;

	printf(" .. successfully added message '%.25s'(cntd)\n", mailbox[mailboxl-1]->msg);
	
	pthread_mutex_unlock(&mutex);
	return 1;
	
}


/* Shifts all the elements in this array to the left by 1 popping off the oldest element */
int mailboxpop()
{
	int i;
	for (i = 0; i < mailboxl-1; i++)
	{
		mailbox[i] = mailbox[i+1];
	}
	mailboxl--;
}

/* Adds `*s` to the end of the user list and increments the usersl (length)*/
int add_to_users(str * username)
{
	pthread_mutex_lock(&mutex);
	
	printf("Trying to add a user now");	
	if (usersinit == 0)
	{
		users = calloc(1, sizeof(str *));
		usersinit = 1;
	}
	if (index_of_user(username) == -1)
	{
		// make space, add the username in, and increment length of users
		users[usersl] = calloc(1, sizeof(char*));
		users[usersl++] = *username;
		
		printf(". Successfully adding user '%s'\n", *username);
		
		pthread_mutex_unlock(&mutex);
		return 1;	
	}
	else
	{
		printf(" but user '%s' already exists .. didn't add user\n", *username);
		
		pthread_mutex_unlock(&mutex);
		return 0;
	}	
}


/* Ends all services for the user - removes it from users array, deletes its messages */
void remove_user_messages(str * username)
{
	pthread_mutex_lock(&mutex);	

	/* Remove user from users array and decrement array length */
	printf("Removing user '%s' from all databases\n", *username);
	remove_user(username);
	remove_messages(username);

	pthread_mutex_unlock(&mutex);
}

/* Remove user from users array and decrement array length. 
	Leaves users array in contigous state. */
int remove_user(str * username) 
{
	printf("Trying to delete user '%s'", *username);
	
	int index = index_of_user(username);
	
	if (index == -1)
	{
		printf("but user '%s' doesn't exist\n", *username);
		return 0;
	}
 	
	// Shift left from index -> end of array
	int i;
	for (i = index; i < usersl-1; i++)
	{
		users[i] = users[i+1];
	}
	
	// set this pointer value to null and deallocate it, decrement usersl
	users[--usersl] = (void *) 0;
	free(users[usersl]);	
	
	printf(" ... deleted user '%s'\n", *username);

	return 1;	
}


/* Remove all messages associated with user from messages array. 
	Leaves messages array in contigous state.  */
int remove_messages(str * username)
{
	printf("Removing messages for '%s' ..\n", *username);
	int i;
	for (i = 0; i < mailboxl; i++)
	{
		if (strcmp(*username, mailbox[i]->user) == 0)
		{
			mailboxpop_at(i);
		}
	}
	
	printf("\n");

	return 1;	
}

/* deletes the element at the `index` and 
	shifts the mailbox[i+1..n] to mailbox[i..n-1] and decrements n */  
int mailboxpop_at(int index)
{		
	printf(" \t.. deleting message '%.8s...\n", mailbox[index]->msg); 

	for (; index < mailboxl-1; index++)
	{
		mailbox[index] = mailbox[index+1];	
	} 
	
	// set this pointer value to null and deallocate it, decrement mailboxl
	mailbox[--mailboxl] = (void *) 0;
	free(mailbox[mailboxl]);

	return 1;
}

/* Deletes specified message */
int delete_message(usermsgid * o)
{	
	pthread_mutex_lock(&mutex);

	printf("Trying to delete message (id: %d) .. ", o->msgid);
	
	// First compares msgid then user
	int i;
	for (i = 0; i < mailboxl; i++)
	{
		if (o->msgid == mailbox[i]->msgid)
		{
			if (strcmp(o->user, mailbox[i]->user) == 0)
			{
				printf(" .. deleting message '%.8s...i\n", mailbox[i]->msg);
				mailboxpop_at(i);	
			}
		} 	
	}
	
	printf("\n");

	pthread_mutex_unlock(&mutex);
	return 1;
}

/* Retrieves message from mailbox */
str retrieve_message(usermsgid * o)
{
	pthread_mutex_lock(&mutex);	

	printf("Trying to retrieve message %s : %d .. ", o->user, o->msgid);
	
	// First compares msgid then user
	int i;
	for (i = 0; i < mailboxl; i++)
	{	
		if (o->msgid == mailbox[i]->msgid)
		{
			
			if (strcmp(o->user, mailbox[i]->user) == 0)
			{
				printf("found it!\n");
				
				pthread_mutex_unlock(&mutex);
				return mailbox[i]->msg;
			} 
		} 	
	}	
	
	printf("couldn't find it..\n");
	pthread_mutex_unlock(&mutex);	

	return calloc(1, sizeof(str));
}

/* Pretty print all users */
int print_users() 
{
	pthread_mutex_lock(&mutex);

	printf("Printing %d users...\n\n", usersl);
	printf("\tuserid : username\n\t-------:---------\n");

	int i;
	for (i = 0; i < usersl; i++)
	{
		printf("\t%6d : '%s'\n", i,  users[i]);
	}
	
	printf("\n");

	pthread_mutex_unlock(&mutex);
	return 1;
}

/* Pretty print all messages */
int print_mailbox()
{
	pthread_mutex_lock(&mutex);
	
	printf("Printing %d messages...\n\n", mailboxl);
	printf("\tuser :\n\t\t: properties\n\t-----:----------\n");

	int i;
	for (i = 0; i < mailboxl; i++)
	{
		printf("\t%s's mailbox::\n\t\tmessage id: %d\n\t\tmessage: %s\n\n",
			 mailbox[i]->user, mailbox[i]->msgid, mailbox[i]->msg);
	}

	printf("\n");

	pthread_mutex_unlock(&mutex);
	return 1;
}

/* Print a specified user's messages */
int print_users_messages(str * username)
{
	pthread_mutex_lock(&mutex);
	
	printf("Printing %s's messages .. \n", *username);

	int i;
	for (i = 0; i < mailboxl; i++)
	{
		if (strcmp(*username, mailbox[i]->user) == 0)
		{
			printf("\t%s\n", mailbox[i]->msg);
		}
	}	
	
	printf("\n");	

	pthread_mutex_unlock(&mutex);
	return 1;
}

/* Returns a listmessages of specified users mailbox */
listmessages list_messages(str * argp)
{
	pthread_mutex_lock(&mutex);

	// initializations
	static struct listmessages result = {0};
	
	printf("User: %s\n", *argp);

	str s = strdup(*argp);
	str * username = &s;

	printf("Printing %s's messages .. ", *username);

	str * temp = calloc(1, sizeof(str*));

	// gathering all relevant messages
	int k = 0;
	int i;
	for (i = 0; i < mailboxl; i++)
	{
		if (strcmp(*username, mailbox[i]->user) == 0)
		{	
			temp[k] = calloc(1, sizeof(str));
			temp[k++] = mailbox[i]->msg;	
		}
	}	

	printf("there are %d of them:\n", k);

	// setting listmessages values
	result.list = calloc(1, sizeof(str *));
	for (i = 0; i < k; i++)
	{
		result.list[i] = calloc(1, sizeof(str));
		result.list[i] = temp[i];
		printf("\t%s\n", temp[i]);
	}
	results.list = k;
	
	printf("\n");	

	pthread_mutex_unlock(&mutex);

	return &result;
}
		
	
int main(int argc, char * argv[]) 
{
	//printf("%d\n", is_in("tim", users, &usersl));	
	//add_to_users("tim", users, &usersl);
	//printf("%d\n", is_in("tim", users, &usersl));	
	/*
	users = calloc(1, sizeof(str *));
	mailbox = calloc(1, sizeof(message *));
	*/

	/* Create users */	
	str name1 = strdup("ishaan\0");
	
	str name2 = strdup("arya\0");

	str name3 = strdup("samir\0");
	
	str name4 = strdup("parul\0");

	str copy = strdup(name1);
	printf("%s -> %s\n", name1, copy);
	
	printf("%s - %p\n", name1, &name1);


	add_to_users(&name1);

	
	print_users();		

	/* add users */
	add_to_users(&name2);
	add_to_users(&name3);
	add_to_users(&name4);
	
	print_users();
	
	/* remove users */
	remove_user(&name2);
	print_users();
	remove_user(&name4);
	print_users();
	
	/* add them back */
	add_to_users(&name2);
	add_to_users(&name4);
	
	print_users_messages(&name1);
	print_users_messages(&name2);
	print_users_messages(&name3);
	print_users_messages(&name4);

	
	printf("%s - %p\n", name1, &name1);


	/* create messages */
	message m1 = {"ishaan\0", 0, "Hello there!\0"};
	message m2 = {"ishaan\0", 1, "How have you been?\0"};
	message m3 = {"arya\0", 2, "I've been great brotha. How about you?\0"};
	message m4 = {"parul\0", 3, "Hello bebus\0"};
	message m5 = {"ishaan\0", 4, "Hi Mom, hows it going?\0"};
	message m6 = {"arya\0", 5, "hello ma\0"};
	message m7 = {"samir\0", 6, "*video*\0"};
	message m8 = {"arya\0", 7, "I'm Ishaan\0"};	
	
	printf("%s - %p\n", name1, &name1);
	
	print_users();
	
	/* add messages */
	add_to_mailbox(&m1);
	add_to_mailbox(&m2);
	add_to_mailbox(&m3);
	add_to_mailbox(&m4);
	add_to_mailbox(&m5);
	add_to_mailbox(&m6);
	add_to_mailbox(&m7);
	add_to_mailbox(&m8);
		
	printf("%s - %p\n", name1, &name1);

	print_mailbox();
	
	/*
	// test delete message	
	usermsgid o = {"arya", 7};
	list_messages(&name2);
	delete_message(&o);
	list_messages(&name2);
	*/

	/* test print user messages	*/
	list_messages(&name1);
	list_messages(&name2);
	list_messages(&name3);
	list_messages(&name4);	

	/* test remove user messages */
	remove_user_messages(&name3);
	print_users();
	print_mailbox();

	/* test retrieve message */
	usermsgid another = {"arya", 2};
	str s1 = retrieve_message(&another);
	printf("%s\n\n", s1);	

	list_messages(&name2);
} 
