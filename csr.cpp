/**********************************************
 ** File: csr.cpp
 ** Project: CMSC 341 Project 1, Fall 2023
 ** Author: Joshua Hur
 ** Date: 10/03/23
 ** Section: 2
 ** E-mail: jhur1@umbc.edu
 **
 ** This is one of the program files for Project 1.
 ** This file simulates Compressed Sparse Row format
 ** that packs members of a matrix into an array.
 ************************************************************************/

#include "csr.h"
///////////////CSR Class Implementation///////////////

// CSR()
// Default constructor for CSR class creates an empty object.
CSR::CSR(){
    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
}

// ~CSR()
// Destructor that deallocates the memory.
CSR::~CSR(){
    clear();
}

// CSR(const CSR & rhs)
// Copy constructor creates a deep copy of rhs
CSR::CSR(const CSR & rhs){
    m_m = rhs.m_m;
    m_n = rhs.m_n;
    m_nonzeros = rhs.m_nonzeros;
    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    
    // Check if the object is empty
    if (rhs.empty()) {
        return;
    }
    
    // Allocate memory of values, column indices, and row indices of the source matrix
    m_values = new int[m_nonzeros];
    m_col_index = new int[m_nonzeros];
    m_row_index = new int[m_m + 1];
    
    // Copy values and column indices
    for (int i = 0; i < m_nonzeros; i++) {
        m_values[i] = rhs.m_values[i];
        m_col_index[i] = rhs.m_col_index[i];
    }
    
    // Copy row indices
    for (int i = 0; i < m_m + 1; i++) {
        m_row_index[i] = rhs.m_row_index[i];
    }
}

// clear()
// Deallocate all memory and converts the object to an empty CSR object.
void CSR::clear(){
    delete[] m_values;
    delete[] m_col_index;
    delete[] m_row_index;
    
    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
}

// empty()
// Check if the object is empty or not.
bool CSR::empty() const{
    return m_values == nullptr && m_col_index == nullptr && m_row_index == nullptr;
}

// compress(int m, int n, int array[], int arraySize)
// Creates a compressed version of the sparse matrix.
void CSR::compress(int m, int n, int array[], int arraySize){
    clear();
    
    // Set matrix dimensions
    m_m = m;
    m_n = n;
    
    // Check if the matrix dimension is 0 x 0
    if (m == 0 || n == 0) {
        return;
    }
    
    // Count non-zero entries in the matrix
    m_nonzeros = 0;
    for (int i = 0; i < arraySize && i < m * n; i++) {
        if (array[i] != 0) {
            m_nonzeros++;
        }
    }
    
    // Allocate memory for the compressed matrix
    m_values = new int[m_nonzeros];
    m_col_index = new int[m_nonzeros];
    m_row_index = new int[m_m + 1];
    
    // Fill the compressed matrix arrays
    int k = 0;
    for (int i = 0; i < m; i++) {
        m_row_index[i] = k;
        for (int j = 0; j < n; j++) {
            if (array[i * n + j] != 0) {
                m_values[k] = array[i * n + j];
                m_col_index[k] = j;
                k++;
            }
        }
    }
    
    m_row_index[m] = m_nonzeros;
}

// getAT(int row, int  col) const
// Return the value at specific row and col in the matrix.
int CSR::getAt(int row, int  col) const{
    
    // Check if the given location is valid or not
    if (row < 0 || row >= m_m || col < 0 || col >= m_n) {
        throw runtime_error("Indices are out of range");
    }
    
    // Search for the value
    for (int i = m_row_index[row]; i < m_row_index[row + 1]; i++) {
        if (m_col_index[i] == col) {
            return m_values[i];
        }
    }
    return 0;
}

// operator==(const CSR & rhs) const
// Check if two matrices are equal.
bool CSR::operator==(const CSR & rhs) const{
    
    // Check if both objects are empty
    if (this->empty() && rhs.empty()) {
        return true;
    }
    
    // Check dimensions and non-zero counts of both object
    if (m_m != rhs.m_m || m_n != rhs.m_n || m_nonzeros != rhs.m_nonzeros) {
        return false;
    }
    
    // Compare values and column indices
    for (int i = 0; i < m_nonzeros; i++) {
        if (m_values[i] != rhs. m_values[i] || m_col_index[i] != rhs.m_col_index[i]) {
            return false;
        }
    }
    
    // Compare row indices
    for (int i = 0; i < m_m + 1; i++) {
        if (m_row_index[i] != rhs.m_row_index[i]) {
            return false;
        }
    }
    
    return true;
}

