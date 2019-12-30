# Λίστα με τα executables που θέλουμε να κάνουμε compile
PROGS = unit_tests

# Λίστα από .o files που χρειάζεται κάθε πρόγραμμα (μια μεταβλητή ανά executable)
# Χρειαζόμαστε και μια υλοποίηση του ADTList, παίρνουμε αυτή μέσω LinkedList!
#
unit_tests_OBJS = unit_tests.o ADTStack.o ADTQueue.o $(MODULES)/UsingLinkedList/ADTList.o

# Ο βασικός κορμός του Makefile
include ../../common.mk