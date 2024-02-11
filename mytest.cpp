/**********************************************
 ** File: mytest.cpp
 ** Project: CMSC 341 Project 1, Fall 2023
 ** Author: Joshua Hur
 ** Date: 10/03/23
 ** Section: 2
 ** E-mail: jhur1@umbc.edu
 **
 ** This is one of the program files for Project 1.
 ** This file simulates Compressed Sparse Row format
 ** that packs members of a matrix into an array.
 ** Performs various tests that program successfully creates the matrix.
 ************************************************************************/

#include "csr.h"

class Tester{
public:
    // bool testCompress(const CSR& csrObject, int expectedM, int expectedN, const int* expectedValues,
    // const int* expectedColIndex, const int* expectedRowIndex, int expectedNonZeros)
    // Case: Verify the compress matrix data against expected data
    // Expected result: Return true if all elements of CSR object match the expected values, else return false
    bool testCompress(const CSR& csrObject, int expectedM, int expectedN, const int* expectedValues,
                      const int* expectedColIndex, const int* expectedRowIndex, int expectedNonZeros) {
        // Check matrix dimensions
        if (csrObject.m_m != expectedM || csrObject.m_n != expectedN) {
            return false;
        }
        
        // Check non-zeros count
        if (csrObject.m_nonzeros != expectedNonZeros) {
            return false;
        }
        
        // Check values, col_index and row_index arrays
        for (int i = 0; i < expectedNonZeros; ++i) {
            if (csrObject.m_values[i] != expectedValues[i] || csrObject.m_col_index[i] != expectedColIndex[i]) {
                return false;
            }
        }
        
        for (int i = 0; i <= expectedM; ++i) {
            if (csrObject.m_row_index[i] != expectedRowIndex[i]) {
                return false;
            }
        }
        
        return true;
    }
    
    // testInsufficientData(const CSR& obj, int m, int n, int arraySize)
    // Case: Check if the provide data is insufficient to represent the matrix of size m * n
    // Expected: Return true if arraySize is less than m * n, else returns false
    bool testInsufficientData(const CSR& obj, int m, int n, int arraySize) {
        int expectedSize = m * n;
        return ((expectedSize != arraySize) && (expectedSize > arraySize));
    }
    
    // testInvalidDataForZeroMatrix(const CSR& obj, int array[], int arraySize)
    // Case: Check if a zero matrix is represented with invalid non-zero data
    // Expected: Return true if CSR object is empty but has non-zero data, else return false
    bool testInvalidDataForZeroMatrix(const CSR& obj, int array[], int arraySize) {
        bool hasData = false;
        
        for (int i = 0; i < arraySize; i++) {
            if (array[i] != 0) {
                hasData = true;
                break;
            }
        }
        
        return obj.empty() && hasData;
    }
    
    // testEqualityOperator(const CSR& a, const CSR& b)
    // Case: Verify the equality operator for two CSR objects
    // Expected: Return true if both objects are equal: both empty of both non-empty and equal,
    //           else return false
    bool testEqualityOperator(const CSR& a, const CSR& b) {
        // Check if both objects are empty or not
        if (!a.empty() && !b.empty()) {
            if (a == b) {
                return true;
            }
            
        } else {
            if (a == b) {
                return true;
            }
        }
        
        return false;
    }
    
    // testGetAtThrowsException(const CSR& csr, int row, int col)
    // Case: Check if accessing matrix data at given row and column throws an exception
    // Expected: Return true if an exception is thrown, else return false
    bool testGetAtThrowsException(const CSR& csr, int row, int col) {
        try {
            csr.getAt(row, col);
            return false;
            
        } catch (const std::runtime_error&) {
            // Exception was caught, so the test passed.
            return true;
        }
    }
    
    // testAssignmentOperator(const CSRList& list1, const CSRList& list2)
    // Case: Verify the assignment operator for two CSRList objects
    // Expected: Return true if the lists are correctly assigned, else return false
    bool testAssignmentOperator(const CSRList& list1, const CSRList& list2) {
        {
            // Check if both lists are empty
            if (list1.empty() && list2.empty()) {
                return true;
            
            // Check if one is empty and another is not
            } else if ((!list1.empty() && list2.empty()) || (list1.empty() && !list2.empty())){
                return false;
                
            }
        }  // Destroying copiedList to check for memory leaks or double deletions
        return true;
    }
    
