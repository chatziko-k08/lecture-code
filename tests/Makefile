# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

# Υλοποιήσεις μέσω dynamic array
#
UsingDynamicArray_ADTVector_test_OBJS = ADTVector_test.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μέσω συνδεδεμένης λίστας
#
UsingLinkedList_ADTList_test_OBJS     = ADTList_test.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω ADTList. Χρειάζονται πέρα από το .o της υλοποίησης (πχ ADTStack.o) και μια υλοποίηση του ADTList!
#
UsingADTList_ADTStack_test_OBJS       = ADTStack_test.o $(MODULES)/UsingADTList/ADTStack.o $(MODULES)/UsingLinkedList/ADTList.o
UsingADTList_ADTQueue_test_OBJS       = ADTQueue_test.o $(MODULES)/UsingADTList/ADTQueue.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω BinarySearchTree
#
UsingBinarySearchTree_ADTSet_test_OBJS  = ADTSet_test.o $(MODULES)/UsingBinarySearchTree/ADTSet.o

# Υλοποιήσεις μέσω HashTable.
#
UsingHashTable_ADTMap_tests_OBJS	= ADTMap_test.o $(MODULES)/UsingHashTable/ADTMap.o

# Υλοποιήσεις μέσω ADTSet. Χρειάζονται πέρα από το .o της υλοποίησης (πχ ADTMap.o) και μια υλοποίηση του ADTSet!
#
UsingADTSet_ADTMap_test_OBJS          = ADTMap_test.o   $(MODULES)/UsingADTSet/ADTMap.o    $(MODULES)/UsingBinarySearchTree/ADTSet.o

# Ο βασικός κορμός του Makefile
include ../common.mk