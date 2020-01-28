# Κάνοντας compile το <foo>_tests.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

# Υλοποιήσεις μέσω dynamic array
#
ADTVector_tests_UsingDynamicArray_OBJS = ADTVector_tests.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μέσω συνδεδεμένης λίστας
#
ADTList_tests_UsingLinkedList_OBJS     = ADTList_tests.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω ADTList. Χρειάζονται πέρα από το .o της υλοποίησης (πχ ADTStack.o) και μια υλοποίηση του ADTList!
#
ADTStack_tests_UsingADTList_OBJS       = ADTStack_tests.o $(MODULES)/UsingADTList/ADTStack.o $(MODULES)/UsingLinkedList/ADTList.o
ADTQueue_tests_UsingADTList_OBJS       = ADTQueue_tests.o $(MODULES)/UsingADTList/ADTQueue.o $(MODULES)/UsingLinkedList/ADTList.o

# Ο βασικός κορμός του Makefile
include ../common.mk