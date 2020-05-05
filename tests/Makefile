# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

# Υλοποιήσεις μέσω dynamic array: ADTVector
#
UsingDynamicArray_ADTVector_test_OBJS = ADTVector_test.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μέσω συνδεδεμένης λίστας: ADTList
#
UsingLinkedList_ADTList_test_OBJS     = ADTList_test.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω ADTList: ADTStack, ADTQueue
# Πέρα από το .o της κάθε υλοποίησης (πχ ADTStack.o), χρειάζεται και μια υλοποίηση του ίδιου του ADTList!
#
UsingADTList_ADTStack_test_OBJS       = ADTStack_test.o $(MODULES)/UsingADTList/ADTStack.o $(MODULES)/UsingLinkedList/ADTList.o
UsingADTList_ADTQueue_test_OBJS       = ADTQueue_test.o $(MODULES)/UsingADTList/ADTQueue.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω Heap: ADTPriorityQueue
# Το Heap χρησιμοποιεί Vector, οπότε χρειαζόμαστε και μια υλοποίηση του ADTVector.
#
UsingHeap_ADTPriorityQueue_test_OBJS  = ADTPriorityQueue_test.o $(MODULES)/UsingHeap/ADTPriorityQueue.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μέσω BinarySearchTree: ADTSet
#
UsingBinarySearchTree_ADTSet_test_OBJS  = ADTSet_test.o $(MODULES)/UsingBinarySearchTree/ADTSet.o

# Υλοποιήσεις μέσω AVL Tree: ADTSet
#
UsingAVL_ADTSet_test_OBJS  = ADTSet_test.o $(MODULES)/UsingAVL/ADTSet.o

# Υλοποιήσεις μέσω B Tree: ADTSet
#
UsingBTree_ADTSet_test_OBJS  = ADTSet_test.o $(MODULES)/UsingBTree/ADTSet.o

# Υλοποιήσεις μέσω HashTable: ADTMap
#
UsingHashTable_ADTMap_test_OBJS	= ADTMap_test.o $(MODULES)/UsingHashTable/ADTMap.o

# Υλοποιήσεις μέσω ADTSet: ADTMap
# Πέρα από το .o της κάθε υλοποίησης (πχ ADTMap.o), χρειάζεται και μια υλοποίηση του ίδιου του ADTSet.
# Οπότε μπορούμενα φτιάξουμε πολλαπλά tests, για διαφορετικές υλοποιήσεις του ADTSet, ώστε να τις τεστάρουμε όλες.
#
UsingADTSet_ADTMap_test1_OBJS          = ADTMap_test.o   $(MODULES)/UsingADTSet/ADTMap.o    $(MODULES)/UsingBinarySearchTree/ADTSet.o
UsingADTSet_ADTMap_test2_OBJS          = ADTMap_test.o   $(MODULES)/UsingADTSet/ADTMap.o    $(MODULES)/UsingAVL/ADTSet.o
UsingADTSet_ADTMap_test3_OBJS          = ADTMap_test.o   $(MODULES)/UsingADTSet/ADTMap.o    $(MODULES)/UsingBTree/ADTSet.o

# Ο βασικός κορμός του Makefile
include ../common.mk