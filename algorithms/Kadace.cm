/*algorithm to calculate the maximum sum
of a sub-array of any given array*/
int maxSumArray(int vector[], int size){
  int k;
  int maxSumSoFar;
  int max;

  maxSumSoFar = 0;
  max = 0;
  k = 0;

  while (k < size){
    max = max + vector[k];
    if (max < 0) {
      max = 0;
    }
    if (maxSumSoFar < max) {
      maxSumSoFar = max;
    }
    k++;
  }
  return maxSumSoFar;
}

void main (void){
  int A[5];
  int maxsum;

  A[0] = 2;
  A[1] = 5;
  A[2] = 3;
  A[3] = 9;
  A[4] = 8;

  maxsum = maxSumArray(A,4);

return;
}
