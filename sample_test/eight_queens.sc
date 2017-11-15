int chess[8][8]={0};
int a[8],b[15],c[15];
int sum=0;                             
int PutQueen(int n)
{
  int col,i,j;
  for(col=0;col<8;++col)
  {
    if((a[col] == 1)&& (b[n+col] == 1) &&(c[n-col+7] == 1))
    {
      chess[n][col]=1;             
      a[col]=0;
      b[n+col]=0;
      c[n-col+7]=0;      
      if(n==7)
      {
        ++sum;          
      }
      else 
        PutQueen(n+1);                 
      chess[n][col]=0;                 
      b[n+col]=1;                                  
      c[n-col+7]=1;
      a[col]=1;                           
    }
  }
  return 0;
}
int main()
{  
  int i;
  for(i=0;i<8;++i)
    a[i]=1;
  for(i=0;i<15;++i)
  {
    b[i]=1;
    c[i]=1;
  }
  PutQueen(0);
  write(sum);
  return 0;
}