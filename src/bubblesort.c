
// tratto da:
// https://github.com/marcotessarotto/exOpSys/blob/master/004.5bubble-sort/bubble-sort.c

void swap_double(double *x, double *y) {
	double t = *x;
	*x = *y;
	*y = t;
}


/*
*
 * la funzione bubble_sort ordina l'array passato come parametro
 * array_dimension: numero di celle dell'array
 */
void bubble_sort_double(double * array, int array_dimension) {

	int n, newn;

	n = array_dimension;

	while (n > 1) {

		newn = 0;

		for (int i = 1; i < n; i++) {
			if (array[i-1] > array[i]) {
				swap_double(&array[i-1], &array[i]);
				newn = i;
			}
		}

		n = newn;
	}


/*
procedure bubbleSort(A : list of sortable items)
    n := length(A)
    repeat
        newn := 0
        for i := 1 to n - 1 inclusive do
            if A[i - 1] > A[i] then
                swap(A[i - 1], A[i])
                newn := i
            end if
        end for
        n := newn
    until n ≤ 1
end procedure
*/

}
