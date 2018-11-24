// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector> 
//#include <stdio.h> 
//#include <stdlib.h> 
#include <string.h> 
#include <time.h>
#include <random>

using namespace std;

int xyTo1D(int x, int y, int length);
void printArray(int a[], int n);
void swap(int a[], int i, int k);
void printMemBlock(vector<int> mem);
void insertFault(vector<int> &mem, int n);
//void insertFaults(vector<int> &mem, vector<int> &faults);
vector<int> generateFaults(vector<int> &mem, int numFaults);
bool isFaulty(vector<int> &mem, int n);
void useSpare(vector<int> &mem, int n, int row_col);
void printCombinations(int a[], int k, int n);
bool checkFixed(vector<int> &mem, int n);
bool solveSimpleIteration(vector<int> mem, string spareOrder, vector<int> faults, vector<int> params);
bool solveSimpleAll(vector<int> mem, vector<string> spareOrder, vector<int> faults, vector<int> params);

// Algorithm from online
int compare(const void *a, const void * b);
int findCeil(char str[], char first, int l, int h);
void sortedPermutations(char str[], vector<string> &rowcol);

vector<int> generateFaults(vector<int> &mem, int numFaults) {
	vector<int> faults;
	vector<int> block0_faults;
	vector<int> block1_faults;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disx(0, 5);
	std::uniform_int_distribution<> disy(0, 11);

	for (int i = 0; i < numFaults; i++) {
		int x = disx(rd);
		int y = disy(rd);
		int n = xyTo1D(x, y, 6);

		if (!isFaulty(mem, n)) {
			//faults.push_back(n);
			if (n > 35) {
				block1_faults.push_back(n);
			}
			else {
				block0_faults.push_back(n);
			}
			insertFault(mem, n);
		}
		else {
			while (isFaulty(mem, n)) {
				x = disx(rd);
				y = disy(rd);
				n = xyTo1D(x, y, 6);
			}
			//faults.push_back(n);
			if (n > 35) {
				block1_faults.push_back(n);
			}
			else {
				block0_faults.push_back(n);
			}
			insertFault(mem, n);
		}
	}
	//std::cout << "GENERATE FAULTS BEGIN" << endl;
		for (int i = 0; i < block0_faults.size(); i++) {
			//std::cout << block0_faults[i] << ", ";
		}
		//std::cout << endl;
		for (int i = 0; i < block1_faults.size(); i++) {
			//std::cout << block1_faults[i] << ", ";
		}
		//std::cout << endl;

	sort(block0_faults.begin(), block0_faults.end());
	sort(block1_faults.begin(), block1_faults.end());
	//std::cout << endl;
	/*
	for (int i = 0; i < block0_faults.size(); i++) {
		std::cout << block0_faults[i] << ", ";
	}
	std::cout << endl;
	for (int i = 0; i < block1_faults.size(); i++) {
		std::cout << block1_faults[i] << ", ";
	}*/

	int i = 0;
	int j = 0;
	while (i < block0_faults.size() && j < block1_faults.size()) {
		int x0 = block0_faults[i] / 6;
		int y0 = block0_faults[i] % 6;
		int x1 = (block1_faults[j] / 6) - 6;
		int y1 = (block1_faults[j] % 6) + 6;
		if (x0 <= x1) {
			faults.push_back(block0_faults[i]);
			i++;
		} else {
			faults.push_back(block1_faults[j]);
			j++;
		}
	}
	while (i < block0_faults.size()) {
		faults.push_back(block0_faults[i]);
		i++;
	}
	while (j < block1_faults.size()) {
		faults.push_back(block1_faults[j]);
		j++;
	}
	//std::cout << endl;

	//std::cout << "GENERATE FAULTS END" << endl;
	return faults;
}

bool isFaulty(vector<int> &mem, int n) {
	if (mem[n] == 1) {
		return true;
	}

	return false;
}

/*
void insertFaults(vector<int> &mem, vector<int> &faults) {
	for (int i = 0; i < faults.size(); i++) {
		insertFault(mem, faults[i]);
	}
}*/