    // testGetAtCSRList(const CSRList& list, int CSRIndex, int row, int col, int expectedValue)
    // Case: Verify the value retrieval from the CSRList at given index, row and column
    // Expected: Return true if retrieved value matches expected value, else return false
    bool testGetAtCSRList(const CSRList& list, int CSRIndex, int row, int col, int expectedValue) {
        {
            try {
                // Attempt to access an element of an empty list
                int value = list.getAt(CSRIndex, row, col);
                return value == expectedValue;
                
            } catch (const std::runtime_error&) {
                return false;
            }
        }
    }
    
    // testGetAtEmptyCSRList(const CSRList& list, int CSRIndex, int row, int col, int expectedValue)
    // Case: Check value retrieval from an empty CSRList
    // Expected: Return true i an exception is thrown when accessing the empty list, else return false
    bool testGetAtEmptyCSRList(const CSRList& list, int CSRIndex, int row, int col, int expectedValue) {
        {
            try {
                // Attempt to access an element of an empty list
                list.getAt(0, 0, 0);
                
            } catch(const runtime_error& e) {
                return true;
            }
            
            return false;
        }
    }
    
private:
};

int main()
{
    try{
        
        Tester tester;
        CSR aCSR;
        int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
        aCSR.compress(4,6,array1,24);
        CSR bCSR(aCSR);
        CSR dCSR;
        CSR eCSR(dCSR);

        // Expected CSR compressed representation for array1
        int expectedValues[] = {10, 20, 30, 40, 50, 60, 70, 80};
        int expectedColIndex[] = {0, 1, 1, 3, 2, 3, 4, 5};
        int expectedRowIndex[] = {0, 2, 4, 7, 8};
        
        if (tester.testCompress(aCSR, 4, 6, expectedValues, expectedColIndex, expectedRowIndex, 8)) {
            cout << "Test Compress passed." << endl;
            
        } else {
            cout << "Test Compress failed." << endl;
        }
        
        if (tester.testInsufficientData(aCSR, 4, 6, 20)) {
            cout << "Test passed: there is insufficient data." << endl;
        } else {
            cout << "Test failed: there is enough data." << endl;
        }
        
        int array3[] = {1, 2};
        CSR testCSR;
        testCSR.compress(0, 0, array3, 2);
        
        if (tester.testInvalidDataForZeroMatrix(testCSR, array3, 2)) {
            cout << "Test passed: there is invalid data for zero matrix." << endl;
        } else {
            cout << "Test failed: the zero matrix is empty." << endl;
        }
        
        if (tester.testEqualityOperator(aCSR, bCSR)) {
            cout << "Test Equality passed." << endl;
            
        } else {
            cout << "Test Equality failed." << endl;
        }
        
        if (tester.testEqualityOperator(dCSR, eCSR)) {
            cout << "Test Equality passed." << endl;
            
        } else {
            cout << "Test Equality failed." << endl;
        }
        
        if (tester.testGetAtThrowsException(aCSR, 3, 5)){
            cout << "Test Get At Throws Exception is passed. The requested number do not exist in the matrix." << endl;
            
        } else {
            cout << "Test Get At Throws Exception is failed. The request number does exist in the matrix." << endl;
        }
        
        CSR cCSR;
        int array2[] = {0,0,0,0,100,200,0,0,300};
        cCSR.compress(3,3,array2,9);
        
        CSRList list1;
        list1.insertAtHead(aCSR);
        list1.insertAtHead(cCSR);

        CSRList list2;
        list2.insertAtHead(cCSR);
        list2.insertAtHead(aCSR);
        
        CSRList list3;
        CSRList list4(list3);
    
        
        if (tester.testAssignmentOperator(list1, list2)) {
            cout << "Test Assignment Operator is passed." << endl;
            
        } else {
            cout << "Test Assignment Operator is failed." << endl;
        }
        
        if (tester.testAssignmentOperator(list1, list3)) {
            cout << "Test Assignment Operator is passed." << endl;
            
        } else {
            cout << "Test Assignment Operator is failed." << endl;
        }
        
        if (tester.testGetAtCSRList (list1, 1, 2, 4, 70)) {
            cout << "Test Get At CSRList passed. The desired value is located at the location." << endl;
            
        } else {
            cout << "Test Get At CSRList failed. The desired value is not located at the location." << endl;
        }
        
        if (tester.testGetAtEmptyCSRList (list3, 1, 2, 4, 70)) {
            cout << "Test Throw an Exception for Get At Empty CSRList passed." << endl;
            
        } else {
            cout << "Test Throw an Exception for Get At Empty CSRList failed." << endl;
        }
    }
    catch (exception &e){
        cout << e.what() << endl;
    }
    return 0;
}
