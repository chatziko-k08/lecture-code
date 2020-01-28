# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

# Υλοποιήσεις μέσω dynamic array
#
UsingDynamicArray.ADTVector.test_OBJS = ADTVector.test.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μέσω συνδεδεμένης λίστας
#
UsingLinkedList.ADTList.test_OBJS     = ADTList.test.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω ADTList. Χρειάζονται πέρα από το .o της υλοποίησης (πχ ADTStack.o) και μια υλοποίηση του ADTList!
#
UsingADTList.ADTStack.test_OBJS       = ADTStack.test.o $(MODULES)/UsingADTList/ADTStack.o $(MODULES)/UsingLinkedList/ADTList.o
UsingADTList.ADTQueue.test_OBJS       = ADTQueue.test.o $(MODULES)/UsingADTList/ADTQueue.o $(MODULES)/UsingLinkedList/ADTList.o

# Ο βασικός κορμός του Makefile
include ../common.mk