int main()
{
	/*        0 - 35          36 - 71
		0 [] [0 0 0 0 0 0] [] [0 0 0 0 0 0] []
		1 [] [0 1 1 0 0 0] [] [0 1 1 0 1 0] []
		2 [] [0 0 0 0 1 0] [] [0 0 0 0 0 0] []
		3 [] [0 0 0 0 0 0] [] [0 0 0 0 0 1] []
		4 [] [0 1 0 0 1 0] [] [0 0 0 0 1 0] []
		5 [] [0 0 0 0 0 0] [] [0 0 0 0 0 1] []
			  0 1 2 3 4 5      0 1 2 3 4 5
		     [][][][][][]      [][][][][][]   2 Single local columns

		     [][][][][][][][][][][][][][][]   Global

			Solve: GR, LC1, LC2, LR1, LR2

		   1 global global row, 1 common col, 2 local cols, 2 local rows
		   3 rows, 3 cols

			  0 - 35          36 - 71
		0 [] [0 0 0 0 0 0]  [0 0 0 0 0 0] [] []
		1 [] [0 1 1 0 0 0]  [0 1 1 0 1 0] [] []  2 single local cols
		2 [] [0 0 0 0 1 0]  [0 0 0 0 0 0] [] []
		3 [] [0 0 0 0 0 0]  [0 0 0 0 0 1] [] []
		4 [] [0 1 0 0 1 0]  [0 0 0 0 1 0] [] []
		5 [] [0 0 0 0 0 0]  [0 0 0 0 0 1] [] []
			  0 1 2 3 4 5    0 1 2 3 4 5
			 [][][][][][]    [][][][][][]    2 Single Local rows

			 [][][][][][]    [][][][][][]    2 single local row

		   Solve: R, R, C, C, R, C

		   [SGR, DGR, SCC, SLC SLC]

		   SGR = 1
		   DGR = 1
		   SCC = 1
		   SLC1 = 1
		   SLC2 = 1
		   RL = 0/1 (lenght of row)
		   Block = 0/1 (Memory Block)

		   If (RL == 1 && DGR == 0)
			Unsolvable
		   else
			 DGR = DGR - 1
			 useSpare
		   If (RL == 0 && SGR == 0)
			Unsolvable
		   else
			 SGR = SGR - 1
			 useSpare
		   If (Block == 0 && SCC == 0)
			 Check Local
			 IF (Block == 0 && SLC1 == 0)
			  Unsolvable
			 else
			   SLC1 = SLC1 - 1
			   useSpare
		   else
			 SCC = SCC - 1
			 useSpare
		   If (Block == 1 && SCC == 0)
			  Check Local
			  If (Block == 1 && SLC2 == 0)
			  Unsolvable
			  else
				SLC2 = SLC2 - 1
				useSpare
		   else
			 SCC = SCC - 1
			 useSpare

		   R R C C C
		   R C R C C
		   R C C R C
		   R C C C R
		   C C C R R
		   C C R C R
		   C C R R C
		   C R C C R
		   C R C R C
		   C R R C C

		*The max fault coverage of 2 rows, 2 cols is 16 if they are all perfectly aligned
	*/
	/*
	Assume BIST already found and labeled the faults
	Scan memory array from top to bottom, left to right for faults
	For each fault, try using a spare row or column
	Try out all row/column combinations

	Example: 2 Rows, 2 Cols
	R-R-C-C
	R-C-R-C
	R-C-C-R
	C-C-R-R
	C-R-C-R
	C-R-R-C

	If a row is used, all faults on that row is fixed
	If a col is used, all faults on that col is fixed


	*/
	int memSize = 72;
	int blockSize = memSize / 2;
	int numFaults = 10; // 11
	int block = 0;
	int rowLength = 0; // 0 - single, 1 - double

	// Spares
	int SGR = 1; // single global row (0)
	int DGR = 1; // double global row (1)
	int SCC = 1; // single common column (2)
	int SLC1 = 1; // single local column block 1 (3)
	int SLC2 = 1; // single local column block 2 (3)

	vector<int> memblock;
	vector<int> faults;
	for (int i = 0; i < memSize; i++) {
		memblock.push_back(0);
	}

	// row - r, col - c
	vector<string> spareOrder;

	int spareRows = 2;
	int spareCols = 2;

	string availableSpares = "";


	int n = availableSpares.size();
	char *char_array = new char[n + 1];
	availableSpares.copy(char_array, availableSpares.size() + 1);
	char_array[availableSpares.size()] = '\0';

	availableSpares = "000111";
	//char char_arrays[] = "01233";
	char char_arrays[] = "0000111";

	time_t start = time(0);
	sortedPermutations(char_arrays, spareOrder);

	std::cout << memblock.size() << endl;
	for (int i = 0; i < spareOrder.size(); i++)
		std::cout << i << " - " << spareOrder[i] << endl;

	std::cout << "Memory Block Without Faults:" << endl;
	printMemBlock(memblock);
	faults = generateFaults(memblock, numFaults);

	std::cout << "\nAfter fault insertion" << endl;
	std::cout << "Total Number of Faults: " << faults.size() << endl;
	printMemBlock(memblock);
	std::cout << endl;
	std::cout << "Fault coordinates:" << endl;

	for (int i = 0; i < faults.size(); i++) {
		std::cout << faults[i] << ", ";
	}
	std::cout << endl;
	for (int i = 0; i < faults.size(); i++) {
		int x = faults[i] / 6;
		int y = faults[i] % 6;
		std::cout << "(" << x << ", " << y << "), ";
	}
	std::cout << endl;

	int faultIndex = 0;
	int spareRow1 = 2;
	int spareCol1 = 1;
	int spareRow2 = 2;
	int spareCol2 = 2;

	vector<int> params;
	params.push_back(faultIndex);
	params.push_back(spareRow1);
	params.push_back(spareCol1);
	params.push_back(spareRow2);
	params.push_back(spareCol2);

	double percent = 0;
	int attempts = 100;
	double numSolved = 0;

	for (int i = 0; i < attempts; i++) {
		for (int i = 0; i < memSize; i++) {
			memblock[i] = 0;
		}
		faults = generateFaults(memblock, numFaults);
		if (solveSimpleAll(memblock, spareOrder, faults, params)) {
			//std::cout << "Memory is not repairable.";
			numSolved++;
		}
	}

	percent = (double)numSolved / attempts;

	std::cout << "After " << attempts << " attempts. The memory was repaired " << numSolved << " times." << endl;
	std::cout << "Percent: " << percent << endl;

	/*
	for (int i = 0; i < spareOrder.size(); i++) {
		faultIndex = 0;
		spareRow1 = 2;
		spareCol1 = 1;
		spareRow2 = 2;
		spareCol2 = 2;
		vector<int> memTemp;
		for (int i = 0; i < memblock.size(); i++)
			memTemp.push_back(memblock[i]);

		std::cout << endl;
		std::cout << "-------------------------" << endl;
		std::cout << "ITERATION: " << i << endl;
		std::cout << "SPARE ORDER: " << spareOrder[i] << endl;
		std::cout << "-------------------------" << endl;

		if (solveSimpleIteration(memblock, spareOrder[i], faults, params)) {
			std::cout << "Solved using combination " << spareOrder[i] << endl;
			break;
		}

		
		
		for (int j = 0; j < spareOrder[i].size(); j++) {
			std::cout << "Current fault index: " << faultIndex << endl;

			std::cout << "spareRow1 = " << spareRow1 << ", " << "spareCol1 = " << spareCol1 << ", "
				<< "spareRow2 = " << spareRow2 << ", " << "spareCol2 = " << spareCol2 << endl;

			// Make sure fault index does not go out of range
			if (faultIndex >= faults.size())
				break;
			int x = stoi(spareOrder[i].substr(j, 1));

			while (checkFixed(memTemp, faults[faultIndex]) && faultIndex < faults.size() - 1) {
				faultIndex++;
				// Fault index is used in a for loop outside, so it must be less than the fault size
				if (faultIndex >= faults.size() - 1)
					break;
			}


			if (faults[faultIndex] > 35) {
				if (x == 1 && spareCol2 == 0) {
					std::cout << "No more spare columns for block 2. Unsolvable." << endl;
					break;
				}
				else if (x == 1 && spareCol2 > 0) {
					spareCol2--;
				}
				else if (x == 0 && spareRow2 == 0) {
					std::cout << "No more spare rows for block 2. Unsolvable." << endl;
					break;
				}
				else if (x == 0 && spareRow2 > 0) {
					spareRow2--;
				}
			}
			else if (faults[faultIndex] <= 35) {
				if (x == 1 && spareCol1 == 0) {
					std::cout << "No more spare columns for block 1. Unsolvable." << endl;
					break;
				}
				else if (x == 1 && spareCol1 > 0) {
					spareCol1--;
				}
				else if (x == 0 && spareRow1 == 0) {
					std::cout << "No more spare rows for block 1. Unsolvable." << endl;
					break;
				}
				else if (x == 0 && spareRow1 > 0) {
					spareRow1--;
				}
			}

			useSpare(memTemp, faults[faultIndex], x);
			// ensure that fault index does not exceed fault size - 1
			faultIndex++;

		}
		int faultCount = 0;
		std::cout << "Faulty cells left: " << endl;
		for (int i = faultIndex; i < faults.size(); i++) {
			if (!checkFixed(memTemp, faults[i])) {
				faultCount++;
				std::cout << faults[i] << ", ";
			}
		}
		std::cout << endl;
		std::cout << "Faults left: " << faultCount << endl;

		//if (spareRows == 0 && spareCols == 0) {
			//break;
		//}

		std::cout << "Memory after repair:" << endl;
		printMemBlock(memTemp);

	}

	//cout << "The memory is not repairable!" << endl;
	*/
} 