// sparseRatio()
// Calculates the sparsity ratior for the CSR object.
int CSR::sparseRatio(){
 
    if (empty()) {
        return 0;
    }
    
    double ratio = ((double) m_nonzeros) / (m_m * m_n);
    return 100 - (int)(ratio * 100);
}


void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}
 

//////////////CSRList Class Implementation///////////////

// CSRList()
// Default constructor of CSRList and creates an empty object.
CSRList::CSRList(){
    m_head = nullptr;
    m_size = 0;
}

// CSRList(const CSRList & rhs)
// Copy constructor creates a copied CSRList object.
CSRList::CSRList(const CSRList & rhs){
    m_head = nullptr;
    m_size = rhs.m_size;
    
    // Check the source list is empty or not
    if (rhs.empty()) {
        return;
    }
    
    m_head = new CSR(*rhs.m_head);
    CSR* current = m_head;
    CSR* rhsCurrent = rhs.m_head -> m_next;
    
    // Iterate throough the source list and copy
    while (rhsCurrent) {
        current -> m_next = new CSR(*rhsCurrent);
        current = current -> m_next;
        rhsCurrent = rhsCurrent -> m_next;
    }
}

// ~CSRList()
// Destructor and deallocates the memory.
CSRList::~CSRList(){
    clear();
}

// empty() const
// Check if the list is empty or not.
bool CSRList::empty() const{
    return m_head == nullptr;
}

// insertAtHead(const CSR & matrix)
// Inserts the CSR object at the head of the another CSRList object.
void CSRList::insertAtHead(const CSR & matrix){
    CSR* newNode = new CSR(matrix);
    newNode -> m_next = m_head;
    m_head = newNode;
    m_size++;
}

// clear()
// Resets the CSRList object
void CSRList::clear(){
    CSR* current = m_head;
    
    // Iterate through the list and delete
    while (current) {
        CSR* NextNode = current -> m_next;
        delete current;
        current = NextNode;
    }
    
    m_head = nullptr;
    m_size = 0;
}

// getAt(int CSRIndex, int row, int col) const
// Returns the value in the matrix at the specific row and col.
int CSRList::getAt(int CSRIndex, int row, int col) const{
    // Check the given CSRIndex is within bounds
    if (CSRIndex < 0 || CSRIndex >= m_size) {
        throw runtime_error("CSRIndex is out of range");
    }
    
    // Navigate to the desired CSR node
    CSR* current = m_head;
    for (int i = 0; i < CSRIndex; i++) {
        current = current -> m_next;
    }
    
    return current -> getAt(row, col);
}

// operator== (const CSRList & rhs) const
// Overloaded equality operator that checks both lists are the same.
bool CSRList::operator== (const CSRList & rhs) const{
    // Check both lists have the same size
    if (m_size != rhs.m_size) {
            return false;
    }

    // Check if both lists are empty or not
    if (m_size == 0 && rhs.m_size == 0) {
        return true;
    }

    // Compare each CSR in the list using the overloaded `==` operator of CSR class.
    CSR* lhsCurrent = m_head;
    CSR* rhsCurrent = rhs.m_head;

    while (lhsCurrent && rhsCurrent) {
        if (!(*lhsCurrent == *rhsCurrent)) {
            return false;
        }
        
        lhsCurrent = lhsCurrent->m_next;
        rhsCurrent = rhsCurrent->m_next;
    }

    return true;
}

// operator=(const CSRList & rhs)
// Overloaded assignment operator that creates an exact deep copy of another object.
const CSRList& CSRList::operator=(const CSRList & rhs){
    // Check for self-assignment
    if (this == &rhs) {
        return *this;
    }
    
    clear();
    
    m_size = rhs.m_size;
    
    if (rhs.empty()) {
        return *this;
    }
    
    // Make a deep copy from the head of the source list
    m_head = new CSR(*rhs.m_head);
    CSR* current = m_head;
    CSR* rhsCurrent = rhs.m_head -> m_next;
    
    while (rhsCurrent) {
        current -> m_next = new CSR(*rhsCurrent);
        current = current -> m_next;
        rhsCurrent = rhsCurrent -> m_next;
    }
    
    return *this;
}

// averageSparseRatio()
// Returns the average sparse ratio for all matrices.
int CSRList::averageSparseRatio(){
    if (empty()) {
        return 0;
    }
    
    int totalRatio = 0;
    
    CSR* current = m_head;
    
    // Sum up the sparsity ratios of all matrices in the list
    while (current) {
        totalRatio += current -> sparseRatio();
        current = current -> m_next;
    }
    
    return totalRatio / m_size;
}


void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}

