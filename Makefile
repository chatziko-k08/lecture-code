all:
	$(MAKE) -C modules/UsingDynamicArray
	$(MAKE) -C modules/UsingLinkedList
	$(MAKE) -C modules/UsingADTList
	$(MAKE) -C programs/vector_example
	$(MAKE) -C programs/int_vector_example
	echo success

run:
	$(MAKE) run -C modules/UsingDynamicArray
	$(MAKE) run -C modules/UsingLinkedList
	$(MAKE) run -C modules/UsingADTList
	$(MAKE) run -C programs/vector_example
	$(MAKE) run -C programs/int_vector_example
	echo success

run-valrind:
	$(MAKE) run-valgrind -C modules/UsingDynamicArray
	$(MAKE) run-valgrind -C modules/UsingLinkedList
	$(MAKE) run-valgrind -C modules/UsingADTList
	$(MAKE) run-valgrind -C programs/vector_example
	$(MAKE) run-valgrind -C programs/int_vector_example
	echo success

clean:
	$(MAKE) clean -C modules/UsingDynamicArray
	$(MAKE) clean -C modules/UsingLinkedList
	$(MAKE) clean -C modules/UsingADTList
	$(MAKE) clean -C programs/vector_example
	$(MAKE) clean -C programs/int_vector_example