struct fault {
	int x;
	int y;
};

void swap(int a[], int i, int k) {
	int temp;
	temp = a[i];
	a[i] = a[k];
	a[k] = temp;
}

bool solveSimpleAll(vector<int> mem, vector<string> spareOrder, vector<int> faults, vector<int> params) {

	for (int i = 0; i < spareOrder.size(); i++) {
		//std::cout << endl;
		//std::cout << "-------------------------" << endl;
		//std::cout << "ITERATION: " << i << endl;
		//std::cout << "SPARE ORDER: " << spareOrder[i] << endl;
		//std::cout << "-------------------------" << endl;

		if (solveSimpleIteration(mem, spareOrder[i], faults, params)) {
			//std::cout << "Solved using combination " << spareOrder[i] << endl;
			return true;
		}
	}

	return false;
}

bool solveSimpleIteration(vector<int> mem, string spareOrder, vector<int> faults, vector<int> params) {
	if (params.size() < 1 || params.size() < 5)
		return false;

	int faultIndex = params[0];
	int spareRow1 = params[1];
	int spareCol1 = params[2];
	int spareRow2 = params[3];
	int spareCol2 = params[4];

	for (int j = 0; j < spareOrder.size(); j++) {
		//std::cout << "Current fault index: " << faultIndex << endl;

		//std::cout << "spareRow1 = " << spareRow1 << ", " << "spareCol1 = " << spareCol1 << ", "
			//<< "spareRow2 = " << spareRow2 << ", " << "spareCol2 = " << spareCol2 << endl;

		// Make sure fault index does not go out of range
		if (faultIndex >= faults.size())
			break;
		int x = stoi(spareOrder.substr(j, 1));

		while (checkFixed(mem, faults[faultIndex]) && faultIndex < faults.size() - 1) {
			faultIndex++;
			// Fault index is used in a for loop outside, so it must be less than the fault size
			if (faultIndex >= faults.size() - 1)
				break;
		}


		if (faults[faultIndex] > 35) {
			if (x == 1 && spareCol2 == 0) {
				//std::cout << "No more spare columns for block 2. Unsolvable." << endl;
				break;
			}
			else if (x == 1 && spareCol2 > 0) {
				spareCol2--;
			}
			else if (x == 0 && spareRow2 == 0) {
				//std::cout << "No more spare rows for block 2. Unsolvable." << endl;
				break;
			}
			else if (x == 0 && spareRow2 > 0) {
				spareRow2--;
			}
		}
		else if (faults[faultIndex] <= 35) {
			if (x == 1 && spareCol1 == 0) {
				//std::cout << "No more spare columns for block 1. Unsolvable." << endl;
				break;
			}
			else if (x == 1 && spareCol1 > 0) {
				spareCol1--;
			}
			else if (x == 0 && spareRow1 == 0) {
				//std::cout << "No more spare rows for block 1. Unsolvable." << endl;
				break;
			}
			else if (x == 0 && spareRow1 > 0) {
				spareRow1--;
			}
		}

		useSpare(mem, faults[faultIndex], x);
		// ensure that fault index does not exceed fault size - 1
		faultIndex++;

	}

	int faultCount = 0;
	//std::cout << "Faulty cells left: " << endl;
	for (int i = faultIndex; i < faults.size(); i++) {
		if (!checkFixed(mem, faults[i])) {
			faultCount++;
			//std::cout << faults[i] << ", ";
		}
	}
	
	//std::cout << "Faults left: " << faultCount << endl;

	if (faultCount == 0) {
		//printMemBlock(mem);
		//std::cout << endl;
		return true;
	}

	//if (spareRows == 0 && spareCols == 0) {
		//break;
	//}

	//std::cout << "Memory after repair:" << endl;
	//printMemBlock(mem);

	return false;
}

