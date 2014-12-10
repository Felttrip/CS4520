###############################################################################
# CS4520 - Project
###############################################################################
#
# Macros to make sure we use the system's C compiler and options to
# create a source listing and additional link libraries.
CC = cc
OPTS = -pthread -lpthread #-lrt

# Default action is to create three processing modules.
#all:	producer consumer parent # alternate means to build the code

all:	producer consumer parent
	@- echo "Initiate building of Producer/Consumer code"
	@- $(MAKE) producer
	@- $(MAKE) consumer
	@- $(MAKE) parent

# Create the producer module.
producer: producer.c project.h
	@- echo "Build the Producer code"
	$(CC) producer.c $(OPTS) -o producer

# Create the consumer module.
	@- echo "Build the Consumer code"
consumer: consumer.c project.h
	$(CC) consumer.c $(OPTS) -o consumer

# Create the parent (main) module.
	@- echo "Build the Parent (main) code"
parent:	parent.c project.h
	$(CC) parent.c $(OPTS) -o parent

# Clean up a directory after a build.
clean:
	@- echo "Cleanup directory after a build"
	@- rm -f parent.lis producer.lis consumer.lis
	@  rm -i parent producer consumer

# End of Makefile
