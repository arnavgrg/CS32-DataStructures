// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n) {
	if (n <= 0) { return false; }

	//Exit condition
	if (n == 1) {
		if (somePredicate(a[0]))
			return true;
		else
			return false;
	}

	//Simplify problem
	if (somePredicate(a[n - 1]))
		return allTrue(a, n - 1);
	else
		return false;
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n) {
	if (n < 0) { n = 0; }

	//Exit Condition
	if (n == 1) {
		if (!somePredicate(a[0]))
			return 1;
		else
			return 0;
	}

	//Simplifying Condition
	if (!somePredicate(a[n - 1]))
		return 1 + countFalse(a, n - 1);
	else
		countFalse(a, n - 1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false. If there is no such
// element, return -1.
int firstFalse(const double a[], int n) {
	if (n <= 0) { return -1; }

	//Exit/Base Condition
	if (n == 1) {
		if (!somePredicate(a[0]))
			return 0;
		else
			return -1;
	}

	//Simplifying Condition
	if (!somePredicate(a[0])) {
		return 0;
	}

	//update index;
	int arr_index = firstFalse(a + 1, n - 1);

	if (arr_index == -1) {
		return -1;
	}
	else
		return 1 + arr_index;
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n) {
	//base condition
	if (n <= 0) { //if n decrements so that n is less than or equal to 1, then return -1
		return -1;
	}
	//simplifying condition
	int arr_index = 1 + indexOfMin(a + 1, n - 1);
	if (a[0] <= a[arr_index])
		return 0;
	else
		return arr_index;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2) {
	//Base Conditions
	if (n1 == 0 && n2 == 0) //if both are empty return true  
		return true;
	if (n1 == 0 && n2 != 0) //if n decrements so that 1 is empty but 2 isn't, then it must return false since the element doesn't match up
		return false;

	//Simplifying Conditions
	if (a2[0] == a1[0])         
		return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1); 
		/*If they match up, then move each array up by one and reduce the size of each array by one */
	else
		/*Move to the next element in array 1, but keep the same element in array 2*/
		return includes(a1 + 1, n1 - 1, a2, n2); 
}