void printMemBlock(vector<int> mem) {

	int rowSize = mem.size() / 12;
	int blockSize = mem.size() / 2;
	for (int i = 0; i < mem.size() / 2; i++) {
		if (i % 6 == 0)
			std::cout << "[ ";
		std::cout << mem[i] << " ";
		if (i % 6 == 5)
			std::cout << "]";

		if (i % 6 == 5) {
			std::cout << " ";
			for (int j = 0; j < rowSize; j++) {
				if ((j + blockSize) % 6 == 0)
					std::cout << "[ ";
				std::cout << mem[j + blockSize + (i/rowSize) * rowSize] << " ";
				if ((j + blockSize) % 6 == 5)
					std::cout << "]" << endl;
			}
		}
	}
}

void insertFault(vector<int> &mem, int n) {
	mem[n] = 1;
}

bool checkFixed(vector<int> &mem, int n) {
	if (mem[n] == 1)
		return false;
	return true;
}

void useSpare(vector<int> &mem, int n, int row_col) {
	int row = n / 6;
	int col = n % 6;
	if (row_col == 0) {
		for (int i = 0; i < 6; i++) {
			mem[i + 6 * row] = 5;
		}
	}
	else if (row_col == 1) {
		for (int i = 0; i < 6; i++) {
			if (n > 35) {
				mem[i * 6 + col + 36] = 5;
			}
			else {
				mem[i * 6 + col] = 5;
			}
		}
	}
}

