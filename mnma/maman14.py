import mmh3
import sys
from nodes import SingleLinkedList
from nodes import ListNode


def main():
    # get m and K fro the user
    global m
    global K
    global T
    m = int(input("Insert the value of m: \n"))
    K = int(input("Insert the value of K: \n"))
    
    # initialize T array
    T = [{"Key": 0, "Value": SingleLinkedList()} for _ in range(m)]

    # Open the file for object to insert into the data structure
    insert()

    # Open file for objects to search
    lookups = open("lookup.txt", "r").readline().split(",")
    for object in lookups:
        search(object)

    # Print T table and liked lists for debug. If needed, un-comment the following for loop
    # for i in range(m):
    #     print('table index is: '+ str(i) + '\n')
    #     print('key is: ' + str(T[i]['Key']) + '\n')
    #     print('valus is: ' + str(T[i]['Value'].output_list()) + '\n')

def insert():
    # This function opens inputs.txt file and for each object in it runs the runHAshFunctions()
    print("Inserting objects to the data structure...\n")
    inputs = open("inputs.txt", "r").readline().split(",")
    for input in inputs:
      runHashFunctions(input)

def search(object):
    # This function checks if the object is in the data structure 
    print('Searching for ' + object + ' in the data structure...\n')
    for i in range(K):
        indexInT=int(mmh3.hash(object, i, signed=True) % m)
        if T[indexInT]['Key'] == 0:
            print('The object: ' + object + '\n' + 'is not part of the data structure.\n')
            return
    print('The object: ' + object + '\n' + 'is part of the data structure.\n')


def runHashFunctions(object):
    # This function runs K hashing functions and runs updateTable() with each output
    for i in range(K):
        indexInT=int(mmh3.hash(object, i, signed=True) % m)
        try:
            updateTable(object, indexInT)
        except:
            print("Index is out of range of the Hash Table")

def updateTable(object, index):
    # This function updates the T array with objects that are inserted to it
    node = ListNode(object)                   # create a node with the value of the object
    T[index]['Key'] = 1                       # update 'Key' to 1
    T[index]['Value'].add_list_item(node)     # add item to the linked list in the 'Value'

if __name__ == "__main__":
    main()
