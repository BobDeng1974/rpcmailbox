Ishaan Taylor
EECS 338: Operating Systems
Assignment 6


Essentials:
	OS 			:	Linux
	Compiler 	:	gcc


Explaination of files:
	1. Local implementation of server functions (not guarenteed identical to server source)
		test.c 						->	C source file for implementation
		test 						->	corresponding executable 
		local-test-output.txt 		->	output of all the printfs in test.c
	2. Unix tools I used to make development easier (*.sh)
		push.sh
		compileclient.sh  --> (deprecated, see makefile)
		compileserver.sh  --> (deprecated, see makefile)
		.bashrc			  --> (deprecated)
	3. RPC related files 
		-that I wrote:
			mailbox.x
			mailbox_client.c
			mailbox_server.c
			server
			client
		-that were generated:
			mailbox.h
			mailbox_clnt.c
			mailbox_svc.c
			mailbox_xdr.c
			Makefile.mailbox
		-that are test output:
			server-output.txt
			client-output.txt
			client-server-sim-output.txt
			gdb-output.txt
	4. makefile
		(theres also a Makefile.mailbox, but I just used my own makefile)


To run code from scratch (local folder on my laptop):
	1. `scp -r . ist2@eecslinab3.case.edu:~/assignment4`
	2. `source push.sh` (type password twice) 
		(not sure why I have to source it, but only way i could get it to work)
	2.5 In case `server` and `client` already exist in the copied folder, `rm server client`.
	3. `make` in eecslinab{2 3 7}
	4. Pick one to run as server: `./server` (usually I pick 3)
	5. Run the other two as client: `./client eecslinab<x>` (where <x> is number of server)


How I tested (code will have comments to follow along):
	1. See mailbox_client.c as source for client testing. I tested the RPC paths by 
		1) Creating 4 names
		2) Removing 2 of them
		3) Inserting messages for the remaining two
		4) Retrieving a message
		5) Listing all messages for a the remaining two usernames
		6) Deleting a message
	I tested this on both eecslinab2 and 7. Both disjointly (client-output.txt and server-output.txt) and nearly simultaneously (see client-server-sim-output.txt).

	2. As the client sometimes gets a seg fault, I've attached a test implementation (test.c) and output file (local-test-output.txt) to attempt to show you that my algorithmic understanding of the system is correct. This is a much more thorough test of the abilities of the mailbox. The steps I took here are:
		1) 	Added 4 users
		2) 	Removed 2 users
		3)	Added them back
		4)	Printed all their messages
		5)	Added 8 messages
		6)	Printed mailbox
		7)	List each users messages
		8)	Remove a user from db
		9)	Show that user was deleted
		10)	Retrieve a message for a user
		11)	List messages for the same user
	In addition, while it is not documented in the code, I did a stress test to see if the system could handle over 100 messages, and the queue was able to work.


There are some memory issues, as the client has had a segmentation fault each time I have tested it. I've tried using gdb and valgrind but I've run out of time for this. If I had more time, I would continue debugging and figuring out the memory issues. (They happen when I try to add a message with RPC, but not locally. It may have to do with a referencing a value in a struct.) Additionally, if I had the basic RPC functionality working, I would construct a rigorous testing tool, allowing hundreds of messages to be inserted with, say, `insert --number 500 --message "..."`. This would allow for more rigorous testing. Additionaly, I would try and run this locally to see if it can handle more than 2 (maybe even spin up a few VMs or more cloud instances).

I enjoyed writing C code for this project, and I learned a lot. Please let me know if you have any questions at all, as I know this submission is not perfect. I am available any time.

Thank you for your time,
Ishaan