void printCombinations(int a[], int k, int n) {
	if (k == n)
		printArray(a, n);
	else {
		for (int i = k; i < n; i++) {
			swap(a, i, k);
			printCombinations(a, k + 1, n);
		}
	}
}

/* Following function is needed for library function qsort(). */
int compare(const void *a, const void * b)
{
	return (*(char *)a - *(char *)b);
}

// A utility function two swap two characters a and b 
void swap(char* a, char* b)
{
	char t = *a;
	*a = *b;
	*b = t;
}

// This function finds the index of the smallest character 
// which is greater than 'first' and is present in str[l..h] 
int findCeil(char str[], char first, int l, int h)
{
	// initialize index of ceiling element 
	int ceilIndex = l;

	// Now iterate through rest of the elements and find 
	// the smallest character greater than 'first' 
	for (int i = l + 1; i <= h; i++)
		if (str[i] > first && str[i] < str[ceilIndex])
			ceilIndex = i;

	return ceilIndex;
}

// Print all permutations of str in sorted order 
void sortedPermutations(char str[], vector<string> &rowcol)
{
	// Get size of string 
	int size = strlen(str);

	// Sort the string in increasing order 
	qsort(str, size, sizeof(str[0]), compare);

	// Print permutations one by one 
	bool isFinished = false;
	while (!isFinished)
	{
		// print this permutation 
		static int x = 1;
		//printf("%d %s \n", x++, str);
		string s(str);
		rowcol.push_back(s);

		// Find the rightmost character which is smaller than its next 
		// character. Let us call it 'first char' 
		int i;
		for (i = size - 2; i >= 0; --i)
			if (str[i] < str[i + 1])
				break;

		// If there is no such chracter, all are sorted in decreasing order, 
		// means we just printed the last permutation and we are done. 
		if (i == -1)
			isFinished = true;
		else
		{
			// Find the ceil of 'first char' in right of first character. 
			// Ceil of a character is the smallest character greater than it 
			int ceilIndex = findCeil(str, str[i], i + 1, size - 1);

			// Swap first and second characters 
			swap(&str[i], &str[ceilIndex]);

			// Sort the string on right of 'first char' 
			qsort(str + i + 1, size - i - 1, sizeof(str[0]), compare);
		}
	}
}

void printArray(int a[], int n) {
	for (int i = 0; i < n; i++) {
		std::cout << a[i] << " ";
	}
	std::cout << endl;
}

int xyTo1D(int x, int y, int length) {
	return (x % length) + (length * y);
}

