
#include <stdio.h>

int main()
{
    int arr[20],size,i,j,value;
    printf("\nenter the size of array:");
    scanf("%d",&size);
    printf("\nenter the array elements\n");
    for(i=0;i<size;i++)
      scanf("%d",&arr[i]);
    printf("\nunsorted list of array is\n");
    for(i=0;i<size;i++)
      printf("%d ",arr[i]);
   for(i=0;i<size;i++)
   {
        value=arr[i+1];
        for(j=i;j>=0;j--)
        {
             if(arr[j]>value)
             {
                  arr[j+1]=arr[j];
                  arr[j]=value;
             }
        }
   }
   printf("\nthe sorted list of array is\n");
   for(i=0;i<size;i++)
     printf("%d ",arr[i]);
    return 0;


}
